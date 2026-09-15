/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractStruPostVTK.h"

#include "FITKCutViewManager.h"
#include "FITKStruIsosurfPostData.h"
#include "FITKStruPostMessageText.h"
#include "FITKStruPostDisplayGroup.h"
#include "FITKIBEStrDataSetReader.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKDeformFilter.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKMeshFeatureVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkIdTypeArray.h>
#include <vtkDataSetReader.h>
#include <vtkCellDataToPointData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdFilter.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkPassThrough.h>

#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
FITKLIBINFOREGISTER(FITKInterfaceStructuralPost, FITKInterfaceStructuralPostVersion);

namespace Interface
{

    FITKAbstractStructuralPostVTK::FITKAbstractStructuralPostVTK(StruPostFileType type):
        _type(type)
    {
        if (_type == StruPostFileType::SPFTVTK) 
        {
            //创建管线
            vtkDataSetReader* vtkReader = vtkDataSetReader::New();
            vtkReader->SetReadAllVectors(true);
            vtkReader->SetReadAllNormals(true);
            vtkReader->SetReadAllFields(true);
            vtkReader->SetReadAllScalars(true);
            vtkReader->SetReadAllColorScalars(true);
            vtkReader->SetReadAllTCoords(true);
            vtkReader->SetReadAllTensors(true);
            _dataSetReader = vtkReader;
        }
        else if (_type == StruPostFileType::SPFTVTP) 
        {
            vtkXMLPolyDataReader* dataReader = vtkXMLPolyDataReader::New();
            _dataSetReader = dataReader;
        }        
        else if (_type == StruPostFileType::SPFTVTU)
        {
            vtkXMLUnstructuredGridReader* dataReader = vtkXMLUnstructuredGridReader::New();
            _dataSetReader = dataReader;
        }
        else if (_type == StruPostFileType::SPFTIBE) {
            FITKIBEStrDataSetReader* dataReader = FITKIBEStrDataSetReader::New();
            _dataSetReader = dataReader;
        }

        //判断数据是否有效
        if (_dataSetReader == nullptr) {
            return;
        }

        //将单元数据转换为点数据（单元云图需要平滑过渡）<By BaGuiJun>
        _cellDataToPoint = vtkCellDataToPointData::New();
        _cellDataToPoint->SetInputConnection(_dataSetReader->GetOutputPort());

        // 追加元素ID。
        _idFilter = vtkIdFilter::New();
        _idFilter->SetCellIds(true);
        _idFilter->SetPointIds(false);
        _idFilter->SetFieldData(true);
        _idFilter->SetPointIdsArrayName(FITKMeshFeatureVTK::GetPointIdArrayName());
        _idFilter->SetCellIdsArrayName(FITKMeshFeatureVTK::GetCellIdArrayName());
        _idFilter->SetInputConnection(_cellDataToPoint->GetOutputPort());

        // 初始化分组显示数据提取器。
        //@{
        _selectNode = vtkSelectionNode::New();
        _section = vtkSelection::New();
        _extractSelection = vtkExtractSelection::New();
        _selectNode->SetContentType(vtkSelectionNode::INDICES);
        _selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
        _section->AddNode(_selectNode);
        _extractSelection->SetInputData(1, _section);

        _idArray = vtkIdTypeArray::New();
        _selectNode->SetSelectionList(_idArray);
        _extractSelection->SetInputConnection(_idFilter->GetOutputPort());

        // 创建形变过滤器。
        _deformFilter = FITKDeformFilter::New();
        this->setEnableDisplayGroup(false);  
        //@}

        // 创建网格特征数据，关闭ID数据生成。
        _feature = new Interface::FITKMeshFeatureVTK(_deformFilter->GetOutputPort());
        _feature->setCellAndPointIdsEnable(false, false);
        _feature->setEnabledFeature(false);

        QString deformVec;
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        if (appSettings != nullptr)
        {
            _unDeformFactor = appSettings->getValue<double>("StructualPost/UnDeformFactor");
            deformVec = appSettings->getValue<QString>("StructualPost/DeformVecName");
        }
        QByteArray ba = deformVec.toLocal8Bit();
        _deformFilter->SetDeformArrayName(ba.data());
        _deformFilter->SetScaleFactor(_unDeformFactor);

        //初始化信息文本 <By BaGuiJun>
        _textData = new FITKStruPostMessageText();

        // 初始化显示分组管理器。
        m_displayGroupManager = new FITKPostGroupManager;

        // 渲染输出过滤器。
        m_renderAlg = vtkPassThrough::New();
        m_renderAlg->SetAllowNullInput(true);
        m_renderAlg->SetInputConnection(_deformFilter->GetOutputPort());
    }

    FITKAbstractStructuralPostVTK::~FITKAbstractStructuralPostVTK()
    {
        _dataSetReader->Delete();
        _deformFilter->Delete();
        m_renderAlg->Delete();

        if (_cutViewManager)
            delete _cutViewManager;

        // 析构分组管理器。
        if (m_displayGroupManager)
        {
            delete m_displayGroupManager;
            m_displayGroupManager = nullptr;
        }

        // 析构数据提取器。
        //@{
        if (_selectNode)
        {
            _selectNode->Delete();
            _selectNode = nullptr;
        }

        if (_section)
        {
            _section->Delete();
            _section = nullptr;
        }

        if (_idArray)
        {
            _idArray->Delete();
            _idArray = nullptr;
        }

        if (_extractSelection)
        {
            _extractSelection->Delete();
            _extractSelection = nullptr;
        }

        if (_textData) {
            delete _textData;
            _textData = nullptr;
        }

        if (_feature)
        {
            delete _feature;
            _feature = nullptr;
        }

        if (_cellDataToPoint) {
            _cellDataToPoint->Delete();
            _cellDataToPoint = nullptr;
        }

        if (_idFilter) {
            _idFilter->Delete();
            _idFilter = nullptr;
        }
    }

    /**
     * @brief 获取结构后处理类型
     *
     * @return StruPostType 返回当前结构后处理的类型。
     */
    StruPostType FITKAbstractStructuralPostVTK::getPostStructualType()
    {
        return SPTNone;
    }

    /**
     * @brief 设置前置案例ID
     *
     * @param id 设置的前置案例的ID。
     */
    void FITKAbstractStructuralPostVTK::setPreCaseID(const int id)
    {
        _preCaseID = id;
    }

    /**
     * @brief 获取前置案例ID
     *
     * @return int 返回当前设置的前置案例的ID。
     */
    int FITKAbstractStructuralPostVTK::getPreCaseID()
    {
        return _preCaseID;
    }

    FITKStruPostMessageText * FITKAbstractStructuralPostVTK::getMessageTextData()
    {
        return _textData;
    }

    void FITKAbstractStructuralPostVTK::updateText()
    {

    }

    void FITKAbstractStructuralPostVTK::setIsCellDataToPointData(bool isCellToPoint)
    {
        if (isCellToPoint == true) {
            _idFilter->SetInputConnection(_cellDataToPoint->GetOutputPort());
        }
        else {
            _idFilter->SetInputConnection(_dataSetReader->GetOutputPort());
        }

        _idFilter->Update();
    }

    void FITKAbstractStructuralPostVTK::setModelVisible(bool flag)
    {
        _isHide = flag;
    }

    bool FITKAbstractStructuralPostVTK::getModelVisible()
    {
        return _isHide;
    }

    QPair<int, int> FITKAbstractStructuralPostVTK::getH5FileStep()
    {
        FITKIBEStrDataSetReader* dataReader = FITKIBEStrDataSetReader::SafeDownCast(_dataSetReader);
        if (_type == StruPostFileType::SPFTIBE) {
            if (dataReader != nullptr) {
                return QPair<int, int>(dataReader->getCurrentStep(), dataReader->getTotalStep());
            }
        }
        return QPair<int, int>(-1, -1);
    }

    void FITKAbstractStructuralPostVTK::setInputFile(const QString & file, const QString &resultFile, int step)
    {
        //更新文件输入
        QByteArray ba = file.toUtf8();
        // 云道读取.h5结果文件
        QByteArray baResult = resultFile.toUtf8();

        if (_type == StruPostFileType::SPFTVTK) {
            vtkDataSetReader* dataReader = vtkDataSetReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return;
            }
            dataReader->SetFileName(ba.data());
        }
        else if (_type == StruPostFileType::SPFTVTP) {
            vtkXMLPolyDataReader* dataReader = vtkXMLPolyDataReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return;
            }
            dataReader->SetFileName(ba.data());
        }
        else if (_type == StruPostFileType::SPFTVTU) {
            vtkXMLUnstructuredGridReader* dataReader = vtkXMLUnstructuredGridReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return;
            }
            dataReader->SetFileName(ba.data());
        }
        else if (_type == StruPostFileType::SPFTIBE) {
            FITKIBEStrDataSetReader* dataReader = FITKIBEStrDataSetReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return;
            }
            // 网格文件
            dataReader->setMeshFile(ba.data());
            // 结果文件
            dataReader->setResultFile(baResult.data());
            // 时间步设置
            dataReader->setStep(step);
            dataReader->Modified();
        }
     
        if (_dataSetReader == nullptr || _deformFilter == nullptr) return;
        if (_idFilter == nullptr) return;

        // 刷新网格特征提取。
        _dataSetReader->Update();
        _deformFilter->Update();

        if (_feature)
        {
            _feature->update();
            //_feature->reset(_deformFilter->GetOutputPort());
        }

        //更新文本信息
        updateText();
    }

    QString FITKAbstractStructuralPostVTK::getInputFile()
    {
        const char* f = "";
        if (_type == StruPostFileType::SPFTVTK) {
            vtkDataSetReader* dataReader = vtkDataSetReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return f;
            }
            f = dataReader->GetFileName();
        }
        else if (_type == StruPostFileType::SPFTVTP) {
            vtkXMLPolyDataReader* dataReader = vtkXMLPolyDataReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return f;
            }
            f = dataReader->GetFileName();
        }
        else if (_type == StruPostFileType::SPFTVTU) {
            vtkXMLUnstructuredGridReader* dataReader = vtkXMLUnstructuredGridReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return f;
            }
            f = dataReader->GetFileName();
        }
        
        return QString::fromUtf8(f);
    }

    void FITKAbstractStructuralPostVTK::resetDisplayGroup()
    {
        if (!_idArray)
        {
            return;
        }

        // 清空分组ID列表。
        _idArray->Reset();
        _idArray->SetNumberOfValues(0);

        // 使用新数据集填充ID。
        vtkDataSet* dataSet = getOutput();
        if (!dataSet)
        {
            return;
        }

        int nCells = dataSet->GetNumberOfCells();
        for (int i = 0; i < nCells; i++)
        {
            _idArray->InsertNextValue(i);
        }

        if (_section)
        {
            _section->Modified();
        }

        if (_extractSelection)
        {
            _extractSelection->Update();
        }
    }

    vtkAlgorithm* FITKAbstractStructuralPostVTK::getDataSetReader()
    {
        return _dataSetReader;
    }

    vtkDataSet * FITKAbstractStructuralPostVTK::getOutput()
    {
        if (!_idFilter)
        {
            return nullptr;
        }

        _idFilter->Update();
        vtkDataSet* ds = vtkDataSet::SafeDownCast(_idFilter->GetOutputDataObject(0));
        return ds;

        /*vtkDataSet* dataSet = nullptr;

        if (_type == StruPostFileType::SPFTVTK) {
            vtkDataSetReader* dataReader = vtkDataSetReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return dataSet;
            }
            dataSet = dataReader->GetOutput();
        }
        else if (_type == StruPostFileType::SPFTVTP) {
            vtkXMLPolyDataReader* dataReader = vtkXMLPolyDataReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return dataSet;
            }
            dataSet = dataReader->GetOutput();
        }
        else if (_type == StruPostFileType::SPFTVTU) {
            vtkXMLUnstructuredGridReader* dataReader = vtkXMLUnstructuredGridReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return dataSet;
            }
            dataSet = dataReader->GetOutput();
        }*/

        //return dataSet;
    }

    FITKDeformFilter* FITKAbstractStructuralPostVTK::getDeformFilter()
    {
        return _deformFilter;
    }

    FITKAbstractViewCut* FITKAbstractStructuralPostVTK::getVisibleViewCut()
    {
        // Get the cut view manager and all cut views.
        if (!_cutViewManager)
        {
            return nullptr;
        }

        // Check the view cut setting first.
        if (!getShowViewCut())
        {
            return nullptr;
        }

        // Check all cut views' visibility.
        int nCut = _cutViewManager->getDataCount();
        for (int i = 0; i < nCut; i++)
        {
            Interface::FITKAbstractViewCut* cut = _cutViewManager->getDataByIndex(i);
            if (!cut)
            {
                continue;
            }

            // Check the visibility.
            if (cut->isVisible())
            {
                return cut;
            }
        }

        return nullptr;
    }

    void FITKAbstractStructuralPostVTK::setUseMeshFeature(bool flag)
    {
        FITKAbstractViewCut* cut = getVisibleViewCut();
        if (cut)
        {
            cut->setUseMeshFeature(flag);
        }
        else
        {
            _feature->setEnabledFeature(flag);

            // 更新管线。
            m_renderAlg->SetInputConnection(_feature->getRenderingFilter()->GetOutputPort());
            m_renderAlg->Modified();
        }
    }

    bool FITKAbstractStructuralPostVTK::getUseMeshFeature()
    {
        if (_feature)
        {
            return _feature->getEnabledFeature();
        }

        return false;
    }

    FITKMeshFeatureVTK* FITKAbstractStructuralPostVTK::getMeshFeature()
    {
        return _feature;
    }

    vtkAlgorithm* FITKAbstractStructuralPostVTK::getOutputAlgorithm()
    {
        // 计算管线使用变形算法输出。
        return _deformFilter;
    }

    vtkAlgorithm* FITKAbstractStructuralPostVTK::getRenderOutputAlgorithm()
    {
        return m_renderAlg;
    }

    vtkExtractSelection* FITKAbstractStructuralPostVTK::getDisplayGroupExtractor()
    {
        return _extractSelection;
    }

    QStringList FITKAbstractStructuralPostVTK::getVariables(VariableLocation loc)
    {
        QStringList variables;
        vtkDataSet* ds = getOutput();
        if (ds == nullptr) return variables;
        //节点值
        if (loc == VariableLocation::VALOCNode)
        {
            vtkPointData* pd = ds->GetPointData();
            if (pd == nullptr) return variables;
            const int na = pd->GetNumberOfArrays();
            //遍历所有变量
            for (int i =0;i<na; ++i)
            {
                QString va = QString(pd->GetArrayName(i));
                variables.append(va);
            }
           
        }
        //单元值
        else if (loc == VariableLocation::VALOCEle)
        {
            vtkCellData* cd = ds->GetCellData();
            if (cd == nullptr) return variables;
            const int na = cd->GetNumberOfArrays();
            //遍历所有变量
            for (int i = 0; i < na; ++i)
            {
                QString va = QString(cd->GetArrayName(i));
                variables.append(va);
            }
        }
        return variables;
    }

    QStringList FITKAbstractStructuralPostVTK::getVariableComponents(VariableLocation loc, 
        const QString& variaable)
    {
        QStringList comps;
        vtkDataArray* array = nullptr;
        QByteArray ba = variaable.toLocal8Bit();
        vtkDataSet* ds = getOutput();
        if (ds == nullptr) return comps;
        //节点值
        if (loc == VariableLocation::VALOCNode)
        {
            vtkPointData* pd = ds->GetPointData();
            if (pd == nullptr) return comps;
            array = pd->GetArray(ba.data());
        }
        //单元值
        else if (loc == VariableLocation::VALOCEle)
        {
            vtkCellData* cd = ds->GetCellData();
            if (cd == nullptr) return comps;
            array = cd->GetArray(ba.data());
        }
        if (array == nullptr) return comps;
        //变量所有组分
        const int ncp = array->GetNumberOfComponents();
        //只有一个组分，是标量，返回空
        if (ncp == 1) return comps;

        if (ncp == 3)
        {
            QStringList names{ "X", "Y", "Z" };
            for (int i = 0; i < ncp; ++i)
            {
                QString name = array->GetComponentName(i);
                comps.append(name.isEmpty() ? names[i] : name);
            }
        }
        else
        {
            for (int i = 0; i < ncp; ++i)
            {
                QString name = array->GetComponentName(i);
                comps.append(name.isEmpty() ? QString("Component_%1").arg(i + 1) : name);
            }
        } 

        return comps;
    }

    void FITKAbstractStructuralPostVTK::getVariableRange(VariableLocation loc, const QString & varName, double * range, int componentIndex)
    {
        vtkDataSet* dataSet = getOutput();
        if (dataSet == nullptr) {
            return;
        }

        QByteArray name = varName.toLocal8Bit();
        char* cName = name.data();
        vtkDataArray* dataArray = nullptr;

        if (loc == VariableLocation::VALOCNode) {
            vtkPointData* pointData = dataSet->GetPointData();
            if (pointData == nullptr) {
                return;
            }
            dataArray = pointData->GetArray(cName);
        }
        else if (loc == VariableLocation::VALOCEle) {
            vtkCellData* cellData = dataSet->GetCellData();
            if (cellData == nullptr) {
                return;
            }
            dataArray = cellData->GetArray(cName);
        }

        if (dataArray == nullptr) {
            return;
        }

        dataArray->GetRange(range, componentIndex);
    }

    int FITKAbstractStructuralPostVTK::getNumberOfCells()
    {
        if (!_dataSetReader)
        {
            return 0;
        }

        vtkDataSet* dataSet = getOutput();

        /*_dataSetReader->Update();

        vtkDataSet* dataSet = nullptr;
        if (_type == StruPostFileType::SPFTVTK) {
            vtkDataSetReader* dataReader = vtkDataSetReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return 0;
            }
            dataSet = dataReader->GetOutput();
        }
        else if (_type == StruPostFileType::SPFTVTP) {
            vtkXMLPolyDataReader* dataReader = vtkXMLPolyDataReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return 0;
            }
            dataSet = dataReader->GetOutput();
        }
        else if (_type == StruPostFileType::SPFTVTU) {
            vtkXMLUnstructuredGridReader* dataReader = vtkXMLUnstructuredGridReader::SafeDownCast(_dataSetReader);
            if (dataReader == nullptr) {
                return 0;
            }
            dataSet = dataReader->GetOutput();
        }
      */

        if (!dataSet)
        {
            return 0;
        }

        return dataSet->GetNumberOfCells();
    }

    void FITKAbstractStructuralPostVTK::nextFrame()
    {

    }

    void FITKAbstractStructuralPostVTK::previousFrame()
    {

    }

    void FITKAbstractStructuralPostVTK::firstFrame()
    {

    }

    void FITKAbstractStructuralPostVTK::lastFrame()
    {

    }

    void FITKAbstractStructuralPostVTK::setCurFrame(int curFrame)
    {
        Q_UNUSED(curFrame);
    }

    int FITKAbstractStructuralPostVTK::frameNum()
    {
        return 0;
    }

    int FITKAbstractStructuralPostVTK::getCurFrame()
    {
        return 0;
    }
  
    void FITKAbstractStructuralPostVTK::enableDeform(bool enable)
    {
        double factor = _unDeformFactor;
        _enableDeform = enable;
        //变形为不变形与变形系数的和
        if (_enableDeform )
            factor += _deformFactor;

        if (_deformFilter)
        {
            _deformFilter->SetScaleFactor(factor);
            _deformFilter->Update();
        }
        _enableDeform = enable;
    }

    void FITKAbstractStructuralPostVTK::setDeformFactor(const double fac)
    {
        _deformFactor = fac;
        if (_enableDeform && _deformFilter)
        {
            double cd = _deformFilter->GetScaleFactor();
            double d = _unDeformFactor + _deformFactor;
            //变形一致，不进行更新操作
            if (fabs(d - cd) < 1e-9) return;
            _deformFilter->SetScaleFactor(d);
            _deformFilter->Update();
        }
    }

    double FITKAbstractStructuralPostVTK::getDeformFactor()
    {
        return _deformFactor;
    }

    void FITKAbstractStructuralPostVTK::setUnDeformFactor(const double fac)
    {
        _unDeformFactor = fac;
    }

    double FITKAbstractStructuralPostVTK::getUnDeformFactor()
    {
        return _unDeformFactor;
    }

    void FITKAbstractStructuralPostVTK::setDeformVariable(const QString& va)
    {
        if (_deformFilter == nullptr) return;
        //变量相同不进行操作
        QString cv = this->getDeformVariable();
        if (cv == va) return;
        QByteArray ba = va.toLocal8Bit();
        _deformFilter->SetDeformArrayName(ba.data());
        _deformFilter->Update();
    }

    QString FITKAbstractStructuralPostVTK::getDeformVariable()
    {
        return QString::fromLocal8Bit(_deformFilter->GetDeformArrayName());
    }

    Interface::FITKCutViewManager* FITKAbstractStructuralPostVTK::getCutViewManager()
    {
        if (_cutViewManager == nullptr)
            _cutViewManager = new FITKCutViewManager(this);
        return _cutViewManager;
    }

    Interface::FITKStruIsoPostDataManager* FITKAbstractStructuralPostVTK::getIsoViewManager() {
        if (_isoViewManager == nullptr) {
            _isoViewManager = new FITKStruIsoPostDataManager(this);
        }
        return _isoViewManager;
    }

    void FITKAbstractStructuralPostVTK::setShowCutWidget(bool visible)
    {
        _cutWidgetVisible = visible;
    }

    bool FITKAbstractStructuralPostVTK::getShowCutWidget()
    {
        return _cutWidgetVisible;
    }

    void FITKAbstractStructuralPostVTK::setShowViewCut(bool flag)
    {
        _cutViewVisible = flag;
    }

    bool FITKAbstractStructuralPostVTK::getShowViewCut()
    {
        return _cutViewVisible;
    }

    void FITKAbstractStructuralPostVTK::setEnableDisplayGroup(bool flag)
    {
        _enableDisplayGroup = flag;

        if (_enableDisplayGroup)
        {
            _deformFilter->SetInputConnection(_extractSelection->GetOutputPort());
        }
        else
        {
            _deformFilter->SetInputConnection(_idFilter->GetOutputPort());
        }
    }

    bool FITKAbstractStructuralPostVTK::getEnableDisplayGroup()
    {
        return _enableDisplayGroup;
    }

    void FITKAbstractStructuralPostVTK::setDisplayGroupEleIndice(const QList<int> & indice)
    {
        if (!_idArray)
        {
            setEnableDisplayGroup(false);
            return;
        }

        if (indice.count() == 0)
        {
            // 重置索引。
            resetDisplayGroup();
        }
        else
        {
            // 填充数据提取数组。
            _idArray->Reset();
            _idArray->SetNumberOfValues(0);

            for (const int & index : indice)
            {
                _idArray->InsertNextValue(index);
            }

            setEnableDisplayGroup(indice.count() != 0);
        }

        _selectNode->Modified();
    }

    QList<int> FITKAbstractStructuralPostVTK::getDisplayGroupEleIndice()
    {
        QList<int> indice;
        if (!_idArray)
        {
            return indice;
        }

        for (int i = 0; i < _idArray->GetNumberOfValues(); i++)
        {
            indice.push_back(_idArray->GetValue(i));
        }

        return indice;
    }

    int FITKAbstractStructuralPostVTK::getNumberOfDisplayGroupMember()
    {
        if (!_idArray)
        {
            return 0;
        }

        return _idArray->GetNumberOfValues();
    }

    QString FITKAbstractStructuralPostVTK::getDisplayGroupName()
    {
        return _groupName;
    }

    FITKPostGroupManager* FITKAbstractStructuralPostVTK::getDisplayGroupManager()
    {
        return m_displayGroupManager;
    }

    void FITKAbstractStructuralPostVTK::setDisplayGroupName(QString name)
    {
        _groupName = name;
    }
}

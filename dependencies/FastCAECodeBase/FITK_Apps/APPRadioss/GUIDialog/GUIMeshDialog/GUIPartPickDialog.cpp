/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPartPickDialog.h"
#include "ui_GUIPartPickDialog.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentCreator.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceNode.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodeGroup.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossElementGroup.h"
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/PickedDataProvider.h"
#include "PickDataProvider/PickedData.h"

#include <QMessageBox>
#include <QDebug>

namespace GUI {
    GUIPartPickDialog::GUIPartPickDialog(Core::FITKActionOperator* oper, QWidget* parent, GUIPickInfo::PickObjType pickType):
        GUIDialogBase(nullptr, parent),_oper(oper),_ui(new Ui::GUIPartPickDialog)
    {
        _pickType = pickType;
        _ui->setupUi(this);
        //设置对话框关闭自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //获取拾取器，绑定拾取结束信号
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        //清除拾取对象所有连接
        if (pickProvider != nullptr)
        {
            QObject::disconnect(pickProvider, SIGNAL(sigDataPicked()), nullptr, nullptr);
            connect(pickProvider, &GraphData::PickedDataProvider::sigDataPicked, this, &GUIPartPickDialog::pickFinishedOper);
        }
        connect(_ui->comboBox_PointMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePickStatus()));
        connect(_ui->comboBox_EleMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePickStatus()));
        this->init();
    }

    GUIPartPickDialog::~GUIPartPickDialog()
    {
        // 断开所有信号连接，防止悬挂指针
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        if (pickProvider != nullptr)
        {
            QObject::disconnect(pickProvider, &GraphData::PickedDataProvider::sigDataPicked, this, &GUIPartPickDialog::pickFinishedOper);
            pickProvider->clearPickedData();
        }
        _pickType = GUIPickInfo::PickObjType::POBNone;
        updatePickStatus();
       
        // 清理UI资源
        if (_ui != nullptr)
        {
            delete _ui;
            _ui = nullptr;
        }
    }
    void GUIPartPickDialog::init()
    {
        if (!_oper)return;
        bool isSingle = false;
        _oper->argValue<bool>("IsSingle", isSingle);
        _ui->comboBox_PointMethod->setEnabled(!isSingle);//如果拾取单点集合，false拾取方式
        _oper->removeArgs("IsSingle");

        _ui->lineEdit_Point->setEnabled(false);
        _ui->lineEdit_Element->setEnabled(false);
        _ui->spinBox_EleByAngle->setVisible(false);
        _ui->spinBox_PointByAngle->setVisible(false);

        auto comMgr = getComponentMgr();
        if (!comMgr)return;
        switch (_pickType)
        {
        case GUI::GUIPickInfo::POBNone:
            break;
        case GUI::GUIPickInfo::POBMeshVert:
        case GUI::GUIPickInfo::POBMeshCell:
        {
            _ui->lineEditName_Point->setText(comMgr->checkName("Set-1"));
            _ui->lineEditName_Element->setText(comMgr->checkName("Set-1"));
            break;
        }
        case GUI::GUIPickInfo::POBMeshVertSurf:
        case GUI::GUIPickInfo::POBMeshCellSurf:
        {
            _ui->lineEditName_Point->setText(comMgr->checkName("Surf-1"));
            _ui->lineEditName_Element->setText(comMgr->checkName("Surf-1"));
            break;
        }
        default:
            break;
        }
        setupUIForPickType();
        updatePickStatus();
    }
    void GUIPartPickDialog::setupUIForPickType()
    {
        switch (_pickType) {
        case GUIPickInfo::PickObjType::POBMeshVert:
        case GUIPickInfo::PickObjType::POBMeshVertSurf:
            _ui->stackedWidget->setCurrentIndex(0);
            break;
        case GUIPickInfo::PickObjType::POBMeshCell:
        case GUIPickInfo::PickObjType::POBMeshCellSurf:
            _ui->stackedWidget->setCurrentIndex(1);
            break;
        default:
            break;
        }
    }
    void GUIPartPickDialog::pickFinishedOper()
    {
        GraphData::PickedDataProvider* provider = GraphData::PickedDataProvider::getInstance();
        if (provider == nullptr) return;
        QList<GraphData::PickedData*> pickList = provider->getPickedList();
        int count = 0;

        switch (_pickType)
        {
        case GUIPickInfo::PickObjType::POBMeshVert:
        case GUIPickInfo::PickObjType::POBMeshVertSurf:
        {
            QSet<int> uniqueIds;
            for (GraphData::PickedData* pickData : pickList)
            {
                if (pickData == nullptr) continue;
                QList<GraphData::PickDataInfo> pickInfo = pickData->getPickOtherInfos();
                for (const GraphData::PickDataInfo& info : pickInfo)
                {
                    uniqueIds.insert(info._id);
                }
            }
            count = uniqueIds.size();
            break;
        }
        case GUIPickInfo::PickObjType::POBMeshCellSurf:
        {
            for (GraphData::PickedData* pickData : pickList)
            {
                if (pickData == nullptr) continue;
                QList<GraphData::PickDataInfo> pickInfo = pickData->getPickOtherInfos();
                for (auto iter = pickInfo.constBegin(); iter != pickInfo.constEnd(); ++iter)
                {
                    count += iter->_otherInfo.size();
                }
            }
            break;
        }
        default:
        {
            for (GraphData::PickedData* pickData : pickList)
            {
                if (pickData == nullptr) continue;
                QList<GraphData::PickDataInfo> pickInfo = pickData->getPickOtherInfos();
                count += pickInfo.size();
            }
            break;
        }
        }
        updatePickCount(count);
    }
    void GUIPartPickDialog::updatePickStatus()
    {
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        if (pickProvider != nullptr){
            pickProvider->clearPickedData();
        }
        // 获取当前拾取信息并设置
        GUI::GUIPickInfoStru pInfo = getCurrentPickInfo();
        GUI::GUIPickInfo::SetPickInfo(pInfo);
       
    }
    GUIPickInfoStru GUIPartPickDialog::getCurrentPickInfo() const
    {
        GUIPickInfoStru pInfo;

        if (!_pickType) {
            pInfo._pickObjType = GUIPickInfo::PickObjType::POBNone;
            pInfo._pickMethod = GUIPickInfo::PickMethod::PMNone;
            return pInfo;
        }

        // 设置拾取对象类型
        pInfo._pickObjType = _pickType;

        // 根据UI控件状态设置拾取方法
        bool useAngleMethod = false;
        QDoubleSpinBox* angleSpinBox = nullptr;

        switch (_pickType) {
        case GUIPickInfo::PickObjType::POBMeshVert:
        case GUIPickInfo::PickObjType::POBMeshVertSurf:
            if (_ui && _ui->comboBox_PointMethod) {
                useAngleMethod = (_ui->comboBox_PointMethod->currentIndex() != 0);
                angleSpinBox = _ui->spinBox_PointByAngle;
            }
            break;
        case GUIPickInfo::PickObjType::POBMeshCell:
        case GUIPickInfo::PickObjType::POBMeshCellSurf:
            if (_ui && _ui->comboBox_EleMethod) {
                useAngleMethod = (_ui->comboBox_EleMethod->currentIndex() != 0);
                angleSpinBox = _ui->spinBox_EleByAngle;
            }
            break;
        default:
            break;
        }

        if (useAngleMethod && angleSpinBox) {
            pInfo._pickMethod = GUIPickInfo::PickMethod::PMByAngle;
            pInfo._pickParam = angleSpinBox->value();
            angleSpinBox->setVisible(true);
        }
        else {
            pInfo._pickMethod = GUIPickInfo::PickMethod::PMIndividually;
            if (angleSpinBox) {
                angleSpinBox->setVisible(false);
            }
        }

        return pInfo;
    }
    void GUIPartPickDialog::updatePickCount(int count)
    {
        if (!_ui) return;

        switch (_pickType) {
        case GUIPickInfo::PickObjType::POBMeshVert:
        case GUIPickInfo::PickObjType::POBMeshVertSurf:
            if (_ui->lineEdit_Point) {
                _ui->lineEdit_Point->setText(QString::number(count));
            }
            break;
        case GUIPickInfo::PickObjType::POBMeshCell:
        case GUIPickInfo::PickObjType::POBMeshCellSurf:
            if (_ui->lineEdit_Element) {
                _ui->lineEdit_Element->setText(QString::number(count));
            }
            break;
        default:
            break;
        }
    }

    void GUIPartPickDialog::on_pushButton_OK_clicked()
    {
        // 获取拾取的数据
        GraphData::PickedDataProvider* provider = GraphData::PickedDataProvider::getInstance();
        if (provider == nullptr) {
            this->reject();
            return;
        }

        QList<GraphData::PickedData*> pickList = provider->getPickedList();
        if (pickList.isEmpty()) {
            this->reject();
            return;
        }

        auto comMgr = getComponentMgr();
        if (!comMgr)return;

        // 确定集合类型和基础名称
        bool success = false;
        switch (_pickType) {
        case GUIPickInfo::POBMeshVert:
        case GUIPickInfo::POBMeshCell: {
            QString name = _ui->lineEditName_Point->text();
            if (name.isEmpty() || comMgr->getDataByName(name)) {
                QMessageBox::warning(this, "Warning", "The name already exists!");
                return;
            }
            success = createSet(pickList, comMgr, name);
            break;
        }
        case GUIPickInfo::POBMeshVertSurf:
        case GUIPickInfo::POBMeshCellSurf: {
            QString name = _ui->lineEditName_Element->text();
            if (name.isEmpty() || comMgr->getDataByName(name)) {
                QMessageBox::warning(this, "Warning", "The name already exists!");
                return;
            }
            success = createSurf(pickList, comMgr, name);
            break;
        }
        default:
            this->reject();
            return;
        }
        if (success) {
            this->accept();
            _oper->execProfession();
        }
        else {
            this->reject();
        }
    }

    bool GUIPartPickDialog::createSet(const QList<GraphData::PickedData*>& pickList, Interface::FITKComponentManager* comMgr, const QString& name)
    {
        const auto getElementGroupTypeByElement = [](Interface::FITKAbstractElement* element)
        {
            if (element == nullptr)
            {
                return Radioss::FITKRadiossElementGroup::GRNone;
            }

            if (element->getElementDim() == 3)
            {
                return Radioss::FITKRadiossElementGroup::GRBRIC;
            }

            if (element->getElementDim() == 2)
            {
                if (element->getNodeCount() == 3)
                {
                    return Radioss::FITKRadiossElementGroup::GRSH3N;
                }
                if (element->getNodeCount() == 4)
                {
                    return Radioss::FITKRadiossElementGroup::GRSHEL;
                }
            }

            return Radioss::FITKRadiossElementGroup::GRNone;
        };

        // 获取必要的数据管理器
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr)  return nullptr;

        Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
        if (meshModel == nullptr)  return nullptr;

        Radioss::FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return false;

        Interface::FITKModelSet* set = nullptr;
        // 确定集合类型
        switch (_pickType) {
        case GUIPickInfo::POBMeshVert:
            set = new Radioss::FITKRadiossNodeGroup();
            break;
        case GUIPickInfo::POBMeshCell: {
            set = new Radioss::FITKRadiossElementGroup();
            break;
        }
        case GUIPickInfo::POBMeshVertSurf:
            set = new Interface::FITKModelSet();
            break;
        default:
            return false;
        }
        // 遍历拾取数据创建集合
        set->setModel(meshModel->getDataObjectID());
        set->setInternal(false);
        for (GraphData::PickedData* pickData : pickList) {
            if (pickData == nullptr) continue;
            QList<GraphData::PickDataInfo> pickInfos = pickData->getPickOtherInfos();
            if (pickInfos.isEmpty()) continue;

            // 添加成员
            for (const GraphData::PickDataInfo& info : pickInfos) {
                set->appendMember(info._id);
            }
        }

        Radioss::FITKRadiossElementGroup* radSet = dynamic_cast<Radioss::FITKRadiossElementGroup*>(set);
        if (radSet != nullptr)
        {
            const int firstElementID = radSet->getFirstMemberID();
            if (firstElementID < 0)
            {
                delete set;
                return false;
            }

            Interface::FITKAbstractElement* firstElement = partMgr->getElementByID(firstElementID).second;
            const Radioss::FITKRadiossElementGroup::ElementGroupType groupType =
                getElementGroupTypeByElement(firstElement);
            if (groupType == Radioss::FITKRadiossElementGroup::GRNone)
            {
                delete set;
                return false;
            }

            radSet->setElementGroupType(groupType);
        }

        // 设置名称
        QString setName = comMgr->checkName(name);
        set->setDataObjectName(setName);
        comMgr->appendDataObj(set);
        _oper->setArgs("SetId", set->getDataObjectID());
        writePythonScript(set);
        return true;
    }

    bool GUIPartPickDialog::createSurf(const QList<GraphData::PickedData*>& pickList, Interface::FITKComponentManager* comMgr, const QString& name)
    {
        if (_pickType == GUIPickInfo::POBMeshVertSurf)
        {

            Interface::FITKMeshSurfaceNode* surfaceNode = new Interface::FITKMeshSurfaceNode();
            surfaceNode->setModel(comMgr->getParentDataID());

            for (GraphData::PickedData* pickData : pickList) {
                if (pickData == nullptr) continue;

                QList<GraphData::PickDataInfo> pickInfos = pickData->getPickOtherInfos();
                if (pickInfos.isEmpty()) continue;
                int modelID = pickData->getPickedDataObjId();
                Radioss::FITKRadiossNodeGroup* set = new Radioss::FITKRadiossNodeGroup();
                set->setInternal(true);
                set->setModel(modelID);
                // 添加成员
                for (const GraphData::PickDataInfo& info : pickInfos) {
                    set->appendMember(info._id);
                }
                QString setName = comMgr->checkName("Set_" + QString::number(modelID));
                set->setDataObjectName(setName);
                comMgr->appendDataObj(set);
                surfaceNode->addMeshSet(comMgr->getParentDataID(), set->getDataObjectID(), 1.0);
            }
            surfaceNode->setDataObjectName(comMgr->checkName(name));
            comMgr->appendDataObj(surfaceNode);
            _oper->setArgs("SurfId", surfaceNode->getDataObjectID());
        }
        else {
            Radioss::FITKRadiossSurfaceSeg* surfaceSeg = new Radioss::FITKRadiossSurfaceSeg();
            surfaceSeg->setModel(comMgr->getParentDataID());

            // 遍历所有拾取数据
            for (GraphData::PickedData* pickData : pickList) {
                if (pickData == nullptr) continue;

                int partID = pickData->getPickedDataObjId();
                Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(partID);
                if (part == nullptr) continue;

                // 从拾取数据中提取表面信息
                QHash<int, QList<int>> surfaceInfo;
                if (!GetSurfInfo(pickData, surfaceInfo, part)) continue;

                // 遍历提取出的表面信息并添加到surfaceSeg
                auto it = surfaceInfo.constBegin();
                while (it != surfaceInfo.constEnd()) {
                    const int& eleID = it.key();
                    const QList<int>& faceIndexs = it.value();

                    for (int faceIndex : faceIndexs) {
                        surfaceSeg->addElementSurface(eleID, faceIndex);
                    }
                    ++it;
                }
            }

            if (surfaceSeg->getAllElement().isEmpty()) {
                delete surfaceSeg;
                surfaceSeg = nullptr;
                return false;
            }

            surfaceSeg->setDataObjectName(comMgr->checkName(name));
            comMgr->appendDataObj(surfaceSeg);
            _oper->setArgs("SurfId", surfaceSeg->getDataObjectID());
            return true;
        }
    }

    Interface::FITKComponentManager * GUIPartPickDialog::getComponentMgr()
    {
        // 获取必要的数据管理器
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr)  return nullptr;

        Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
        if (meshModel == nullptr)  return nullptr;
        Interface::FITKComponentManager* comMgr = meshModel->getComponentManager();
        comMgr->setParentDataID(meshModel->getDataObjectID());
        return comMgr;
    }

    //从拾取数据中提取表面信息
    bool GUIPartPickDialog::GetSurfInfo(GraphData::PickedData* pickData, QHash<int, QList<int>>& info, Radioss::FITKRadiossPart* part)
    {
        if (pickData == nullptr || part == nullptr) return false;

        // 获取面信息映射
        QList<GraphData::PickDataInfo> pickInfos = pickData->getPickOtherInfos();

        if (pickInfos.isEmpty()) return false;
        // 遍历拾取的单元
        for (const GraphData::PickDataInfo& pickInfo : pickInfos) {
            //获取单元ID和单元的表面索引
            int eleIndex = pickInfo._id;
            QList<int> faceIndexs = pickInfo._otherInfo;
            // 获取单元对象
            Interface::FITKAbstractElement* element = part->getElementAt(eleIndex);
            if (element == nullptr) continue;
            int eleID = element->getEleID();

            // 获取单元类型
            Interface::FITKModelEnum::FITKEleType eleType = element->getEleType();

            // 存储单元类型和面索引的组合
            for (int faceIndex : faceIndexs) {
                if (faceIndex >= 0) {
                    faceIndex = getSurfaceInxde(faceIndex, eleType);
                    if (!info.contains(eleID))
                        info.insert(eleID, QList<int>());
                    info[eleID].append(faceIndex);
                }
            }
        }
        
        return !info.isEmpty();
    }
    void GUIPartPickDialog::on_pushButton_Cancel_clicked()
    {
        // 取消按钮点击处理
        this->close();
    }

    void GUIPartPickDialog::on_spinBox_EleByAngle_valueChanged(double value)
    {
        Q_UNUSED(value);
        updatePickStatus();
    }

    void GUIPartPickDialog::on_spinBox_PointByAngle_valueChanged(double value)
    {
        Q_UNUSED(value);
        updatePickStatus();
    }
    int GUIPartPickDialog::getSurfaceInxde(int surfaceLabel, Interface::FITKModelEnum::FITKEleType type)
    {
        if(type == Interface::FITKModelEnum::FITKEleType::Tet4 ||
           type == Interface::FITKModelEnum::FITKEleType::Tet10)
        {
            switch (surfaceLabel)
            {
            case 0: return 1; break;
            case 1: return 2; break;
            case 2: return 3; break;
            case 3: return 0; break;
            }
        }
        else if(type == Interface::FITKModelEnum::FITKEleType::Hex8 ||
                type == Interface::FITKModelEnum::FITKEleType::Hex20)
        {
            switch (surfaceLabel)
            {
            case 0: return 5; break;
            case 1: return 3; break;
            case 2: return 2; break;
            case 3: return 4; break;
            case 4: return 0; break;
            case 5: return 1; break;
            }
        }
        else if(type == Interface::FITKModelEnum::FITKEleType::Tri3 ||
                 type == Interface::FITKModelEnum::FITKEleType::Tri6)
        {
            switch (surfaceLabel)
            {
            case 0: return 1; break;
            case 1: return 2; break;
            case 2: return 0; break;
            }
        }
        else if (type == Interface::FITKModelEnum::FITKEleType::Quad4 ||
                 type == Interface::FITKModelEnum::FITKEleType::Quad8)
        {
            switch (surfaceLabel)
            {
            case 0: return 1; break;
            case 1: return 2; break;
            case 2: return 3; break;
            case 3: return 0; break;
            }
        }
        switch (surfaceLabel)
        {
        case 0: return 1; break;
        case 1: return 2; break;
        case 2: return 3; break;
        case 3: return 4; break;
        case 4: return 5; break;
        case 5: return 0; break;
        }
    }
    void GUIPartPickDialog::writePythonScript(Interface::FITKAbstractModelComponent* component)
    {
        if (!component) return;
        QStringList pythonCmd;
        switch (_pickType)
        {
        case GUIPickInfo::POBMeshVert: {
            Radioss::FITKRadiossNodeGroup* set = dynamic_cast<Radioss::FITKRadiossNodeGroup*>(component);
            if (!set) return;
            QList<int> member = set->getAbsoluteMember();
            QStringList memberStrList;
            for (int id : member) {
                memberStrList.append(QString::number(id));
            }
            pythonCmd.append("modelSet=ModelSet()");
            pythonCmd.append(QString("modelSet.createModelSet(%1, [%2])").arg(0).arg(memberStrList.join(", ")));
            pythonCmd.append(QString("modelSet.setName(\"%1\")").arg(set->getDataObjectName()));
            break;
        }
        case GUIPickInfo::POBMeshCell: {
            Radioss::FITKRadiossElementGroup* set = dynamic_cast<Radioss::FITKRadiossElementGroup*>(component);
            if (!set) return;
            QList<int> member = set->getAbsoluteMember();
            QStringList memberStrList;
            for (int id : member) {
                memberStrList.append(QString::number(id));
            }
            pythonCmd.append("modelSet=ModelSet()");
            pythonCmd.append(QString("modelSet.createModelSet(%1, [%2])").arg(1).arg(memberStrList.join(", ")));
            pythonCmd.append(QString("modelSet.setName(\"%1\")").arg(set->getDataObjectName()));
            break;
        }
        case GUIPickInfo::POBMeshVertSurf:
        case GUIPickInfo::POBMeshCellSurf:
            return;
        default:
            return;
        }
        
        this->saveScript(pythonCmd);
    }
    QString GUIPartPickDialog::getMemberListString(const QList<GraphData::PickedData*>& pickList)
    {
        QList<int> memberIds;
        for (GraphData::PickedData* pickData : pickList) {
            if (pickData == nullptr) continue;
            QList<GraphData::PickDataInfo> pickInfos = pickData->getPickOtherInfos();
            for (const GraphData::PickDataInfo& info : pickInfos) {
                memberIds.append(info._id);
            }
        }
        QStringList memberStrList;
        for (int id : memberIds) {
            memberStrList.append(QString::number(id));
        }
        return QString("[%1]").arg(memberStrList.join(", "));
    }
}

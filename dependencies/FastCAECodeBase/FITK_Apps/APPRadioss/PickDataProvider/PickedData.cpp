/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PickedData.h"
#include "GUIPickInfo.h"

#include "GraphDataVTKAdaptor/GraphVTKObject3D.h"
#include "GraphDataVTKAdaptor/GraphVTKCommons.h"
#include "GraphDataVTKAdaptor/GraphDataVTKPartSetManager.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKKeyMouseStates.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor2D.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKShellFeatureEdges.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"

#include <vtkPlanes.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSet.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkMapper.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkIdTypeArray.h>
#include <QVector>

namespace GraphData
{
    PickedData::PickedData(GUI::GUIPickInfoStru pickedInfo, vtkActor* pickedActor, int pickedIndex, double* pickedWorldPos, bool isPreview) :
        m_pickedInfo(pickedInfo), m_pickedActor(pickedActor), m_pickedIndex(pickedIndex), m_isPreview(isPreview)
    {
        // 初始化鼠标操作方式。（点击）
        m_mouseOper = PickedMouseType::PickedMouseClick;

        if (pickedWorldPos)
        {
            m_pickedWorldPos[0] = pickedWorldPos[0];
            m_pickedWorldPos[1] = pickedWorldPos[1];
            m_pickedWorldPos[2] = pickedWorldPos[2];
        }

        // 初始化。
        init();
    }

    PickedData::PickedData(GUI::GUIPickInfoStru pickedInfo, vtkActor* pickedActor, vtkPlanes* cutPlane) :
        m_pickedInfo(pickedInfo), m_pickedActor(pickedActor), m_cutPlane(cutPlane)
    {
        // 初始化鼠标操作方式。（框选）
        m_mouseOper = PickedMouseType::PickedMouseRubber;

        // 初始化。
        init();
    }

    PickedData::PickedData(GUI::GUIPickInfoStru pickedInfo, vtkActor2D* pickedActor, bool isPreview) :
        m_pickedInfo(pickedInfo), m_pickedActor2D(pickedActor), m_isPreview(isPreview)
    {
        // 初始化鼠标操作方式。（点击）
        m_mouseOper = PickedMouseType::PickedMouseClick;

        // 初始化。
        init2D();
    }

    PickedData::PickedData()
    {

    }

    PickedData::~PickedData()
    {
        // 析构前恢复高亮（参考点、坐标轴）状态。
        clearHighlight();
    }

    PickedData* PickedData::getCopy()
    {
        if (m_needToCal || !m_isValid)
        {
            return nullptr;
        }

        PickedData* data = new PickedData;
        data->m_type = m_type;
        data->m_dataObjId = m_dataObjId;

        data->m_pickDataInfo = m_pickDataInfo;
        data->m_graphObject = m_graphObject;

        data->m_pickedActor = m_pickedActor;
        data->m_pickedActor2D = m_pickedActor2D;
        data->m_pickedIndex = m_pickedIndex;
        data->m_pickedInfo = m_pickedInfo;
        data->FaceMap = this->FaceMap;
        data->FaceMapInternal = this->FaceMapInternal;
        data->Ids = this->Ids;
        data->m_otherIds = this->m_otherIds;
        return data;
    }

    bool PickedData::isSameAs(PickedData* data)
    {
        if (!data)
        {
            return false;
        }

        if (data->m_type == m_type &&
            data->m_dataObjId == m_dataObjId)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void PickedData::sort()
    {
        //合并其他参数中同一id的数据
        QList<PickDataInfo> temp = m_pickDataInfo;
        m_pickDataInfo.clear();
        for (const PickDataInfo& info : temp) {
            bool isOk = false;
            QList<int>& otherInfos = getOtherInfo(info._id, isOk);
            if (isOk) {
                for (const int& otherInfo : info._otherInfo) {
                    if (otherInfos.contains(otherInfo)) {
                        continue;
                    }
                    otherInfos.append(otherInfo);
                }
            }
            else {
                m_pickDataInfo.append(info);
            }
        }
    }

    void PickedData::add(PickedData* data)
    {
        if (data == nullptr) {
            return;
        }

        //特殊合并，合并单元表面数据
        QList<int> myEleIds = {};
        for (PickDataInfo& myEleSurf : m_pickDataInfo) {
            myEleIds.append(myEleSurf._id);
        }
        for (PickDataInfo eleSurf : data->getPickOtherInfos()) {
            if (myEleIds.contains(eleSurf._id)) {
                bool isOk = false;
                QList<int>& myEleSurfIndexs = getOtherInfo(eleSurf._id, isOk);
                if (isOk == false) {
                    continue;
                }
                for (int eleSurfIndex : eleSurf._otherInfo) {
                    if (myEleSurfIndexs.contains(eleSurfIndex)) {
                        continue;
                    }
                    myEleSurfIndexs.append(eleSurfIndex);
                }
            }
            else {
                m_pickDataInfo << eleSurf;
            }
        }

        //额外拷贝后者键盘按下情况。
        m_keyFlagsHash = data->m_keyFlagsHash;

        // 合并 m_otherIds
        QHash<int, int> otherIds = data->getPickedOtherIDs();
        for (auto it = otherIds.constBegin(); it != otherIds.constEnd(); ++it) {
            m_otherIds.insert(it.key(), it.value());
        }

        // 合并后析构。
        delete data;
    }

    void PickedData::saveKeyFlags(Qt::Key qKey, bool flag)
    {
        m_keyFlagsHash[qKey] = flag;
    }

    void PickedData::subtract(PickedData* data)
    {
        if (data == nullptr) {
            return;
        }

        //特殊去除，去除单元表面数据
        QList<int> myEleIds = {};
        for (PickDataInfo& myEleSurf : m_pickDataInfo) {
            myEleIds.append(myEleSurf._id);
        }
        for (PickDataInfo eleSurf : data->getPickOtherInfos()) {
            if (myEleIds.contains(eleSurf._id)) {
                bool isOk = false;
                QList<int>& myEleSurfIndexs = getOtherInfo(eleSurf._id, isOk);
                if (isOk == false) {
                    continue;
                }
                for (int eleSurfIndex : eleSurf._otherInfo) {
                    myEleSurfIndexs.removeOne(eleSurfIndex);
                }

                //判断数据是否被去除为空，若为空移除该数据
                if (myEleSurfIndexs.isEmpty() == true) {
                    removeOtherInfo(eleSurf._id);
                }
            }
        }

        // 移除 m_otherIds
        QHash<int, int> otherIdsToRemove = data->getPickedOtherIDs();
        for (int key : otherIdsToRemove.keys()) {
            m_otherIds.remove(key);
        }

        // 合并后析构。
        delete data;
    }

    void PickedData::init()
    {
        // 尝试转换为自定义Actor。
        FITKGraphActor* fActor = FITKGraphActor::SafeDownCast(m_pickedActor);
        if (!fActor)
        {
            return;
        }

        // 获取演员存储的可视化对象。
        Exchange::GraphVTKObject3D* obj = fActor->getGraphObjectAs<Exchange::GraphVTKObject3D>();
        if (!obj)
        {
            return;
        }

        // 保存可视化对象。
        m_graphObject = obj;
        m_dataObjId = obj->getDataId();

        Exchange::GraphVTKCommons::ShapeInfo sInfo = obj->getShapeInfo();

        m_type = m_pickedInfo._pickObjType;

        m_dataObjId = sInfo.DataObjId;

        if (m_type == GUI::GUIPickInfo::PickObjType::POBNone) {
            return;
        }

        m_needToCal = true;
        m_isValid = true;
    }

    void PickedData::init2D()
    {
        // 尝试转换为自定义Actor。
        FITKGraphActor2D* fActor2D = FITKGraphActor2D::SafeDownCast(m_pickedActor2D);
        if (!fActor2D)
        {
            return;
        }

        // 获取演员存储的可视化对象。
        Exchange::GraphVTKObject3D* obj = fActor2D->getGraphObjectAs<Exchange::GraphVTKObject3D>();
        if (!obj)
        {
            return;
        }

        // 保存可视化对象。
        m_graphObject = obj;
        m_dataObjId = obj->getDataId();

        Exchange::GraphVTKCommons::ShapeInfo sInfo = obj->getShapeInfo();

        m_type = m_pickedInfo._pickObjType;

        m_dataObjId = sInfo.DataObjId;

        if (m_type == GUI::GUIPickInfo::PickObjType::POBNone) {
            return;
        }

        m_needToCal = false;
        m_isValid = true;
    }

    void PickedData::initManual()
    {
        // 通过操作器获取可视化对象。
        Core::FITKOperatorRepo* operatorRepo = Core::FITKOperatorRepo::getInstance();
        EventOper::GraphEventOperator* operPre = operatorRepo->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!operPre)
        {
            return;
        }

        m_graphObject = operPre->getModelGraphObjectByDataId(m_dataObjId);

        // 判断数据是否可用。
        m_needToCal = false;
        m_isValid = m_graphObject != nullptr;
    }

    void PickedData::setPickedGraphObject(Exchange::GraphVTKObject3D* obj)
    {
        if (!obj)
        {
            return;
        }

        // 断开旧数据信号。
        if (m_graphObject)
        {
            disconnect(m_graphObject, &Exchange::GraphVTKObject3D::destroyed, this, &PickedData::slot_resetGraphObject);
        }

        // 保存数据，连接信号。
        m_graphObject = obj;
        connect(m_graphObject, &Exchange::GraphVTKObject3D::destroyed, this, &PickedData::slot_resetGraphObject, Qt::UniqueConnection);
    }

    QList<int>& PickedData::getOtherInfo(int eleID, bool& isOk)
    {
        for (PickDataInfo& eleSurfData : m_pickDataInfo) {
            if (eleSurfData._id == eleID) {
                isOk = true;
                return eleSurfData._otherInfo;
            }
        }
        QList<int> temp;
        isOk = false;
        return temp;
    }

    void PickedData::removeOtherInfo(int eleID)
    {
        for (int i = 0; i < m_pickDataInfo.size(); i++) {
            if (m_pickDataInfo[i]._id == eleID) {
                m_pickDataInfo.removeAt(i);
                break;
            }
        }
    }

    void PickedData::slot_resetGraphObject()
    {
        m_graphObject = nullptr;
    }

    void PickedData::highlight()
    {
        if (!m_graphObject)
        {
            return;
        }

        // 如果是预选并且没有在高亮则预选。
        if (m_isPreview)
        {
            if (!m_graphObject->isHighlighting())
            {
                m_graphObject->preHighlight();
            }
        }
        else
        {
            m_graphObject->highlight();
        }
    }

    void PickedData::clearHighlight()
    {
        // 取消高亮。
        if (!m_graphObject)
        {
            return;
        }

        // 如果是预选高亮参考点需要额外判断是否为高亮状态。
        if (m_isPreview)
        {
            // 如果为高亮状态则还原为高亮。
            if (m_graphObject->isHighlighting())
            {
                m_graphObject->highlight();
            }
            // 否则取消预选高亮。
            else
            {
                m_graphObject->disHighlight();
            }
        }
        // 高亮则直接取消高亮。
        else
        {
            m_graphObject->disHighlight();
        }
    }

    void PickedData::calculateFinsish()
    {
        m_needToCal = false;

        m_mouseOper = PickedMouseType::PickedNoneType;
        m_pickedIndex = -1;
        m_cutPlane = nullptr;

        m_pickedWorldPos[0] = 0;
        m_pickedWorldPos[1] = 0;
        m_pickedWorldPos[2] = 0;
    }

    bool PickedData::needToCalculate()
    {
        return m_needToCal;
    }

    bool PickedData::isValid()
    {
        return m_isValid;
    }

    bool PickedData::isEmpty()
    {
        // 如果数据对象ID为空则一定为空拾取数据。
        if (m_dataObjId < 0)
        {
            return true;
        }

        bool isEmpty = m_pickDataInfo.isEmpty();
        return isEmpty;
    }

    bool PickedData::isFinished()
    {
        // 单选情况下数据不为空则直接视为拾取结束。
        if (m_pickedInfo._pickMethod == GUI::GUIPickInfo::PMSingle)
        {
            return true;
        }
        // 多选模式均通过键盘按下事件进行判断拾取结束。
        else
        {
            if (m_keyFlagsHash[Qt::Key::Key_Shift] && m_keyFlagsHash[Qt::Key::Key_Control])
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    bool PickedData::contains(vtkActor* actor, int index)
    {
        if (!actor || index < 0 || !m_graphObject)
        {
            return false;
        }

        // 首先判断是否为同一演员，参考点直接返回。
        if (actor != m_pickedActor)
        {
            return false;
        }

        int id = -1;

        switch (m_pickedInfo._pickObjType) {
        case GUI::GUIPickInfo::PickObjType::POBGeoVert:
            id = m_graphObject->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickGeoVertex);
            break;
        case GUI::GUIPickInfo::PickObjType::POBGeoEdge:
            id = m_graphObject->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickGeoEdge);
            break;
        case GUI::GUIPickInfo::PickObjType::POBGeoFace:
            id = m_graphObject->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickGeoFace);
            break;
        case GUI::GUIPickInfo::PickObjType::POBGeoSolid:
            id = m_graphObject->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickGeoSolid);
            break;
        default:
            return false;
        }

        if (id == -1)
        {
            return false;
        }

        bool isOk = false;
        getOtherInfo(id, isOk);
        return isOk;
    }

    bool PickedData::contains(vtkActor2D* actor)
    {
        if (!actor || !m_graphObject)
        {
            return false;
        }

        // 新版本
        //@{
        return m_graphObject->contains(actor);
        //@}
    }

    GUI::GUIPickInfoStru PickedData::getPickedInfo()
    {
        return m_pickedInfo;
    }

    vtkActor* PickedData::getPickedActor()
    {
        return m_pickedActor;
    }

    int PickedData::getPickedIndex()
    {
        return m_pickedIndex;
    }

    PickedMouseType PickedData::getPickedMouseType()
    {
        return m_mouseOper;
    }

    vtkPlanes* PickedData::getCutPlane()
    {
        return m_cutPlane;
    }

    GUI::GUIPickInfo::PickObjType PickedData::getPickedDataType()
    {
        return m_type;
    }

    QList<PickDataInfo> PickedData::getPickOtherInfos()
    {
        return m_pickDataInfo;
    }

    void PickedData::addPickOtherInfo(PickDataInfo info)
    {
        m_pickDataInfo.append(info);
    }

    int PickedData::getPickedDataObjId()
    {
        return m_dataObjId;
    }

    void PickedData::appendPickedOtherId(int pickId, int pickOtherId)
    {
        m_otherIds.insert(pickId, pickOtherId);
    }

    QHash<int, int> PickedData::getPickedOtherIDs()
    {
        return m_otherIds;
    }

    Exchange::GraphVTKObject3D* PickedData::getPickedGraphObejct()
    {
        return m_graphObject;
    }

    void PickedData::getPartMeshEleNodeSelection(vtkUnstructuredGrid* ugrid)
    {
        //获取部件数据，为空跳出
        Exchange::GraphVTKObject3D* gobj = m_graphObject;
        Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(m_dataObjId);
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
        Radioss::FITKRadiossNodes* nodes = meshModel->getNodes();
        if (!part || !gobj) {
            return;
        }
        Exchange::PartMeshDataVTK* partData{ nullptr };
        bool isTrue = Exchange::GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!isTrue || !partData) {
            return;
        }
        //通过ID获取实际的VTK索引
        vtkIdTypeArray* idArrag = vtkIdTypeArray::New();
        vtkSelectionNode* selectNode = vtkSelectionNode::New();
        vtkExtractSelection* extractSelection = vtkExtractSelection::New();
        switch (m_pickedInfo._pickObjType)
        {
            //节点直接提取
        case GUI::GUIPickInfo::PickObjType::POBMeshVert:
        case GUI::GUIPickInfo::PickObjType::POBMeshVertSurf:
        {
            QList<PickDataInfo> infos = m_pickDataInfo;
            for (const PickDataInfo& infos : infos) {
                int id = infos._id;
                QVector<int> indexs = gobj->getVTKCellIdsByShapeId(id, Exchange::GraphVTKCommons::ShapePickMode::PickMeshVertex);
                for (int index : indexs)
                {
                    idArrag->InsertNextValue(index);
                }
                
            }

            selectNode->SetFieldType(vtkSelectionNode::SelectionField::POINT);
            extractSelection->SetInputData(partData->_meshData);
            break;

        }

        //整个单元直接提取
        case GUI::GUIPickInfo::PickObjType::POBMeshCell:
        case GUI::GUIPickInfo::PickObjType::POBMeshCellSurf:
        {
            QList<PickDataInfo> infos = m_pickDataInfo;
            for (const PickDataInfo& infos : infos) {
                int id = infos._id;
                int index = part->getEleIndexByID(id);
                idArrag->InsertNextValue(index);
            }
            selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
            extractSelection->SetInputData(partData->_meshData);

        }
        default:
            break;
        }
        //提取数据
        vtkSelection* selection = vtkSelection::New();
        selectNode->SetContentType(vtkSelectionNode::INDICES);
        selection->AddNode(selectNode);
        extractSelection->SetInputData(1, selection);
        selectNode->SetSelectionList(idArrag);
        extractSelection->Update();
        //数据拷贝
        ugrid->DeepCopy(vtkDataSet::SafeDownCast(extractSelection->GetOutput()));

        //释放
        idArrag->Delete();
        selectNode->Delete();
        extractSelection->Delete();
        selection->Delete();

    }

    void PickedData::getPartMeshEleSurfSelection(vtkUnstructuredGrid* ugrid)
    {
        //获取部件数据，为空跳出
        Exchange::GraphVTKObject3D* gobj = m_graphObject;
        Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(m_dataObjId);
        if (!part || !gobj) {
            return;
        }
        Exchange::PartMeshDataVTK* partData{ nullptr };
        bool isTrue = Exchange::GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!isTrue || !partData) {
            return;
        }
        vtkDataSet* meshData = partData->_meshData;
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkUnstructuredGrid> mesh = vtkUnstructuredGrid::SafeDownCast(meshData);
        points->DeepCopy(mesh->GetPoints());
        ugrid->SetPoints(points);
        //获取单元表面信息
        QList<PickDataInfo> infos = m_pickDataInfo;
        for (PickDataInfo info : infos) {
            int cellIndex = info._id;
            QList<int> faceIndexs = info._otherInfo;
            for (const int& faceIndex : faceIndexs) {
                if (faceIndex == -1)//特征边
                {

                }
                else
                {
                    vtkCell* fullCell = meshData->GetCell(cellIndex);
                    vtkCell* faceCell = fullCell->GetFace(faceIndex);
                    //体单元表面
                    if (faceCell)
                    {
                        vtkIdList* faceCellPointIds = faceCell->GetPointIds();
                        vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
                        for (int j = 0; j < faceCellPointIds->GetNumberOfIds(); j++) {
                            cell->InsertNextId(faceCellPointIds->GetId(j));
                        }
                        ugrid->InsertNextCell(faceCell->GetCellType(), cell);
                    }
                    else //壳单元表面
                    {
                        vtkIdList* fullCellPointIds = fullCell->GetPointIds();
                        vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
                        for (int j = 0; j < fullCellPointIds->GetNumberOfIds(); j++) {
                            cell->InsertNextId(fullCellPointIds->GetId(j));
                        }
                        ugrid->InsertNextCell(fullCell->GetCellType(), cell);
                    }
                }
            }
        }
    }

    void PickedData::getDataSet(vtkUnstructuredGrid* ugrid)
    {
        // 未计算过的数据直接跳出。
        if (m_needToCal || !ugrid || !m_graphObject)
        {
            return;
        }
        vtkDataSet* dataSet{ nullptr };
        //ui拾取类型与渲染拾取类型映射
        Exchange::GraphVTKCommons::ShapePickMode pickMode = Exchange::GraphVTKCommons::PickNone;
        switch (m_type) {
        case GUI::GUIPickInfo::PickObjType::POBGeoVert: {
            pickMode = Exchange::GraphVTKCommons::ShapePickMode::PickGeoVertex;
            dataSet = m_graphObject->getMesh(pickMode);
            break;
        }
        case GUI::GUIPickInfo::PickObjType::POBGeoEdge: {
            pickMode = Exchange::GraphVTKCommons::ShapePickMode::PickGeoEdge;
            dataSet = m_graphObject->getMesh(pickMode);
            break;
        }
        case GUI::GUIPickInfo::PickObjType::POBGeoFace: {
            pickMode = Exchange::GraphVTKCommons::ShapePickMode::PickGeoFace;
            dataSet = m_graphObject->getMesh(pickMode);
            break;
        }
        case GUI::GUIPickInfo::PickObjType::POBGeoSolid: {
            pickMode = Exchange::GraphVTKCommons::ShapePickMode::PickGeoSolid;
            dataSet = m_graphObject->getMesh(pickMode);
            break;
        }
        case GUI::GUIPickInfo::PickObjType::POBMeshVert:
        case GUI::GUIPickInfo::PickObjType::POBMeshCell: 
        case GUI::GUIPickInfo::PickObjType::POBMeshVertSurf: {
            getPartMeshEleNodeSelection(ugrid);
            break;
        }
        case GUI::GUIPickInfo::PickObjType::POBMeshCellSurf: {
            getPartMeshEleSurfSelection(ugrid);
            break;
        }
        default:
            return;
        }

        if (dataSet == nullptr) {
            return;
        }

        int nCell = dataSet->GetNumberOfCells();

        // 创建ID数组，加速合并数据。
        vtkIntArray* idArray = vtkIntArray::New();
        idArray->SetNumberOfComponents(1);
        idArray->SetNumberOfValues(nCell);
        idArray->FillComponent(0, 0);

        //根据形状id获取VTK数据id
        for (const PickDataInfo& info : m_pickDataInfo) {
            const QVector<int> subCellIds = m_graphObject->getVTKCellIdsByShapeId(info._id, pickMode, QVariant::fromValue(info._otherInfo));
            for (const int& cId : subCellIds)
            {
                idArray->SetValue(cId, 1);
            }
        }

        // 通过ID获取实际VTK索引。
        vtkIdTypeArray* selectIdArray = vtkIdTypeArray::New();
        vtkSelectionNode* selectNode = vtkSelectionNode::New();
        vtkExtractSelection* extractSelection = vtkExtractSelection::New();

        for (int i = 0; i < nCell; i++)
        {
            if (idArray->GetValue(i) == 1)
            {
                selectIdArray->InsertNextValue(i);
            }
        }

        // 从原始网格数据提取。
        selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
        extractSelection->SetInputData(dataSet);

        // 提取VTK数据。
        //@{
        vtkSelection* section = vtkSelection::New();
        selectNode->SetContentType(vtkSelectionNode::INDICES);
        section->AddNode(selectNode);
        extractSelection->SetInputData(1, section);
        selectNode->SetSelectionList(selectIdArray);
        extractSelection->Update();
        //@}

        // 数据拷贝。
        ugrid->DeepCopy(extractSelection->GetOutput());

        // 析构。
        //@{
        idArray->Delete();
        selectNode->Delete();
        section->Delete();
        selectIdArray->Delete();
        extractSelection->Delete();
        //@}
    }

    void PickedData::getPickedWorldPosition(double* pos)
    {
        if (!pos)
        {
            return;
        }

        for (int i = 0; i < 3; i++)
        {
            pos[i] = m_pickedWorldPos[i];
        }
    }
}
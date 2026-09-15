/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PickedDataCalculator.h"
#include "PickedData.h"

#include "GraphDataVTKAdaptor/GraphVTKCommons.h"
#include "GraphDataVTKAdaptor/GraphVTKObject3D.h"
#include "GraphDataVTKAdaptor/GraphDataVTKPartSetManager.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKKeyMouseStates.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKShellFeatureEdges.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKExtractGeometry.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKPolyDataTool.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKMeshFeatureVTK.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include <vtkActor.h>
#include <vtkMapper.h>
#include <vtkPlanes.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDataSet.h>
namespace GraphData
{
    PickedDataCalculator::PickedDataCalculator(GraphData::PickedData* pickedData) :
        m_pickedData(pickedData)
    {

    }

    void PickedDataCalculator::calculate()
    {
        if (!m_pickedData)
        {
            return;
        }

        //点选拾取方式
        if (m_pickedData->getPickedMouseType() == PickedMouseType::PickedMouseClick) {
            // 获取拾取时的方式。
            GUI::GUIPickInfoStru pickInfo = m_pickedData->getPickedInfo();
            switch (pickInfo._pickMethod) {
            case GUI::GUIPickInfo::PickMethod::PMIndividually:
            case GUI::GUIPickInfo::PickMethod::PMSingle: {
                Individually();
                break;
            }
            case GUI::GUIPickInfo::PickMethod::PMByAngle: {
                // 处理角度算法拾取
                ByAngle();
                break;
            }
            default: return;
            }
        }
        //框选拾取方式
        else if (m_pickedData->getPickedMouseType() == PickedMouseType::PickedMouseRubber)
        {
            byAreaPick();
        }

        // 不管计算完成都将是否需要计算标识置为false。
        m_pickedData->calculateFinsish();
    }

    void PickedDataCalculator::Individually()
    {
        Exchange::GraphVTKObject3D* gobj = m_pickedData->getPickedGraphObejct();
        int index = m_pickedData->getPickedIndex();
        if (gobj == nullptr || index < 0) {
            return;
        }

        int id = -1;
        int vTopoIndex = -1;
        // 根据拾取数据类型进行不同数据获取。
        switch (m_pickedData->getPickedDataType()) {
        case GUI::GUIPickInfo::PickObjType::POBGeoVert:
            //查找几何点
            id = gobj->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickGeoVertex);
            vTopoIndex = gobj->getVirTopoIndexByShapeId(id, Exchange::GraphVTKCommons::ShapePickMode::PickGeoVertex);
            m_pickedData->addPickOtherInfo(PickDataInfo(id));
            m_pickedData->appendPickedOtherId(id, vTopoIndex);
            break;
        case GUI::GUIPickInfo::PickObjType::POBGeoEdge:
            //查找几何线
            id = gobj->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickGeoEdge);
            vTopoIndex = gobj->getVirTopoIndexByShapeId(id, Exchange::GraphVTKCommons::ShapePickMode::PickGeoEdge);
            m_pickedData->addPickOtherInfo(PickDataInfo(id));
            m_pickedData->appendPickedOtherId(id, vTopoIndex);
            break;
        case GUI::GUIPickInfo::PickObjType::POBGeoFace:
            //查找几何面
            id = gobj->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickGeoFace);
            vTopoIndex = gobj->getVirTopoIndexByShapeId(id, Exchange::GraphVTKCommons::ShapePickMode::PickGeoFace);
            m_pickedData->addPickOtherInfo(PickDataInfo(id));
            m_pickedData->appendPickedOtherId(id, vTopoIndex);
            break;
        case GUI::GUIPickInfo::PickObjType::POBGeoSolid:
            //查找几何体
            id = gobj->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickGeoSolid);
            vTopoIndex = gobj->getVirTopoIndexByShapeId(id, Exchange::GraphVTKCommons::ShapePickMode::PickGeoSolid);
            m_pickedData->addPickOtherInfo(PickDataInfo(id));
            m_pickedData->appendPickedOtherId(id, vTopoIndex);
            break;
        case GUI::GUIPickInfo::PickObjType::POBMeshVert:
            id = gobj->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickMeshVertex);
            m_pickedData->addPickOtherInfo(PickDataInfo(id));
            break;
        case GUI::GUIPickInfo::PickObjType::POBMeshVertSurf:
            //查找网格表面点
            id = gobj->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickMeshVertexSurf);
            m_pickedData->addPickOtherInfo(PickDataInfo(id));
            break;
        case GUI::GUIPickInfo::PickObjType::POBMeshCell:
            //查找网格单元
            id = gobj->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::ShapePickMode::PickMeshCell);
            m_pickedData->addPickOtherInfo(PickDataInfo(id));
            break;
        case GUI::GUIPickInfo::PickObjType::POBMeshCellSurf:
            IndividuallyByMeshCellSurf();
            break;
        default:
            break;
        }
    }

    void PickedDataCalculator::IndividuallyByMeshCellSurf()
    {
        if (m_pickedData == nullptr) return;
        //获取GraphVTKObjectPartMesh
        Exchange::GraphVTKObject3D* gobj = m_pickedData->getPickedGraphObejct();
        Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(m_pickedData->getPickedDataObjId());
        Exchange::PartMeshDataVTK* partData{ nullptr };
        int isTrue = Exchange::GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!isTrue || partData == nullptr) return;
        int index = m_pickedData->getPickedIndex();
        if (index < 0) return;

        Interface::FITKMeshFeatureVTK* meshFeature = partData->_meshFeature;
        if (meshFeature == nullptr) return;
        //获取表面提取过滤器
        FITKSurfaceFilter* surfaceFilter = meshFeature->getSurfaceFilter();
        int orgCellIndex = -1;
        int faceIndex = -1;
        surfaceFilter->getSurfaceCellInfo(index, orgCellIndex, faceIndex);
        PickDataInfo otherInfo;
        otherInfo._id = orgCellIndex;
        otherInfo._otherInfo.append(faceIndex);
        m_pickedData->addPickOtherInfo(otherInfo);

    }
    void PickedDataCalculator::ByAngle()
    {
        if (m_pickedData == nullptr) return;
        switch (m_pickedData->getPickedDataType())
        {
        case GUI::GUIPickInfo::POBMeshVert:
        {
            ByAngleMeshPoint();
            break;
        }
        case GUI::GUIPickInfo::POBMeshVertSurf:
        {
            ByAngleMeshSurfPoint();
            break;
        }
        case GUI::GUIPickInfo::POBMeshCell:
        {
            ByAngleMeshCell();
            break;
        }
        case GUI::GUIPickInfo::POBMeshCellSurf:
        {
            ByAngleMeshCellSurf();
            break;
        }
        }
    }
    void PickedDataCalculator::ByAngleMeshPoint()
    {
        if (m_pickedData == nullptr) return;
        Exchange::GraphVTKObject3D* gobj = m_pickedData->getPickedGraphObejct();
        Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(m_pickedData->getPickedDataObjId());
        Exchange::PartMeshDataVTK* partData{ nullptr };
        int isTrue = Exchange::GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!isTrue || partData == nullptr) return;

        //获取网格特征
        Interface::FITKMeshFeatureVTK* meshFeature = partData->_meshFeature;
        if (meshFeature == nullptr) return;
        //获取表面提取器
        FITKSurfaceFilter* surfaceFilter = meshFeature->getSurfaceFilter();
        if (surfaceFilter == nullptr) return;

        int index = m_pickedData->getPickedIndex();
        // 查找对应的表面点
        int surfId= surfaceFilter->getSurfacePointIdByOriginPointId(index);
 
        if (surfId == -1) {
            return;
        }

        //获取表面特征数据与法向数据
        vtkPolyData* surfacePoly = meshFeature->getFeatureMesh(1);
        vtkDataArray* normals = meshFeature->getCellNormals();
        Common::FITKPolyDataTool tool(surfacePoly, normals);

        //根据拾取单元id与拾取角度，获取面id
        QList<int> surfPointIds = tool.getCoPlanePointIDs(surfId, m_pickedData->getPickedInfo()._pickParam);

        for (int& surfPoint : surfPointIds) {
            //当前表面节点VTKid对应的真实节点的全局索引
            int realGlobalIndex = surfaceFilter->getOriginPointIdBySurfacePointId(surfPoint);
            int pointId = gobj->getShapeIdByVTKCellId(realGlobalIndex,Exchange::GraphVTKCommons::PickMeshVertex);
            m_pickedData->addPickOtherInfo(PickDataInfo(pointId));
        }
    }

    void PickedDataCalculator::ByAngleMeshSurfPoint()
    {
        if (m_pickedData == nullptr) return;
        Exchange::GraphVTKObject3D* gobj = m_pickedData->getPickedGraphObejct();
        Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(m_pickedData->getPickedDataObjId());
        Exchange::PartMeshDataVTK* partData{ nullptr };
        int isTrue = Exchange::GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!isTrue || partData == nullptr) return;

        //获取网格特征
        Interface::FITKMeshFeatureVTK* meshFeature = partData->_meshFeature;
        if (meshFeature == nullptr) return;
        //获取表面提取器
        FITKSurfaceFilter* surfaceFilter = meshFeature->getSurfaceFilter();
        if (surfaceFilter == nullptr) return;

        // 查找对应的表面点
        int surfId = m_pickedData->getPickedIndex();

        if (surfId == -1) {
            return;
        }

        //获取表面特征数据与法向数据
        vtkPolyData* surfacePoly = meshFeature->getFeatureMesh(1);
        vtkDataArray* normals = meshFeature->getCellNormals();
        Common::FITKPolyDataTool tool(surfacePoly, normals);

        //根据拾取单元id与拾取角度，获取面id
        QList<int> surfPointIds = tool.getCoPlanePointIDs(surfId, m_pickedData->getPickedInfo()._pickParam);

        for (int& surfPoint : surfPointIds) {
            //当前表面节点VTKid对应的真实节点的全局索引
            int realGlobalIndex = surfaceFilter->getOriginPointIdBySurfacePointId(surfPoint);
            int pointId = gobj->getShapeIdByVTKCellId(realGlobalIndex, Exchange::GraphVTKCommons::PickMeshVertex);
            m_pickedData->addPickOtherInfo(PickDataInfo(pointId));
        }
    }

    void PickedDataCalculator::ByAngleMeshCell()
    {
        if (m_pickedData == nullptr) return;

        Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(m_pickedData->getPickedDataObjId());
        Exchange::PartMeshDataVTK* partData{ nullptr };
        int isTrue = Exchange::GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!isTrue || partData == nullptr) return;

        //获取网格特征
        Interface::FITKMeshFeatureVTK* meshFeature = partData->_meshFeature;
        if (meshFeature == nullptr) return;

        FITKSurfaceFilter* surfaceFilter = meshFeature->getSurfaceFilter();
        if (surfaceFilter == nullptr) return;
        //获取到的单元id（网格单元VTKid）在表面提取后表面上对应的VTKid
        int surfId= m_pickedData->getPickedIndex();

        //如果surfid不在，则该是去不在表面上
        if (surfId == -1) {
            return;
        }
        //获取表面特征数据与法向数据
        vtkPolyData* surfacePoly = meshFeature->getFeatureMesh(1);
        vtkDataArray* normals = meshFeature->getCellNormals();
        Common::FITKPolyDataTool tool(surfacePoly, normals);

        //根据拾取单元id与拾取角度，获取面id
        QList<int> surfCellIds = tool.getCoPlaneCellIDs(surfId, m_pickedData->getPickedInfo()._pickParam);
        for (int& surfCell : surfCellIds)
        {
            //当前表面单元VTKid对应的真实单元的VTKid
            int realId = surfaceFilter->getOriginCellIdBySurfaceCellId(surfCell);
            //根据真实单元的id获取数据结构单元id
            int id = part->getEleIDByIndex(realId);
            m_pickedData->addPickOtherInfo(PickDataInfo(id));
        }
    }
    void PickedDataCalculator::ByAngleMeshCellSurf()
    {
        if (m_pickedData == nullptr) return;

        Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(m_pickedData->getPickedDataObjId());
        Exchange::PartMeshDataVTK* partData{ nullptr };
        int isTrue = Exchange::GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!isTrue || partData == nullptr) return;

        vtkDataSet* partGrid = partData->_meshData;
        if (partGrid == nullptr) return;

        Interface::FITKMeshFeatureVTK* meshFeature = partData->_meshFeature;
        if (meshFeature == nullptr) return;

        FITKSurfaceFilter* surfaceFilter = meshFeature->getSurfaceFilter();
        if (surfaceFilter == nullptr) return;
        // 获取表面特征数据与法向数据
        vtkPolyData* surfacePoly = meshFeature->getFeatureMesh(1);
        vtkDataArray* normals = meshFeature->getCellNormals();
        if (surfacePoly == nullptr || normals == nullptr) return;
        Common::FITKPolyDataTool tool(surfacePoly, normals);
        // 获取全局节点数据用于节点ID转换
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return;

        Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
        if (meshModel == nullptr) return;

        Radioss::FITKRadiossNodes* radiossNodes = meshModel->getNodes();
        if (radiossNodes == nullptr) return;
        // 根据拾取单元id与拾取角度，获取共面的表面单元id
        QList<int> surfCellIds = tool.getCoPlaneCellIDs(m_pickedData->getPickedIndex(), m_pickedData->getPickedInfo()._pickParam);
        for (int& surfCellId : surfCellIds) {
            // 表面单元在实际单元中的面索引
            int faceIndex = -1;
            // 当前表面单元VTKid对应的真实单元VTKid
            int realVTKCellId = -1;
            surfaceFilter->getSurfaceCellInfo(surfCellId, realVTKCellId, faceIndex);

            // 添加拾取额外参数
            PickDataInfo otherInfo;
            otherInfo._id = realVTKCellId;
            otherInfo._otherInfo.append(faceIndex);
            m_pickedData->addPickOtherInfo(otherInfo);
        }
    }
    void PickedDataCalculator::byAreaPick()
    {
        Exchange::GraphVTKObject3D* gobj = m_pickedData->getPickedGraphObejct();
        vtkPlanes* planes = m_pickedData->getCutPlane();
        vtkActor* actor = m_pickedData->getPickedActor();
        if (!gobj || !planes || !actor)
        {
            return;
        }

        // 获取拾取数据集。
        vtkDataSet* dataSet{ nullptr };
        Exchange::GraphVTKCommons::ShapePickMode sType = Exchange::GraphVTKCommons::PickNone;

        // 根据拾取数据类型进行不同数据获取。
        switch (m_pickedData->getPickedDataType())
        {
        case  GUI::GUIPickInfo::PickObjType::POBGeoVert:
            sType = Exchange::GraphVTKCommons::ShapePickMode::PickGeoVertex;
            dataSet = actor->GetMapper()->GetInputAsDataSet();
            break;
        case  GUI::GUIPickInfo::PickObjType::POBGeoEdge:
            sType = Exchange::GraphVTKCommons::ShapePickMode::PickGeoEdge;
            dataSet = actor->GetMapper()->GetInputAsDataSet();
            break;
        case  GUI::GUIPickInfo::PickObjType::POBGeoFace:
            sType = Exchange::GraphVTKCommons::ShapePickMode::PickGeoFace;
            dataSet = actor->GetMapper()->GetInputAsDataSet();
            break;
        case  GUI::GUIPickInfo::PickObjType::POBGeoSolid:
            sType = Exchange::GraphVTKCommons::ShapePickMode::PickGeoSolid;
            dataSet = actor->GetMapper()->GetInputAsDataSet();
            break;
        case  GUI::GUIPickInfo::PickObjType::POBMeshVert:
            byAreaPickByMeshPoint();
            break;
        case  GUI::GUIPickInfo::PickObjType::POBMeshVertSurf:
            byAreaPickedSurfNodeDataSet();
            break;
        case  GUI::GUIPickInfo::PickObjType::POBMeshCell:
            byAreaPickedCellDataSet();
            break;
        case  GUI::GUIPickInfo::PickObjType::POBMeshCellSurf:
            byAreaPickByMeshCellSurf();
            return;
        default:
            return;
        }

        if (!dataSet)
        {
            return;
        }

        FITKExtractGeometry* extractor = FITKExtractGeometry::New();
        extractor->SetImplicitFunction(planes);
        extractor->SetInputData(dataSet);
        extractor->Update();

        // 通过单元索引获取单元ID。
        const QList<int> cellsIndex = extractor->getSelectOriginalCells();
        if (cellsIndex.isEmpty())
        {
            return;
        }

        // 被拾取形状编号。
        QVector<int> shapeIds;

        // 预处理拾取单元数据。（加速判断拾取子Id包含关系）
        int nCells = dataSet->GetNumberOfCells();
        QVector<int> cellPickedFlags;
        cellPickedFlags.resize(nCells);
        cellPickedFlags.fill(0);

        //添加拾取信息
        for (const int& index : cellsIndex)
        {
            int id = gobj->getShapeIdByVTKCellId(index, sType);
            if (shapeIds.contains(id))
            {
                continue;
            }
            shapeIds.push_back(id);
            cellPickedFlags[index] = 1;
            bool isFullPicked = true;
            isFullPicked &= (cellPickedFlags[index] == 1);
            if (isFullPicked)
            {
                m_pickedData->addPickOtherInfo(PickDataInfo(id));
                int vTopoIndex = gobj->getVirTopoIndexByShapeId(id, sType);
                m_pickedData->appendPickedOtherId(id, vTopoIndex);
            }
        }
        shapeIds.clear();
        extractor->Delete();
    }

    void PickedDataCalculator::byAreaPickByMeshPoint()
    {
        Exchange::GraphVTKObject3D* gobj = m_pickedData->getPickedGraphObejct();
        vtkPlanes* planes = m_pickedData->getCutPlane();
        vtkActor* actor = m_pickedData->getPickedActor();
        if (!gobj || !planes || !actor) {
            return;
        }

        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return;
        Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
        if (meshModel == nullptr) return;
        Radioss::FITKRadiossNodes* radiossNodes = meshModel->getNodes();
        if (radiossNodes == nullptr) return;

        // 获取拾取数据集。
        vtkDataSet* dataSet = actor->GetMapper()->GetInputAsDataSet();
        if (!dataSet) {
            return;
        }
        FITKExtractGeometry* extractor = FITKExtractGeometry::New();
        extractor->SetImplicitFunction(planes);
        extractor->SetInputData(dataSet);
        extractor->Update();

        // 通过点索引获取点ID。
        const QList<int> pointsIndex = extractor->getSelectOriginalPoints();
        if (pointsIndex.isEmpty()) {
            return;
        }
        for (int index : pointsIndex)
        {
            int id = gobj->getShapeIdByVTKCellId(index, Exchange::GraphVTKCommons::PickMeshVertex);

            m_pickedData->addPickOtherInfo(PickDataInfo(id));
        }
        extractor->Delete();
    }

    void PickedDataCalculator::byAreaPickByMeshCellSurf()
    {
        Exchange::GraphVTKObject3D* gobj = m_pickedData->getPickedGraphObejct();
        vtkPlanes* planes = m_pickedData->getCutPlane();
        vtkActor* actor = m_pickedData->getPickedActor();
        if (!gobj || !planes || !actor) {
            return;
        }
        Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(m_pickedData->getPickedDataObjId());
        Exchange::PartMeshDataVTK* partData{ nullptr };
        int isTrue = Exchange::GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!isTrue || partData == nullptr) {
            return;
        }
        //获取表面提取过滤器
        FITKSurfaceFilter* surfaceFilter = partData->_meshFeature->getSurfaceFilter();
        vtkDataSet* dataSet =partData->_meshData;
        if (!dataSet || !surfaceFilter) {
            return;
        }
        //获取当前拾取数据
        vtkDataSet* pickDataSet = actor->GetMapper()->GetInputAsDataSet();
        if(!pickDataSet) {
            return;
        }
        FITKExtractGeometry* extractor = FITKExtractGeometry::New();
        extractor->SetImplicitFunction(planes);
        extractor->SetInputData(pickDataSet);
        extractor->Update();

        // 通过单元索引获取单元ID。
        const QList<int> cellsIndex = extractor->getSelectOriginalCells();
        if (cellsIndex.isEmpty()) {
            return;
        }
        for (const int& index : cellsIndex)
        {
            int orgCellIndex = -1;
            int faceIndex = -1;
            surfaceFilter->getSurfaceCellInfo(index, orgCellIndex, faceIndex);

            PickDataInfo otherInfo;
            otherInfo._id = orgCellIndex;
            otherInfo._otherInfo.append(faceIndex);
            m_pickedData->addPickOtherInfo(otherInfo);
        }
        extractor->Delete();
    }

    void PickedDataCalculator::byAreaPickedSurfNodeDataSet()
    {
        Exchange::GraphVTKObject3D* gobj = m_pickedData->getPickedGraphObejct();
        vtkPlanes* planes = m_pickedData->getCutPlane();
        vtkActor* actor = m_pickedData->getPickedActor();
        Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(m_pickedData->getPickedDataObjId());
        Exchange::PartMeshDataVTK* partData = nullptr;
        bool isTrue = Exchange::GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!gobj || !planes || !actor || !part || !isTrue)
        {
            return;
        }
        vtkDataSet* dataSet = actor->GetMapper()->GetInputAsDataSet();
        if (!dataSet)
        {
            return;
        }

        FITKSurfaceFilter* surfaceFilter = partData->_meshFeature->getSurfaceFilter();
        if (!surfaceFilter)
        {
            return;
        }
        FITKExtractGeometry* extractor = FITKExtractGeometry::New();
        extractor->SetImplicitFunction(planes);
        extractor->SetInputData(dataSet);
        extractor->Update();
        Exchange::GraphVTKCommons::ShapePickMode sType = Exchange::GraphVTKCommons::PickMeshVertexSurf;
        // 通过点索引获取点ID。
        const QList<int> pointsIndex = extractor->getSelectOriginalPoints();

        QVector<int> shapeIds;

        // 预处理拾取单元数据。（加速判断拾取子Id包含关系）
        int nCells = dataSet->GetNumberOfCells();
        QVector<int> cellPickedFlags;
        cellPickedFlags.resize(nCells);
        cellPickedFlags.fill(0);

        //添加拾取信息
        for (const int& index : pointsIndex)
        {
            int id = gobj->getShapeIdByVTKCellId(index, sType);
            if (shapeIds.contains(id))
            {
                continue;
            }
            shapeIds.push_back(id);
            cellPickedFlags[index] = 1;
            bool isFullPicked = true;
            isFullPicked &= (cellPickedFlags[index] == 1);
            if (isFullPicked)
            {
                m_pickedData->addPickOtherInfo(PickDataInfo(id));
                int vTopoIndex = gobj->getVirTopoIndexByShapeId(id, sType);
                m_pickedData->appendPickedOtherId(id, vTopoIndex);
            }
        }
        extractor->Delete();
        return;
    }

    void PickedDataCalculator::byAreaPickedCellDataSet()
    {
        Exchange::GraphVTKObject3D* gobj = m_pickedData->getPickedGraphObejct();
        vtkPlanes* planes = m_pickedData->getCutPlane();
        vtkActor* actor = m_pickedData->getPickedActor();
        Radioss::FITKRadiossPart* part = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(m_pickedData->getPickedDataObjId());
        Exchange::PartMeshDataVTK* partData = nullptr;
        bool isTrue = Exchange::GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!gobj || !planes || !actor || !part || !isTrue)
        {
            return;
        }
        vtkDataSet* dataSet = partData->_meshData;
        if (!dataSet)
        {
            return;
        }

        FITKExtractGeometry* extractor = FITKExtractGeometry::New();
        extractor->SetImplicitFunction(planes);
        extractor->SetInputData(dataSet);
        extractor->Update();
        Exchange::GraphVTKCommons::ShapePickMode sType = Exchange::GraphVTKCommons::PickMeshCell;
        // 通过点索引获取点ID。
        const QList<int> cellsIndice = extractor->getSelectOriginalCells();

        QVector<int> shapeIds;

        // 预处理拾取单元数据。（加速判断拾取子Id包含关系）
        int nCells = dataSet->GetNumberOfCells();
        QVector<int> cellPickedFlags;
        cellPickedFlags.resize(nCells);
        cellPickedFlags.fill(0);

        //添加拾取信息
        for (const int& index : cellsIndice)
        {
            int id = part->getEleIDByIndex(index);
            m_pickedData->addPickOtherInfo(PickDataInfo(id));
        }
        extractor->Delete();
        return;
    }

}
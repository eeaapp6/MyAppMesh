/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphDataVTKPartSetManager.h"
#include "GraphVTKCommons.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshEntity.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshVS.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKMeshFeatureVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKMeshVTKMap.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKShellFeatureEdges.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKIdTypeArrayFilter.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKVertexGlyphFilter.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKExtractCellsByDim.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkDoubleArray.h>
#include <vtkPolygon.h>
#include <vtkDataSet.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPolyDataNormals.h>
#include <vtkDataSetAlgorithm.h>
#include <vtkUnstructuredGridAlgorithm.h>
#include <vtkIdFilter.h>
#include <vtkPassThrough.h>
#include <vtkAppendFilter.h>
#include <vtkVertexGlyphFilter.h>
#include <QDebug>

namespace Exchange
{
    //静态变量初始化
    GraphDataVTKPartSetManager* GraphDataVTKPartSetManager::_instance = nullptr;

    QMutex GraphDataVTKPartSetManager::m_mutex;

    GraphDataVTKPartSetInfo::GraphDataVTKPartSetInfo()
    {
        _pointGrid = vtkUnstructuredGrid::New();
        _pointGrid->SetPoints(vtkSmartPointer<vtkPoints>::New());
        _lineGrid = vtkUnstructuredGrid::New();
        _lineGrid->SetPoints(vtkSmartPointer<vtkPoints>::New());
        _faceGrid = vtkPolyData::New();
        _faceGrid->SetPoints(vtkSmartPointer<vtkPoints>::New());
        _faceGrid->SetPolys(vtkSmartPointer<vtkCellArray>::New());
        _solidGrid = vtkPolyData::New();
        _solidGrid->SetPoints(vtkSmartPointer<vtkPoints>::New());
        _solidGrid->SetPolys(vtkSmartPointer<vtkCellArray>::New());
    }

    GraphDataVTKPartSetInfo::~GraphDataVTKPartSetInfo()
    {
        if (_pointGrid) {
            _pointGrid->Delete();
            _pointGrid = nullptr;
        }

        if (_lineGrid) {
            _lineGrid->Delete();
            _lineGrid = nullptr;
        }

        if (_faceGrid) {
            _faceGrid->Delete();
            _faceGrid = nullptr;
        }
        if (_solidGrid) {
            _solidGrid->Delete();
            _solidGrid = nullptr;
        }
    }

    void GraphDataVTKPartSetInfo::clearData()
    {
        if (_pointGrid == nullptr || _lineGrid == nullptr || _faceGrid == nullptr)return;
        _pointGrid->Reset();
        _lineGrid->Reset();
        _faceGrid->Reset();
        _solidGrid->Reset();
        _point_VTKId_CellID.clear();
        _line_VTKId_CellID.clear();
        _face_VTKId_CellID.clear();
        _solid_VTKId_CellID.clear();
        _partPointHash.clear();
        _partLineHash.clear();
        _partFaceHash.clear();
    }

    void GraphDataVTKPartSetManager::updateGeoCommand(int geoCommandId)
    {
        Interface::FITKAbsGeoCommand* geoCommand = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(geoCommandId);
        if (geoCommand == nullptr)return;
        Interface::FITKAbsGeoShapeAgent* shapeAgent = geoCommand->getShapeAgent();
        if (shapeAgent == nullptr)return;

        if (_geoCommandInfos.keys().contains(geoCommandId) == true) {
            _geoCommandInfos[geoCommandId]->clearData();
            //网格数据创建
            createGeoPointGrid(_geoCommandInfos[geoCommandId]);
            createGeoLineGrid(_geoCommandInfos[geoCommandId]);
            createGeoFaceGrid(_geoCommandInfos[geoCommandId]);
            createGeoSolidGrid(_geoCommandInfos[geoCommandId]);
        }
        else {
            //网格数据创建
            GraphDataVTKPartSetInfo* partInfo = new GraphDataVTKPartSetInfo();
            partInfo->_geoCommandID = geoCommandId;
            createGeoPointGrid(partInfo);
            createGeoLineGrid(partInfo);
            createGeoFaceGrid(partInfo);
            createGeoSolidGrid(partInfo);
            _geoCommandInfos.insert(geoCommandId, partInfo);
            //几何命令对象清除，管理中清除相关数据
            connect(geoCommand, &Interface::FITKAbsGeoCommand::dataObjectDestoried, [=]() {
                delete _geoCommandInfos[geoCommandId];
                _geoCommandInfos.remove(geoCommandId);
                });
        }
    }

    GraphDataVTKPartSetInfo* GraphDataVTKPartSetManager::getGeoCommandInfo(int geoCommandId)
    {
        return _geoCommandInfos.value(geoCommandId);
    }

    Interface::FITKGeoMeshVSPt* GraphDataVTKPartSetManager::getGeoPointData(int virTopoId)
    {
        for (GraphDataVTKPartSetInfo* partInfo : _geoCommandInfos.values()) {
            if (partInfo == nullptr)continue;
            //判断当前part信息中是否包含该虚拓扑
            if (partInfo->_partPointHash.keys().contains(virTopoId) == false)continue;
            return partInfo->_partPointHash.value(virTopoId);
        }
        return nullptr;
    }

    Interface::FITKGeoMeshVSEdgeEntity* GraphDataVTKPartSetManager::getGeoLineData(int virTopoId)
    {
        for (GraphDataVTKPartSetInfo* partInfo : _geoCommandInfos.values()) {
            if (partInfo == nullptr)continue;
            //判断当前part信息中是否包含该虚拓扑
            if (partInfo->_partLineHash.keys().contains(virTopoId) == false)continue;
            return partInfo->_partLineHash.value(virTopoId);
        }
        return nullptr;
    }

    Interface::FITKGeoMeshVSFaceEntity* GraphDataVTKPartSetManager::getGeoFaceData(int virTopoId)
    {
        for (GraphDataVTKPartSetInfo* partInfo : _geoCommandInfos.values()) {
            if (partInfo == nullptr)continue;
            //判断当前part信息中是否包含该虚拓扑
            if (partInfo->_partFaceHash.keys().contains(virTopoId) == false)continue;
            return partInfo->_partFaceHash.value(virTopoId);
        }
        return nullptr;
    }
    bool GraphDataVTKPartSetManager::getPartMeshData(int partId, PartMeshDataVTK*& ioData, bool update)
    {
        //获取网格数据
        if (partId == GraphVTKCommons::_unusedNodesPartId)
        {
            //单独处理未使用节点
            ioData = _partMeshDataHash.value(partId, nullptr);
            if(ioData)
            {
                if (update)
                {
                    bool isTrue = createUnusedNodesPartVTK(ioData);
                    modifyVtkObj(ioData->_filterVertex);
                    return isTrue;
                }
                return true;
            }
            ioData = new PartMeshDataVTK();
            ioData->_partId = partId;
            _partMeshDataHash.insert(partId, ioData);
            return createUnusedNodesPartVTK(ioData);
        }
        ioData = _partMeshDataHash.value(partId, nullptr);
        if (ioData)
        {
            if (update)
            {
                bool isTrue = createPartMeshDataVTK(partId, ioData);
                modifyVtkObj(ioData->_filterSurface);
                modifyVtkObj(ioData->_filterSurfVert);
                modifyVtkObj(ioData->_filterVertex);

                return isTrue;
            }
            return true;
        }
        ioData = new PartMeshDataVTK();
        ioData->_partId = partId;
        _partMeshDataHash.insert(partId, ioData);

        bool isTrue = createPartMeshDataVTK(partId, ioData);

        return isTrue;
    }
    void Exchange::GraphDataVTKPartSetManager::initialize()
    {
        return;
    }

    void GraphDataVTKPartSetManager::finalize()
    {
        for (PartMeshDataVTK* data : _partMeshDataHash.values())
        {
            if (data)
            {
                if (data->_meshFeature)
                {
                    delete data->_meshFeature;
                    data->_meshFeature = nullptr;
                }

                deleteVtkObj(data->_filterSurface);
                deleteVtkObj(data->_filterVertex);
                deleteVtkObj(data->_filterSurfVert);

                if (data->_meshData)
                {
                    data->_meshData->Delete();
                    data->_meshData = nullptr;
                }

                delete data;
            }
        }
        _partMeshDataHash.clear();

        for (GraphDataVTKPartSetInfo* info : _geoCommandInfos.values())
        {
            if (info)
            {
                delete info;
            }
        }
        _geoCommandInfos.clear();
    }
    void GraphDataVTKPartSetManager::createGeoPointGrid(GraphDataVTKPartSetInfo* partInfo)
    {
        if (partInfo == nullptr)return;
        Interface::FITKAbsGeoCommand* geoCommand = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(partInfo->_geoCommandID);
        if (geoCommand == nullptr)return;
        Interface::FITKAbsGeoShapeAgent* shapeAgent = geoCommand->getShapeAgent();
        if (shapeAgent == nullptr)return;
        Interface::FITKGeometryMeshVS* triData = shapeAgent->getMeshVS();
        if (triData == nullptr)return;

        // 获取虚拓扑管理器
        Interface::FITKVirtualTopoManager* topoMgr = shapeAgent->getVirtualTopoManager();
        if (!topoMgr) return;
        Interface::FITKShapeVirtualTopoManager* vsMgrPoint = topoMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSPoint);
        if (!vsMgrPoint) return;

        const QHash<int, Interface::FITKGeoMeshVSPt*> pointsHash = triData->getPointItems();
        vtkUnstructuredGrid* pointGrid = partInfo->_pointGrid;
        if (pointGrid == nullptr)return;
        vtkPoints* points = pointGrid->GetPoints();

        int nVerts = vsMgrPoint->getDataCount();
        for (int i = 0; i < nVerts; i++) {
            Interface::FITKAbsVirtualTopo* vTopoV = vsMgrPoint->getDataByIndex(i);
            if (!vTopoV) continue;
            if (!vTopoV->isEnable()) continue;
            int pointId = vTopoV->getDataObjectID();
            Interface::FITKGeoMeshVSPt* pt = pointsHash.value(pointId);
            if (!pt) continue;

            int pointVTKID = points->InsertNextPoint(pt->x(), pt->y(), pt->z());
            vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
            cell->InsertNextId(pointVTKID);
            int vtkid = pointGrid->InsertNextCell(VTKCellType::VTK_VERTEX, cell);
            partInfo->_point_VTKId_CellID.insert(vtkid, pointId);
            partInfo->_partPointHash.insert(pointId, pt);

            // 记录虚拓扑ID与索引
            partInfo->_pointVirTopoIdToIndex.insert(pointId, i);
        }
    }

    void GraphDataVTKPartSetManager::createGeoLineGrid(GraphDataVTKPartSetInfo* partInfo)
    {
        if (partInfo == nullptr)return;
        Interface::FITKAbsGeoCommand* geoCommand = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(partInfo->_geoCommandID);
        if (geoCommand == nullptr)return;
        Interface::FITKAbsGeoShapeAgent* shapeAgent = geoCommand->getShapeAgent();
        if (shapeAgent == nullptr)return;
        Interface::FITKGeometryMeshVS* triData = shapeAgent->getMeshVS();
        if (triData == nullptr)return;

        Interface::FITKVirtualTopoManager* topoMgr = shapeAgent->getVirtualTopoManager();
        if (!topoMgr) return;
        Interface::FITKShapeVirtualTopoManager* vsMgrEdge = topoMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if (!vsMgrEdge) return;

        QHash<int, Interface::FITKGeoMeshVSEdgeEntity*> lineHash = triData->getEdgeItems();
        vtkUnstructuredGrid* lineGrid = partInfo->_lineGrid;
        if (lineGrid == nullptr)return;
        vtkPoints* points = lineGrid->GetPoints();

        int nEdges = vsMgrEdge->getDataCount();
        for (int i = 0; i < nEdges; i++) {
            Interface::FITKAbsVirtualTopo* vTopoE = vsMgrEdge->getDataByIndex(i);
            if (!vTopoE) continue;
            if (!vTopoE->isEnable()) continue;
            int edgeId = vTopoE->getDataObjectID();
            Interface::FITKGeoMeshVSEdgeEntity* edge = lineHash.value(edgeId);
            if (!edge) continue;

            const QList<Interface::FITKGeoMeshVSPt*>& pts = edge->getPoints();
            vtkSmartPointer<vtkIdList> polyLineCell = vtkSmartPointer<vtkIdList>::New();
            for (Interface::FITKGeoMeshVSPt* pt : pts) {
                int ptIndex = points->InsertNextPoint(pt->x(), pt->y(), pt->z());
                polyLineCell->InsertNextId(ptIndex);
            }
            int vtkID = lineGrid->InsertNextCell(VTKCellType::VTK_POLY_LINE, polyLineCell);
            partInfo->_line_VTKId_CellID.insert(vtkID, edgeId);
            partInfo->_partLineHash.insert(edgeId, edge);

            // 记录虚拓扑ID与索引
            partInfo->_lineVirTopoIdToIndex.insert(edgeId, i);
        }
    }

    void GraphDataVTKPartSetManager::createGeoFaceGrid(GraphDataVTKPartSetInfo* partInfo)
    {
        if (partInfo == nullptr)return;
        Interface::FITKAbsGeoCommand* geoCommand = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(partInfo->_geoCommandID);
        if (geoCommand == nullptr)return;
        Interface::FITKAbsGeoShapeAgent* shapeAgent = geoCommand->getShapeAgent();
        if (shapeAgent == nullptr)return;
        Interface::FITKGeometryMeshVS* triData = shapeAgent->getMeshVS();
        if (triData == nullptr)return;

        Interface::FITKVirtualTopoManager* topoMgr = shapeAgent->getVirtualTopoManager();
        if (!topoMgr) return;
        Interface::FITKShapeVirtualTopoManager* vsMgrFace = topoMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (!vsMgrFace) return;

        const QHash<int, Interface::FITKGeoMeshVSFaceEntity*> facesHash = triData->getFaceItems();
        vtkPolyData* faceGrid = partInfo->_faceGrid;
        if (faceGrid == nullptr)return;
        vtkPoints* points = faceGrid->GetPoints();

        int ptIndexStart = 0;
        int nFaces = vsMgrFace->getDataCount();
        for (int i = 0; i < nFaces; i++) {
            Interface::FITKAbsVirtualTopo* vTopoF = vsMgrFace->getDataByIndex(i);
            if (!vTopoF) continue;
            if (!vTopoF->isEnable()) continue;
            int faceId = vTopoF->getDataObjectID();
            Interface::FITKGeoMeshVSFaceEntity* face = facesHash.value(faceId);
            if (!face) continue;

            const QList<Interface::FITKGeoMeshVSPt*>& facePoints = face->getPoints();
            for (Interface::FITKGeoMeshVSPt* facePoint : facePoints) {
                points->InsertNextPoint(facePoint->x(), facePoint->y(), facePoint->z());
            }
            const QList<Interface::FITKGeoMeshVSTri*>& tris = face->getTriangles();
            for (Interface::FITKGeoMeshVSTri* tri : tris) {
                const int* nodes = tri->getNode();
                int ptIndex1 = ptIndexStart + nodes[0];
                int ptIndex2 = ptIndexStart + nodes[1];
                int ptIndex3 = ptIndexStart + nodes[2];
                vtkSmartPointer<vtkIdList> triCell = vtkSmartPointer<vtkIdList>::New();
                triCell->InsertNextId(ptIndex1);
                triCell->InsertNextId(ptIndex2);
                triCell->InsertNextId(ptIndex3);
                int vtkID = faceGrid->InsertNextCell(VTKCellType::VTK_TRIANGLE, triCell);
                partInfo->_face_VTKId_CellID.insert(vtkID, faceId);
            }
            partInfo->_partFaceHash.insert(faceId, face);

            // 记录虚拓扑ID与索引
            partInfo->_faceVirTopoIdToIndex.insert(faceId, i);

            ptIndexStart += facePoints.count();
        }
        calculatePointNormals(faceGrid);
    }

    void GraphDataVTKPartSetManager::createGeoSolidGrid(GraphDataVTKPartSetInfo* partInfo)
    {
        if (partInfo == nullptr) return;
        Interface::FITKAbsGeoCommand* geoCommand = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(partInfo->_geoCommandID);
        if (geoCommand == nullptr) return;
        Interface::FITKAbsGeoShapeAgent* shapeAgent = geoCommand->getShapeAgent();
        if (shapeAgent == nullptr) return;
        Interface::FITKGeometryMeshVS* triData = shapeAgent->getMeshVS();
        if (triData == nullptr) return;

        Interface::FITKVirtualTopoManager* topoMgr = shapeAgent->getVirtualTopoManager();
        if (!topoMgr) return;
        Interface::FITKShapeVirtualTopoManager* vsMgrSolid = topoMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        if (!vsMgrSolid) return;

        // 获取所有面数据用于构建体
        const QHash<int, Interface::FITKGeoMeshVSFaceEntity*> facesHash = triData->getFaceItems();
        vtkPolyData* solidGrid = partInfo->_solidGrid;
        if (solidGrid == nullptr) return;
        vtkPoints* points = solidGrid->GetPoints();

        int ptIndexStart = 0;
        int nSolids = vsMgrSolid->getDataCount();
        for (int i = 0; i < nSolids; i++) {
            Interface::FITKAbsVirtualTopo* vTopoS = vsMgrSolid->getDataByIndex(i);
            if (!vTopoS) continue;
            if (!vTopoS->isEnable()) continue;

            int solidId = vTopoS->getDataObjectID();

            // 遍历实体所有的面
            Interface::FITKVirtualTopoMapper vMapper;
            vMapper.mapTopo(vTopoS, Interface::FITKGeoEnum::VTopoShapeType::VSFace);

            int nFaces = vMapper.length();
            for (int j = 0; j < nFaces; j++) {
                Interface::FITKAbsVirtualTopo* vTopoF = vMapper.virtualTopo(j);
                if (!vTopoF) continue;
                if (!vTopoF->isEnable()) continue;

                int faceId = vTopoF->getDataObjectID();
                Interface::FITKGeoMeshVSFaceEntity* face = facesHash.value(faceId);
                if (!face) continue;

                // 添加面的所有点
                const QList<Interface::FITKGeoMeshVSPt*>& facePoints = face->getPoints();
                for (Interface::FITKGeoMeshVSPt* facePoint : facePoints) {
                    points->InsertNextPoint(facePoint->x(), facePoint->y(), facePoint->z());
                }

                // 添加面的所有三角形
                const QList<Interface::FITKGeoMeshVSTri*>& tris = face->getTriangles();
                for (Interface::FITKGeoMeshVSTri* tri : tris) {
                    const int* nodes = tri->getNode();
                    int ptIndex1 = ptIndexStart + nodes[0];
                    int ptIndex2 = ptIndexStart + nodes[1];
                    int ptIndex3 = ptIndexStart + nodes[2];

                    vtkSmartPointer<vtkIdList> triCell = vtkSmartPointer<vtkIdList>::New();
                    triCell->InsertNextId(ptIndex1);
                    triCell->InsertNextId(ptIndex2);
                    triCell->InsertNextId(ptIndex3);
                    int vtkID = solidGrid->InsertNextCell(VTKCellType::VTK_TRIANGLE, triCell);

                    // 记录体ID与VTK单元ID的映射
                    partInfo->_solid_VTKId_CellID.insert(vtkID, solidId);
                }

                ptIndexStart += facePoints.count();
            }

            // 记录虚拓扑ID与索引的映射
            partInfo->_solidVirTopoIdToIndex.insert(solidId, i);
        }

        // 计算法向量
        calculatePointNormals(solidGrid);
    }

    void GraphDataVTKPartSetManager::calculatePointNormals(vtkPolyData* polyData)
    {
        if (!polyData)
        {
            return;
        }

        // For computing points nromals.
        //@{
        vtkSmartPointer<vtkPoints> tempPts = vtkSmartPointer<vtkPoints>::New();
        //@}

        int nPts = polyData->GetNumberOfPoints();

        vtkSmartPointer<vtkDoubleArray> pointsNormals = vtkSmartPointer<vtkDoubleArray>::New();
        pointsNormals->SetNumberOfComponents(3);
        pointsNormals->SetNumberOfTuples(nPts);
        pointsNormals->FillComponent(0, 0);
        pointsNormals->FillComponent(1, 0);
        pointsNormals->FillComponent(2, 0);

        int cellId = 0;
        vtkIdType npts = 0;
#if VTK_MAJOR_VERSION < 9
        vtkIdType* pts = nullptr;
#else
        const vtkIdType* pts = nullptr;
#endif

        // Add two double tuple 3.
        auto addDouble3 = [&](double* v1, double* v2, double* result)
            {
                for (int i = 0; i < 3; i++)
                {
                    result[i] = v1[i] + v2[i];
                }
            };

        vtkPoints* points = polyData->GetPoints();

        double result[3]{ 0., 0., 0. };
        for (cellId = 0, polyData->GetPolys()->InitTraversal(); polyData->GetPolys()->GetNextCell(npts, pts); ++cellId)
        {
            tempPts->Reset();
            tempPts->SetNumberOfPoints(0);
            for (vtkIdType i = 0; i < npts; ++i)
            {
                tempPts->InsertNextPoint(points->GetPoint(pts[i]));
            }

            double norCellCurrent[3];
            vtkPolygon::ComputeNormal(tempPts, norCellCurrent);

            for (vtkIdType i = 0; i < npts; ++i)
            {
                int iPt = pts[i];
                double* norPtCurrent = pointsNormals->GetTuple3(iPt);

                // Add normals.
                addDouble3(norPtCurrent, norCellCurrent, result);
                pointsNormals->SetTuple3(iPt, result[0], result[1], result[2]);
            }
        }

        double flipDirection = 1.0;
        for (vtkIdType i = 0; i < nPts; ++i)
        {
            double* ptNormal = pointsNormals->GetTuple3(i);
            const double length = sqrt(pow(ptNormal[0], 2) + pow(ptNormal[1], 2) + pow(ptNormal[2], 2)) * flipDirection;
            if (!qFuzzyCompare(length, 0.))
            {
                ptNormal[0] /= length;
                ptNormal[1] /= length;
                ptNormal[2] /= length;
            }

            pointsNormals->SetTuple3(i, ptNormal[0], ptNormal[1], ptNormal[2]);

            qDebug() << ptNormal[0] << ptNormal[1] << ptNormal[2];
        }

        polyData->GetPointData()->SetNormals(pointsNormals);
    }

    Radioss::FITKRadiossNodes* GraphDataVTKPartSetManager::getRadiossNodes()
    {
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr) return nullptr;
        Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
        if (meshModel == nullptr) return nullptr;
        Radioss::FITKRadiossNodes* nodes = meshModel->getNodes();
        return nodes;
    }
    bool GraphDataVTKPartSetManager::createPartMeshDataVTK(int partId, PartMeshDataVTK*& ioData)
    {
        Radioss::FITKRadiossPart* partData = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossPart>(partId);
        if (partData == nullptr) return false;
        Radioss::FITKRadiossNodes* nodeData = getRadiossNodes();
        if (nodeData == nullptr) return false;
        vtkUnstructuredGrid* meshData = vtkUnstructuredGrid::New();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        meshData->SetPoints(points);

        vtkSmartPointer<vtkIntArray> vtkIdByNodeIdArray = vtkSmartPointer<vtkIntArray>::New();
        vtkIdByNodeIdArray->SetName(NODEID_BY_VTKID_ARRAY_NAME);
        vtkIdByNodeIdArray->SetNumberOfComponents(1);
        meshData->GetPointData()->AddArray(vtkIdByNodeIdArray);
        QList<int> nodeIds = partData->getAllNodeIDs();
        ioData->_originalNodeIds = nodeIds;
        std::sort(nodeIds.begin(), nodeIds.end());
        QHash<int, int> nodeIdToPointIndexMap; // 创建从节点ID到点索引的映射
        for (int nodeId : nodeIds)
        {
            Core::FITKNode* node = nodeData->getNodeByID(nodeId);
            if (node == nullptr) continue;
            int pointIndex = meshData->GetPoints()->InsertNextPoint(node->x(), node->y(), node->z());
            nodeIdToPointIndexMap.insert(nodeId, pointIndex);// 填充映射
            vtkIdByNodeIdArray->InsertNextValue(nodeId);
        }

        ioData->_nodeIdToPointIndex = nodeIdToPointIndexMap;

        vtkSmartPointer<vtkIntArray> partIdArrayCell = vtkSmartPointer<vtkIntArray>::New();
        partIdArrayCell->SetName(VTKID_BY_NODEID_ARRAY_NAME);
        partIdArrayCell->SetNumberOfComponents(1);
        meshData->GetCellData()->AddArray(partIdArrayCell);

        for (int i = 0; i < partData->getElementCount(); i++)
        {
            Interface::FITKAbstractElement* element = partData->getElementAt(i);
            if (element == nullptr) continue;
            Interface::FITKModelEnum::FITKEleType type = element->getEleType();
            if (!Interface::eleTypeHash.contains(type)) continue;

            partIdArrayCell->InsertNextValue(element->getEleID());
            vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
            for (int j = 0; j < element->getNodeCount(); j++)
            {
                const int nodeId = element->getNodeID(j);
                if (!nodeIdToPointIndexMap.contains(nodeId)) continue;
                const int pointIndex = nodeIdToPointIndexMap.value(nodeId);
                if (pointIndex < 0) continue;
                idList->InsertNextId(pointIndex);
            }
            meshData->InsertNextCell(Interface::eleTypeHash[type], idList);
        }
        meshData->Squeeze();
        meshData->Modified();

        deleteVtkObj(ioData->_meshData);
        deleteVtkObj(ioData->_filterSurface);
        deleteVtkObj(ioData->_filterVertex);
        deleteVtkObj(ioData->_filterSurfVert);

        ioData->_meshData = meshData;
        if (ioData->_meshFeature)
        {
            delete ioData->_meshFeature;
        }
        ioData->_meshFeature = new Interface::FITKMeshFeatureVTK(ioData->_meshData);
        ioData->_meshFeature->update();

        vtkIdFilter* idOutput = ioData->_meshFeature->getIdFilter();
        FITKSurfaceFilter* surfOutput = ioData->_meshFeature->getSurfaceFilter();
        FITKShellFeatureEdges* shellEdgeOutput = ioData->_meshFeature->getShellEdgeFilter();
        if (!idOutput || !surfOutput || !shellEdgeOutput)
        {
            return false;
        }

        connect(partData, &Radioss::FITKRadiossPart::dataObjectDestoried,
            this, &GraphDataVTKPartSetManager::slot_partDeleted, Qt::UniqueConnection);
        // 创建并连接新的过滤器
        FITKIdTypeArrayFilter* idArrFilterSurf = FITKIdTypeArrayFilter::New();
        idArrFilterSurf->SetIdArrayName(VTKID_BY_NODEID_ARRAY_NAME);
        idArrFilterSurf->SetIntValue(partId);
        idArrFilterSurf->SetInputConnection(surfOutput->GetOutputPort());
        ioData->_filterSurface = idArrFilterSurf;

        FITKVertexGlyphFilter* verts = FITKVertexGlyphFilter::New();
        verts->SetInputConnection(idOutput->GetOutputPort());
        ioData->_filterVertex = verts;

        vtkVertexGlyphFilter* surfVertGlyph = vtkVertexGlyphFilter::New();
        surfVertGlyph->SetInputConnection(ioData->_filterSurface->GetOutputPort());
        ioData->_filterSurfVert = surfVertGlyph;

        return true;
    }

    bool GraphDataVTKPartSetManager::createUnusedNodesPartVTK(PartMeshDataVTK*& ioData)
    {
        Radioss::FITKRadiossNodes* nodeData = getRadiossNodes();
        if (nodeData == nullptr) return false;

        // 1. 从 _partMeshDataHash 收集所有已使用的节点ID
        QSet<int> usedNodeIds;
        for (PartMeshDataVTK* partMeshData : _partMeshDataHash.values())
        {
            // 确保 partMeshData 有效，并且不是“未使用节点”部件本身
            if (partMeshData && partMeshData->_partId != GraphVTKCommons::_unusedNodesPartId)
            {
                const QList<int>& nodeIds = partMeshData->_originalNodeIds;
                for (int nodeId : nodeIds)
                {
                    usedNodeIds.insert(nodeId);
                }
            }
        }

        // 2. 创建非结构化网格来存储未使用的节点
        vtkSmartPointer<vtkUnstructuredGrid> meshData = vtkSmartPointer<vtkUnstructuredGrid>::New();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        meshData->SetPoints(points);

        // 3. 为节点ID创建数组，并添加到PointData中 (VTK点索引 -> 节点ID)
        vtkSmartPointer<vtkIntArray> vtkIdByNodeIdArray = vtkSmartPointer<vtkIntArray>::New();
        vtkIdByNodeIdArray->SetName(NODEID_BY_VTKID_ARRAY_NAME);
        vtkIdByNodeIdArray->SetNumberOfComponents(1);
        meshData->GetPointData()->AddArray(vtkIdByNodeIdArray);

        QList<int> unusedNodeIdList;
        QHash<int, int> nodeIdToPointIndexMap; // 创建从节点ID到点索引的映射
        // 4. 遍历所有节点，识别并添加未使用的节点到网格中
        for (int i = 0; i < nodeData->getNodeCount(); ++i)
        {
            int nodeId = nodeData->getNodeIDByIndex(i);
            if (!usedNodeIds.contains(nodeId))
            {
                Core::FITKNode* node = nodeData->getNodeByID(nodeId);
                if (node)
                {
                    vtkIdType pointId = points->InsertNextPoint(node->x(), node->y(), node->z());
                    nodeIdToPointIndexMap.insert(nodeId, pointId); // 填充映射

                    vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
                    idList->InsertNextId(pointId);
                    meshData->InsertNextCell(VTK_VERTEX, idList);
                    vtkIdByNodeIdArray->InsertNextValue(nodeId);
                    unusedNodeIdList.append(nodeId);
                }
            }
        }

        ioData->_originalNodeIds = unusedNodeIdList;
        ioData->_nodeIdToPointIndex = nodeIdToPointIndexMap;

        // 5. 更新 ioData 中的 VTK 数据和过滤器
        deleteVtkObj(ioData->_meshData);
        ioData->_meshData = meshData;
        if (ioData->_meshData) ioData->_meshData->Register(nullptr); // 手动增加引用计数

        // 由于 GraphVTKObjectPartMesh 直接使用 _meshData，不再需要 _filterVertex
        deleteVtkObj(ioData->_filterVertex);
        ioData->_filterVertex = nullptr;

        // 清理其他不需要的过滤器
        deleteVtkObj(ioData->_filterSurface);
        deleteVtkObj(ioData->_filterSurfVert);

        return true;
    }

    void GraphDataVTKPartSetManager::slot_partDeleted(Core::FITKAbstractDataObject* objDel)
    {
        if (!objDel)
        {
            return;
        }

        int dataId = objDel->getDataObjectID();

        if (_partMeshDataHash.contains(dataId))
        {
            // Delete the vtk filters.
            PartMeshDataVTK* data = _partMeshDataHash.take(dataId);
            if (data)
            {
                if (data->_meshFeature)
                {
                    delete data->_meshFeature;
                    data->_meshFeature = nullptr;
                }

                deleteVtkObj(data->_filterSurface);
                deleteVtkObj(data->_filterVertex);
                deleteVtkObj(data->_filterSurfVert);

                if (data->_meshData)
                {
                    data->_meshData->Delete();
                    data->_meshData = nullptr;
                }

                // Delete the whole data.
                delete data;
            }
        }
    }
}

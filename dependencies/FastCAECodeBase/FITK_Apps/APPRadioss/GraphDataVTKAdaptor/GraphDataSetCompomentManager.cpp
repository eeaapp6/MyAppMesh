/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphDataSetCompomentManager.h"
#include "GraphVTKCommons.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractAssInstance.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurface.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceNode.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"

#include <vtkUnstructuredGrid.h>
#include <vtkExtractSelection.h>
#include <vtkSelection.h>
#include <vtkTransform.h>
#include <vtkAppendFilter.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>
#include <vtkPolygon.h>
#include <vtkTransformFilter.h>
#include <QDebug>
#include <QFile>
#include <QTextStream>
namespace Exchange
{
    //静态变量初始化
    GraphDataSetCompomentManager* GraphDataSetCompomentManager::_instance = nullptr;
    QMutex GraphDataSetCompomentManager::m_mutex;

    vtkUnstructuredGrid * GraphDataSetCompomentManager::getSetGridData(int setId, int instanceId, bool update)
    {
        Interface::FITKModelSet* set = FITKDATAREPO->getTDataByID<Interface::FITKModelSet>(setId);
        if (set == nullptr) {
            return nullptr;
        }

        //判断是否有装配,没有key值为模型id
        int keyId = instanceId;
        vtkUnstructuredGrid* setGrid = nullptr;
        if (keyId == -1){
            keyId = set->getAbsModelID();
        }

        //获取现有的集合数据
        if (_setGridHash.contains(setId)){
            setGrid = _setGridHash[setId][instanceId];
        }

        //判断现有的集合数据是否有效、是否需要刷新数据
        if (setGrid && !update){
            return setGrid;
        }

        //判断集合渲染数据是否需要创建
        bool hasCreated = setGrid != nullptr;

        //需要创建删除原来的集合数据
        if (hasCreated){
            deleteSetGridData(setId);
        }

        //创建集合渲染数据
        setGrid = generateSetGrid(set, instanceId);
        if (!setGrid)
        {
            return nullptr;
        }

        _setGridHash[setId][keyId] = setGrid;

        if (!hasCreated){
            connect(set, &Interface::FITKModelSet::dataObjectDestoried, this, [=] {
                QHash<int, vtkUnstructuredGrid*> & setHash = _setGridHash[setId];
                for (vtkUnstructuredGrid* grid : setHash.values()) {
                    if (grid) {
                        grid->Delete();
                    }
                }

                setHash.clear();
                _setGridHash.take(setId);
            }, Qt::UniqueConnection);
        }
        return setGrid;
    }

    vtkUnstructuredGrid * GraphDataSetCompomentManager::getSurfGridData(int surfId, int instanceId, bool update)
    {
        //检验数据有效性
        Interface::FITKMeshSurface* surface = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKMeshSurface>(surfId);
        if (surface == nullptr){
            return nullptr;
        }

        int keyId = instanceId;
        vtkUnstructuredGrid* surfaceGrid = nullptr;
        if (keyId == -1){
            //如果key（所属模型id）不存在，则获取默认所属模型id（一般为partId）
            keyId = surface->getAbsModelID();
        }

        //获取现有模型中已存在的表面数据
        if (_surfGridHash.contains(surfId)){
            surfaceGrid = _surfGridHash[surfId][keyId];
        }

        //如果现有模型中存在且不需要刷新，返回
        if (surfaceGrid && !update){
            return surfaceGrid;
        }

        //记录模型是否需要重新创建
        bool hasCreated = surfaceGrid != nullptr;

        //需要重新创建则删除原有的
        if (hasCreated){
            deleteSurfGridData(surfId);
        }

        //生成表面渲染数据
        surfaceGrid = generateSurfaceGrid(surface, instanceId);
        if (!surfaceGrid){
            return nullptr;
        }

        _surfGridHash[surfId][keyId] = surfaceGrid;

        if (!hasCreated){
            connect(surface, &Interface::FITKMeshSurface::dataObjectDestoried, this, [=]{
                QHash<int, vtkUnstructuredGrid*> & surfaceHash = _surfGridHash[surfId];
                for (vtkUnstructuredGrid* grid : surfaceHash.values()){
                    if (grid){
                        grid->Delete();
                    }
                }

                surfaceHash.clear();
                _surfGridHash.take(surfId);
            }, Qt::UniqueConnection);
        }
        return surfaceGrid;
        return nullptr;
    }

    void GraphDataSetCompomentManager::deleteSetGridData(int setId)
    {
        Interface::FITKModelSet* set = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKModelSet>(setId);
        if (set == nullptr){
            return;
        }

        if (!_setGridHash.contains(setId)){
            return;
        }

        QHash<int, vtkUnstructuredGrid*> subSetGridHash = _setGridHash.take(setId);
        for (vtkUnstructuredGrid* ugrid : subSetGridHash){
            if (ugrid){
                ugrid->Delete();
                ugrid = nullptr;
            }
        }
    }

    void GraphDataSetCompomentManager::deleteSurfGridData(int surfId)
    {
        Interface::FITKMeshSurface* surf = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKMeshSurface>(surfId);
        if (surf == nullptr) {
            return;
        }

        if (!_surfGridHash.contains(surfId)){
            return;
        }

        QHash<int, vtkUnstructuredGrid*> subSurfGridHash = _surfGridHash.take(surfId);
        for (vtkUnstructuredGrid* ugrid : subSurfGridHash){
            if (ugrid){
                ugrid->Delete();
                ugrid = nullptr;
            }
        }
    }
    int GraphDataSetCompomentManager::getPartNodeIdByRadiossNodes(Radioss::FITKRadiossPart* part)
    {
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!radiossCase) return -1;
        Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
        if (meshModel == nullptr) return -1;
        Radioss::FITKRadiossNodes* nodes = meshModel->getNodes();
        if (nodes == nullptr) return -1;
        Radioss::FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return -1;
        int index = partMgr->getDataIndex(part);
        if (index == -1) return -1;
        int partIdIndex = 0;
        for (int i = 0; i < index; i++)
        {
            Radioss::FITKRadiossPart* radiossPart = partMgr->getDataByIndex(i);
            QList<int> partNodes = radiossPart->getAllNodeIDs();
            partIdIndex += partNodes.count();
        }
        return partIdIndex;
    }
    void Exchange::GraphDataSetCompomentManager::initialize()
    {
        return;
    }

    void GraphDataSetCompomentManager::finalize()
    {
        return;
    }

    vtkUnstructuredGrid* GraphDataSetCompomentManager::generateSetGrid(Interface::FITKModelSet* set, int instanceId)
    {
        if (set == nullptr) {
            return nullptr;
        }
        //判断集合是否属于MeshModel
        Radioss::FITKRadiossMeshModel* meshModel = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossMeshModel>(set->getAbsModelID());
        if (!meshModel) return nullptr;

        instanceId = set->getAbsModelID();

        // 获取集合的类型
        Interface::FITKModelEnum::FITKModelSetType type = set->getModelSetType();

        // 获取集合信息
        const QList<int>& ids = set->getAbsoluteMember();
        vtkUnstructuredGrid* setDataSet = nullptr;
        bool isValid = false;

        if (!isValid && meshModel)
        {
            isValid = true;
            setDataSet = generateSetDataSetWithMesh(meshModel, type, ids);
        }
        return (isValid && setDataSet) ? setDataSet : nullptr;
    }

    vtkUnstructuredGrid * GraphDataSetCompomentManager::generateSetDataSetWithMesh(Radioss::FITKRadiossMeshModel * meshModel, int type, const QList<int>& ids)
    {
        if (meshModel == nullptr) return nullptr;

        // 获取全局数据和节点管理器
        Radioss::FITKRadiossNodes* radiossNodes = getGlobalNodes();
        if (!radiossNodes) return nullptr;
        Radioss::FITKRadiossPartManager* partMgr = meshModel->getPartsManager();
        if (partMgr == nullptr) return nullptr;
        // 创建VTK网格对象
        vtkUnstructuredGrid* grid = vtkUnstructuredGrid::New();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        grid->SetPoints(points);

        // 创建ID存储数组
        vtkSmartPointer<vtkIntArray> idArray = vtkSmartPointer<vtkIntArray>::New();
        idArray->SetName(GraphVTKCommons::_dataCellIDArratName.toUtf8().constData());
        idArray->SetNumberOfComponents(1);
        grid->GetCellData()->AddArray(idArray);

        // 根据集合类型处理
        if (type == Interface::FITKModelEnum::FMSNode) {
            // 为每个在ModelSet中的Part节点创建VTK点和顶点单元
            for (int i = 0; i < radiossNodes->getNodeCount(); i++) {
                // 检查节点是否在ModelSet中
                int nodeId = radiossNodes->getNodeIDByIndex(i);
                if (!ids.contains(nodeId)) continue;

                Core::FITKNode* node = radiossNodes->getNodeAt(i);
                if (node == nullptr) continue;

                // 获取节点坐标
                double coords[3] = { 0.0, 0.0, 0.0 };
                node->getCoor(coords);

                // 添加点到VTK
                int pointId = points->InsertNextPoint(coords);

                // 创建顶点单元
                vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
                cell->InsertNextId(pointId);
                int cellId = grid->InsertNextCell(VTK_VERTEX, cell);

                // 存储节点ID
                if (cellId != -1) {
                    idArray->InsertNextValue(nodeId);
                }
            }
        }
        else if (type == Interface::FITKModelEnum::FMSElem) {
            // 节点填充
            QHash<int, int> pointId_vtkId = {};
            for (int i = 0; i < radiossNodes->getNodeCount(); i++) {
                Core::FITKNode* node = radiossNodes->getNodeAt(i);
                if (node == nullptr) continue;
                double point[3] = { 0, 0, 0 };
                node->getCoor(point);
                //创建点
                int pointId = points->InsertNextPoint(point);
                pointId_vtkId.insert(node->getNodeID(), pointId);
            }
            // 处理单元
            for (int elemId : ids) {
                QPair<Radioss::FITKRadiossPart*, Interface::FITKAbstractElement*> partAndElement = partMgr->getElementByID(elemId);
                Interface::FITKAbstractElement* element = partAndElement.second;

                if (element == nullptr) continue;

                // 创建单元
                vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
                QList<int> elementNodes = element->getAllNodes();

                for (int nodeId : elementNodes) {
                    if (pointId_vtkId.contains(nodeId)) {
                        cell->InsertNextId(pointId_vtkId.value(nodeId));
                    }
                }

                // 根据单元类型创建VTK单元
                int cellId = -1;
                switch (element->getEleType()) {
                case Interface::FITKModelEnum::EleNone: break;
                case Interface::FITKModelEnum::Line2:
                    cellId = grid->InsertNextCell(VTK_LINE, cell);
                    break;
                case Interface::FITKModelEnum::Line3:
                    cellId = grid->InsertNextCell(VTK_QUADRATIC_EDGE, cell);
                    break;
                case Interface::FITKModelEnum::Tri3:
                    cellId = grid->InsertNextCell(VTK_TRIANGLE, cell);
                    break;
                case Interface::FITKModelEnum::Tri6:
                    cellId = grid->InsertNextCell(VTK_QUADRATIC_TRIANGLE, cell);
                    break;
                case Interface::FITKModelEnum::Quad4:
                    cellId = grid->InsertNextCell(VTK_QUAD, cell);
                    break;
                case Interface::FITKModelEnum::Quad8:
                    cellId = grid->InsertNextCell(VTK_QUADRATIC_QUAD, cell);
                    break;
                case Interface::FITKModelEnum::Tet4:
                    cellId = grid->InsertNextCell(VTK_TETRA, cell);
                    break;
                case Interface::FITKModelEnum::Tet10:
                    cellId = grid->InsertNextCell(VTK_QUADRATIC_TETRA, cell);
                    break;
                case Interface::FITKModelEnum::Wedge6:
                    cellId = grid->InsertNextCell(VTK_WEDGE, cell);
                    break;
                case Interface::FITKModelEnum::Hex8:
                    cellId = grid->InsertNextCell(VTK_HEXAHEDRON, cell);
                    break;
                case Interface::FITKModelEnum::Hex20:
                    cellId = grid->InsertNextCell(VTK_QUADRATIC_HEXAHEDRON, cell);
                    break;
                case Interface::FITKModelEnum::Polygon:
                    cellId = grid->InsertNextCell(VTK_POLYGON, cell);
                    break;
                }

                // 存储单元ID
                if (cellId != -1) {
                    idArray->InsertNextValue(element->getEleID());
                }
            }
        }
        return grid;
    }

    vtkUnstructuredGrid* GraphDataSetCompomentManager::generateSurfaceGrid(Interface::FITKMeshSurface* surface, int instanceId)
    {
        Interface::FITKMeshSurfaceNode* nodeSurf = dynamic_cast<Interface::FITKMeshSurfaceNode*>(surface);
        Radioss::FITKRadiossSurfaceSeg* seg = dynamic_cast<Radioss::FITKRadiossSurfaceSeg*>(surface);
        if(nodeSurf) {
            return generateNodeSurface(nodeSurf, instanceId);
        }
        else if (seg)
        {
            return generateElementSurfaceSeg(seg, instanceId);
        }

        return nullptr;
    }
    vtkUnstructuredGrid* GraphDataSetCompomentManager::generateNodeSurface(Interface::FITKMeshSurfaceNode* nodeSurf, int instanceId)
    {
        if (nodeSurf == nullptr)
        {
            return nullptr;
        }

        // 创建VTK网格对象
        vtkUnstructuredGrid* grid = vtkUnstructuredGrid::New();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        grid->SetPoints(points);

        // 获取全局节点
        Radioss::FITKRadiossNodes* radiossNodes = getGlobalNodes();
        if (!radiossNodes)
        {
            grid->Delete();
            return nullptr;
        }
        QList<int> nodeIds;
        //获取表面所有集合
        for(int setIndex = 0; setIndex < nodeSurf->getSetCount(); setIndex++)
        {
            Interface::FITKModelSet* set = nodeSurf->getSetAt(setIndex);
            if (set == nullptr)
            {
                continue;
            }
            //获取集合信息
            nodeIds.append(set->getAbsoluteMember());   
        }

        // 遍历节点ID，创建VTK点和顶点单元
        for (int nodeId : nodeIds)
        {
            Core::FITKNode* node = radiossNodes->getNodeByID(nodeId);
            if (node == nullptr)
            {
                continue;
            }

            // 获取节点坐标
            double coords[3] = { 0.0, 0.0, 0.0 };
            node->getCoor(coords);

            // 添加点到VTK
            vtkIdType pointId = points->InsertNextPoint(coords);

            // 创建顶点单元
            vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
            cell->InsertNextId(pointId);
            grid->InsertNextCell(VTK_VERTEX, cell);
        }
        return grid;
    }

    vtkUnstructuredGrid* GraphDataSetCompomentManager::generateElementSurfaceSeg(Radioss::FITKRadiossSurfaceSeg* eleSurfSeg, int instanceId)
    {
        if (eleSurfSeg == nullptr) return nullptr;

        //创建网格对象
        vtkUnstructuredGrid* gridData = vtkUnstructuredGrid::New();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        gridData->SetPoints(points);
        gridData->Reset();
        points->Reset();

        Radioss::FITKRadiossNodes* radiossNodes = getGlobalNodes();
        if (!radiossNodes) return nullptr;

        QHash<int, vtkIdType> nodeIdToPointId; // 用于跟踪已添加的节点ID和VTK点ID

        //获取所有表面单元
        QList<int> surfElements = eleSurfSeg->getAllElement();
        for (int index = 0; index < surfElements.size(); index++)
        {
            const int elementId = surfElements.at(index);
            //获取表面节点
            QList<QList<int>> surfaceList = eleSurfSeg->getSurface(elementId);
            if (surfaceList.isEmpty()) continue;

            //添加面单元的节点
            for (QList<int> nodeIds : surfaceList)
            {
                vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
                for (const int nodeId : nodeIds)
                {
                    vtkIdType pointId;
                    if (nodeIdToPointId.contains(nodeId))
                    {
                        // 如果节点已经存在，直接获取点ID
                        pointId = nodeIdToPointId.value(nodeId);
                    }
                    else
                    {
                        // 如果节点不存在，则创建新点并存储其ID
                        Core::FITKNode* node = radiossNodes->getNodeByID(nodeId);
                        if (node == nullptr) continue;
                        double point[3] = { 0,0,0 };
                        node->getCoor(point);
                        //创建点
                        pointId = points->InsertNextPoint(point);
                        nodeIdToPointId.insert(nodeId, pointId);
                    }
                    cell->InsertNextId(pointId);
                }

                // 根据面节点数量确定VTK单元类型
                int vtkCellType = VTK_POLYGON; // 默认使用多边形
                const int numNodes = nodeIds.size();
                switch (numNodes) {
                case 3:
                    vtkCellType = VTK_TRIANGLE;
                    break;
                case 4:
                    vtkCellType = VTK_QUAD;
                    break;
                case 6:
                    vtkCellType = VTK_QUADRATIC_TRIANGLE;
                    break;
                case 8:
                    vtkCellType = VTK_QUADRATIC_QUAD;
                    break;
                default:
                    vtkCellType = VTK_POLYGON;
                    break;
                }

                gridData->InsertNextCell(vtkCellType, cell);
            }
        }
        return gridData;
    }

    Radioss::FITKRadiossNodes* GraphDataSetCompomentManager::getGlobalNodes()
    {
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!radiossCase) return nullptr;
        Radioss::FITKRadiossMeshModel* radiossMeshModel = radiossCase->getMeshModel();
        if (!radiossMeshModel) return nullptr;
        Radioss::FITKRadiossNodes* radiossNodes = radiossMeshModel->getNodes();
        if (!radiossNodes) return nullptr;
        return radiossNodes;
    }

}
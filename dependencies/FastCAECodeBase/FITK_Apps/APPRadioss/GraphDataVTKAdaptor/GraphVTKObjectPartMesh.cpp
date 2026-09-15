/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectPartMesh.h"
#include "GraphDataSetCompomentManager.h"
#include "GraphVTKObjectHighlighting.h"
#include "GraphDataVTKPartSetManager.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKMeshFeatureVTK.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKShellFeatureEdges.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"

#include <QVector>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkCellData.h>
#include <vtkProperty.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtkThreshold.h>
#include <vtkDataSetAlgorithm.h>
#include <vtkMapper.h>
namespace Exchange {
    GraphVTKObjectPartMesh::GraphVTKObjectPartMesh(Radioss::FITKRadiossPart* dataObj) :
        GraphVTKObjectModelBase(dataObj)
    {
        if (dataObj == nullptr) return;
        initActor();
        generateGraph(dataObj);
        updateGraphProperty();
    }
    GraphVTKObjectPartMesh::~GraphVTKObjectPartMesh()
    {
    }
    void GraphVTKObjectPartMesh::generateGraph(Radioss::FITKRadiossPart* part)
    {
        PartMeshDataVTK* partData{ nullptr };
        int partID = part->getDataObjectID();
        bool isTrue = GraphDataVTKPartSetManager::getInstance()->getPartMeshData(partID, partData);
        if (!isTrue || partData == nullptr) return;

        if (partData->_meshFeature)
        {
            FITKSurfaceFilter* surfaceFilter = partData->_meshFeature->getSurfaceFilter();
            FITKShellFeatureEdges* featureEdges = partData->_meshFeature->getFeatureEdgeFilter();
            if (surfaceFilter == nullptr) return;

            _modelFilter = surfaceFilter;
            _modelEdgeFilter = featureEdges;

            _faceActor->setInputConnection(partData->_filterSurface->GetOutputPort());
            addActor(_faceActor);

            if (m_graphObjHighlight)
            {
                m_graphObjHighlight->setModelInputConnection(surfaceFilter->GetOutputPort());
            }

            _surfPointActor->setInputConnection(partData->_filterSurfVert->GetOutputPort());
            addActor(_surfPointActor);

            _pointActor->setInputConnection(partData->_filterVertex->GetOutputPort());
            addActor(_pointActor);
        }
        else
        {
            vtkUnstructuredGrid* grid = vtkUnstructuredGrid::SafeDownCast(partData->_meshData);
            _pointActor->setInputDataObject(grid);
            addActor(_pointActor);
            if (m_graphObjHighlight)
            {
                m_graphObjHighlight->setModelInputDataObject(partData->_meshData);
                m_graphObjHighlight->setActorType(ActorType::VertexActor);
                m_graphObjHighlight->setVertexSize(5);
            }
        }
    }
    void GraphVTKObjectPartMesh::initActor()
    {
        Radioss::FITKRadiossPart* part = dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj);
        if (part == nullptr) return;
        QColor color = part->getColor();
        this->setRenderLayer(0);

        _pointActor = FITKGraphActor::New();
        _pointActor->setActorType(ActorType::VertexActor); //点类型
        _pointActor->SetPickable(false);
        _pointActor->setGraphObject(this);
        _pointActor->SetVisibility(true);
        _pointActor->GetProperty()->SetPointSize(5);
        _pointActor->GetProperty()->SetOpacity(1);
        _pointActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphVTKCommons::_mesh3DGraphOffset);

        _faceActor = FITKGraphActor::New();
        _faceActor->setActorType(ActorType::SurfaceActor); //面类型
        _faceActor->SetPickable(false);
        _faceActor->setGraphObject(this);
        _faceActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
        _faceActor->GetProperty()->SetOpacity(1);
        _faceActor->GetProperty()->SetEdgeVisibility(true);
        _faceActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphVTKCommons::_mesh3DGraphOffset);
        setLightPropertiesForSurface(_faceActor);

        _surfPointActor = FITKGraphActor::New();
        _surfPointActor->setActorType(ActorType::VertexActor); //表面点类型
        _surfPointActor->SetPickable(false);
        _surfPointActor->setGraphObject(this);
        _surfPointActor->SetVisibility(false);
        _surfPointActor->GetProperty()->SetPointSize(5);
        _surfPointActor->GetProperty()->SetOpacity(1);
        _surfPointActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphVTKCommons::_mesh3DGraphOffset);
    }

    bool GraphVTKObjectPartMesh::updateGraphData(bool forceUpdate)
    {
        Radioss::FITKRadiossPart* part = dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj);
        if (part == nullptr) return false;
        PartMeshDataVTK* partData{ nullptr };
        bool isTrue = GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData, forceUpdate);
        if (!partData || !isTrue) return false;

        if (_faceActor)
            _faceActor->update();
        if (_pointActor)
            _pointActor->update();
        return true;
    }
    void GraphVTKObjectPartMesh::updateGraphProperty()
    {
        AppFrame::FITKAppSettings* settings = FITKAPP->getAppSettings();
        if (!settings) return;
        QVariant meshLineWidth = settings->getVarient("GraphSetting/MeshLineWidth");
        if (!meshLineWidth.isValid())
            meshLineWidth = 1;
        QVariant meshPointSize = settings->getVarient("GraphSetting/MeshPointSize");
        if (!meshPointSize.isValid())
            meshPointSize = 5;
        QVariant meshSHPSize = settings->getVarient("GraphSetting/MeshSPHSize");
        if (!meshSHPSize.isValid())
            meshSHPSize = 5;
        QVariant meshPointColor = settings->getVarient("GraphSetting/MeshPointColor");
        if (!meshPointColor.isValid())
            meshPointColor = QColor(255, 255, 255);
        QVariant meshLineColor = settings->getVarient("GraphSetting/MeshLineColor");
        if (!meshLineColor.isValid())
            meshLineColor = QColor(0, 0, 0);
        updateLineWidth(meshLineWidth.value<double>());
        updatePointSize(meshPointSize.value<double>());
        updateSHPSize(meshSHPSize.value<int>());
        updateSubColor(meshPointColor.value<QColor>(), 0);
        updateSubColor(meshLineColor.value<QColor>(), 1);
    }

    void GraphVTKObjectPartMesh::update(bool forceUpdate)
    {
        //更新可见
        updateVisibility();

        if (forceUpdate == false)
        {
            return;
        }
        updateGraphData(forceUpdate);
        generateGraph(dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj));
    }
    void GraphVTKObjectPartMesh::updateVisibility()
    {
        if (!getVisibility())
        {
            if (_faceActor)
                _faceActor->SetVisibility(false);
            if (_pointActor)
                _pointActor->SetVisibility(false);
            return;
        }

        //获取运行时设置
        AppFrame::FITKAppSettings* runSetting = FITKAPP->getAppSettings();

        //获取点、线、面的显示状态
        QVariant pointVisible = runSetting->getVarient("GraphSetting/MeshPointVisible");
        QVariant lineVisible = runSetting->getVarient("GraphSetting/MeshLineVisible");
        QVariant partVisible = runSetting->getVarient("GraphSetting/PartVisible");

        bool bPointVisible = pointVisible.toBool();
        bool bLineVisible = lineVisible.toBool();
        bool bPartVisible = partVisible.toBool();

        if (_faceActor)
        {
            Radioss::FITKRadiossPart* part = dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj);
            if (part)
            {
                QColor partColor = part->getColor();
                QVariant meshLineColorVar = runSetting->getVarient("GraphSetting/MeshLineColor");
                QColor lineColor = meshLineColorVar.value<QColor>();

                if (bPartVisible)
                {
                    _faceActor->SetVisibility(true);
                    _faceActor->GetProperty()->SetRepresentationToSurface();
                    _faceActor->GetProperty()->SetEdgeVisibility(bLineVisible);
                    // 当面可见时，使用模型自身的颜色
                    _faceActor->GetProperty()->SetColor(partColor.redF(), partColor.greenF(), partColor.blueF());
                }
                else if (bLineVisible)
                {
                    _faceActor->SetVisibility(true);
                    _faceActor->GetProperty()->SetRepresentationToWireframe();
                    // 当只有线框可见时，使用线的颜色作为模型颜色
                    _faceActor->GetProperty()->SetColor(lineColor.redF(), lineColor.greenF(), lineColor.blueF());
                }
                else
                {
                    _faceActor->SetVisibility(false);
                }
            }
        }

        if (_pointActor)
        {
            _pointActor->SetVisibility(bPointVisible);
        }
    }
    void GraphVTKObjectPartMesh::updateSubColor(QColor color, QVariant otherPar)
    {
        if (otherPar.isValid()) {
            int element = otherPar.toInt();
            switch (element) {
            case 0:
                if (_pointActor)
                    _pointActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
                break;
            case 1:
                if (_faceActor)
                    _faceActor->GetProperty()->SetEdgeColor(color.redF(), color.greenF(), color.blueF());
                break;
            default:
                break;
            }
        }
        // 当颜色更新时，刷新显示状态以确保正确的颜色应用
        updateVisibility();
    }
    void GraphVTKObjectPartMesh::updateModelColor(QColor color)
    {
        // 检查对象是否有效
        Radioss::FITKRadiossPart* part = dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj);
        if (part == nullptr) return;
        part->setColor(color);
        // 更新单元演员颜色
        if (_faceActor != nullptr) {
            _faceActor->GetProperty()->SetOpacity(color.alphaF());
        }
        // 颜色更新后，调用updateVisibility来根据当前状态正确设置actor颜色
        updateVisibility();
    }
    void GraphVTKObjectPartMesh::updatePointSize(int size)
    {
        if (_pointActor)
            _pointActor->GetProperty()->SetPointSize(size);
    }
    void GraphVTKObjectPartMesh::updateLineWidth(int width)
    {
        if (_faceActor)
            _faceActor->GetProperty()->SetLineWidth(width);
    }
    void GraphVTKObjectPartMesh::updateSHPSize(int size)
    {
        if (_faceActor)
            _faceActor->GetProperty()->SetPointSize(size);
    }
    void GraphVTKObjectPartMesh::setVisible(bool visibility)
    {
        if (visibility == false)
        {
            if (_faceActor)
                _faceActor->SetVisibility(false);
            if (_pointActor)
                _pointActor->SetVisibility(false);
            if (_surfPointActor)
                _surfPointActor->SetVisibility(false);
        }
        else
        {
            updateVisibility();
        }
    }
    void GraphVTKObjectPartMesh::setCellVisible(bool visibility)
    {
        if (!getVisibility()) return;

        updateVisibility();
    }
    void GraphVTKObjectPartMesh::setPointVisibility(bool visibility)
    {
        if (!getVisibility()) return;

        updateVisibility();
    }
    void GraphVTKObjectPartMesh::setLineVisibility(bool visibility)
    {
        if (!getVisibility()) return;

        updateVisibility();
    }
    void GraphVTKObjectPartMesh::setPickMode(GraphVTKCommons::ShapePickMode mode)
    {
        auto setActorPickable = [=](FITKGraphActor* fActor, bool isOn) {
            if (fActor) {
                fActor->SetPickable(isOn);
            }
            };

        switch (mode) {
        case GraphVTKCommons::PickMeshVertex:
            setActorPickable(_pointActor, true);
            setActorPickable(_faceActor, false);
            setActorPickable(_surfPointActor, false);
            break;
        case GraphVTKCommons::PickMeshCell:
            setActorPickable(_pointActor, false);
            setActorPickable(_faceActor, true);
            setActorPickable(_surfPointActor, false);
            break;
        case GraphVTKCommons::PickMeshCellSurf:
            setActorPickable(_pointActor, false);
            setActorPickable(_faceActor, true);
            setActorPickable(_surfPointActor, false);
            break;
        case GraphVTKCommons::PickMeshVertexSurf:
            setActorPickable(_pointActor, false);
            setActorPickable(_faceActor, false);
            setActorPickable(_surfPointActor, true);
            _surfPointActor->SetVisibility(true);
            break;
        default:
            setActorPickable(_pointActor, false);
            setActorPickable(_faceActor, false);
            setActorPickable(_surfPointActor, false);
            _surfPointActor->SetVisibility(false);
            break;
        }

        //记录拾取类型
        GraphVTKObject3D::setPickMode(mode);
    }
    int GraphVTKObjectPartMesh::getShapeIdByVTKCellId(int vtkCellId, GraphVTKCommons::ShapePickMode pickType, QVariant otherInfo)
    {
        int shapeID = -1;
        Radioss::FITKRadiossPart* part = dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj);
        if (part == nullptr) return false;
        PartMeshDataVTK* partData{ nullptr };
        bool isTrue = GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!partData || !isTrue) return false;

        switch (pickType)
        {
        case GraphVTKCommons::PickMeshVertex: {
            vtkIntArray* idsArray = vtkIntArray::SafeDownCast(partData->_meshData->GetPointData()
                ->GetArray(NODEID_BY_VTKID_ARRAY_NAME));
            shapeID = idsArray->GetValue(vtkCellId);
            break;
        }
        case GraphVTKCommons::PickMeshCell: {
            FITKSurfaceFilter* surfaceFilter = partData->_meshFeature->getSurfaceFilter();
            vtkIntArray* idsArray = vtkIntArray::SafeDownCast(partData->_meshData->GetCellData()
                ->GetArray(VTKID_BY_NODEID_ARRAY_NAME));
            int orgCellIndex = -1;
            int faceIndex = -1;
            surfaceFilter->getSurfaceCellInfo(vtkCellId, orgCellIndex, faceIndex);
            shapeID = idsArray->GetValue(orgCellIndex);
            break;
        }
        case GraphVTKCommons::PickMeshVertexSurf: {
            FITKSurfaceFilter* surfaceFilter = partData->_meshFeature->getSurfaceFilter();
            vtkIntArray* idsArray = vtkIntArray::SafeDownCast(partData->_meshData->GetPointData()
                ->GetArray(NODEID_BY_VTKID_ARRAY_NAME));
            int orgPointIndex = surfaceFilter->getOriginPointIdBySurfacePointId(vtkCellId);

            shapeID = idsArray->GetValue(orgPointIndex);
            break;
        }
        }

        return shapeID;
    }
    const QVector<int> GraphVTKObjectPartMesh::getVTKCellIdsByShapeId(int shapeId, GraphVTKCommons::ShapePickMode pickType, QVariant otherInfo)
    {
        QVector<int> vtkIds;
        Radioss::FITKRadiossPart* part = dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj);
        if (part == nullptr) return vtkIds;
        PartMeshDataVTK* partData{ nullptr };
        bool isTrue = GraphDataVTKPartSetManager::getInstance()->getPartMeshData(part->getDataObjectID(), partData);
        if (!partData || !isTrue) return vtkIds;

        switch (pickType)
        {
        case GraphVTKCommons::PickMeshVertex:
        case GraphVTKCommons::PickMeshVertexSurf: {
            int vtkid = partData->_nodeIdToPointIndex.value(shapeId, -1);
            if (vtkid >= 0)
            {
                vtkIds.append(vtkid);
            }
            break;
        }
        }
        return vtkIds;
    }
    bool GraphVTKObjectPartMesh::getVisibility()
    {
        Radioss::FITKRadiossPart* part = dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj);
        if (part == nullptr) {
            return false;
        }

        return this->Superclass::getVisibility() && part->isEnable();
    }
}
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectModel.h"
#include "GraphDataVTKPartSetManager.h"
#include "GraphVTKObjectHighlighting.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshVS.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshEntity.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandProp.h"

#include <vtkObject.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include <vtkCellType.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPolygon.h>

namespace Exchange
{
    GraphVTKObjectModel::GraphVTKObjectModel(Interface::FITKAbsGeoCommand* geoCommand)
        : GraphVTKObjectModelBase(geoCommand)
    {
        if (!geoCommand) {
            return;
        }

        // 生成可视化数据
        if (!generateDataSet()) {
            return;
        }

        // 生成可视化对象
        generateGraph();
        updateGraphProperty();
    }

    GraphVTKObjectModel::~GraphVTKObjectModel()
    {

    }

    void GraphVTKObjectModel::setPickMode(GraphVTKCommons::ShapePickMode mode)
    {
        // Lambda for set actor pickable.
        auto setActorPickable = [=](FITKGraphActor* fActor, bool isOn) {
            if (fActor) {
                fActor->SetPickable(isOn);
            }
            };

        switch (mode) {
        case GraphVTKCommons::PickNone:
            setActorPickable(_pointActor, false);
            setActorPickable(_lineActor, false);
            setActorPickable(_faceActor, false);
            setActorPickable(_solidActor, false);
            break;
        case GraphVTKCommons::PickGeoVertex:
            setActorPickable(_pointActor, true);
            setActorPickable(_lineActor, false);
            setActorPickable(_faceActor, false);
            setActorPickable(_solidActor, false);
            break;
        case GraphVTKCommons::PickGeoEdge:
            setActorPickable(_pointActor, false);
            setActorPickable(_lineActor, true);
            setActorPickable(_faceActor, false);
            setActorPickable(_solidActor, false);
            break;
        case GraphVTKCommons::PickGeoFace:
            setActorPickable(_pointActor, false);
            setActorPickable(_lineActor, false);
            setActorPickable(_faceActor, true);
            setActorPickable(_solidActor, false);
            break;
        case GraphVTKCommons::PickGeoSolid:
            setActorPickable(_pointActor, false);
            setActorPickable(_lineActor, false);
            setActorPickable(_faceActor, false);
            setActorPickable(_solidActor, true);
            break;
        default:
            return;
        }

        //记录拾取类型
        GraphVTKObject3D::setPickMode(mode);
    }

    int GraphVTKObjectModel::getShapeIdByVTKCellId(int vtkCellId, GraphVTKCommons::ShapePickMode pickType, QVariant otherInfo)
    {
        int id = -1;
        if (vtkCellId < 0) {
            return id;
        }

        // 从管理器中获取当前数据对象的信息
        GraphDataVTKPartSetManager* partSetManager = GraphDataVTKPartSetManager::getInstance();
        Interface::FITKAbsGeoCommand* geoCommand = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (geoCommand == nullptr) return id;

        GraphDataVTKPartSetInfo* geoInfo = partSetManager->getGeoCommandInfo(geoCommand->getDataObjectID());
        if (geoInfo == nullptr) return id;

        switch (pickType) {
        case GraphVTKCommons::ShapePickMode::PickGeoSolid: id = geoInfo->_solid_VTKId_CellID.value(vtkCellId); break;
        case GraphVTKCommons::ShapePickMode::PickGeoFace: id = geoInfo->_face_VTKId_CellID.value(vtkCellId); break;
        case GraphVTKCommons::ShapePickMode::PickGeoEdge: id = geoInfo->_line_VTKId_CellID.value(vtkCellId); break;
        case GraphVTKCommons::ShapePickMode::PickGeoVertex: id = geoInfo->_point_VTKId_CellID.value(vtkCellId); break;
        }
        return id;
    }

    const QVector<int> GraphVTKObjectModel::getVTKCellIdsByShapeId(int shapeId, GraphVTKCommons::ShapePickMode pickType, QVariant otherInfo)
    {
        QVector<int> cellIds;

        // 从管理器中获取当前数据对象的信息
        GraphDataVTKPartSetManager* partSetManager = GraphDataVTKPartSetManager::getInstance();
        Interface::FITKAbsGeoCommand* geoCommand = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (geoCommand == nullptr) return cellIds;

        GraphDataVTKPartSetInfo* geoInfo = partSetManager->getGeoCommandInfo(geoCommand->getDataObjectID());
        if (geoInfo == nullptr) return cellIds;

        switch (pickType) {
        case GraphVTKCommons::ShapePickMode::PickGeoSolid: {
            QList<int> vtkIDs = geoInfo->_solid_VTKId_CellID.keys(shapeId);
            for (int vtkID : vtkIDs) {
                cellIds.append(vtkID);
            }
            break;
        }
        case GraphVTKCommons::ShapePickMode::PickGeoFace: {
            QList<int> vtkIDs = geoInfo->_face_VTKId_CellID.keys(shapeId);
            for (int vtkID : vtkIDs) {
                cellIds.append(vtkID);
            }
            break;
        }
        case GraphVTKCommons::ShapePickMode::PickGeoEdge: {
            cellIds.append(geoInfo->_line_VTKId_CellID.key(shapeId));
            break;
        }
        case GraphVTKCommons::ShapePickMode::PickGeoVertex: {
            cellIds.append(geoInfo->_point_VTKId_CellID.key(shapeId));
            break;
        }
        }

        return cellIds;
    }

    int GraphVTKObjectModel::getVirTopoIndexByShapeId(int shapeId, GraphVTKCommons::ShapePickMode pickType)
    {
        // 获取当前数据对象的信息
        GraphDataVTKPartSetManager* partSetManager = GraphDataVTKPartSetManager::getInstance();
        Interface::FITKAbsGeoCommand* geoCommand = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (geoCommand == nullptr) return -1;
        GraphDataVTKPartSetInfo* geoInfo = partSetManager->getGeoCommandInfo(geoCommand->getDataObjectID());
        if (geoInfo == nullptr) return -1;

        switch (pickType) {
        case GraphVTKCommons::PickGeoVertex:
            return geoInfo->_pointVirTopoIdToIndex.value(shapeId, -1);
        case GraphVTKCommons::PickGeoEdge:
            return geoInfo->_lineVirTopoIdToIndex.value(shapeId, -1);
        case GraphVTKCommons::PickGeoFace:
            return geoInfo->_faceVirTopoIdToIndex.value(shapeId, -1);
        case GraphVTKCommons::PickGeoSolid:
            return geoInfo->_solidVirTopoIdToIndex.value(shapeId, -1);
        default:
            return -1;
        }
    }

    vtkDataSet* GraphVTKObjectModel::getMesh(GraphVTKCommons::ShapePickMode pickType)
    {
        // 从管理器中获取当前数据对象的信息
        GraphDataVTKPartSetManager* partSetManager = GraphDataVTKPartSetManager::getInstance();
        Interface::FITKAbsGeoCommand* geoCommand = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (geoCommand == nullptr) return nullptr;

        GraphDataVTKPartSetInfo* geoInfo = partSetManager->getGeoCommandInfo(geoCommand->getDataObjectID());
        if (geoInfo == nullptr) return nullptr;

        switch (pickType) {
        case GraphVTKCommons::ShapePickMode::PickGeoVertex:
            return geoInfo->_pointGrid;
        case GraphVTKCommons::ShapePickMode::PickGeoEdge:
            return geoInfo->_lineGrid;
        case GraphVTKCommons::ShapePickMode::PickGeoFace:
            return geoInfo->_faceGrid;
        case GraphVTKCommons::ShapePickMode::PickGeoSolid:
            return geoInfo->_solidGrid;
        }
        return nullptr;
    }

    void GraphVTKObjectModel::update(bool forceUpdate)
    {
        updateVisibility();
        if (forceUpdate == true) {
            generateDataSet();
        }
    }

    void GraphVTKObjectModel::setVisible(bool visibility)
    {
        if (_pointActor == nullptr || _lineActor == nullptr || _faceActor == nullptr || _solidActor == nullptr)return;
        if (visibility == false) {
            _pointActor->SetVisibility(visibility);
            _lineActor->SetVisibility(visibility);
            _faceActor->SetVisibility(visibility);
            _solidActor->SetVisibility(visibility);
        }
        else
        {
            updateVisibility();
        }

    }

    void GraphVTKObjectModel::setCellVisible(bool visibility)
    {
        if (!getVisibility()) return;
        _faceActor->SetVisibility(visibility);
        _solidActor->SetVisibility(visibility);
    }

    void GraphVTKObjectModel::setPointVisibility(bool visibility)
    {
        if (!getVisibility()) return;
        _pointActor->SetVisibility(visibility);
    }

    void GraphVTKObjectModel::setLineVisibility(bool visibility)
    {
        if (!getVisibility()) return;
        _lineActor->SetVisibility(visibility);
    }

    bool GraphVTKObjectModel::getVisibility()
    {
       //获取数据可见性
        bool isEnable = this->Superclass::getVisibility();
        //判断引用关系可见性
        bool beReferenced = false;
        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (cmd == nullptr) return false;
        //是否开启引用计数状态
        beReferenced = cmd->getEnableReferenceList();
        //判断是否是被删除获取被编辑状态
        beReferenced &= cmd->getCommandStatus() != Interface::FITKGeoEnum::FITKGeoStatus::FGSDeleted &&
            cmd->getCommandStatus() != Interface::FITKGeoEnum::FITKGeoStatus::FGSModified;
        //获取当前命令是被引用数量
        beReferenced &= (cmd->getReferencedCmdCount() == 0);
        return beReferenced &= isEnable;
    }

    void GraphVTKObjectModel::updateVisibility()
    {
        bool isVisibility = getVisibility();
        AppFrame::FITKAppSettings* runSetting = FITKAPP->getAppSettings();
        //获取点显示状态
        QVariant visible = true;
        visible = runSetting->getVarient("GraphSetting/GeoPointVisible");
        if(!visible.isValid())
            visible = true;
        _pointActor->SetVisibility(isVisibility && visible.value<bool>());
        //获取线显示状态
        visible = runSetting->getVarient("GraphSetting/GeoLineVisible");
        if(!visible.isValid())
            visible = true;
        _lineActor->SetVisibility(isVisibility && visible.value<bool>());
        //获取几何显示状态
        visible = runSetting->getVarient("GraphSetting/GeoVisible");
        if(!visible.isValid())
            visible = true;
        _faceActor->SetVisibility(isVisibility && visible.value<bool>());
        _solidActor->SetVisibility(isVisibility && visible.value<bool>());
    }

    void GraphVTKObjectModel::updateSubColor(QColor color, QVariant otherPar)
    {
        if (otherPar.isValid()) {
            int element = otherPar.toInt();
            switch (element) {
            case 0:
                if (_pointActor)
                    _pointActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
                break;
            case 1:
                if (_lineActor)
                    _lineActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
                break;
            default:
                break;
            }
        }
    }

    void Exchange::GraphVTKObjectModel::updateModelColor(QColor color)
    {
        if (_faceActor)
        _faceActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
        if (_solidActor)
        _solidActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
    }
    void GraphVTKObjectModel::updatePointSize(int size)
    {
        if (_pointActor)
            _pointActor->GetProperty()->SetPointSize(size);
    }
    void GraphVTKObjectModel::updateLineWidth(int width)
    {
        if (_lineActor)
            _lineActor->GetProperty()->SetLineWidth(width);
    }
    bool GraphVTKObjectModel::generateDataSet()
    {
        Interface::FITKAbsGeoCommand* geoCommand = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        GraphDataVTKPartSetManager* partSetManager = GraphDataVTKPartSetManager::getInstance();
        if (geoCommand == nullptr || partSetManager == nullptr) return false;

        int geoCommandId = geoCommand->getDataObjectID();
        partSetManager->updateGeoCommand(geoCommandId);
        return true;
    }

    void GraphVTKObjectModel::generateGraph()
    {
        Interface::FITKAbsGeoCommand* geoCommand = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        GraphDataVTKPartSetManager* partSetManager = GraphDataVTKPartSetManager::getInstance();
        if (geoCommand == nullptr || partSetManager == nullptr) return;
        Interface::FITKGeoCommandProp* porp = geoCommand->getCommandProp();
        if (porp == nullptr) return;
        QColor color = porp->getColor();
        int geoCommandId = geoCommand->getDataObjectID();
        GraphDataVTKPartSetInfo* geoInfo = partSetManager->getGeoCommandInfo(geoCommandId);
        if (geoInfo == nullptr) return;
        this->setRenderLayer(0);

        //获取运行时设置
        AppFrame::FITKAppSettings* runSetting = FITKAPP->getAppSettings();
        //获取点显示状态
        QVariant visible = true;
        visible = runSetting->getVarient("GraphSetting/GeoPointVisible");
        if(!visible.isValid())
            visible = true;

        // 创建点演员
        _pointActor = FITKGraphActor::New();
        // 演员中存储渲染对象
        _pointActor->setGraphObject(this);
        _pointActor->setActorType(ActorType::VertexActor);
        _pointActor->SetPickable(false);
        _pointActor->SetVisibility(visible.value<bool>());
        _pointActor->GetProperty()->SetPointSize(5);
        _pointActor->GetProperty()->SetOpacity(1);
        _pointActor->GetProperty()->SetColor(255,255,255);
        _pointActor->setInputDataObject(geoInfo->_pointGrid);
        // 设置偏移参数
        _pointActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphVTKCommons::_geoGraphOffset);
        addActor(_pointActor);

        //获取线显示状态
        visible = runSetting->getValue<bool>("GraphSetting/GeoLineVisible");
        if(!visible.isValid())
            visible = true;
        // 创建线演员
        _lineActor = FITKGraphActor::New();
        // 演员中存储渲染对象
        _lineActor->setGraphObject(this);
        _lineActor->setActorType(ActorType::EdgeActor);
        _lineActor->SetPickable(false);
        _lineActor->SetVisibility(visible.value<bool>());
        _lineActor->GetProperty()->SetLineWidth(2);
        //QColor lineColor = GraphVTKCommons::_geoEdgeColor;
        _lineActor->GetProperty()->SetColor(0,0,0);
        _lineActor->setInputDataObject(geoInfo->_lineGrid);
        // 设置偏移参数
        _lineActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphVTKCommons::_geoGraphOffset);
        addActor(_lineActor);

        //获取几何显示状态
        visible = runSetting->getValue<bool>("GraphSetting/GeoVisible");
        if(!visible.isValid())
            visible = true;
        // 创建面演员
        _faceActor = FITKGraphActor::New();
        // 演员中存储渲染对象
        _faceActor->setGraphObject(this);
        _faceActor->setActorType(ActorType::SurfaceActor);
        _faceActor->SetPickable(false);
        _faceActor->SetVisibility(visible.value<bool>());
        _faceActor->GetProperty()->SetRepresentation(2);
        _faceActor->GetProperty()->SetEdgeVisibility(false);
        _faceActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
        _faceActor->setInputDataObject(geoInfo->_faceGrid);
        // 设置偏移参数
        _faceActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphVTKCommons::_geoGraphOffset);
        addActor(_faceActor);

        // 创建体演员
        _solidActor = FITKGraphActor::New();
        // 演员中存储渲染对象
        _solidActor->setGraphObject(this);
        _solidActor->setActorType(ActorType::SolidActor);
        _solidActor->SetPickable(false);
        _solidActor->SetVisibility(visible.value<bool>());
        _solidActor->GetProperty()->SetRepresentation(2);
        _solidActor->GetProperty()->SetEdgeVisibility(false);
        _solidActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
        _solidActor->setInputDataObject(geoInfo->_solidGrid);
        //设置偏移参数
        _solidActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphVTKCommons::_geoGraphOffset);
        addActor(_solidActor);

        m_graphObjHighlight->setModelInputDataObject(geoInfo->_lineGrid);
        m_graphObjHighlight->setActorType(2); //边界高亮
    }
    void GraphVTKObjectModel::updateGraphProperty()
    {
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        if (!appSettings) return;
        QVariant pointSize = appSettings->getVarient("GraphSetting/GeoPointSize");
        QVariant lineWidth = appSettings->getVarient("GraphSetting/GeoLineWidth");
        QVariant pointColor = appSettings->getVarient("GraphSetting/GeoPointColor");
        QVariant lineColor = appSettings->getVarient("GraphSetting/GeoLineColor");
        updatePointSize(pointSize.value<double>());
        updateLineWidth(lineWidth.value<double>());
        updateSubColor(pointColor.value<QColor>(), 0);
        updateSubColor(lineColor.value<QColor>(), 1);
    }
}   // namespace Exchange

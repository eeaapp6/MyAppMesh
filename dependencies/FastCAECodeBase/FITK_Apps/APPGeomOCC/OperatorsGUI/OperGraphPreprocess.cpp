/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGraphPreprocess.h"

// OCC
#include <TopoDS_Shape.hxx>

// APP
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoReferencePlane.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"

// Graph widget and object
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObject3D.h"
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObjectModelBase.h"

// Graph data manager
#include "GraphDataProvider/GraphProviderManager.h"
#include "GraphDataProvider/GraphModelProvider.h"
#include "GraphDataProvider/GraphMarkerProvider.h"

// Pick data manager
#include "PickDataProvider/ContextMenuHandler.h"
#include "PickDataProvider/GraphPickedDataProvider.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphInteractionOperator.h"

namespace GUIOper
{
    void OperGraphPreprocess::initialize3D()
    {
        // 初始化基准平面。
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return;
        }

        // 获取基准元素管理器。
        Interface::FITKDatumList* datumMgr = cmdList->getDatumManager();
        if (!datumMgr)
        {
            return;
        }

        // XOY.
        Interface::FITKAbsGeoReferenceThreePointsPlane* xoy = Interface::FITKInterfaceGeometryFactory::getInstance()->createDatumT<Interface::FITKAbsGeoReferenceThreePointsPlane>
            (Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane, false);
        xoy->setDataObjectName("Plane XOY");
        xoy->setPoint(0, { 0., 0., 0. });
        xoy->setPoint(1, { 0., 1., 0. });
        xoy->setPoint(2, { 1., 0., 0. });      
        xoy->update();
        datumMgr->appendDataObj(xoy);

        // YOZ.
        Interface::FITKAbsGeoReferenceThreePointsPlane* yoz = Interface::FITKInterfaceGeometryFactory::getInstance()->createDatumT<Interface::FITKAbsGeoReferenceThreePointsPlane>
            (Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane, false);
        yoz->setDataObjectName("Plane YOZ");
        yoz->setPoint(0, { 0., 0., 0. });
        yoz->setPoint(1, { 0., 0., 1. });
        yoz->setPoint(2, { 0., 1., 0. });
        yoz->update();
        datumMgr->appendDataObj(yoz);

        // ZOX.
        Interface::FITKAbsGeoReferenceThreePointsPlane* zox = Interface::FITKInterfaceGeometryFactory::getInstance()->createDatumT<Interface::FITKAbsGeoReferenceThreePointsPlane>
            (Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane, false);
        zox->setDataObjectName("Plane ZOX");
        zox->setPoint(0, { 0., 0., 0. });
        zox->setPoint(1, { 1., 0., 0. });
        zox->setPoint(2, { 0., 0., 1. });    
        zox->update();
        datumMgr->appendDataObj(zox);

        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        //// 初始化背景颜色。
        //float topColorF3[3]{ 1.f, 1.f, 1.f };
        //float btmColorF3[3]{ 0.90196f, 0.90196f, 0.95686f };
        //graphWidget->setBackgroundColor(topColorF3, btmColorF3);

        // 初始化拾取数据管理器。
        GraphData::GraphPickedDataProvider::getInstance();

        // 初始化三维窗口右键菜单处理器。
        GraphEvent::ContextMenuHandler::GetInstance();

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);

        // 获取工具标识可视化对象管理器。
        GraphData::GraphMarkerProvider* markProvider = GRAPH_PROVIDER_MGR->getMarkerProvider(graphWidget);
        if (!modelProvider || !markProvider)
        {
            return;
        }
        
        QList<Exchange::FITKOCCGraphObject3D*> objs;
        
        // 初始化基准元素对象并添加至窗口内。
        objs = modelProvider->getAllDatumGraphObjects(true);
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            if (obj)
            {
                addGraphObjectToWidget(obj, graphWidget, false);
            }
        }

        // 初始化拾取工具对象并添加至窗口内。
        objs = markProvider->getAllPickToolGraphObjects();
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            if (obj)
            {
                addGraphObjectToWidget(obj, graphWidget, false);

                // 隐藏对象。
                obj->setVisible(false);
            }
        }

        // 初始化默认基准元素状态。
        EventOper::GraphInteractionOperator* operPick = FITKOPERREPO->getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");
        if (operPick)
        {
            operPick->setCustomPickMode();
        }
    }

    void OperGraphPreprocess::updateGraph(int dataObjId, bool forceUpdate)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取或创建可视化对象。
        QList<Exchange::FITKOCCGraphObject3D*> objs = getGraphObjectsByDataId(dataObjId);

        // 添加至三维窗口。
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            if (!obj)
            {
                continue;
            }

            obj->update(forceUpdate);

            addGraphObjectToWidget(obj, graphWidget, false);
        }
    }

    void OperGraphPreprocess::updateGraphs()
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        QList<Exchange::FITKOCCGraphObject3D*> objs;

        // 模型 & 2D草图可视化。
        objs << modelProvider->getAllCommandGraphObjects(true);

        // 基准元素可视化。
        objs << modelProvider->getAllDatumGraphObjects(true);

        // 添加至可视化窗口。
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            if (obj)
            {
                addGraphObjectToWidget(obj, graphWidget, false);

                obj->update();
            }
        }
    }

    void OperGraphPreprocess::updateGraphRecursively(int id, int refLayer, bool recDown)
    {
        // 初始化前处理操作器。
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!operGraph)
        {
            return;
        }

        // 检查数据ID。
        Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(id);
        if (!cmd)
        {
            return;
        }

        // 更新可视化对象。
        operGraph->updateGraph(id);

        // 更新引用数据对象。
        if (refLayer != 0)
        {
            int subLayer = refLayer == -1 ? -1 : refLayer - 1;

            if (recDown)
            {
                int nRefedCmd = cmd->getReferencedCmdCount();
                for (int i = 0; i < nRefedCmd; i++)
                {
                    Interface::FITKAbsGeoCommand* cmdRef = cmd->getReferencedCmdByIndex(i);
                    if (!cmdRef)
                    {
                        continue;
                    }

                    // 递归更新。
                    updateGraphRecursively(cmdRef->getDataObjectID(), subLayer, recDown);
                }
            }
            else
            {
                int nRefCmd = cmd->getReferenceCmdCount();
                for (int i = 0; i < nRefCmd; i++)
                {
                    Interface::FITKAbsGeoCommand* cmdRef = cmd->getReferenceCmdByIndex(i);
                    if (!cmdRef)
                    {
                        continue;
                    }

                    // 递归更新。
                    updateGraphRecursively(cmdRef->getDataObjectID(), subLayer, recDown);
                }
            }
        }
    }

    void OperGraphPreprocess::updateGraphVisibility(int dataObjId)
    {
        // 获取或创建可视化对象。
        QList<Exchange::FITKOCCGraphObject3D*> objs = getGraphObjectsByDataId(dataObjId);

        // 刷新可见性。
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            if (!obj)
            {
                continue;
            }

            obj->updateVisibility();
        }
    }

    void OperGraphPreprocess::preview(int dataObjId, EventOper::PreviewType type, QColor color)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 检查数据仓库是否存在该数据。
        Core::FITKAbstractDataObject* dataObj = FITKDATAREPO->getDataByID(dataObjId);
        if (!dataObj)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        Exchange::FITKOCCGraphObject3D* obj{ nullptr };
        bool isValid = false;

        // 检查数据ID是否为基准元素。（必须先于模型判断）
        Interface::FITKAbsGeoDatum* datum = dynamic_cast<Interface::FITKAbsGeoDatum*>(dataObj);
        if (datum && !isValid)
        {
            obj = modelProvider->getDatumPreviewGraphObject(dataObjId);
            isValid = true;

            // 切换显示模式。
            if (obj)
            {
                if (!color.isValid())
                {
                    color = QColor(255, 150, 100);
                }

                obj->setColor(color);
                obj->setViewMode(Exchange::FITKOCCCommons::ShapeMeshViewMode::SMVM_Shade, true);
            }
        }

        // 检查数据ID是否为模型。（模型预览默认显示整个数据对象）
        Interface::FITKAbsGeoCommand* model = dynamic_cast<Interface::FITKAbsGeoCommand*>(dataObj);
        if (model && !isValid)
        {
            obj = modelProvider->getModelPreviewGraphObject(dataObjId);
            isValid = true;
        }

        // 检查数据ID是否为虚拓扑。（根据预览类型分配适配器）
        Interface::FITKAbsVirtualTopo* vTopo = dynamic_cast<Interface::FITKAbsVirtualTopo*>(dataObj);
        if (vTopo && !isValid)
        {
            switch (type)
            {
            case EventOper::PT_Default:
            {
                // 预览虚拓扑形状。
                obj = modelProvider->getTopoShapePreviewGraphObject(dataObjId);
                isValid = true;

                break;
            }
            case EventOper::PT_IndexLabels:
            {
                // 线/面/体/组合体虚拓扑。（预览点/线/面索引）
                if (vTopo->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSEdge ||
                    vTopo->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSFace ||
                    vTopo->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSSolid ||
                    vTopo->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSAssembly)
                {
                    obj = modelProvider->getTopoLabelPreviewGraphObject(dataObjId);
                    isValid = true;
                }

                break;
            }
            case EventOper::PT_Direction:
            {
                // 线/面虚拓扑。（预览线/面方向）
                if (vTopo->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSEdge ||
                    vTopo->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSFace)
                {
                    obj = modelProvider->getTopoDirectionPreviewGraphObject(dataObjId);
                    isValid = true;
                }

                break;
            }
            default:
                break;
            }
        }

        if (!obj)
        {
            return;
        }

        // 修改可视化属性。
        obj->setColor(color);
        obj->update();

        // 添加到三维窗口。
        addGraphObjectToWidget(obj, graphWidget, false);
    }

    void OperGraphPreprocess::clearPreview(int dataObjId)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        modelProvider->clearPreviewObject(dataObjId);
    }

    void OperGraphPreprocess::showGlobalComponent(int dataObjId)
    {
        // 先清除其余几何分组显示。
        clearGlobalComponent();

        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        // 获取几何分组可视化对象。
        Exchange::FITKOCCGraphObject3D* obj = modelProvider->getGlobalComponentGraphObject(dataObjId);
        if (!obj)
        {
            return;
        }

        // 添加至窗口。
        addGraphObjectToWidget(obj, graphWidget, false);
        obj->setVisible(true);
    }

    void OperGraphPreprocess::clearGlobalComponent()
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        // 获取全部分组可视化对象。
        QList<Exchange::FITKOCCGraphObject3D*> objs = modelProvider->getGlobalComponentGraphObjects();

        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            // 移出窗口。
            if (obj)
            {
                obj->setVisible(false);
                obj->removeFromGraphWidget();
            }
        }
    }

    void OperGraphPreprocess::setDatumsVisibility(bool visible)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        // 获取全部基准元素可视化对象。
        QList<Exchange::FITKOCCGraphObject3D*> objs = modelProvider->getAllDatumGraphObjects(true);

        // 添加至三维窗口。
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            if (!obj)
            {
                continue;
            }

            addGraphObjectToWidget(obj, graphWidget, false);

            obj->setVisible(visible);
            obj->update();
        }
    }

    void OperGraphPreprocess::setSketchTopMost(int dataObjId)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        // 获取全部或单个草图对象。
        QList<Exchange::FITKOCCGraphObject3D*> objs;
        if (dataObjId == -1)
        {
            objs << modelProvider->getAllSkecth2DGraphObjects();
        }
        else
        {
            objs << modelProvider->getSketch2DGraphObject(dataObjId);
        }
        
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            if (obj)
            {
                obj->enableTopMost(true);
            }
        }
    }

    void OperGraphPreprocess::clearSketchTopMost()
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        // 取消置顶。
        QList<Exchange::FITKOCCGraphObject3D*> objs = modelProvider->getAllSkecth2DGraphObjects();
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            if (obj)
            {
                obj->enableTopMost(false);
            }
        }
    }

    void OperGraphPreprocess::setShapeTransparency(int cmdId, int geomType, QList<int> geoIndice, double trans)
    {
        // 获取几何命令管理器。
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return;
        }

        Interface::FITKAbsGeoCommand* cmd = cmdList->getDataByID(cmdId);

        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!cmd || !graphWidget)
        {
            return;
        }

        // 获取拓扑管理器。
        Interface::FITKShapeVirtualTopoManager* vTopoMgr = cmd->getShapeVirtualTopoManager((Interface::FITKGeoEnum::VTopoShapeType)geomType);
        if (!vTopoMgr)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        // 获取几何可视化对象。
        Exchange::FITKOCCGraphObject3D* obj = modelProvider->getModelGraphObject(cmdId);
        Exchange::FITKOCCGraphObjectModelBase* modelObj = Exchange::FITKOCCGraphObjectModelBase::SafeDownCast(obj);
        if (!modelObj)
        {
            return;
        }

        // 设置几何透明度。
        for (const int & geoIndex : geoIndice)
        {
            Interface::FITKAbsVirtualTopo* vTopo = vTopoMgr->getDataByIndex(geoIndex);
            if (!vTopo)
            {
                continue;
            }

            OCC::FITKOCCTopoShape* occShape = vTopo->getShapeT<OCC::FITKOCCTopoShape>();
            if (!occShape)
            {
                continue;
            }

            const TopoDS_Shape & shape = occShape->getTopoShape();
            if (shape.IsNull())
            {
                continue;
            }

            modelObj->setSubShapeTransparency(shape, trans);
        }
    }

    void OperGraphPreprocess::setShapeHidden(int cmdId, int geomType, QList<int> geoIndice)
    {
        // 获取几何命令管理器。
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return;
        }

        Interface::FITKAbsGeoCommand* cmd = cmdList->getDataByID(cmdId);

        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!cmd || !graphWidget)
        {
            return;
        }

        // 获取几何管理器。
        Interface::FITKShapeVirtualTopoManager* vTopoMgr = cmd->getShapeVirtualTopoManager((Interface::FITKGeoEnum::VTopoShapeType)geomType);
        if (!vTopoMgr)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        // 获取几何可视化对象。
        Exchange::FITKOCCGraphObject3D* obj = modelProvider->getModelGraphObject(cmdId);
        Exchange::FITKOCCGraphObjectModelBase* modelObj = Exchange::FITKOCCGraphObjectModelBase::SafeDownCast(obj);
        if (!modelObj)
        {
            return;
        }

        // 隐藏几何。
        for (const int & geoIndex : geoIndice)
        {
            Interface::FITKAbsVirtualTopo* vTopo = vTopoMgr->getDataByIndex(geoIndex);
            if (!vTopo)
            {
                continue;
            }

            OCC::FITKOCCTopoShape* occShape = vTopo->getShapeT<OCC::FITKOCCTopoShape>();
            if (!occShape)
            {
                continue;
            }

            const TopoDS_Shape & shape = occShape->getTopoShape();
            if (shape.IsNull())
            {
                continue;
            }

            modelObj->setSubShapeHidden(shape);
        }
    }

    void OperGraphPreprocess::clearShapeFaceTransparency()
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取模型可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (!modelProvider)
        {
            return;
        }

        // 取消透明度。
        QList<Exchange::FITKOCCGraphObject3D*> objs = modelProvider->getAllModelGraphObjects();
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            Exchange::FITKOCCGraphObjectModelBase* modelObj = Exchange::FITKOCCGraphObjectModelBase::SafeDownCast(obj);
            if (modelObj)
            {
                modelObj->clearSubShapeTransparency();
            }
        }
    }

    void OperGraphPreprocess::highlight(int dataObjId, QVariant info, QColor color)
    {
        Q_UNUSED(color);

        //// 获取可视化窗口。
        //Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        //if (!graphWidget)
        //{
        //    return;
        //}

        // 全部高亮。
        QList<Exchange::FITKOCCGraphObject3D*> objs = getGraphObjectsByDataId(dataObjId);
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            if (!obj)
            {
                continue;
            }

            obj->highlight();
        }
    }

    void OperGraphPreprocess::clearHighlight()
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取模型与符号可视化对象管理器，并清除高亮。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        if (modelProvider)
        {
            modelProvider->clearHighlight();
        }

        // 清除几何分组高亮。
        clearGlobalComponent();
    }

    void OperGraphPreprocess::reRender(bool fitView)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        if (fitView)
        {
            graphWidget->fitView();
        }
    }

    bool OperGraphPreprocess::enableSketchMode(bool isOn, int virtualTopoID)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget) return false;

        if (!isOn)
        {
            return graphWidget->enableSketchMode(isOn);
        }

        //判断草绘模式拾取的是基准面还是几何面
        //几何面
        Interface::FITKAbsVirtualTopo* topoObject = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(virtualTopoID);
        //基准面
        Interface::FITKAbsGeoDatumPlane* datumPlane = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoDatumPlane>(virtualTopoID);

        auto geoTools = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator();
        if (geoTools == nullptr) return false;
        auto faceTool = geoTools->createFaceTool();
        if (faceTool == nullptr) return false;

        if (topoObject != nullptr) //几何面
        {
            double pos[3], dir[3], updir[3];
            if (!faceTool->getPlaneByID(virtualTopoID, pos, dir, updir)) return false;
            return graphWidget->enableSketchMode(isOn, pos, dir, updir);
        }
        else if (datumPlane != nullptr) //基准面
        {
            double pos[3], dir[3], updir[3];
            datumPlane->getPosition(pos);
            datumPlane->getNormal(dir);
            datumPlane->getUp(updir);
            return graphWidget->enableSketchMode(isOn, pos, dir, updir);
        }

        return false;
    }

    bool OperGraphPreprocess::enableSketchMode(bool isOn, double* pos, double* nor, double* up)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget) return false;

        if (!isOn)
        {
            return graphWidget->enableSketchMode(isOn);
        }

        return graphWidget->enableSketchMode(isOn, pos, nor, up);
    }
}  // namespace GUIOper

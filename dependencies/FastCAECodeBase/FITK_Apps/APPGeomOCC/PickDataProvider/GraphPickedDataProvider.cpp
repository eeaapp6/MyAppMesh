/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphPickedDataProvider.h"
#include "GraphPickedData.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/GraphToolOperator.h"
#include "OperatorsInterface/GraphInteractionOperator.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentFactory.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"

// Render
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCCInterface.h"
#include "FITK_Component/FITKOCCGraphAdaptor/FITKAIS_ObjectBase.h"
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObject3D.h"

// GUI
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"

// PickInfo
#include "GUIPickInfo.h"

// OCC
#include <BRepAlgoAPI_Common.hxx>
#include <TopoDS.hxx>
#include <TopTools_ShapeMapHasher.hxx>
#include <TopoDS_Edge.hxx>
#include <Standard_Handle.hxx>
#include <BRepTools.hxx>
#include <AIS_InteractiveContext.hxx>
#include <TopExp_Explorer.hxx>
#include <ShapeFix_Shell.hxx>

// QT
#include <QSet>

namespace GraphData
{
    // 静态变量初始化。
    GraphPickedDataProvider* GraphPickedDataProvider::s_instance{ nullptr };

    GraphPickedDataProvider* GraphPickedDataProvider::getInstance()
    {
        // 获取实例。
        if (!s_instance)
        {
            s_instance = new GraphPickedDataProvider;
        }

        return s_instance;
    }

    void GraphPickedDataProvider::Delete()
    {
        if (s_instance)
        {
            delete s_instance;
        }

        s_instance = nullptr;
    }

    GraphPickedDataProvider::GraphPickedDataProvider()
    {
        //初始化调用
        init();
    }

    GraphPickedDataProvider::~GraphPickedDataProvider()
    {
        for (GraphPickedData* item : m_pickedDataList)
        {
            delete item;
        }
        m_pickedDataList.clear();
    }

    void GraphPickedDataProvider::init()
    {
        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKGLODATA->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow) return;

        // 获取可视化区。
        GUI::RenderWidget* renderWidget = mainWindow->getRenderWidget();
        if (!renderWidget) return;

        // 获取当前窗口，尝试转换为三维窗口。
        auto* w = renderWidget->getGraph3DWidget();
        m_graphWidget = dynamic_cast<Render::FITKGraph3DWindowOCC*>(w);
        if (m_graphWidget == nullptr) return;
        m_context = m_graphWidget->getContext();
        if (m_context == nullptr) return;
        connect(m_graphWidget, &Render::FITKGraph3DWindowOCC::sig_picked, this, &GraphPickedDataProvider::updatePickData, Qt::ConnectionType::UniqueConnection);
    }

    void GraphPickedDataProvider::updatePickData()
    {
        if (m_context == nullptr || m_graphWidget == nullptr) return;

        // 清除历史拾取数据。
        clearPickedData(false);

        bool updateFlag = false;

        // 获取拾取类型信息。
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        switch (pickInfo._pickObjType)
        {
        case GUI::GUIPickInfo::PickObjType::POBJCustom: // 右键菜单支持。
        case GUI::GUIPickInfo::PickObjType::POBJVert:
        case GUI::GUIPickInfo::PickObjType::POBJEdge:
        case GUI::GUIPickInfo::PickObjType::POBJWire:
        case GUI::GUIPickInfo::PickObjType::POBJFace:
        case GUI::GUIPickInfo::PickObjType::POBJShell:
        case GUI::GUIPickInfo::PickObjType::POBJSolid:
        case GUI::GUIPickInfo::PickObjType::POBJAssembly:
        case GUI::GUIPickInfo::PickObjType::POBJShape:
        {
            // 处理拾取工具。
            updatePickInfo();

            // 处理模型拾取。
            updateFlag = updatePickShapes();
            if (updateFlag)
            {
                // 发送拾取信号。
                emit sig_dataPicked();
            }

            break;
        }
        case GUI::GUIPickInfo::PickObjType::POBJDirecEdge:
        case GUI::GUIPickInfo::PickObjType::POBJDirecFace:
        case GUI::GUIPickInfo::PickObjType::POBJEdgeProjPos:
        case GUI::GUIPickInfo::PickObjType::POBJFaceProjPos:
        {
            // 处理拾取工具。
            updatePickInfo();
            break;
        }
        default:
            break;
        }

        if (!updateFlag)
        {
            // 清除拾取高亮。
            m_graphWidget->clearSelection();
        }
    }

    bool GraphPickedDataProvider::updatePickShapes()
    {
        QVector<TopoDS_Shape> shapes;
        m_graphWidget->getSelectedShapes(shapes);

        Interface::FITKGeoEnum::VTopoShapeType vST = Interface::FITKGeoEnum::VTopoShapeType::VSNone;

        //获取拾取方式及附加信息。
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();

        // 检查拾取类型，Custom默认拾取模式下拾取到多种不同类型则清除拾取数据。
        bool hasSelectedGeo = false;
        bool hasSelectedDatum = false;

        for (m_context->InitSelected(); m_context->MoreSelected(); m_context->NextSelected())
        {
            // 获取拾取数据类型。
            AIS_InteractiveObject* obj = m_context->SelectedInteractive().get();
            if (!obj)
            {
                continue;
            }

            // 获取OCC对象类。
            AIS_KindOfInteractive objType = obj->Type();
            FITKAIS_ObjectBase* fitkObj = dynamic_cast<FITKAIS_ObjectBase*>(obj);
            if (!fitkObj)
            {
                continue;
            }

            // 判断对象类型。（跳过非基准元素与模型）
            if (objType != AIS_KindOfInteractive::AIS_KOI_Datum && objType != AIS_KindOfInteractive::AIS_KOI_Shape)
            {
                continue;
            }

            // 获取拾取对象信息。
            GraphInfo info;
            info.Type = (GraphDataType)fitkObj->getGraphObject()->getGraphInfo().Type;
            info.DataObjId = fitkObj->getDataObjectId();

            // 获取虚拓扑管理器。
            Exchange::FITKOCCGraphObject3D* gObject = fitkObj->getGraphObject();
            if (gObject == nullptr) continue;
            Interface::FITKAbsGeoCommand* commd = gObject->getDataObjAs<Interface::FITKAbsGeoCommand>();
            if (commd == nullptr) continue;
            Interface::FITKAbsGeoShapeAgent* agent = commd->getShapeAgent();
            if (agent == nullptr) continue;
            Interface::FITKVirtualTopoManager* topoManager = agent->getVirtualTopoManager();
            if (topoManager == nullptr) continue;

            // 拾取到的形状虚拓扑。
            Interface::FITKAbsVirtualTopo* vTopo = nullptr;

            switch (info.Type)
            {
            case DatumPointGraph:
            {
                // 获取第一个点虚拓扑。
                Interface::FITKShapeVirtualTopoManager* vTopoMgr = topoManager->getShapeVirtualTopoManager
                (Interface::FITKGeoEnum::VTopoShapeType::VSPoint);
                vTopo = vTopoMgr->getDataByIndex(0);

                hasSelectedDatum = true;

                break;
            }
            case DatumLineGraph:
            {
                // 获取第一个线虚拓扑。
                Interface::FITKShapeVirtualTopoManager* vTopoMgr = topoManager->getShapeVirtualTopoManager
                (Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
                vTopo = vTopoMgr->getDataByIndex(0);

                hasSelectedDatum = true;

                break;
            }
            case DatumPlaneGraph:
            {
                // 获取第一个面虚拓扑。
                Interface::FITKShapeVirtualTopoManager* vTopoMgr = topoManager->getShapeVirtualTopoManager
                (Interface::FITKGeoEnum::VTopoShapeType::VSFace);
                vTopo = vTopoMgr->getDataByIndex(0);

                hasSelectedDatum = true;

                break;
            }
            case ModelGraph:
            case SketchGraph:
            {
                // 模型与草图获取拾取形状查找虚拓扑对象。
                auto selectedShape = m_context->SelectedShape();
                switch (selectedShape.ShapeType())
                {
                case TopAbs_ShapeEnum::TopAbs_VERTEX:
                    vST = Interface::FITKGeoEnum::VTopoShapeType::VSPoint;
                    break;
                case TopAbs_ShapeEnum::TopAbs_EDGE:
                    vST = Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
                    break;
                case TopAbs_ShapeEnum::TopAbs_WIRE:
                    vST = Interface::FITKGeoEnum::VTopoShapeType::VSWire;
                    break;
                case TopAbs_ShapeEnum::TopAbs_FACE:
                    vST = Interface::FITKGeoEnum::VTopoShapeType::VSFace;
                    break;
                case TopAbs_ShapeEnum::TopAbs_SHELL:
                    vST = Interface::FITKGeoEnum::VTopoShapeType::VSShell;
                    break;
                case TopAbs_ShapeEnum::TopAbs_SOLID:
                    vST = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
                    break;
                case TopAbs_ShapeEnum::TopAbs_COMPOUND:
                    vST = Interface::FITKGeoEnum::VTopoShapeType::VSAssembly;
                    break;
                default: return false;
                };

                vTopo = topoManager->getVirtualTopo(&selectedShape, vST);

                hasSelectedGeo = true;

            } break;
            default: break;
            }

            // 检查虚拓扑对象。
            if (!vTopo)
            {
                continue;
            }

            // 创建拾取数据对象。
            GraphPickedData* data = getPickedDataByCmdId(commd->getDataObjectID());
            if (data)
            {
                data->appendVirtualTopo(vTopo);
            }
            else
            {
                if (hasSelectedDatum)
                {
                    data = new GraphPickedData(pickInfo, commd->getDataObjectID());
                }
                else
                {
                    data = new GraphPickedData(pickInfo, agent, vTopo);
                }

                m_pickedDataList.append(data);
            }

            data->setGraphInfo(info);
            data->setGraphObject(fitkObj->getGraphObject());
        }

        // Custom模式拾取数据类型校验。
        if (pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJCustom
            && hasSelectedGeo && hasSelectedDatum)
        {
            clearPickedData();
        }

        return true;
    }

    bool GraphPickedDataProvider::updatePickInfo()
    {
        // 获取可视化对象操作器。
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        EventOper::GraphToolOperator* operTool = FITKOPERREPO->getOperatorT<EventOper::GraphToolOperator>("GraphTool");
        EventOper::GraphInteractionOperator* operPick = FITKOPERREPO->getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");
        if (!operGraph || !operTool || !operPick || !m_context || !m_graphWidget)
        {
            return false;
        }

        //// 查找是否拾取到对象。
        //bool hasSelect = m_context->HasSelectedShape();
        //if (!hasSelect)
        //{
        //    return false;
        //}

        // 判断是否为拾取方向。
        if (m_context->NbSelected() == 1)
        {
            for (m_context->InitSelected(); m_context->MoreSelected(); m_context->NextSelected())
            {
                Handle(AIS_InteractiveObject) interObj = m_context->SelectedInteractive();
                FITKAIS_ObjectBase* shapeObj = dynamic_cast<FITKAIS_ObjectBase*>(interObj.get());
                if (!shapeObj)
                {
                    break;
                }

                Exchange::FITKOCCGraphObject3D* obj = shapeObj->getGraphObject();
                if (!obj)
                {
                    break;
                }

                // 根据不同类型执行操作。
                Exchange::FITKOCCCommons::GraphDataType type = obj->getGraphInfo().Type;
                switch (type)
                {
                case Exchange::FITKOCCCommons::PickToolDirection:
                {
                    // 拾取到方向对象则翻转方向。
                    int role = ROLE_INPUT_PICK_DIRECTION_REVERSE;
                    obj->setUserData(role, !obj->getUserDataAs<bool>(role));
                    obj->disHighlight();
                    obj->update();

                    // 发送拾取信号。
                    emit sig_directionPicked();

                    return true;
                }
                default:
                    break;
                }
            }
        }

        // 获取拾取类型信息。
        GraphPickedData* data{ nullptr };
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        switch (pickInfo._pickObjType)
        {
        case GUI::GUIPickInfo::PickObjType::POBJEdgeProjPos:
        case GUI::GUIPickInfo::PickObjType::POBJFaceProjPos:
        {
            // 拾取投影坐标如果额外拾取到形状，默认放于开头。
            updatePickShapes();

            Exchange::FITKOCCGraphObject3D* obj3d = operTool->getPickToolObject(EventOper::PickToolType::PTT_Proj_Pos);
            data = new GraphPickedData(pickInfo, obj3d);

            // 发送拾取信号。
            emit sig_projPosPicked();

            break;
        }
        default:
            return false;
        }

        if (!data)
        {
            return false;
        }

        // 添加至拾取信息表。
        m_pickedDataList.push_back(data);

        return true;
    }

    GraphPickedData* GraphPickedDataProvider::getPickedDataByCmdId(int cmdId)
    {
        // 遍历数据获取命令ID。
        for (GraphPickedData* pickData : m_pickedDataList)
        {
            if (pickData && pickData->getGraphInfo().DataObjId == cmdId)
            {
                return pickData;
            }
        }

        return nullptr;
    }

    void GraphPickedDataProvider::clearPickedData(bool clearSelecttion)
    {
        if (clearSelecttion && m_graphWidget)
        {
            m_graphWidget->clearSelection();
        }

        for (GraphPickedData* item : m_pickedDataList)
        {
            delete item;
        }

        m_pickedDataList.clear();

        // 预览对象 操作器
        auto graphPreprocessOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper == nullptr) return;

        //清除所有高亮
        graphPreprocessOper->clearHighlight();
    }

    bool GraphPickedDataProvider::isPickStatus()
    {
        // 获取拾取方式及附加信息。
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        if (pickInfo._pickObjType != GUI::GUIPickInfo::PickObjType::POBJNone) return true;
        return false;
    }

    QList<GraphPickedData*> GraphPickedDataProvider::getPickedList()
    {
        return m_pickedDataList;
    }

    void GraphPickedDataProvider::removeSelected(int cmdId, Interface::FITKGeoEnum::VTopoShapeType sType, int vTopoIndex)
    {
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return;
        }

        Interface::FITKAbsGeoCommand* cmd = cmdList->getDataByID(cmdId);
        if (!cmd)
        {
            return;
        }

        // 获取虚拓扑对象以及形状数据。
        Interface::FITKAbsVirtualTopo* vTopo = cmd->getVirtualTopoByIndex(sType, vTopoIndex);
        if (!vTopo)
        {
            return;
        }

        OCC::FITKOCCTopoShape* vsTopo = vTopo->getShapeT<OCC::FITKOCCTopoShape>();
        if (!vsTopo)
        {
            return;
        }

        // 获取OCC形状。
        const TopoDS_Shape & shape = vsTopo->getTopoShape();

        // 移除选中。
        m_graphWidget->removeSelectedShape(shape);
    }
}

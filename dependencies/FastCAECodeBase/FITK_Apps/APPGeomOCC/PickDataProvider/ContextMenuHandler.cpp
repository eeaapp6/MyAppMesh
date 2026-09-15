/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ContextMenuHandler.h"

// Qt
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QDialog>
#include <QMetaEnum>

// OCC
#include <AIS_InteractiveContext.hxx>

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKGUIRepo.h"

// Render
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoMultiDelete.h"

// Graph
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObject3D.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"

// GUI
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"

// Pick data
#include "GraphPickedDataProvider.h"
#include "GraphPickedData.h"
#include "GUIPickInfo.h"

namespace GraphEvent
{
    // 初始化实例。
    ContextMenuHandler* ContextMenuHandler::s_instance{ nullptr };

    ContextMenuHandler::ContextMenuHandler()
    {
        // 绑定可视化窗口右键信号。
        init();

        // 初始化菜单。
        initMenu();
    }

    ContextMenuHandler::~ContextMenuHandler()
    {
        auto deleteAction = [&](QAction* & act)
        {
            if (act)
            {
                delete act;
                act = nullptr;
            }
        };

        // 析构右键菜单。
        if (m_menu)
        {
            delete m_menu;
            m_menu = nullptr;
        }

        // 析构按钮。
        deleteAction(m_actDelete);
        deleteAction(m_actEdit);
        deleteAction(m_actHide);
        deleteAction(m_actFitSelection); 
    }

    ContextMenuHandler* ContextMenuHandler::GetInstance()
    {
        // 实例化。
        if (!s_instance)
        {
            s_instance = new ContextMenuHandler;
        }

        return s_instance;
    }

    void ContextMenuHandler::init()
    {
        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKGLODATA->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow)
        {
            return;
        }

        // 获取可视化区。
        GUI::RenderWidget* renderWidget = mainWindow->getRenderWidget();
        if (!renderWidget)
        {
            return;
        }

        // 获取当前窗口，尝试转换为三维窗口。
        m_graphWidget = dynamic_cast<Render::FITKGraph3DWindowOCC*>(renderWidget->getGraph3DWidget());
        if (!m_graphWidget)
        {
            return;
        }

        // 获取OCC交互上下文。
        m_context = m_graphWidget->getContext();

        // 绑定窗口菜单信号。
        connect(m_graphWidget, &Render::FITKGraph3DWindowOCC::sig_contextMenu,
            this, &ContextMenuHandler::slot_windowContextMenu, Qt::UniqueConnection);
    }

    void ContextMenuHandler::initMenu()
    {
        // 初始化菜单。
        m_menu = new QMenu;

        // 初始化功能按钮。
        //@{
        // 编辑。
        m_actEdit = m_menu->addAction(tr("Edit"), this, &ContextMenuHandler::slot_actEdit);
        m_actEdit->setObjectName("actionContextMenuEdit");

        // 隐藏。
        m_actHide = m_menu->addAction(tr("Hide"), this, &ContextMenuHandler::slot_actHide);

        // 删除。
        m_actDelete = m_menu->addAction(tr("Remove Selected Object(s)"), this, &ContextMenuHandler::slot_actDelete);

        m_menu->addSeparator();

        // 重置视角。
        m_menu->addAction(QIcon(":/app/icons/toolbars/view/view_fit.svg"), tr("Fit All"), this, [=] 
        {
            if (m_graphWidget)
            {
                m_graphWidget->fitView();
            }
        });

        // 重置视角（局部选中）。
        m_actFitSelection = m_menu->addAction(QIcon(":/app/icons/toolbars/view/view_fit.svg"), tr("Fit Selection"), this, [=]
        {
            if (m_graphWidget)
            {
                m_graphWidget->fitSelected();
            }
        });
        //@}
    }

    void ContextMenuHandler::slot_windowContextMenu(QPoint pos)
    {
        if (!m_menu || !m_graphWidget)
        {
            return;
        }

        // 未选中则禁用移除按钮。
        if (m_actDelete)
        {
            // 是否需要启用移除按钮。
            bool enableDelete = canDeleteGeometry();
            m_actDelete->setEnabled(enableDelete);
        }

        // 选中的全部为模型则显示隐藏按钮。
        if (m_actHide)
        {
            // 是否需要启用隐藏按钮。
            bool enableHide = canHideGeometry();
            m_actHide->setEnabled(enableHide);
        }

        // 选中一个模型则显示编辑按钮。
        if (m_actEdit)
        {
            // 是否需要启用编辑按钮。
            QString dataName;
            bool enableEdit = canEditGeometry(dataName);

            // 更新按钮文本。
            if (enableEdit)
            {
                m_actEdit->setText(tr("Edit - %1").arg(dataName));
            }

            m_actEdit->setVisible(enableEdit);
        }

        // 未拾取形状则无法进行局部视角重置。
        if (m_actFitSelection)
        {
            m_actFitSelection->setEnabled(m_graphWidget->hasSelection());
        }

        // 弹出菜单。
        m_menu->exec(pos);
    }

    void ContextMenuHandler::slot_actDelete()
    {
        // 需要被移除的几何命令ID列表。（创建移除命令处理）
        QList<int> geoCmdIds;

        // 需要被移除的基准元素命令ID列表。（删除基准元素数据）
        QList<int> datumCmdIds;

        // 获取被拾取对象。
        QList<GraphData::GraphPickedData*> dataList = GraphData::GraphPickedDataProvider::getInstance()->getPickedList();
        for (GraphData::GraphPickedData* data : dataList)
        {
            if (!data)
            {
                continue;
            }

            // 检查拾取数据是否为模型或草图。
            if (data->getGraphInfo().Type == GraphData::GraphDataType::ModelGraph ||
                data->getGraphInfo().Type == GraphData::GraphDataType::SketchGraph)
            {
                geoCmdIds.push_back(data->getGraphInfo().DataObjId);
            }
            // 检查拾取数据是否为基准元素。
            else if (data->getGraphInfo().Type == GraphData::GraphDataType::DatumPointGraph ||
                data->getGraphInfo().Type == GraphData::GraphDataType::DatumLineGraph ||
                data->getGraphInfo().Type == GraphData::GraphDataType::DatumPlaneGraph)
            {
                datumCmdIds.push_back(data->getGraphInfo().DataObjId);
            }
        }

        if (geoCmdIds.count() == 0 && datumCmdIds.count() == 0)
        {
            return;
        }

        // 提示信息。
        int ret = QMessageBox::information(nullptr, tr("Tips"), tr("Do you want to remove the selected object(s)?"), tr("OK"), tr("Cancel"));
        if (ret == 1)
        {
            return;
        }

        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 移除几何命令。
        if (geoCmdIds.count() != 0)
        {
            // 创建移除指令。
            Interface::FITKAbsGeoMultiDelete* deleteCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT
                <Interface::FITKAbsGeoMultiDelete>(Interface::FITKGeoEnum::FGTMultiDelete);
            if (!deleteCmd)
            {
                return;
            }

            deleteCmd->setDataObjectName(QString("Delete-%1").arg(cmdList->getDataCount()));

            // 设置被移除命令ID并添加至命令列表。
            deleteCmd->setDeleteCommandIDs(geoCmdIds);
            deleteCmd->update();
            cmdList->appendDataObj(deleteCmd);

            // 递归刷新对象。
            for (const int & id : geoCmdIds)
            {
                updateGraph(id);
            }
        }

        // 移除基准元素。
        if (datumCmdIds.count() != 0)
        {
            Interface::FITKDatumList* datumMgr = cmdList->getDatumManager();
            if (datumMgr)
            {
                for (const int & id : datumCmdIds)
                {
                    datumMgr->removeDataByID(id);
                }
            }
        }

        // 刷新几何树。
        GUI::MainWindow* mainWindow = FITKGLODATA->getMainWindowT<GUI::MainWindow>();
        if (mainWindow)
        {
            mainWindow->updateGeometryTree();
        }
    }

    void ContextMenuHandler::slot_actEdit()
    {
        // 获取被拾取对象。
        int nPicked = 0;
        Interface::FITKAbsGeoCommand* cmd = getFirstPickedEditableCommand(nPicked);
        if (!cmd)
        {
            return;
        }

        // 获取命令类型，查询操作器。
        QString operKey;
        Interface::FITKGeoEnum::FITKGeometryComType geoType = cmd->getGeometryCommandType();

        // 基准元素。
        if (geoType == Interface::FITKGeoEnum::FITKGeometryComType::FGTDatum)
        {
            Interface::FITKAbsGeoDatum* datum = dynamic_cast<Interface::FITKAbsGeoDatum*>(cmd);
            if (!datum)
            {
                return;
            }

            Interface::FITKGeoEnum::FITKDatumType datumType = datum->getDatumType();
            QString datumTypeName;

            // 根据基准元素形状类型拼接类型名。
            if (datumType >= Interface::FITKGeoEnum::FDTPoint && datumType < Interface::FITKGeoEnum::FDTLine)
            {
                datumTypeName = "Point";
            }
            else if (datumType >= Interface::FITKGeoEnum::FDTLine && datumType < Interface::FITKGeoEnum::FDTPlane)
            {
                datumTypeName = "Line";
            }
            else if (datumType >= Interface::FITKGeoEnum::FDTPlane)
            {
                datumTypeName = "Plane";
            }

            operKey = QString("actionReference%1Edit").arg(datumTypeName);
        }
        // 几何。
        else
        {
            QString geoTypeName = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKGeometryComType>().valueToKey(geoType);
            if (geoTypeName.isEmpty())
            {
                return;
            }

            operKey = QString("action%1Edit").arg(geoTypeName.mid(3));
        }

        Core::FITKActionOperator* operGeo = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>(operKey);
        if (!operGeo)
        {
            return;
        }

        // 执行编辑功能。
        operGeo->setArgs("ID", cmd->getDataObjectID());
        operGeo->setEmitter(m_actEdit);
        operGeo->actionTriggered();
    }

    void ContextMenuHandler::slot_actHide()
    {
        // 初始化前处理操作器。
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!operGraph)
        {
            return;
        }

        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 获取被拾取对象。
        QList<GraphData::GraphPickedData*> dataList = GraphData::GraphPickedDataProvider::getInstance()->getPickedList();
        for (GraphData::GraphPickedData* data : dataList)
        {
            if (!data)
            {
                continue;
            }

            // 检查拾取数据是否为模型或草图。
            if (data->getGraphInfo().Type == GraphData::GraphDataType::ModelGraph ||
                data->getGraphInfo().Type == GraphData::GraphDataType::SketchGraph)
            {
                Interface::FITKAbsGeoCommand* geoCmd = cmdList->getDataByID(data->getGraphInfo().DataObjId);
                if (!geoCmd)
                {
                    continue;
                }

                // 禁用数据。
                geoCmd->enable(false);

                // 刷新可视化对象。
                operGraph->updateGraphVisibility(geoCmd->getDataObjectID());
            }
        }
    }

    void ContextMenuHandler::updateGraph(int id, int refLayer)
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
            int subLayer = refLayer - 1;

            int nRefCmd = cmd->getReferenceCmdCount();
            for (int i = 0; i < nRefCmd; i++)
            {
                Interface::FITKAbsGeoCommand* cmdRef = cmd->getReferenceCmdByIndex(i);
                if (!cmdRef)
                {
                    continue;
                }

                // 递归更新。
                updateGraph(cmdRef->getDataObjectID(), subLayer);
            }
        }
    }

    bool ContextMenuHandler::canDeleteGeometry()
    {
        // 判断是否为草绘模式。
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return false;
        }

        if (cmdList->getCurrentSketchData())
        {
            return false;
        }

        // 判断是否为建模窗口弹出状态。
        QList<QDialog*> dlgs = FITKGUIREPO->getGUIObjects<QDialog>();
        for (QDialog* dlg : dlgs)
        {
            if (!dlg)
            {
                continue;
            }

            if (dlg->isVisible())
            {
                return false;
            }
        }

        // 判断是否为建模功能拾取状态。
        if (GUI::GUIPickInfo::GetPickInfo()._pickObjType != GUI::GUIPickInfo::PickObjType::POBJCustom)
        {
            return false;
        }

        // 获取被拾取对象。
        bool hasSelectedCommand = false;
        QList<GraphData::GraphPickedData*> dataList = GraphData::GraphPickedDataProvider::getInstance()->getPickedList();
        for (GraphData::GraphPickedData* data : dataList)
        {
            if (!data)
            {
                continue;
            }

            // 检查拾取数据是否为模型、草图。
            switch (data->getGraphInfo().Type)
            {
            case GraphData::GraphDataType::ModelGraph:
            case GraphData::GraphDataType::SketchGraph:
            {
                hasSelectedCommand = true;
                break;
            }
            // 检查拾取数据是否为可编辑的基准元素。
            case GraphData::GraphDataType::DatumPointGraph:
            case GraphData::GraphDataType::DatumLineGraph:
            case GraphData::GraphDataType::DatumPlaneGraph:
            {
                Interface::FITKAbsGeoDatum* datum = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoDatum>(data->getGraphInfo().DataObjId);
                if (!datum)
                {
                    continue;
                }

                // 可编辑视为拾取到基准元素。
                if (datum->editable())
                {
                    hasSelectedCommand = true;
                    break;
                }

                break;
            }
            default:
                break;
            }
        }

        return hasSelectedCommand;
    }

    bool ContextMenuHandler::canEditGeometry(QString & dataName)
    {
        // 判断是否为建模功能拾取状态。
        if (GUI::GUIPickInfo::GetPickInfo()._pickObjType != GUI::GUIPickInfo::PickObjType::POBJCustom)
        {
            return false;
        }

        // 获取被拾取对象。
        int nPicked = 0;
        Interface::FITKAbsGeoCommand* cmd = getFirstPickedEditableCommand(nPicked);
        if (!cmd || nPicked != 1)
        {
            return false;
        }

        dataName = cmd->getDataObjectName();

        return true;
    }

    Interface::FITKAbsGeoCommand* ContextMenuHandler::getFirstPickedEditableCommand(int & nPicked)
    {
        // 获取被拾取对象。
        QList<GraphData::GraphPickedData*> dataList = GraphData::GraphPickedDataProvider::getInstance()->getPickedList();
        nPicked = dataList.count();

        if (nPicked == 0)
        {
            return nullptr;
        }

        GraphData::GraphPickedData* data = dataList.first();
        if (!data)
        {
            return nullptr;
        }

        // 获取命令数据。
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return nullptr;
        }

        Interface::FITKAbsGeoCommand* cmd{ nullptr };

        // 检查拾取数据是否为模型或草图。
        switch (data->getGraphInfo().Type)
        {
        case GraphData::GraphDataType::ModelGraph:
        case GraphData::GraphDataType::SketchGraph:
        {
            cmd = cmdList->getDataByID(data->getGraphInfo().DataObjId);
            break;
        }
        // 检查拾取数据是否为可编辑的基准元素。
        case GraphData::GraphDataType::DatumPointGraph:
        case GraphData::GraphDataType::DatumLineGraph:
        case GraphData::GraphDataType::DatumPlaneGraph:
        {
            Interface::FITKDatumList* datumList = cmdList->getDatumManager();
            Interface::FITKAbsGeoDatum* datum = datumList->getDataByID(data->getGraphInfo().DataObjId);
            if (datum && datum->editable())
            {
                cmd = datum;
            }

            break;
        }
        default:
            break;
        }

        return cmd;
    }

    bool ContextMenuHandler::canHideGeometry()
    {
        // 获取被拾取对象。
        QList<GraphData::GraphPickedData*> dataList = GraphData::GraphPickedDataProvider::getInstance()->getPickedList();
        if (dataList.count() == 0)
        {
            return false;
        }

        bool hasSelectedCommand = false;

        for (GraphData::GraphPickedData* data : dataList)
        {
            if (!data)
            {
                continue;
            }

            // 检查拾取数据是否为模型、草图。
            switch (data->getGraphInfo().Type)
            {
            case GraphData::GraphDataType::ModelGraph:
            case GraphData::GraphDataType::SketchGraph:
            {
                hasSelectedCommand = true;
                break;
            }
            // 检查拾取数据是否为可编辑的基准元素。
            case GraphData::GraphDataType::DatumPointGraph:
            case GraphData::GraphDataType::DatumLineGraph:
            case GraphData::GraphDataType::DatumPlaneGraph:
            {
                Interface::FITKAbsGeoDatum* datum = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoDatum>(data->getGraphInfo().DataObjId);
                if (!datum)
                {
                    continue;
                }

                // 可编辑视为拾取到基准元素。
                if (datum->editable())
                {
                    return false;
                }

                break;
            }
            default:
                break;
            }
        }

        return hasSelectedCommand;
    }
}
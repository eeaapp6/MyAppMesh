/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperTreeEvent.h"

// Qt
#include <QTreeWidgetItem>
#include <QMetaEnum>
#include <QAction>

// APP
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

// GUI
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/TreeWidget.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"

namespace GUIOper
{
    void OperTreeEvent::updateGeometryTree()
    {
        // 获取模型树。
        GUI::TreeWidget* treeWidget = getGeometryTreeWidget();
        if (!treeWidget)
        {
            return;
        }

        // 刷新树。
        treeWidget->updateTree();
    }

    void OperTreeEvent::mouseClicked(QTreeWidgetItem* item)
    {
        if (!item)
        {
            return;
        }

        // 获取节点类型。
        GUI::TreeWidget::TreeItemType type = (GUI::TreeWidget::TreeItemType)item->type();
        switch (type)
        {
        case GUI::TreeWidget::GeometryItem:
            break;
        case GUI::TreeWidget::ComponentItem:
            break;
        default:
            return;
        }

        // 获取数据ID。
        int dataId = item->data(0, Qt::UserRole).toInt();

        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!operGraph)
        {
            return;
        }

        // 清除高亮。
        operGraph->clearHighlight();
        operGraph->clearGlobalComponent();

        // 高亮模型。
        if (type == GUI::TreeWidget::GeometryItem)
        {
            operGraph->highlight(dataId);
        }
        // 高亮分组。
        else if (type == GUI::TreeWidget::ComponentItem)
        {
            operGraph->showGlobalComponent(dataId);
        }
    }

    void OperTreeEvent::mouseDoubleClicked(QTreeWidgetItem* item)
    {
        if (!item)
        {
            return;
        }

        // 获取节点类型。
        GUI::TreeWidget::TreeItemType type = (GUI::TreeWidget::TreeItemType)item->type();
        switch (type)
        {
        case GUI::TreeWidget::GeometryItem:
            break;
        case GUI::TreeWidget::ComponentItem:
            break;
        default:
            return;
        }

        // 获取数据ID。
        int dataId = item->data(0, Qt::UserRole).toInt();

        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!operGraph)
        {
            return;
        }

        // 清除高亮。
        operGraph->clearHighlight();
        operGraph->clearGlobalComponent();

        // 编辑模型。
        if (type == GUI::TreeWidget::GeometryItem)
        {
            Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(dataId);
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

            // 创建临时发射器。
            QAction* actTemp = new QAction;
            actTemp->setObjectName("actionEdit");            

            // 执行编辑功能。
            operGeo->setArgs("ID", cmd->getDataObjectID());
            operGeo->setEmitter(actTemp);
            operGeo->actionTriggered();

            // 析构发射器。
            delete actTemp;
        }
        // 编辑分组。
        else if (type == GUI::TreeWidget::ComponentItem)
        {
            return;
        }
    }

    GUI::TreeWidget* OperTreeEvent::getGeometryTreeWidget()
    {
        // 获取全局数据中的主窗体。
        GUI::MainWindow* mainWindow = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow)
        {
            return nullptr;
        }

        return mainWindow->getTreeWidget();
    }
}  // namespace GUIOper

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TreeWidget.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"
#include <QHeaderView>
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKGUIRepo.h"
#include "MainWindow.h"
#include "OperatorsInterface/TreeEventOperator.h"

#include <QMetaEnum>
#include <QDialog>

namespace GUI
{
    TreeWidget::TreeWidget(MainWindow* parent) : QTreeWidget(parent), m_MainWindow(parent)
    {
        // 点击与双击信号。
        connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slot_itemClicked(const QModelIndex &)));
        connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slot_itemDoubleClicked(const QModelIndex &)));

        header()->hide();

        // 初始化几何根节点
        m_GeoRootItem = new QTreeWidgetItem(this, TreeItemType::GeometryRootItem);
        m_GeoRootItem->setText(0, tr("Geometry"));
        m_GeoRootItem->setData(0, Qt::UserRole, 0);
        addTopLevelItem(m_GeoRootItem);
        // 初始化几何分组根节点
        m_GeoGroupRootItem = new QTreeWidgetItem(this, TreeItemType::ComponentRootItem);
        m_GeoGroupRootItem->setText(0, tr("Groups"));
        m_GeoGroupRootItem->setData(0, Qt::UserRole, 1);
        addTopLevelItem(m_GeoGroupRootItem);

        updateTree();
    }

    void TreeWidget::updateTree() {
        auto geoCommandList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (geoCommandList == nullptr) return;

        // 仅获取根命令数据。
        QList<Interface::FITKAbsGeoCommand*> rootCmdList = geoCommandList->getRootCommandList();
        const int count = rootCmdList.count();
        removeChildItem(m_GeoRootItem);
        for (Interface::FITKAbsGeoCommand* geoCommnad : rootCmdList)
        {
            if (geoCommnad == nullptr) continue;
            // 判断是否为移除命令并且命令是否为被移除或被修改状态。
            if (!geoCommnad->getDataValidInGUI())
            {
                continue;
            }
            updateCommandItem(m_GeoRootItem, geoCommnad);
        }

        // 更新几何分组
        removeChildItem(m_GeoGroupRootItem);
        auto compManager = geoCommandList->getGlobalGeoCompManager();
        if (compManager == nullptr) return;
        const int groupCount = compManager->getDataCount();
        for (int i = 0; i < groupCount; ++i) {
            auto group = compManager->getDataByIndex(i);
            if (group == nullptr) continue;

            auto subItem = new QTreeWidgetItem(m_GeoGroupRootItem, TreeItemType::ComponentItem);

            // 初始化节点属性。
            subItem->setText(0, QString("%1").arg(group->getDataObjectName()));
            subItem->setData(0, Qt::UserRole, group->getDataObjectID());

        }


        expandAll();
    }

    void TreeWidget::updateCommandItem(QTreeWidgetItem* item, Interface::FITKAbsGeoCommand* geoCommnad)
    {
        auto subItem = new QTreeWidgetItem(item, TreeItemType::GeometryItem);

        // 初始化节点属性。
        subItem->setText(0, QString("%1").arg(geoCommnad->getDataObjectName()));// .arg(geoCommnad->isEnable() ? "Show" : "Hide"));
        subItem->setData(0, Qt::UserRole, geoCommnad->getDataObjectID());

        // 获取引用命令。
        int nRef = geoCommnad->getReferenceCmdCount();
        for (int i = 0; i < nRef; i++)
        {
            Interface::FITKAbsGeoCommand* subCommand = geoCommnad->getReferenceCmdByIndex(i);
            if (!subCommand)
            {
                continue;
            }

            // 判断是否为移除命令并且命令是否为被移除或被修改状态。
            if (!subCommand->getDataValidInGUI())
            {
                continue;
            }

            // 递归生成子节点。
            updateCommandItem(subItem, subCommand);
        }

        // 如果是草绘，展开下一级
        if (geoCommnad->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D) {
            auto sketchCmd = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(geoCommnad);
            if (sketchCmd == nullptr)
                return;

            auto childCount = sketchCmd->getDataCount();
            for (int j = 0; j < childCount; ++j) {
                auto sCmd = sketchCmd->getDataByIndex(j);
                if (sCmd == nullptr) continue;
                auto childItem = new QTreeWidgetItem(subItem, TreeItemType::GeometryItem);
                childItem->setText(0, QString("%1").arg(sCmd->getDataObjectName()));// .arg(sCmd->isEnable() ? "Show" : "Hide"));
                childItem->setData(0, Qt::UserRole, sCmd->getDataObjectID());
            }
        }
    }

    void TreeWidget::removeItem(QTreeWidgetItem * item)
    {
        removeChildItem(item);
        delete item;//最后将自己删除
    }

    void TreeWidget::removeChildItem(QTreeWidgetItem * item)
    {
        int count = item->childCount();
        for (int i = 0; i < count; i++)
        {
            QTreeWidgetItem *childItem = item->child(0);//删除子节点
            removeChildItem(childItem);
            delete childItem;
        }
    }

    QAction *TreeWidget::addContextMenuAction(const QString &text, const QString &objName, int id)
    {
        if (m_ContextMenu == nullptr)
            return nullptr;
        auto action = m_ContextMenu->addAction(text);
        action->setObjectName(objName);

        if (m_MainWindow != nullptr)
            connect(action, &QAction::triggered, [action, id] {
            // 获取对应操作器
            auto operatorRepo = Core::FITKOperatorRepo::getInstance();
            if (operatorRepo == nullptr)
                return;

            auto oper = operatorRepo->getOperatorT<Core::FITKActionOperator>(action->objectName());
            if (oper == nullptr)
                return;
            oper->setEmitter(action);
            oper->setArgs("ID", id);
            oper->actionTriggered();
        });
        return action;
    }

    void TreeWidget::contextMenuEvent(QContextMenuEvent * event)
    {
        // 建模状态下禁止部分菜单按钮。
        bool modeling = isModeling();

        QTreeWidgetItem* item = itemAt(event->pos());
        if (item == nullptr) return;

        if (m_ContextMenu == nullptr) {
            m_ContextMenu = new QMenu(this);
        }

        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        QList<Interface::FITKAbsGeoCommand*> rootCmdList = cmdList->getRootCommandList();

        m_ContextMenu->clear();

        int id = item->data(0, Qt::UserRole).toInt();
        auto cmd = cmdList->getDataByID(id);
        // 几何菜单
        if (cmd != nullptr) {
            QString enumName = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKGeometryComType>().valueToKey(cmd->getGeometryCommandType());
            if (enumName.isEmpty()) return;

            QString typeName = enumName.mid(3);

            /** 布尔操作需要特殊处理 */
            /*@{*/
            if (typeName == "Bool") {
                auto boolCmd = dynamic_cast<Interface::FITKAbsGeoOperBool*>(cmd);
                if (boolCmd == nullptr) return;

                switch (boolCmd->getBoolOperType())
                {
                case Interface::FITKAbsGeoOperBool::GBTAdd:
                    typeName = "Unite";
                    break;
                case Interface::FITKAbsGeoOperBool::GBTCommon:
                    typeName = "Intersect";
                    break;
                case Interface::FITKAbsGeoOperBool::GBTCut:
                    typeName = "Subtract";
                    break;
                default:
                    return;
                }
            }
            /*@}*/
            /** 平移变换需要特殊处理 */
            /*@{*/
            if (typeName.startsWith("Transform")) {
                typeName = "Transform";
            }
            /*@}*/

            // 非根节点模型无法编辑。
            if (rootCmdList.contains(cmd))
            {
                if (cmd->isEnable()) {
                    addContextMenuAction(tr("Hide"), QString("action%1Hide").arg(typeName), id);
                }
                else {
                    addContextMenuAction(tr("Show"), QString("action%1Show").arg(typeName), id);
                }
            }

            addContextMenuAction(tr("Edit - %1").arg(cmd->getDataObjectName()), QString("action%1Edit").arg(typeName), id);

            // 非建模状态显示移除按钮。
            if (!modeling)
            {
                addContextMenuAction(tr("Delete"), QString("action%1Delete").arg(typeName), id);
            }
        }
        else {
            auto group = cmdList->getGlobalGeoCompManager()->getDataByID(id);
            if (group == nullptr) return;

            // addContextMenuAction(tr("Edit"), QString("actionComponentEdit"), id);
            addContextMenuAction(tr("Delete"), QString("actionComponentDelete"), id);

        }


        m_ContextMenu->exec(event->globalPos());
    }

    bool TreeWidget::isModeling()
    {
        // 获取几何数据，判断是否为草图模式。
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList->getCurrentSketchData())
        {
            return true;
        }

        // 判断是否存在显示状态的建模窗口。
        QList<QDialog*> dlgs = FITKGUIREPO->getGUIObjects<QDialog>();
        for (QDialog* dlg : dlgs)
        {
            if (dlg && dlg->isVisible())
            {
                return true;
            }
        }

        return false;
    }

    void TreeWidget::slot_itemClicked(const QModelIndex & index)
    {
        // 获取点击到的节点。
        QTreeWidgetItem* item = this->itemFromIndex(index);
        if (!item)
        {
            return;
        }

        // 执行操作器功能。
        EventOper::TreeEventOperator* treeOper = FITKOPERREPO->getOperatorT<EventOper::TreeEventOperator>("TreeEvent");
        if (treeOper)
        {
            treeOper->mouseClicked(item);
        }
    }

    void TreeWidget::slot_itemDoubleClicked(const QModelIndex & index)
    {
        // 获取双击到的节点。
        QTreeWidgetItem* item = this->itemFromIndex(index);
        if (!item)
        {
            return;
        }

        // 执行操作器功能。
        EventOper::TreeEventOperator*  treeOper = FITKOPERREPO->getOperatorT<EventOper::TreeEventOperator>("TreeEvent");
        if (treeOper)
        {
            treeOper->mouseDoubleClicked(item);
        }
    }
}

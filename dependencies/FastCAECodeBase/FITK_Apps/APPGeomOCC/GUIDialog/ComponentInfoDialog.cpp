/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ComponentInfoDialog.h"
#include "ui_ComponentInfoDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"

#include <QMessageBox>


namespace GUI {

    ComponentInfoDialog::ComponentInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::ComponentInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        on_comb_type_currentIndexChanged(0);
    }

    ComponentInfoDialog::~ComponentInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    ComponentInfoDialog* ComponentInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKGlobalGeoComponent* group)
    {
        static ComponentInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new ComponentInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (group == nullptr)
        {
            // 获取全局变量
            auto compManager = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>()->getGlobalGeoCompManager();
            uniqueObj->m_Ui->le_name->setText(compManager->checkName(QString("Group-%1").arg(compManager->getDataCount() + 1)));
            uniqueObj->m_Ui->comb_type->setCurrentIndex(0);
            uniqueObj->m_Ui->tw_group->clear();
        }
        else {
            uniqueObj->m_dataId = group->getDataObjectID();
            uniqueObj->m_Ui->le_name->setText(group->getDataObjectName());
            uniqueObj->m_Ui->comb_type->setCurrentIndex(-1);
            switch (group->getGeoType())
            {
            case Interface::FITKModelEnum::FITKModelSetType::FMSPoint:
                uniqueObj->m_Ui->comb_type->setCurrentIndex(0);
                break;
            case Interface::FITKModelEnum::FITKModelSetType::FMSEdge:
                uniqueObj->m_Ui->comb_type->setCurrentIndex(1);
                break;
            case Interface::FITKModelEnum::FITKModelSetType::FMSSurface:
                uniqueObj->m_Ui->comb_type->setCurrentIndex(2);
                break;
            case Interface::FITKModelEnum::FITKModelSetType::FMSSolid:
                uniqueObj->m_Ui->comb_type->setCurrentIndex(3);
                break;
            default:
                break;
            }
            uniqueObj->m_Ui->tw_group->clear();
            const int count = group->getDataCount();
            QHash<int, QTreeWidgetItem*> roots{};
            for (int i = 0; i < count; ++i) {
                auto cmdGroup = group->getDataByIndex(i);
                if (cmdGroup == nullptr) continue;
                auto shapeAgent = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoShapeAgent>(cmdGroup->getAbsModelID());
                if (shapeAgent == nullptr) continue;
                auto cmdId = shapeAgent->getGeoCommand()->getDataObjectID();
                auto virtualIndexs = cmdGroup->getMember();

                auto rootItem = new QTreeWidgetItem(uniqueObj->m_Ui->tw_group, { QString("%1").arg(cmdId) });
                rootItem->setData(0, Qt::UserRole, cmdId);
                for (auto index : virtualIndexs) {
                    auto childItem = new QTreeWidgetItem(rootItem, { QString("%1").arg(index) });
                    childItem->setData(0, Qt::UserRole, index);
                    rootItem->addChild(childItem);
                }
                roots.insert(cmdId, rootItem);
            }
            int index = 0;
            for (auto item : roots.values()) {
                uniqueObj->m_Ui->tw_group->insertTopLevelItem(index++, item);
            }
            uniqueObj->m_Ui->tw_group->expandAll();

        }
        /*@}*/

        return uniqueObj;
    }

    bool ComponentInfoDialog::checkDialogData()
    {
        // 数据检查
        if (m_Ui->le_name->text().isEmpty() || m_Ui->tw_group->topLevelItemCount() == 0) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        return true;
    }

    void ComponentInfoDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 重置窗口数据
        delete m_PreviewCmd;
        m_PreviewCmd = nullptr;
        m_dataId = -1;
    }

    void ComponentInfoDialog::on_pb_remove_clicked()
    {
        auto items = m_Ui->tw_group->selectedItems();
        if (items.isEmpty()) return;
        QList<QTreeWidgetItem*> roots{};
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->blockSignals(true);
        Interface::FITKGeoEnum::VTopoShapeType type;
        switch (m_Ui->comb_type->currentIndex())
        {
        case 0:
            type = Interface::FITKGeoEnum::VTopoShapeType::VSPoint;
            break;
        case 1:
            type = Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
            break;
        case 2:
            type = Interface::FITKGeoEnum::VTopoShapeType::VSFace;
            break;
        case 3:
            type = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
            break;
        default:
            break;
        }
        // 移除子节点
        for (auto item : items) {
            if (item == nullptr) continue;
            int count = item->childCount();
            if (count == 0)//没有子节点，直接删除
            {
                auto parentItem = item->parent();
                pickDataProvider->removeSelected(parentItem->data(0, Qt::UserRole).toInt(), type, item->data(0, Qt::UserRole).toInt());
                delete item;
                continue;
            }
            roots.append(item);
        }
        // 移除顶级节点
        for (auto item : roots) {
            for (int i = item->childCount() - 1; i >= 0; --i)
            {
                QTreeWidgetItem *childItem = item->child(i);//删除子节点
                if (childItem == nullptr) continue;
                pickDataProvider->removeSelected(item->data(0, Qt::UserRole).toInt(), type, childItem->data(0, Qt::UserRole).toInt());
                // 最多有两级，所以直接删除即可，不需要再遍历
                delete childItem;
            }
            delete item;//最后将自己删除
        }
        pickDataProvider->blockSignals(false);
    }

    void ComponentInfoDialog::on_pb_clear_clicked()
    {
        m_Ui->tw_group->clear();
    }

    void ComponentInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->tw_group->clear();

        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMIndividually;
        switch (index)
        {
        case 0:
            pickInfo._pickObjType = GUIPickInfo::POBJVert;
            break;
        case 1:
            pickInfo._pickObjType = GUIPickInfo::POBJEdge;
            break;
        case 2:
            pickInfo._pickObjType = GUIPickInfo::POBJFace;
            break;
        case 3:
            pickInfo._pickObjType = GUIPickInfo::POBJSolid;
            break;
        default:
            break;
        }
        GUIPickInfo::SetPickInfo(pickInfo);

        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            m_Ui->tw_group->clear();

            QHash<int, QTreeWidgetItem*> roots{};
            for (auto pickData : pickedList) {
                if (pickData == nullptr) continue;
                auto virtualShapes = pickData->getVirtualShapes();

                for (auto shape : virtualShapes) {
                    if (!roots.contains(shape.CmdId)) {
                        auto rootItem = new QTreeWidgetItem(m_Ui->tw_group, { QString("%1").arg(shape.CmdId) });
                        rootItem->setData(0, Qt::UserRole, shape.CmdId);
                        roots.insert(shape.CmdId, rootItem);
                    }
                    auto rootItem = roots.value(shape.CmdId);
                    auto childItem = new QTreeWidgetItem(rootItem, { QString("%1").arg(shape.VirtualTopoIndex) });
                    childItem->setData(0, Qt::UserRole, shape.VirtualTopoIndex);
                    rootItem->addChild(childItem);
                }

            }
            int index = 0;
            for (auto item : roots.values()) {
                m_Ui->tw_group->insertTopLevelItem(index++, item);
            }
            m_Ui->tw_group->expandAll();
        });

    }

    void ComponentInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        QList<Interface::VirtualShape> members{};
        auto topCount = m_Ui->tw_group->topLevelItemCount();
        for (int i = 0; i < topCount; ++i) {
            auto item = m_Ui->tw_group->topLevelItem(i);
            if (item == nullptr || item->childCount() == 0) continue;
            auto cmdId = item->data(0, Qt::UserRole).toInt();

            for (int j = 0; j < item->childCount(); ++j) {
                auto childItem = item->child(j);
                if (childItem == nullptr) continue;
                auto virtualIndex = childItem->data(0, Qt::UserRole).toInt();
                members.append(Interface::VirtualShape(cmdId, -1, virtualIndex));
            }
        }
        m_Operator->setArgs("Name", m_Ui->le_name->text());
        switch (m_Ui->comb_type->currentIndex())
        {
        case 0:
            m_Operator->setArgs("Type", Interface::FITKModelEnum::FITKModelSetType::FMSPoint);
            break;
        case 1:
            m_Operator->setArgs("Type", Interface::FITKModelEnum::FITKModelSetType::FMSEdge);
            break;
        case 2:
            m_Operator->setArgs("Type", Interface::FITKModelEnum::FITKModelSetType::FMSSurface);
            break;
        case 3:
            m_Operator->setArgs("Type", Interface::FITKModelEnum::FITKModelSetType::FMSSolid);
            break;
        default:
            break;
        }

        m_Operator->setArgs("Members", QVariant::fromValue(members));

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }


    void ComponentInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}


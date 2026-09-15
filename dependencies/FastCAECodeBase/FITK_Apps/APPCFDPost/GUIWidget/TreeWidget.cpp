/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TreeWidget.h"

#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostEnum.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"

#include <QMenu>

#define TreeItemPostDataID Qt::UserRole+1
#define TreeItemType Qt::UserRole+2

Q_DECLARE_METATYPE(Interface::FITKPostDataType)

namespace GUI
{
    TreeWidget::TreeWidget(QWidget* parent) :
        QTreeWidget(parent)
    {
        _postDataManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        _postGraphManager = Interface::PostGraphObjectManager::getInstance();

        //右键 不可少否则右键无反应
        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotItemClecked(QTreeWidgetItem*, int)));
        connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onModelCustomContextMenu(QPoint)));

        //隐藏列标题
        setHeaderHidden(true);

        updateTree();
    }

    TreeWidget::~TreeWidget()
    {
        if (_rootItem != nullptr)
            _rootItem->takeChildren();
    }

    void TreeWidget::updateTree()
    {
        this->clear();
        _itemHash.clear();

        _rootItem = new QTreeWidgetItem;
        _rootItem->setText(0, "Graph Object");
        this->addTopLevelItem(_rootItem);
        QTreeWidgetItem* currentItem = nullptr;
        QTreeWidgetItem* firstItem = nullptr;
        for (int i = 0; i < _postDataManager->getDataCount(); i++) {
            Interface::FITKAbstractCFDPostData* postData = _postDataManager->getDataByIndex(i);
            if (postData == nullptr)continue;
            int dataID = postData->getDataObjectID();
            int parentID = postData->getCFDPostParentID();
            QString dataName = postData->getDataObjectName();
            QTreeWidgetItem* parentItem = _itemHash.value(parentID);
            QTreeWidgetItem* item = nullptr;
            if (parentItem) {
                item = new QTreeWidgetItem(parentItem);
            }
            else {
                item = new QTreeWidgetItem(_rootItem);
            }
            _itemHash.insert(dataID, item);
            if (i == 0) {
                firstItem = item;
            }
            if (_currentObjID == dataID) {
                currentItem = item;
            }
            Qt::CheckState state = Qt::Checked;
            if (postData->isEnable() == false)state = Qt::Unchecked;
            item->setText(0,dataName);
            item->setData(0, TreeItemPostDataID, dataID);
            Interface::FITKPostDataType type = postData->getPostDataType();
            item->setData(0, TreeItemType, QVariant::fromValue(type));
            item->setCheckState(0, state);
        }
        //展开全部子集
        setItemsExpandable(true);
        expandAll();

        if (currentItem) {
            setCurrentItem(currentItem);
            slotItemClecked(currentItem, 0);
        }
        else {
            setCurrentItem(firstItem);
            slotItemClecked(firstItem, 0);
        }
    }

    int TreeWidget::getCurrentDataID()
    {
        return _currentObjID;
    }

    void TreeWidget::slotItemClecked(QTreeWidgetItem * item, int index)
    {
        Q_UNUSED(index);
        if (item == nullptr)return;
        int objID = item->data(0, TreeItemPostDataID).toInt();
        _currentObjID = objID;
        Interface::FITKPostDataType type = item->data(0, TreeItemType).value<Interface::FITKPostDataType>();
        QString actionName = "";
        QMenu menu;

        if (!actionName.isEmpty()) {
            QObject sender;
            sender.setObjectName(actionName);
            auto acOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>(actionName);
            if (acOper == nullptr)return;
            acOper->setEmitter(&sender);
            acOper->setArgs("objID", objID);
            acOper->actionTriggered();
        }

        //更新渲染窗口
        bool show = (item->checkState(0) == Qt::Checked);
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->updateShow(objID, show);
        graphOper->reRender();

        //更新信息界面
        EventOper::ParaWidgetInterfaceOperator* infoOper = FITKOPERREPO->getOperatorT<EventOper::ParaWidgetInterfaceOperator>("OperatorsWidget");
        if (infoOper) {
            infoOper->CurrentObjectChange(objID);
        }
    }

    void TreeWidget::onModelCustomContextMenu(QPoint point)
    {
        Q_UNUSED(point);

        QTreeWidgetItem* item = this->currentItem();
        if (item == nullptr)return;
        int curItemChildNum = item->childCount();
        int objID = item->data(0, TreeItemPostDataID).toInt();
        Interface::FITKPostDataType type = item->data(0, TreeItemType).value<Interface::FITKPostDataType>();
        QString actionName = "";
        QMenu menu;

        //删除是否可用
        bool deleteIsEnable = true;
        if (curItemChildNum > 0)deleteIsEnable = false;

        switch (type){
        case Interface::FITKPostDataType::Post_None:
            break;
        case Interface::FITKPostDataType::Post_Steady:
            addMenuActions(menu, "actionGraphDelete", "Delete", deleteIsEnable);
            break;
        case Interface::FITKPostDataType::Post_UnSteady:
            addMenuActions(menu, "actionGraphDelete", "Delete", deleteIsEnable);
            break;
        case Interface::FITKPostDataType::Post_Isosurface:
            addMenuActions(menu, "actionGraphDelete", "Delete", deleteIsEnable);
            addMenuActions(menu, "actionIsosurfEdit", "Edit");
            break;
        case Interface::FITKPostDataType::Post_ClipPlane:
        case Interface::FITKPostDataType::Post_ClipSphere:
        case Interface::FITKPostDataType::Post_ClipCylinder:
        case Interface::FITKPostDataType::Post_ClipBox:
            addMenuActions(menu, "actionGraphDelete", "Delete", deleteIsEnable);
            addMenuActions(menu, "actionClipEdit", "Edit");
            break;
        case Interface::FITKPostDataType::Post_SlicePlane:
        case Interface::FITKPostDataType::Post_SliceSphere:
        case Interface::FITKPostDataType::Post_SliceCylinder:
        case Interface::FITKPostDataType::Post_SliceBox:
            addMenuActions(menu, "actionGraphDelete", "Delete", deleteIsEnable);
            addMenuActions(menu, "actionSliceEdit", "Edit");
            break;
        case Interface::FITKPostDataType::Post_StreamLine:
            addMenuActions(menu, "actionGraphDelete", "Delete", deleteIsEnable);
            addMenuActions(menu, "actionStreamlineEdit", "Edit");
            break;
        case Interface::FITKPostDataType::Post_Glyph:
            addMenuActions(menu, "actionGraphDelete", "Delete", deleteIsEnable);
            addMenuActions(menu, "actionGlyphEdit", "Edit");
            break;
        case  Interface::FITKPostDataType::Post_Threshold:
            addMenuActions(menu, "actionGraphDelete", "Delete", deleteIsEnable);
            addMenuActions(menu, "actionThresholdEdit", "Edit");
            break;
        case Interface::FITKPostDataType::Post_Deform:
            addMenuActions(menu, "actionGraphDelete", "Delete", deleteIsEnable);
            addMenuActions(menu, "actionDeformEdit", "Edit");
            break;
        case Interface::FITKPostDataType::Post_Probe:
            addMenuActions(menu, "actionGraphDelete", "Delete", deleteIsEnable);
            addMenuActions(menu, "actionProbeEdit", "Edit");
            break;
        }

        if (menu.actions().size() == 0) return;
        // 在鼠标点击位置执行上下文菜单
        menu.exec(QCursor::pos());
    }

    void TreeWidget::slotAcitonClicked()
    {
        QTreeWidgetItem* item = this->currentItem();
        if (item == nullptr)return;
        int objID = item->data(0, TreeItemPostDataID).toInt();

        QObject* senderObject = sender();
        if (senderObject == nullptr)return;

        auto acOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>(senderObject->objectName());
        if (acOper == nullptr)return;
        acOper->setEmitter(senderObject);
        acOper->setArgs("objID", objID);
        acOper->actionTriggered();
    }

    void TreeWidget::addMenuActions(QMenu & menu, QString actions, QString objectName, bool isEnable)
    {
        QAction* act = menu.addAction(objectName); // 添加动作
        act->setObjectName(actions);
        act->setEnabled(isEnable);
        connect(act, SIGNAL(triggered()), this, SLOT(slotAcitonClicked()));
    }
}

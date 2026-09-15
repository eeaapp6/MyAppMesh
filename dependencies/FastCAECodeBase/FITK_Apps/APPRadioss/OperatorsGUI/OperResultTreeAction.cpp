/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperResultTreeAction.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossPostData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruPost3DManager.h"
#include "GUIDialog/GUIPostDialog/GUICSVCurveDialog.h"
#include "GUIWidget/GUIEnumType.h"

#include <QTreeWidgetItem>
#include <QAction>

namespace GUIOper
{
    OperatorsResultTreeAction::~OperatorsResultTreeAction()
    {
        // 重置选中节点的数据ID
        _selectedItemDataID = -1;
    }
    bool OperatorsResultTreeAction::execGUI()
    {
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;
        // 检查触发器是否为空，如果为空则返回false
        if (_emitter == nullptr) return false;
        // 获取触发器的对象名称
        QString name = _emitter->objectName();
        if (name.isEmpty()) return false;
        Radioss::FITKRadiossPostData* postData = FITKAPP->getGlobalData()->getPostData<Radioss::FITKRadiossPostData>();
        if (!postData) return false;
        // 初始化参数哈希表
        QHash<QString, QVariant> args = QHash<QString, QVariant>();
        args.insert("SelectedItemDataID", _selectedItemDataID);

        if (name == "actionDelete3DPost") deleteData(postData->getPost3DManager(), _selectedItemDataID);
        else if (name == "actionDelete2DPost") deleteData(postData->getPost2DManager(), _selectedItemDataID);
        else if (name == "actionShowXYPlot")
        {
            Radioss::FITKRadiossPost2DManager* post2DManager = postData->getPost2DManager();
            if (!post2DManager) return false;
            Radioss::FITKRadiossPost2DData* post2DData = post2DManager->getDataByID(_selectedItemDataID);
            if (!post2DData) return false;
            this->setArgs("ImportFile", post2DData->getFileName());
            GUI::GUICSVCurveDialog* dialog = new GUI::GUICSVCurveDialog(this, mw);
            dialog->show();
        }
       
        return false;
    }
    bool OperatorsResultTreeAction::execProfession()
    {
        return false;
    }
    void OperatorsResultTreeAction::preArgs()
    {
        // 初始化选中节点的数据ID
        _selectedItemDataID = -1;

        // 获取选中项列表参数
        QVariant selectedItems;
        this->argValue("SelectedItems", selectedItems);
        if (selectedItems.isNull()) return;

        // 将参数转换为TreeWidgetItem列表指针并检查有效性
        QList<QTreeWidgetItem*>* listSelectedItems = static_cast<QList<QTreeWidgetItem*>*>(selectedItems.value<void*>());
        if (listSelectedItems == nullptr || listSelectedItems->isEmpty()) return;

        // 获取第一个选中的项的ID
        _selectedItemDataID = listSelectedItems->first()->data(0, GUI::TreeAdditionalInfo::TAIPostID).toInt();
    }
}
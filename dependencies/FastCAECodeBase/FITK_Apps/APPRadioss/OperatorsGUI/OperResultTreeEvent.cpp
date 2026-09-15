/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperResultTreeEvent.h"
#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"
#include "GUIWidget/GUIEnumType.h"
#include "GUIWidget/ResultTreeWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossPostData.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruPost3DManager.h"

#include <QTreeWidgetItem>
#include <QMenu>
#include <QAction>
#include <QComboBox>
#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace GUIOper
{
    void OperResultTreeEvent::updateTree()
    {
        //更新树的操作
        GUI::MainWindow* mainWindow = GUI::MainWindow::GetMainWindowFromFramework();
        if (!mainWindow) return;
        GUI::ControlPanel* controlPanel = mainWindow->getControlPanel();
        if (!controlPanel) return;
        GUI::ResultTreeWidget* resultTreeWidget = controlPanel->getResultTree();
        if (!resultTreeWidget) return;
        resultTreeWidget->updateTreeWidget();
    }

    void OperResultTreeEvent::on_itemClicked(QTreeWidget * w, QTreeWidgetItem * item, int col)
    {
        Q_UNUSED(w);
        Q_UNUSED(col);
        GUI::MainWindow* mw = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (!item || !mw) return;
        Radioss::FITKRadiossPostData * postData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (!postData) return;
        Interface::FITKStruPost3DManager* post3DDataMgr = postData->getPost3DManager();
        if (!post3DDataMgr) return;
        //获取类型
        GUI::PostTreeItemType type = (GUI::PostTreeItemType)item->type();
        switch (type)
        {
        case GUI::PITNone:
            break;
        case GUI::PITPost3DRoot:
            break;
        case GUI::PITPost3D:
            break;
        case GUI::PITStepRoot:
            break;
        case GUI::PITStep: {
            int postID = item->data(0, GUI::TreeAdditionalInfo::TAIPostID).toInt();
            int stepIndex = item->data(0, GUI::TreeAdditionalInfo::TAIStepIndex).toInt();
            postData->setCurrentPostDataID(postID);
            Interface::FITKAbstractStructuralPostVTK* post3DData = post3DDataMgr->getDataByID(postID);
            if (post3DData) post3DData->setCurFrame(stepIndex);
            break;
        }
        default:
            break;
        }
        this->resetViewport();
    }

    void OperResultTreeEvent::on_itemDoubleClicked(QTreeWidget * w, QTreeWidgetItem * item, int col)
    {
        Q_UNUSED(w);
        Q_UNUSED(item);
        Q_UNUSED(col);
        //双击树节点事件处理
    }

    void OperResultTreeEvent::on_itemStateChanged(QTreeWidget * w, QTreeWidgetItem * item, bool state, int column)
    {
        Q_UNUSED(w);
        Q_UNUSED(item);
        Q_UNUSED(state);
        Q_UNUSED(column);
        //状态改变事件
    }
    void OperResultTreeEvent::resetViewport()
    {
        Core::FITKActionOperator* graphOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionUpdatePostGraph");
        if (graphOper) graphOper->execProfession();
    }
}
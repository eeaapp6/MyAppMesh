/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ToolBarBase.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"
#include "PostGraphAdaptor/PostGraphProperty.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"

#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUI
{
    ToolBarBase::ToolBarBase(QWidget * parent):
        QToolBar(parent)
    {
        _postDataManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        _postGraphManager = Interface::PostGraphObjectManager::getInstance();
    }

    ToolBarBase::~ToolBarBase()
    {

    }

    QAction* ToolBarBase::createAction(QString actionName, QString iconPath, QString actionTitle)
    {
        if (actionName.isEmpty())return nullptr;
        QAction* action = new QAction(this);
        action->setObjectName(actionName);
        action->setIcon(QIcon(iconPath));
        action->setToolTip(actionTitle);
        return action;
    }
    void ToolBarBase::reRender()
    {
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->reRender();
    }
}
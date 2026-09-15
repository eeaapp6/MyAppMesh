/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ToolBarEvent.h"

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
    ToolBarEvent::ToolBarEvent(QWidget* parent) :
        ToolBarBase(parent)
    {
        _extremumAction = createAction("actionExtremum", "", "Extremum");
        _extremumAction->setText(tr("Extremum"));
        this->addAction(_extremumAction);

        QAction* action = createAction("actionColorLibary", "", QString(tr("")));
        action->setText(tr("ColorLibary"));
        this->addAction(action);

        action = createAction("actionLight", "", "Light");
        action->setText(tr("Light"));
        action->setCheckable(true);
        this->addAction(action);

        this->addSeparator();
        updateCurrentGraphObj(-1);
    }

    ToolBarEvent::~ToolBarEvent()
    {

    }

    void ToolBarEvent::updateCurrentGraphObj(int objID)
    {
        _currentDataID = objID;
        _extremumAction->setEnabled(false);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;

        _extremumAction->setEnabled(true);
    }
}
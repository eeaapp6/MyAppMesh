/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsSwitchCFDPost.h"

#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIDialog/GUICalculateDialog/SwitchCFDPostDialog.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace ModelOper
{
    OperatorsSwitchCFDPost::OperatorsSwitchCFDPost()
    {

    }

    OperatorsSwitchCFDPost::~OperatorsSwitchCFDPost()
    {

    }

    bool OperatorsSwitchCFDPost::execGUI()
    {
		GUI::SwitchCFDPostDialog* dialog = new GUI::SwitchCFDPostDialog(this);
		dialog->show();

        return true;
    }

    bool OperatorsSwitchCFDPost::execProfession()
    {
        return true;
    }
}


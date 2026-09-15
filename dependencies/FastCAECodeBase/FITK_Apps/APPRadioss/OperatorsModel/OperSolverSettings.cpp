/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGmshSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "GUIDialog/GUIPostDialog/GUISolverSettingDialog.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperSolverSettings.h"


namespace ModelOper
{
    bool OperSolverSettings::execGUI()
    {
        GUI::GUISolverSettingDialog* dialog = new GUI::GUISolverSettingDialog(this, FITKAPP->getGlobalData()->getMainWindow());
        dialog->show();
        return false;
    }
    bool OperSolverSettings::execProfession()
    {
        return true;
    }
}

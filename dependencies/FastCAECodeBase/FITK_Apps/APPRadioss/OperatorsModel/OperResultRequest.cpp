/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperResultRequest.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "GUIDialog/GUIPostDialog/GUIRadiossResultRequestDialog.h"

namespace ModelOper
{
    bool OperResultRequest::execGUI()
    {
        GUI::GUIRadiossResultRequestDialog* dialog = new GUI::GUIRadiossResultRequestDialog(this, FITKAPP->getGlobalData()->getMainWindow());
        dialog->show();
        return false;
    }
    bool OperResultRequest::execProfession()
    {
        return true;
    }
}

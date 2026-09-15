/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorWorkingDir.h"
#include "FITK_Component/FITKWidget/FITKWorkingDirDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

bool GUIOper::OperatorWorkingDir::execGUI()
{
    //主界面
    AppFrame::FITKGlobalData* g = FITKAPP->getGlobalData();
    if (g == nullptr) return false;
    //弹出对话框
    Comp::FITKWorkingDirDialog d(g->getMainWindow());
    return d.exec() == QDialog::Accepted;
}

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGmshSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Component/FITKGmshExeDriver/GUIGmshSettings.h"
#include <QWidget>

bool ModelOper::OperGmshSettings::execGUI(){
    ////获取动作名称
    //QString actionName = _emitter->objectName();
    //QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    //if (!mw) return false;
    //if (actionName == "actionGmshSettings")
    //{
    //    GUI::GUIGmshSettings* dialog = new GUI::GUIGmshSettings(this,mw);
    //    dialog->show();
    //}
    return false;
}

bool ModelOper::OperGmshSettings::execProfession()
{


    return true;
}

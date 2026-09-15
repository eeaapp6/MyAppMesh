/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorColorBar.h"
#include "GUIDialog/GUIPostDialog/GUIColorBarDialog.h"
#include "OperatorsInterface/GraphEventOperator.h"

bool ModelOper::OperatorColorBar::execGUI()
{
    EventOper::GraphEventOperator* graphOper = Core::FITKOperatorRepo::getInstance()
        ->getOperatorT<EventOper::GraphEventOperator>("GraphPostProcess");
    if (!graphOper)return false;

    GUI::GUIColorBarDialog dlg(graphOper);
    dlg.exec();
    return false;
}

bool ModelOper::OperatorColorBar::execProfession()
{
    return false;
}

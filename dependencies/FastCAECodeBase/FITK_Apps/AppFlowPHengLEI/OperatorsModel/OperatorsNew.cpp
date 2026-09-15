/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsNew.h"
#include "GUIWidget/NewProjectDialog.h"

namespace ModelOper
{
    OperatorsNew::OperatorsNew()
    {

    }

    OperatorsNew::~OperatorsNew()
    {

    }

    bool OperatorsNew::execGUI()
    {
		GUI::NewProjectDialog* dialog = new GUI::NewProjectDialog();
		dialog->show();
        return false;
    }

    bool OperatorsNew::execProfession()
    {
        return false;
    }
}


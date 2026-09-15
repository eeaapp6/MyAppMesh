/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKActionOperator.h"


namespace Core
{

    FITKActionOperator::~FITKActionOperator()
    {

    }

    bool FITKActionOperator::actionTriggered()
    {
        //执行预处理
        this->preArgs();
        //Ui相关操作
        bool OK = this->execGUI();
        if (!OK) return false;
        //执行具体业务逻辑
        return this->execProfession();
    }

    bool FITKActionOperator::execGUI()
    {
        return true;
    }

    bool FITKActionOperator::execProfession()
    {
        return true;
    }

    void FITKActionOperator::preArgs()
    {

    }

}




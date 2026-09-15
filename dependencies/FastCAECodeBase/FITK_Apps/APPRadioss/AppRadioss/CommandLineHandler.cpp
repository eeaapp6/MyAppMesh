/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CommandLineHandler.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"

void CommandLineHandler::exec()
{
    AppFrame::FITKCommandLineHandler::exec();
    //查找参数
    int index = _args.indexOf("-rad");
    if (index >= 0)
        //-i 后面的参数为文件名
        this->importRad(index + 1);
 
}

void CommandLineHandler::importRad(const int index)
{
    //获取文件名称
    QString  file = this->argValue(index);
    if (file.isEmpty()) return;
    //操作器
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionImportRad");
    if (oper == nullptr) return;
    //执行操作器
    oper->setArgs("FileName", file);
    oper->setArgs("Emitter", "actionImportRad");
    oper->execProfession();
}
 
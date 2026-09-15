/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "WorkBenchHandler.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include <QDir>

void CFDPostWorkBenchHandler::execHandler()
{
    //文件数量
    const int n = this->getInputFilesCount();
    for (int i = 0; i < n; ++i)
    {
        //遍历文件
        AppFrame::IOFileInfo finfo = this->getInputFileInfo(i);

        if (finfo._isFile)
        {
            this->importFile(finfo);
        }
        else
        {
            this->importFolder(finfo);
        } 
    }

}

void CFDPostWorkBenchHandler::execOutput()
{
 
}
 
 
void CFDPostWorkBenchHandler::importFile(AppFrame::IOFileInfo finfo)
{
    QString fileName = QString("%1/%2").arg(finfo._path).arg(finfo._name);
    //获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpen");
    if (oper == nullptr) return;
    oper->setArgs("files", fileName); 
    //执行操作
    oper->execProfession();
    Core::FITKThreadPool::getInstance()->wait();
}

void CFDPostWorkBenchHandler::importFolder(AppFrame::IOFileInfo finfo)
{
    QDir dir(finfo._path);
    if (!dir.exists()) return;
    QString type = QString("*.%1").arg(finfo._suffix);
    dir.setNameFilters(QStringList() << type);
    // 获取文件信息列表
    QFileInfoList fileInfoList = dir.entryInfoList();
    if (fileInfoList.isEmpty())return;

    QString filename = finfo._path + "/" + finfo._name + "..." + finfo._suffix;

    //获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpen");
    if (oper == nullptr) return;
    oper->setArgs("files", filename);
    //执行操作
    oper->execProfession();
    Core::FITKThreadPool::getInstance()->wait();

}


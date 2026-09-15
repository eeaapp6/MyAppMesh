/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "WorkBenchHandler.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"

void OCCAppWorkBenchHandler::execHandler()
{
    //文件数量
    const int n = this->getInputFilesCount();
    for (int i = 0; i<n; ++i)
    {
        //遍历文件
        AppFrame::IOFileInfo finfo = this->getInputFileInfo(i);
        if (finfo._suffix == "brep" || finfo._suffix == "step")
        {
            //导入文件
            this->importGeoFile(QString("%1/%2").arg(finfo._path).arg(finfo._name));
        }
        if (finfo._suffix == "hdf5" && this->isProjectFileMode())
        {
            //打开工程文件
            this->openProjectFile(QString("%1/%2").arg(finfo._path).arg(finfo._name));
        }
    }

}

void OCCAppWorkBenchHandler::execOutput()
{
    const int n = this->getOutputFilesCount();
    for (int i = 0; i < n; ++i)
    {
        AppFrame::IOFileInfo finfo = this->getOututFileInfo(i);
        const QString fileName = QString("%1/%2").arg(finfo._path).arg(finfo._name);
        if (finfo._suffix == "hdf5")
        {
            this->saveProjectFile(fileName);
            continue;
        }

        this->exportGeoFile(fileName);
    }
}

void OCCAppWorkBenchHandler::importGeoFile(const QString & fileName)
{
    //获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("OperImportManager");
    if (oper == nullptr) return;
    oper->setArgs("FileNames", QStringList(fileName)); 
    //执行操作
    oper->execProfession();
}

void OCCAppWorkBenchHandler::openProjectFile(const QString & fileName)
{
    //获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpenProject");
    if (oper == nullptr) return;
    oper->setArgs("FileName", fileName);
    //执行操作
    oper->execProfession();
}

void OCCAppWorkBenchHandler::saveProjectFile(const QString & fileName)
{
    //获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionSaveProject");
    if (oper == nullptr) return;
    oper->setArgs("FileName", fileName);
    //执行操作
    oper->execProfession();
    Core::FITKThreadPool::getInstance()->wait();
}

void OCCAppWorkBenchHandler::exportGeoFile(const QString & fileName)
{
    //获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionExport");
    if (oper == nullptr) return;
    oper->setArgs("FileName", fileName);
    //执行操作
    oper->execProfession();
    Core::FITKThreadPool::getInstance()->wait();
}


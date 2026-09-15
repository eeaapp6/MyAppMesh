/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GlobalFiles.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"

namespace
{
    QString sendMessageToServer(const QString& message, bool isToServer = true)
    {
        if (isToServer)
        {
            HttpPython::FITKHttpCommons::sendMessageToServer(message);
        }
        return message;
    }
}
Files* FilesWapper::new_Files()
{
    return new Files;
}
void FilesWapper::delete_Files(Files* F)
{
    if (F) delete F;
}
QString FilesWapper::static_Files_ImportRadFile(const QString& fileName)
{
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionImportRad");
    oper->setArgs("FileName", fileName);
    bool isOk = oper->execProfession();
    this->waitThreadPool();
    if (isOk)
    {
        return sendMessageToServer(QString("Import file succeeded. File: %1").arg(fileName));
    }
    else
    {
        return sendMessageToServer(QString("Import file failed: %1").arg(fileName));
    }
}
QString FilesWapper::static_Files_ExportRadFile(const QString& fileName)
{
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionExportRad");
    oper->setArgs("FileName", fileName);
    oper->execProfession();
    this->waitThreadPool();
    return sendMessageToServer(QString("succeeded. File: %1").arg(fileName));
}

QString FilesWapper::static_Files_ImportMeshFile(const QString &fileName)
{
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionImportMesh");
    oper->setArgs("FileName", fileName);
    bool isOk = oper->execProfession();
    this->waitThreadPool();
    if (isOk)
    {
        return sendMessageToServer(QString("Import file succeeded. File: %1").arg(fileName));
    }
    else
    {
        return sendMessageToServer(QString("Import file failed: %1").arg(fileName));
    }
}

QString FilesWapper::static_Files_OpenProject(const QString &fileName)
{
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpen");
    oper->setArgs("FileName", fileName);
    bool isOk = oper->execProfession();
    this->waitThreadPool();
    if (isOk)
    {
        return sendMessageToServer(QString("Open file succeeded. File: %1").arg(fileName));
    }
    else
    {
        return sendMessageToServer(QString("Open file failed: %1").arg(fileName));
    }
}
QString FilesWapper::static_Files_SaveProject(const QString& fileName)
{
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionSave");
    oper->setArgs("FileName", fileName);
    oper->execProfession();
    this->waitThreadPool();
    return sendMessageToServer(QString("succeeded. File: %1").arg(fileName));
}

QString FilesWapper::static_Files_GetVTKResultFilePath()
{
    this->waitThreadPool();
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionStartSolver");
    QString vtkResultPath{};
    bool isOk = oper->argValue("vtkResultPath", vtkResultPath);
    if (isOk)
    {
        return sendMessageToServer(QString(".vtk result file path is: %1").arg(vtkResultPath));
    }
    else
    {
        return sendMessageToServer(QString("No result .vtk file"));
    }
}

void FilesWapper::waitThreadPool()
{
    Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
    if (pool == nullptr)return;
    pool->wait();
}

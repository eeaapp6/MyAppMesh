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
#include "PythonInterfaceCommons.h"

Files* FilesWapper::new_Files()
{
    PythonInterfaceCommons::sendMessageToHttpServer("Create FilesWapper success!");
    return new Files;
}
void FilesWapper::delete_Files(Files* F)
{
    if (F == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Delete Files failed!");
        return;
    }

    delete F;
    PythonInterfaceCommons::sendMessageToHttpServer("Delete Files success!");
}
void FilesWapper::static_Files_ImportInpFile(const QString & fileName, const QString& level)
{
    //获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionImportAsParts");
    if (oper == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Import inp file failed!");
        return;
    }

    //设置参数，与界面统一
    QString m = "actionImportAsModel";
    if (level.toLower() == "assembly")
    {
        m = "actionImportAsAssembly";
    }
    else if (level.toLower() == "part")
    {
        m = "actionImportAsParts";
    }

    //读取
    oper->setArgs("FileName", fileName);
    oper->setArgs("Emitter", m);
    bool result = oper->execProfession();
    if (result == false)
    {
        PythonInterfaceCommons::sendMessageToHttpServer(QString("Import inp file failed: %1").arg(fileName));
        return;
    }

    PythonInterfaceCommons::sendMessageToHttpServer(QString("Import inp file success: %1").arg(fileName));
}
void FilesWapper::static_Files_ExportInpFile(const QString& fileName)
{
    //获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionexport_inp");
    if (oper == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Export inp file failed!");
        return;
    }

    oper->setArgs("FileName", fileName);
    //写出
    bool result = oper->execProfession();
    if (result == false)
    {
        PythonInterfaceCommons::sendMessageToHttpServer(QString("Export inp file failed: %1").arg(fileName));
        return;
    }

    PythonInterfaceCommons::sendMessageToHttpServer(QString("Export inp file success: %1").arg(fileName));
}

void FilesWapper::static_Files_ExportCCXInpFile( const QString& fileName)
{
    // 获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionexport_ccxinp");
    if (oper == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Export ccx inp file failed!");
        return;
    }

    oper->setArgs("FileName", fileName);
    //写出
    bool result = oper->execProfession();
    if (result == false)
    {
        PythonInterfaceCommons::sendMessageToHttpServer(QString("Export ccx inp file failed: %1").arg(fileName));
        return;
    }

    PythonInterfaceCommons::sendMessageToHttpServer(QString("Export ccx inp file success: %1").arg(fileName));
}

void FilesWapper::static_Files_OpenDataBase(const QString& fileName)
{
    // 获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionimport_hdf5");
    if (oper == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Open database failed!");
        return;
    }

    oper->setArgs("FileName", fileName);
    //写出
    bool result = oper->execProfession();
    if (result == false)
    {
        PythonInterfaceCommons::sendMessageToHttpServer(QString("Open database failed: %1").arg(fileName));
        return;
    }

    PythonInterfaceCommons::sendMessageToHttpServer(QString("Open database success: %1").arg(fileName));
}

void FilesWapper::static_Files_SaveDataBase(const QString& fileName)
{
    // 获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionexport_hdf5");
    if (oper == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Save database failed!");
        return;
    }

    oper->setArgs("FileName", fileName);
    //写出
    bool result = oper->execProfession();
    if (result == false)
    {
        PythonInterfaceCommons::sendMessageToHttpServer(QString("Save database failed: %1").arg(fileName));
        return;
    }

    PythonInterfaceCommons::sendMessageToHttpServer(QString("Save database success: %1").arg(fileName));
}

void FilesWapper::static_Files_OpenVTKFiles(const QString& fileName)
{
    // 获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionImportVTK");
    if (oper == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Open vtk files failed!");
        return;
    }

    oper->setArgs("FileNames", fileName);
    //写出
    bool result = oper->execProfession();
    if (result == false)
    {
        PythonInterfaceCommons::sendMessageToHttpServer(QString("Open vtk files failed: %1").arg(fileName));
        return;
    }

    PythonInterfaceCommons::sendMessageToHttpServer(QString("Open vtk files success: %1").arg(fileName));
}

void FilesWapper::static_Files_ExportGeoFile(const QString& fileName)
{
    // 获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionExport");
    if (oper == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Export geo file failed!");
        return;
    }

    oper->setArgs("FileName", fileName);
    //写出
    bool result = oper->execProfession();
    if (result == false)
    {
        PythonInterfaceCommons::sendMessageToHttpServer(QString("Export geo file failed: %1").arg(fileName));
        return;
    }

    this->waitThreadPool();
    PythonInterfaceCommons::sendMessageToHttpServer(QString("Export geo file success: %1").arg(fileName));
}
void FilesWapper::waitThreadPool()
{
    Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
    if (pool == nullptr)return;
    pool->wait();
}

 

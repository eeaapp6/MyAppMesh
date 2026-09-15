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

Files* FilesWapper::new_Files()
{
    return new Files;
}
void FilesWapper::delete_Files(Files* F)
{
    if (F) delete F;
}

void FilesWapper::static_Files_ImportGeoFile(const QString& fileName)
{
    //获取操作器
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionImportGeometry");
    if (oper == nullptr) return;
    //设置参数，与界面统一
    oper->setArgs("FileName", fileName);
    oper->setArgs("SenderName", "actionImportGeometry");
    oper->execProfession();
    this->initSleep();
}

void FilesWapper::static_Files_ImportOFMeshFile(const QString &fileName)
{
     //获取操作器
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionImportOpenFoamMesh");
    if (oper == nullptr) return;
    //设置参数，与界面统一

    oper->setArgs("FileName", fileName);
    oper->setArgs("SenderName", "actionImportOpenFoamMesh");
    oper->execProfession();
    this->initSleep();
}

void FilesWapper::static_Files_ImportMeshFile(const QString &fileName)
{
    //获取操作器
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionImportMesh");
    if (oper == nullptr) return;
    //设置参数，与界面统一
    
    oper->setArgs("FileName", fileName);
    oper->setArgs("SenderName", "actionImportMesh");
    oper->execProfession();
    this->initSleep();
}

void FilesWapper::initSleep()
{
    //_sleep(100);
    //获取线程池
    Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
    if (pool == nullptr)return;
    //等待线程结束
    pool->wait();
}
 
 

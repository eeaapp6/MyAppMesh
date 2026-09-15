/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKProgramTaskManager.h"
#include "FITKProgramDriverFactory.h"

namespace AppFrame
{
    void FITKProgramTaskManeger::startProgram(const int type, const QString& programName, FITKProgramInputInfo* info)
    {
        auto fac = FITKProgramDriverFactory::getInstance();
        auto fun = fac->getDriverCreateFun(type, programName);
        if (fun == nullptr) return;

        FITKAbstractProgramerDriver* driver = fun();
        if (driver == nullptr) return;
        driver->setInputInfo(info);
        this->appendDataObj(driver);
        driver->start();
        
    }

    //创建第三方驱动程序 返回值不能手动销毁,由第三方程序执行任务管理器统一管理
    FITKAbstractProgramerDriver* FITKProgramTaskManeger::createProgram(const int type, const QString & programName, FITKProgramInputInfo * info)
    {
        auto fac = FITKProgramDriverFactory::getInstance();
        auto fun = fac->getDriverCreateFun(type, programName);
        //获取构造函数指针
        if (fun == nullptr) return nullptr;
        //调用构造
        FITKAbstractProgramerDriver* driver = fun();
        if (driver == nullptr) return nullptr;
        driver->setInputInfo(info);
        this->appendDataObj(driver);
        return driver;
    }

    void FITKProgramTaskManeger::killAll()
    {
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractProgramerDriver* d = this->getDataByIndex(i);
            if (d == nullptr) continue;
            d->stop();
            delete d;
        }
    }

    void FITKProgramTaskManeger::appendDataObj(Core::FITKAbstractDataObject* item)
    {
        FITKAbstractProgramerDriver* driver = dynamic_cast<FITKAbstractProgramerDriver*>(item);
        if (driver == nullptr) return;

        connect(driver, &FITKAbstractProgramerDriver::sig_Finish, this, &FITKProgramTaskManeger::programFinishedSlot ,Qt::QueuedConnection);
        //连接信号槽
        connect(driver, &FITKAbstractProgramerDriver::sig_Finish, this, [=]() { emit sig_ProgramFinished(driver); }, Qt::QueuedConnection);

        Core::FITKAbstractDataManager<FITKAbstractProgramerDriver>::appendDataObj(item);
    }

    void FITKProgramTaskManeger::programFinishedSlot()
    {
        QObject* sender = this->sender();
        FITKAbstractProgramerDriver* driver = dynamic_cast<FITKAbstractProgramerDriver*>(sender);
        if (driver == nullptr || !this->isContains(driver)) return;
        disconnect(driver, &FITKAbstractProgramerDriver::sig_Finish, this, &FITKProgramTaskManeger::programFinishedSlot);
//        delete driver;
        this->removeDataObj(driver);

    }

}




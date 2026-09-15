/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractIO.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

FITKLIBINFOREGISTER(FITKInterfaceIO, FITKInterfaceIOVersion)


namespace Interface
{
    FITKAbstractIO::~FITKAbstractIO()
    {
    }

    void FITKAbstractIO::setFileName(const QString& fileName)
    {
        //记录文件名称
        _fileName = fileName;
    }

    QString FITKAbstractIO::getFileName() const
    {
        //直接返回文件名称
        return _fileName;
    }

    void FITKAbstractIO::setDataObject(Core::FITKAbstractDataObject* data)
    {
        _data = data;
    }

    void FITKAbstractIO::push2ThreadPool()
    {
        //直接在线程池运行
        Core::FITKThreadPool::getInstance()->execTask(this);
    }
}




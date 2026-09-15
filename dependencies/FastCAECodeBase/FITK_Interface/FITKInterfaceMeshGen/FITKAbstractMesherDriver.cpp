/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractMesherDriver.h"

namespace Interface
{
    FITKAbstractMesherDriver::~FITKAbstractMesherDriver()
    {

    }


    void FITKAbstractMesherDriver::setDataObject(Core::FITKAbstractDataObject* data)
    {
        //临时数据
        _data = data;
    }

    void FITKAbstractMesherDriver::setArgs(const QStringList& args)
    {
        //启动参数
        _args = args;
    }

    void FITKAbstractMesherDriver::startMesher(QStringList info /*= QStringList()*/)
    {
        //纯虚函数
    }

    void FITKAbstractMesherDriver::stopMesher(QStringList info /*= QStringList()*/)
    {
        //纯虚函数
    }

    void FITKAbstractMesherDriver::insertDataObject(QString key, Core::FITKAbstractDataObject* value)
    {
        _dataObject.insert(key, value);
    }
}



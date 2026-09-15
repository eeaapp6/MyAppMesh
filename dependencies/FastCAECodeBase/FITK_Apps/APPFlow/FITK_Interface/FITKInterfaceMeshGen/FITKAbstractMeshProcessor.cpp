/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractMeshProcessor.h"

Interface::FITKAbstractMeshProcessor::~FITKAbstractMeshProcessor()
{
    //纯虚函数
}

void Interface::FITKAbstractMeshProcessor::setArgs(const QStringList & args)
{
    _args = args;
}

void Interface::FITKAbstractMeshProcessor::start(QStringList info /*= QStringList()*/)
{
    //纯虚函数
}

void Interface::FITKAbstractMeshProcessor::insertDataObject(QString key, Core::FITKAbstractDataObject* value)
{
    _dataObject.insert(key, value);
}

Core::FITKAbstractDataObject* Interface::FITKAbstractMeshProcessor::getDataObject(QString key)
{
    // 尝试获取数据。
    return _dataObject.value(key, nullptr);
}

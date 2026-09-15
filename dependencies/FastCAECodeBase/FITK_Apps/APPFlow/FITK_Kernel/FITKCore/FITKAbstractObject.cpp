/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/


#include "FITKAbstractObjectContainer.hpp"
#include "FITKAbstractObject.hpp"
#include "FITKScriptTextRepo.h"

/**
 * ~FITKAbstractObjectPrivate()
 * 析构函数，用于清理资源。
 * 此函数不接受参数，也不返回任何值。
 */
Core::FITKAbstractObjectPrivate::~FITKAbstractObjectPrivate()
{
    // 遍历容器中的所有对象，调用它们的ObjectDestoried函数，然后清空容器
    {
        for (FITKAbstractObjectContainerPrivate* c : _containers)
            c->ObjectDestoried();
        _containers.clear();
    }
}

Core::FITKAbstractObject::~FITKAbstractObject()
{

}

bool Core::FITKAbstractObject::isEnable()
{
    return  _objEnable;
}

void Core::FITKAbstractObject::enable(bool en /*= true*/)
{
    _objEnable = en;
}

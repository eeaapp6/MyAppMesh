/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractObjectContainer.hpp"
#include "FITKAbstractDataObject.h"

namespace Core
{
 /**
 * FITKAbstractObjectContainerPrivate构造函数。
 * 初始化_object成员并将其添加到所属FITKAbstractObjectPrivate的_containers列表中。
 * @param absObj 指向关联的FITKAbstractObjectPrivate实例的指针。
 */
FITKAbstractObjectContainerPrivate::FITKAbstractObjectContainerPrivate(FITKAbstractObjectPrivate* absObj)
    :_object(absObj)
{
    if (absObj == nullptr) return;
    absObj->_containers.append(this);
}


FITKAbstractObjectContainerPrivate::FITKAbstractObjectContainerPrivate()
{

}

/**
 * FITKAbstractObjectContainerPrivate析构函数。
 * 从所属FITKAbstractObjectPrivate的_containers列表中移除自身。
 */
FITKAbstractObjectContainerPrivate::~FITKAbstractObjectContainerPrivate()
{
    if (_object == nullptr) return;
    _object->_containers.removeOne(this);
}

/**
 * 获取与此容器关联的FITKAbstractObjectPrivate实例的指针。
 * @return 指向FITKAbstractObjectPrivate实例的指针，如果未设置则为nullptr。
 */
Core::FITKAbstractObjectPrivate* FITKAbstractObjectContainerPrivate::getObject()
{
    return _object;
}


void FITKAbstractObjectContainerPrivate::setObj(FITKAbstractObjectPrivate* absObj)
{
    if(_object != nullptr)
        _object->_containers.removeOne(this);
    _object = absObj;
    if (absObj == nullptr) return;
    absObj->_containers.append(this);
}

/**
 * 当对象被销毁时调用此函数，将_object指针设为nullptr。
 * 用于断开与FITKAbstractObjectPrivate实例的连接。
 */
void FITKAbstractObjectContainerPrivate::ObjectDestoried()
{
    _object = nullptr;
}
}

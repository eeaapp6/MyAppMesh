/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKComponentInterface.h"

namespace AppFrame
{
    FITKComponentInterface::~FITKComponentInterface()
    {
        //析构，清理数据，会收内存
    }

    void FITKComponentInterface::init()
    {
        //注册接口等操作
    }

    QWidget* FITKComponentInterface::getWidget(const int indexPort)
    {
        //错误值
        return nullptr;
    }

    QString FITKComponentInterface::getComponentName()
    {
        //抽象类，错误值
        return QString();
    }

    bool FITKComponentInterface::exec(const int indexPort)
    {
        return false;
    }

    void FITKComponentInterface::setComponentObjName(const QString & name)
    {
        //名称
        _compObjName = name;
    }

    QString FITKComponentInterface::getComponentObjName() const
    {
        return _compObjName;
    }

    CallBackFuns FITKComponentInterface::getCallBackFuns(int key)
    {
        if (_callBackFuns.contains(key))
            return _callBackFuns[key];
        return nullptr;
    }

    void FITKComponentInterface::setCallBackFuns(int k, CallBackFuns f)
    {
        _callBackFuns.insert(k, f);
    }

    void FITKComponentInterface::removeCallBackFun(int k)
    {
        _callBackFuns.remove(k);
    }

    void FITKComponentInterface::setDataObject(const QString& name, Core::FITKAbstractDataObject* data)
    {
        _dataObjects[name] = data;
    }

    Core::FITKAbstractDataObject* FITKComponentInterface::getDataObject(const QString& name)
    {
        return _dataObjects.value(name);
    }

    void FITKComponentInterface::resetDataObjects()
    {
        _dataObjects.clear();
    }

    QList<int> FITKComponentInterface::getUnResetDataObj()
    {
        return QList<int>();
    }

    void FITKComponentInterface::setParentThreadTask(Core::FITKThreadTask* task)
    {
        _parentTask = task;
    }

}



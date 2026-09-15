/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKViewAdaptorFactory.h"

#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Adaptor
{
    //静态变量初始化
    FITKViewAdaptorFactory* FITKViewAdaptorFactory::_instance = nullptr;
    QMutex FITKViewAdaptorFactory::m_mutex;

    Adaptor::FITKAbstractViewAdaptor* FITKViewAdaptorFactory::create(const QString& graphObjType, const QString & className)
    {
        //首先根据文件查找
        if (!_createFuns.contains(graphObjType)) return nullptr;
        VIEWTypeFuns funs = _createFuns.value(graphObjType);
        if (!funs.contains(className)) return nullptr;
        //找到创建函数指针
        auto f = funs.value(className);
        if (f == nullptr) return nullptr;
        return f();
    }

    Adaptor::FITKAbstractViewAdaptor* FITKViewAdaptorFactory::create(const QString& graphObjType, Core::FITKAbstractObject* obj)
    {
        if (obj == nullptr) return nullptr;
        //获取类名，带命名空间
        const QString className = obj->getClassName();
        //根据类名创建
        return this->create(graphObjType, className);
    }

    void FITKViewAdaptorFactory::initialize()
    {

    }

    void FITKViewAdaptorFactory::finalize()
    {
        _createFuns.clear();
    }

    FITKViewAdaptorFactoryDeleator::~FITKViewAdaptorFactoryDeleator()
    {
        if (FITKViewAdaptorFactory::_instance)
            delete FITKViewAdaptorFactory::_instance;
        FITKViewAdaptorFactory::_instance = nullptr;
    }

    static FITKViewAdaptorFactoryDeleator _FVADFDELEATOR_;

}    // namespace Adaptor
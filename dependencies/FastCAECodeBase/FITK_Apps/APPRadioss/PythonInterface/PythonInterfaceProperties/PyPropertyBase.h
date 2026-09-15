/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyPropertyBase.h
 * @brief 属性基类 为python装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
#ifndef _PY_PROPERTY_BASE_H___
#define _PY_PROPERTY_BASE_H___

#include <QString>

#include "PythonInterfacePropertiesAPI.h"

namespace Interface
{
    class FITKAbstractSectionManager;
}
namespace  Radioss
{
    class FITKPropAbstract;
}

/**
 * @brief 属性基类 为python装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
namespace PyInterface
{
    class PythonInterfacePropertiesAPI PyPropertyBase
    {
    public:
        explicit PyPropertyBase(const QString& name);
        explicit PyPropertyBase();
        virtual ~PyPropertyBase() = default;

        /**
         * @brief 获取名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-12
         */
        QString getName();

        /**
         * @brief 设置名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-12
         */
        bool setName(const QString& name);

        /**
         * @brief 创建属性
         * @param[i] name 属性名称
         * @param[i] type 属性类型
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-12
         */
        bool createProperty(const QString& name, int type);

        /**
         * @brief 获取属性指针
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-12
         */
        Radioss::FITKPropAbstract* getProperty();

        /**
         * @brief 刷新渲染
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-12
         */
        void updateRender();

    protected:
        Radioss::FITKPropAbstract* _property{};

        Interface::FITKAbstractSectionManager* _propertyManager{};
    };
}

#endif
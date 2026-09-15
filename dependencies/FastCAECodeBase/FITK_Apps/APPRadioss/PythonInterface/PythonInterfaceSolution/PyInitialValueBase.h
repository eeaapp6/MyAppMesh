/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyInitialValueBase.h
 * @brief 初始值速度场类，为 Python 装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-14
 */
#ifndef _PY_INITIAL_VALUE_BASE_H___
#define _PY_INITIAL_VALUE_BASE_H___

#include <QString>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKAbstractInitialField;
    class FITKInitialFieldManager;
}

namespace PyInterface
{
    /**
     * @brief 初始值速度场类，为 Python 装饰器接口准备的封装类
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    class PythonInterfaceSolutionAPI PyInitialValueBase
    {
    public:
        explicit PyInitialValueBase(const QString& name);
        explicit PyInitialValueBase();
        virtual ~PyInitialValueBase() = default;

        QString getName();

        bool setName(const QString& name);

        bool createInitialValue(const QString& name, int type);

        bool setValue(double x, double y, double z);

        bool setNodeGroup(const QString& name);

        //bool setSystemID(int systemID);

        Radioss::FITKAbstractInitialField* getInitialValue();

        void updateRender();

    public:
        Radioss::FITKAbstractInitialField* _initialValue{};

        Radioss::FITKInitialFieldManager* _initialValueManager{};
    };
}

#endif
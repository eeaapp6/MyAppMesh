/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyEquationOfStateBase.h
 * @brief EOS 基类，为 Python 装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-26
 */
#ifndef _PY_EQUATION_OF_STATE_BASE_H___
#define _PY_EQUATION_OF_STATE_BASE_H___

#include <QString>

#include "PythonInterfacePropertiesAPI.h"

namespace Radioss
{
    class FITKAbstractEquationOfState;
    class FITKEquationOfStateManager;
}

namespace PyInterface
{
    /**
     * @brief EOS 基类，为 Python 装饰器接口准备的封装类
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    class PythonInterfacePropertiesAPI PyEquationOfStateBase
    {
    public:
        explicit PyEquationOfStateBase(const QString& name);
        explicit PyEquationOfStateBase();
        virtual ~PyEquationOfStateBase() = default;

        QString getName();

        bool setName(const QString& name);

        bool createEquationOfState(const QString& name, int type);

        bool addMaterial(const QString& materialName);

        bool removeMaterial(const QString& materialName);

        bool setP0(double value);

        Radioss::FITKAbstractEquationOfState* getEquationOfState();

        void updateRender();

    protected:
        Radioss::FITKAbstractEquationOfState* _equationOfState{};

        Radioss::FITKEquationOfStateManager* _equationOfStateManager{};
    };
}

#endif
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyFailureModelBase.h
 * @brief 失效模型基类，为 Python 装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-26
 */
#ifndef _PY_FAILURE_MODEL_BASE_H___
#define _PY_FAILURE_MODEL_BASE_H___

#include <QString>

#include "PythonInterfacePropertiesAPI.h"

namespace Radioss
{
    class FITKAbstractFailureModel;
    class FITKRadiossFailureModelManager;
}

namespace PyInterface
{
    /**
     * @brief 失效模型基类，为 Python 装饰器接口准备的封装类
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    class PythonInterfacePropertiesAPI PyFailureModelBase
    {
    public:
        explicit PyFailureModelBase(const QString& name);
        explicit PyFailureModelBase();
        virtual ~PyFailureModelBase() = default;

        QString getName();

        bool setName(const QString& name);

        bool createFailureModel(const QString& name, int type);

        bool addMaterial(const QString& materialName);

        bool removeMaterial(const QString& materialName);

        Radioss::FITKAbstractFailureModel* getFailureModel();

        void updateRender();

    protected:
        Radioss::FITKAbstractFailureModel* _failureModel{};

        Radioss::FITKRadiossFailureModelManager* _failureModelManager{};
    };
}

#endif
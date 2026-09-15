/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyProbeBase.h
 * @brief 探针基类，为 Python 装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-26
 */
#ifndef _PY_PROBE_BASE_H___
#define _PY_PROBE_BASE_H___

#include <QString>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKAbstractProbe;
    class FITKProbeManager;
}

namespace PyInterface
{
    /**
     * @brief 探针基类，为 Python 装饰器接口准备的封装类
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    class PythonInterfaceSolutionAPI PyProbeBase
    {
    public:
        explicit PyProbeBase(const QString& name);
        explicit PyProbeBase();
        virtual ~PyProbeBase() = default;

        /**
         * @brief 获取名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-26
         */
        QString getName();

        /**
         * @brief 设置名称
         * @param[i] name 探针名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-26
         */
        bool setName(const QString& name);

        /**
         * @brief 创建探针对象
         * @param[i] name 探针名称
         * @param[i] type 探针类型，见 FITKAbstractProbe::ProbeType
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-26
         */
        bool createProbe(const QString& name, int type);

        /**
         * @brief 获取探针对象指针
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-26
         */
        Radioss::FITKAbstractProbe* getProbe();

        /**
         * @brief 刷新树和预处理图形
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-26
         */
        void updateRender();

    protected:
        Radioss::FITKAbstractProbe* _probe{};

        Radioss::FITKProbeManager* _probeManager{};
    };
}

#endif
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyInteractionBase.h
 * @brief 相互作用基类，为 Python 装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
#ifndef _PY_INTERACTION_BASE_H___
#define _PY_INTERACTION_BASE_H___

#include <QString>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKAbstractInteraction;
    class FITKInteractionManager;
}

namespace PyInterface
{
    /**
     * @brief 相互作用基类，为 Python 装饰器接口准备的封装类
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    class PythonInterfaceSolutionAPI PyInteractionBase
    {
    public:
        explicit PyInteractionBase(const QString& name);
        explicit PyInteractionBase();
        virtual ~PyInteractionBase() = default;

        /**
         * @brief 获取名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-25
         */
        QString getName();

        /**
         * @brief 设置名称
         * @param[in] name 相互作用名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-25
         */
        bool setName(const QString& name);

        /**
         * @brief 创建相互作用对象
         * @param[in] name 相互作用名称
         * @param[in] type 相互作用类型，见 FITKAbstractInteraction::InteractionType
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-25
         */
        bool createInteraction(const QString& name, int type);

        /**
         * @brief 设置主面
         * @param[in] name 主面名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-25
         */
        bool setMasterSurface(const QString& name);

        /**
         * @brief 设置从节点组
         * @param[in] name 从节点组名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-25
         */
        bool setNodeGroup(const QString& name);

        /**
         * @brief 获取相互作用对象指针
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-25
         */
        Radioss::FITKAbstractInteraction* getInteraction();

        /**
         * @brief 刷新树和预处理图形
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-25
         */
        void updateRender();

    protected:
        Radioss::FITKAbstractInteraction* _interaction{};

        Radioss::FITKInteractionManager* _interactionManager{};
    };
}

#endif
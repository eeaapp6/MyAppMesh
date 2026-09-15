/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyBCSBase.h
 * @brief 边界条件基类，为 Python 装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-22
 */
#ifndef _PY_BCS_BASE_H___
#define _PY_BCS_BASE_H___

#include <QString>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKAbstractBCS;
    class FITKRadiossBCSManager;
}

namespace PyInterface
{
    /**
     * @brief 边界条件基类，为 Python 装饰器接口准备的封装类
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    class PythonInterfaceSolutionAPI PyBCSBase
    {
    public:
        explicit PyBCSBase(const QString& name);
        explicit PyBCSBase();
        virtual ~PyBCSBase() = default;

        /**
         * @brief 获取名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-22
         */
        QString getName();

        /**
         * @brief 设置名称
         * @param[i] name 边界条件名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-22
         */
        bool setName(const QString& name);

        /**
         * @brief 创建边界条件对象
         * @param[i] name 边界条件名称
         * @param[i] type 边界条件类型，见 FITKAbstractBCS::BCSType
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-22
         */
        bool createBCS(const QString& name, int type);

        // /**
        //  * @brief 设置坐标系 ID
        //  * @param[i] id 坐标系 ID
        //  * @author wangning (2185896382@qq.com)
        //  * @date 2026-05-22
        //  */
        // bool setCoordinateSystemID(int id);

        /**
         * @brief 设置第一节点组
         * @param[i] name 节点组名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-22
         */
        bool setNodeGroup1(const QString& name);

        /**
         * @brief 设置第二节点组（可选）
         * @param[i] name 节点组名称
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-22
         */
        bool setNodeGroup2(const QString& name);

        /**
         * @brief 获取边界条件对象指针
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-22
         */
        Radioss::FITKAbstractBCS* getBCS();

        /**
         * @brief 刷新树和预处理图形
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-22
         */
        void updateRender();

    protected:
        Radioss::FITKAbstractBCS* _bcs{};

        Radioss::FITKRadiossBCSManager* _bcsManager{};
    };
}

#endif
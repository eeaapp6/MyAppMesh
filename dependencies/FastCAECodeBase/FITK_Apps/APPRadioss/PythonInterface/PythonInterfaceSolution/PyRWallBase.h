/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyRWallBase.h
 * @brief 刚性墙基类 为python装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-13
 */
#ifndef _PY_RWALL_BASE_H___
#define _PY_RWALL_BASE_H___


#include <QString>
#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKAbstractRWall;
    class FITKRadiossRWallManager;
}
/**
 * @brief 刚性墙基类 为python装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-13
 */
namespace PyInterface
{
    class PythonInterfaceSolutionAPI PyRWallBase
    {
    public:
        explicit PyRWallBase(const QString& name);
        explicit PyRWallBase();
        virtual ~PyRWallBase() = default;

        /**
         * @brief  获取名称
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-12
         */
        QString getName();
        /**
         * @brief  设置名称
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-12
         */
        bool setName(const QString& name);
        /**
         * @brief  创建刚性墙
         * @param[i] name 刚性墙名称
         * @param[i] type 刚性墙类型
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-12
         */
        bool createRWall(const QString& name, int type);

        /**
         * @brief  设置参考节点ID
         * @param[i] value 参考节点ID
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-14
         */
        //bool setNodeId(int value);

        /**
         * @brief  设置滑移类型
         * @param[i] type 滑移类型
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-14
         */
        bool setSlidingType(int type);
        /**
         * @brief  设置搜索距离
         * @param[i] value 搜索距离
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-14
         */
        bool setDSearch(double value);
        /**
         * @brief  设置滤波因子
         * @param[i] value 滤波因子
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-14
         */
        bool setFilteringFactor(double value);
        /**
         * @brief  设置滤波标志
         * @param[i] value 滤波标志
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-14
         */
        bool setFilteringFlag(int value);

        /**
         * @brief  设置第一节点组
         * @param[i] name 节点组名称
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-14
         */
        bool setNodeGroup1(const QString& name);

        /**
         * @brief  设置第二节点组
         * @param[i] name 节点组名称
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-14
         */
        bool setNodeGroup2(const QString& name);

        //获取刚性墙指针
        Radioss::FITKAbstractRWall* getRWall();

        //刷新渲染
        void updateRender();

    protected:
        Radioss::FITKAbstractRWall* _rWall{};

        Radioss::FITKRadiossRWallManager* _rWallManager{};
    };
}

#endif
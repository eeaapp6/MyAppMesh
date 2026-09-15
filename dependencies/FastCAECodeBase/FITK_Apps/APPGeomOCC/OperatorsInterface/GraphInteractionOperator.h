/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    GraphInteractionOperator.h
 * @brief   三维拾取操作器接口抽象类。
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-04-17
 *********************************************************************/

#ifndef __GRAPHINTERACTIONOPERATOR_H__
#define __GRAPHINTERACTIONOPERATOR_H__

#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"

#include "OperatorsInterfaceAPI.h"

#include <QColor>

 // 前置声明

namespace Comp
{
    class FITKGraph3DWindowVTK;
}

namespace EventOper
{
    /**
     * @brief   三维拾取操作器接口抽象类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-04-17
     */
    class OperatorsInterfaceAPI GraphInteractionOperator : public Core::FITKAbstractOperator
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-04-17
         */
        explicit GraphInteractionOperator() = default;

        /**
         * @brief   析构函数。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-04-17
         */
        virtual ~GraphInteractionOperator() = default;

        /**
         * @brief   根据当前拾取对象类型与拾取方法对当前所有已实例化模型
         *          及全部基准元素开启或关闭拾取状态。[虚函数]
         * @param   dataObjId：生效数据ID（-1则对所有数据生效）[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        virtual void setGraphStateByPickInfo(int dataObjId = -1);

        /**
         * @brief   开启或关闭可视化对象拾取状态。[虚函数]
         * @param   model：是否开启模型拾取
         * @param   sketch：是否开启草图拾取
         * @param   datum：是否开启基准元素拾取
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-14
         */
        virtual void setEnableGraphPickable(bool model, bool sketch, bool datum);

        /**
         * @brief   设置为默认拾取状态。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        virtual void setCustomPickMode();

        /**
         * @brief   完全关闭拾取状态。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        virtual void clearPickMode();

    };
}   // namespace EventOper

#endif // __GRAPHINTERACTIONOPERATOR_H__
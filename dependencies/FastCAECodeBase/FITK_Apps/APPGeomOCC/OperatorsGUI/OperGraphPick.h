/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperGraphPick.h
 * @brief   拾取相关辅助功能操作器。
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-31
 *********************************************************************/

#ifndef __OPERGRAPHPICK_H__
#define __OPERGRAPHPICK_H__

#include "OperatorsInterface/GraphInteractionOperator.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

 // 前置声明
namespace Render
{
    class FITKGraph3DWindowOCC;
}

namespace GUIOper
{
    /**
     * @brief   拾取相关辅助功能操作器。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-31
     */
    class OperGraphPick : public EventOper::GraphInteractionOperator
    {
    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        explicit OperGraphPick() = default;

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        ~OperGraphPick() = default;

        /**
         * @brief   根据当前拾取对象类型与拾取方法对当前所有已实例化模型
         *          及全部基准元素开启或关闭拾取状态。[重写]
         * @param   dataObjId：生效数据ID（-1则对所有数据生效）[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        void setGraphStateByPickInfo(int dataObjId = -1) override;

        /**
         * @brief   开启或关闭可视化对象拾取状态。[重写]
         * @param   model：是否开启模型拾取
         * @param   sketch：是否开启草图拾取
         * @param   datum：是否开启基准元素拾取
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-14
         */
        void setEnableGraphPickable(bool model, bool sketch, bool datum) override;

        /**
         * @brief   设置为默认拾取状态。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        void setCustomPickMode() override;

        /**
         * @brief   完全关闭拾取状态。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        void clearPickMode() override;

    private:
        /**
         * @brief   获取当前激活状态的OCC渲染窗口。
         * @return  渲染窗口
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        Render::FITKGraph3DWindowOCC* getGraphWidget();

    };

    Register2FITKOperatorRepo(GraphPick, OperGraphPick);

}  // namespace GUIOper

#endif // __OPERGRAPHPICK_H__

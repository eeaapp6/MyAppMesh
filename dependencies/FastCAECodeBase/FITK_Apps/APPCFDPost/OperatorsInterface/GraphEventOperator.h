/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphEventOperator.h
 * @brief  三维可视化对象数据操作器接口类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/

#ifndef __GRAPHEVENTOPERATOR_H__
#define __GRAPHEVENTOPERATOR_H__

#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"
#include "OperatorsInterfaceAPI.h"

namespace EventOper
{
    class OperatorsInterfaceAPI GraphEventOperator : public Core::FITKAbstractOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit GraphEventOperator() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~GraphEventOperator() = default;
        /**
         * @brief    更新所有
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual void updateAll();
        /**
         * @brief    更新单个
         * @param[i] postDataID  后处理数据id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual void updateSingle(int postDataID);
        /**
         * @brief    更新灯光
         * @param[i] dataID      数据id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        virtual void updateLight(int dataID);
        /**
         * @brief    更新是否显示
         * @param[i] postDataID     数据id
         * @param[i] isShow         是否显示
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-11
         */
        virtual void updateShow(int postDataID, bool isShow);
        /**
         * @brief    更新是否全部旋转可见性
         * @param[i] isVis                      是否全部可见
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-19
         */
        virtual void updateAllRotateVis(bool isVis);
        /**
         * @brief    刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual void reRender();
    };
}  

#endif
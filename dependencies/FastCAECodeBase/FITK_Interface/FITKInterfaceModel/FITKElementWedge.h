/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKElementWedge.h
 * @brief  三棱柱单元
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-13
 *
 */
#ifndef _FITKELEMENT_WEDGE_H__
#define _FITKELEMENT_WEDGE_H__

#include "FITKInterfaceModelAPI.h"
#include "FITKAbstractElement.h"

namespace Interface
{
    /**
     * @brief 6节点三棱柱
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKInterfaceModelAPI FITKElementWedge6 : public FITKAbstractElement
    {
    public:
        explicit FITKElementWedge6();
        virtual ~FITKElementWedge6() = default;

        /**
        * @brief 获取单元类型
        * @return FITKEleType
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-28
        */
        virtual FITKModelEnum::FITKEleType getEleType() override;

        /**
         * @brief       获取高阶标记。[虚函数][重写]
         * @return      是否为高阶单元
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-05
         */
        virtual bool getQuadraticFlag() override;

        /**
         * @brief 获取单元节点数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual int getNodeCount() override;

        /**
         * @brief 获取单元包含的面数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual int getFaceCount()override;
        /**
         * @brief 获取单元包含的边数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual int getEdgeCount()override;
        /**
         * @brief 获取第index个面
         * @param[i]  index         面索引，从0开始
         * @param[i]  lowerOrder    是否降低阶次
         * @return QList<int>       面的节点索引，其中的成员是指单元中的节点索引，其成员大小不超过getNodeCount
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual QList<int> getFace(const int index, const bool & lowerOrder)override;
        /**
        * @brief 获取第index个边
        * @param[i]  index         边索引，从0开始
        * @return QList<int>       边的节点索引，其中的成员是指单元中的节点索引，其成员大小不超过getNodeCount
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-28
        */
        virtual QList<int> getEdge(const int index) override;
        /**
         * @brief 获取单元维度
         * @return int  1维线单元 2维壳单元 3维体单元
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-09
         */
        virtual int getElementDim() override;
    };

    /**
     * @brief       15节点三棱柱单元。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-02-02
     */
    class FITKInterfaceModelAPI FITKElementWedge15 : public FITKAbstractElement
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-02
         */
        explicit FITKElementWedge15();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-02
         */
        virtual ~FITKElementWedge15() = default;

        /**
         * @brief       获取单元类型。[虚函数][重写]
         * @return      单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-02
         */
        virtual FITKModelEnum::FITKEleType getEleType() override;

        /**
         * @brief       获取高阶标记。[虚函数][重写]
         * @return      是否为高阶单元
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-02
         */
        virtual bool getQuadraticFlag() override;

        /**
         * @brief       获取节点数量。[虚函数][重写]
         * @return      节点数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-02
         */
        virtual int getNodeCount() override;

        /**
         * @brief       获取单元面数量。[虚函数][重写]
         * @return      单元面数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-02
         */
        virtual int getFaceCount()override;

        /**
         * @brief       获取单元边数量。[虚函数][重写]
         * @return      单元边数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-02
         */
        virtual int getEdgeCount()override;

        /**
         * @brief       获取单元面。[虚函数][重写]
         * @param[in]   index：面索引
         * @param[in]   lowerOrder：是否降阶为低阶
         * @return      单元面节点索引列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-02
         */
        virtual QList<int> getFace(const int index, const bool & lowerOrder)override;

        /**
         * @brief       获取单元边。[虚函数][重写]
         * @param[in]   index：边索引
         * @return      单元边节点索引列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-02
         */
        virtual QList<int> getEdge(const int index) override;

        /**
         * @brief       获取单元维度。[虚函数][重写]
         * @return      单元维度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-02
         */
        virtual int getElementDim() override;

    };
}


#endif


/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKElementLine.h
 * @brief  线单元声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-13
 *
 */
#ifndef _FITKELEMENT_LINE_H__
#define _FITKELEMENT_LINE_H__

#include "FITKInterfaceModelAPI.h"
#include "FITKAbstractElement.h"

namespace Interface
{
    /**
     * @brief 抽象线单元类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-13
     */
    class FITKInterfaceModelAPI FITKAbstractElementLine : public FITKAbstractElement
    {
    public:
        explicit FITKAbstractElementLine() = default;
        virtual ~FITKAbstractElementLine() = default;
        /**
         * @brief 设置线单元方向节点ID
         * @param[i]  dirNodeID   方向节点ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-12-29
         */
        void setDirectionNodeID(const unsigned int dirNodeID);
        /**
         * @brief 获取线单元方向节点ID
         * @return unsigned int 方向节点ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-12-29
         */
        unsigned int getDirectionNodeID() const;

    protected:
        /**
         * @brief 线单元方向节点ID，用于标记单元方向
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-12-29
         */
        unsigned int _dirNodeID{ 0 };


    };

    /**
     * @brief 2节点线
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKInterfaceModelAPI FITKElementLine2 : public FITKAbstractElementLine
    {
    public:
        explicit FITKElementLine2();
        virtual ~FITKElementLine2() = default;

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
         * @param[i]  lowerOrder   是否降低阶次
         * @return QList<int>       面的节点索引，其中的成员是指单元中的节点索引，其成员大小不超过getNodeCount
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual QList<int> getFace(const int index, const bool & lowerOrder = false)override { return QList<int>(); };
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
     * @brief 3节点线
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKInterfaceModelAPI FITKElementLine3 : public FITKAbstractElementLine
    {
    public:
        explicit FITKElementLine3();
        virtual ~FITKElementLine3() = default;
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
        virtual int getFaceCount() override;
        /**
         * @brief 获取单元包含的边数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual int getEdgeCount() override;
        /**
         * @brief 获取第index个面
         * @param[i]  index         面索引，从0开始
         * @param[i]  lowerOrder   是否降低阶次
         * @return QList<int>       面的节点索引，其中的成员是指单元中的节点索引，其成员大小不超过getNodeCount
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual QList<int> getFace(const int index, const bool & lowerOrder = false) override { return QList<int>(); }
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
}


#endif


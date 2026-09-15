/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKElementVertex.h
 * @brief  点单元声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-10-23
 * 
 */
#ifndef _FITKINTERFACE_ELEMENT_VERTEX_H__
#define _FITKINTERFACE_ELEMENT_VERTEX_H__

#include "FITKInterfaceModelAPI.h"
#include "FITKAbstractElement.h"

namespace Interface
{
    /**
     * @brief 1节点点单元
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-10-23
     */
    class FITKInterfaceModelAPI FITKElementVertex1: public FITKAbstractElement
    { 
    public:
        /** 
         * @brief Construct a new FITKElementVertex1 object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-23
         */
        explicit FITKElementVertex1();
        /**
         * @brief Destroy the FITKElementVertex1 object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-23
         */
        virtual ~FITKElementVertex1() = default;

        /**
        * @brief 获取单元类型
        * @return FITKEleType
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-10-23
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
         * @date 2025-10-23
         */
        virtual int getNodeCount() override;

        /**
         * @brief 获取单元包含的面数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-23
         */
        virtual int getFaceCount()override;
        /**
        * @brief 获取单元包含的边数量
        * @return int
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-10-23
        */
        virtual int getEdgeCount()override;
        /**
         * @brief 获取第index个面
         * @param[i]  index         面索引，从0开始
         * @param[i]  lowerOrder    是否降低阶次
         * @return QList<int>       面的节点索引，其中的成员是指单元中的节点索引，其成员大小不超过getNodeCount
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-23
         */
        virtual QList<int> getFace(const int index, const bool & lowerOrder = false)override;
        /**
        * @brief 获取第index个边
        * @param[i]  index         边索引，从0开始
        * @return QList<int>       边的节点索引，其中的成员是指单元中的节点索引，其成员大小不超过getNodeCount
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-10-23
        */
        virtual QList<int> getEdge(const int index) override;
        /**
         * @brief 获取单元维度
         * @return int  1维线单元 2维壳单元 3维体单元
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-23
         */
        virtual int getElementDim();
    };
}

#endif
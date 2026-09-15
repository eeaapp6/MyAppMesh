/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAssElement.h
 * @brief 装配单元，连接器，弹簧等
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-24
 *
 */
#ifndef _FITK_ASS_ELEMENTS_H__
#define _FITK_ASS_ELEMENTS_H__

#include "FITKInterfaceModelAPI.h"
#include <QObject>
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"


namespace Interface
{
    class FITKAbaAssembly;
    /**
     * @brief 抽象单元类型
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-24
     */
    class FITKInterfaceModelAPI FITKAbstractAssemblyElement
    {
    public:
        /**
         * @brief Construct a new FITKAbstractAssemblyElement object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        explicit FITKAbstractAssemblyElement() ;
        /**
         * @brief Destroy the FITKAbstractAssemblyElement object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual ~FITKAbstractAssemblyElement() = default;
        /**
         * @brief 获取单元ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        int getElementID()const;

        /**
         * @brief 设置单元ID
         * @param[i] int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-03
         */
        void setElementID(const int id);

        /**
         * @brief 复制到另一个单元
         * @param[i] e 输出的单元
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual void copyTo(FITKAbstractAssemblyElement*) = 0;

    private:
        /**
         * @brief 单元ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        int _elementID{ -1 };
    };
    /**
     * @brief 一个点的单元，质点等类型
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-24
     */
    class FITKInterfaceModelAPI FITKAssemblyElementVertex: public FITKAbstractAssemblyElement
    {
    public:
        using FITKAbstractAssemblyElement::FITKAbstractAssemblyElement;
        /**
         * @brief Destroy the FITKAssemblyElementVertex object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual ~FITKAssemblyElementVertex() = default;
        /**
         * @brief 数据拷贝到单元
         * @param[i]  e            单元
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual void copyTo(FITKAbstractAssemblyElement* e) override ;
        /**
         * @brief 设置模型ID
         * @param[i]  mid           模型ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        void setModelID(const int mid);

        /**
         * @brief       设置子模型ID。
         * @param[in]   subMid：子模型ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-14
         */
        void setSubModelID(const int subMid);

        /**
         * @brief 设置节点ID
         * @param[i]  id            节点ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        void setNodeID(const int id);
        /**
         * @brief 获取坐标
         * @param[i]  coor          坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        void getCoor(double* coor);

        /**
         * @brief 获取节点1
         * @param[o]  mid           模型ID
         * @param[o]  id            节点ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-01
         */
        void getNode(int& mid, int& id);

        /**
         * @brief       获取节点子模型ID。
         * @return      子模型ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-14
         */
        int getSubModelID();

    private:
        /**
         * @brief 模型ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        int _nodeModelID{ -1 };

        /**
         * @brief       子模型ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-14
         */
        int _nodeSubModelID{ -1 };

        /**
         * @brief 节点ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        int _nodeID{ -1 };
    };
     /**
     * @brief 线单元
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-24
     */
    class FITKInterfaceModelAPI FITKAssemblyElementLine: public FITKAbstractAssemblyElement
    {
    public:
        using FITKAbstractAssemblyElement::FITKAbstractAssemblyElement;
        virtual ~FITKAssemblyElementLine() = default;
        /**
         * @brief 数据拷贝到单元
         * @param[i]  e            单元
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual void copyTo(FITKAbstractAssemblyElement* e) override;
        /**
         * @brief 设置节点1
         * @param[i]  mid           模型ID（装配实例、装配体）
         * @param[i]  id            节点ID
         * @param[i]  subMid        子模型ID（几何、网格模型）
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        void setNode1(const int mid, const int id, const int subMid = 0);

        /**
         * @brief 设置节点2
         * @param[i]  mid           模型ID（装配实例、装配体）
         * @param[i]  id            节点ID
         * @param[i]  subMid        子模型ID（几何、网格模型）
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        void setNode2(const int mid, const int id, const int subMid = 0);

        /**
         * @brief       获取节点1子模型ID。
         * @return      子模型ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-14
         */
        int getSubModelID1();

        /**
         * @brief       获取节点2子模型ID。
         * @return      子模型ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-14
         */
        int getSubModelID2();

        /**
         * @brief 获取节点1
         * @param[o]  mid           模型ID
         * @param[o]  id            节点ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-01
         */
        void getNode1(int& mid, int& id);
        /**
         * @brief 获取节点2
         * @param[o]  mid           模型ID
         * @param[o]  id            节点ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-01
         */
        void getNode2(int& mid, int& id);
        /**
         * @brief 获取坐标
         * @param[o]  coor1         坐标，三分量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        void getCoor1(double* coor1);
        /**
         * @brief 获取第二点坐标
         * @param[o]  coor2          坐标值，三分量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        void getCoor2(double* coor2);
        /**
         * @brief 获取第一点标签
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        QString getTextLabel1();
        /**
         * @brief 获取第二点标签
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        QString getTextLabel2();
        /**
         * @brief 交换第一点与第二点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-01
         */
        void swap();

    private:
        /**
         * @brief 第一个点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        QPair<int, int> _node1{};
        /**
         * @brief 第二个点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        QPair<int, int> _node2{};

        /**
         * @brief       节点1子模型ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-14
         */
        int _subMid1 = 0;

        /**
         * @brief       节点2子模型ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-14
         */
        int _subMid2 = 0;

    };


    /**
     * @brief 单元集合
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-24
     */
    class FITKInterfaceModelAPI FITKAssemblyElementSet : public FITKModelSet
    {
    public:
        /**
         * @brief Construct a new FITKAssemblyElementSet object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        explicit FITKAssemblyElementSet() = default;
        /**
         * @brief Destroy the FITKAssemblyElementSet object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual  ~FITKAssemblyElementSet() = default;

        /**
        * @brief 获取集合类型
        * @return FITKModelEnum::FITKModelSetType
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-17
        */
        virtual FITKModelEnum::FITKModelSetType getModelSetType() override;
    };


}



#endif

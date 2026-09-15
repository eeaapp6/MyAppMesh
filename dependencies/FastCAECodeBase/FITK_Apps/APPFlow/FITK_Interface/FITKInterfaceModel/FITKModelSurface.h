/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKModelSurface.h
 * @brief       模型表面抽象类型。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-07-28
 *********************************************************************/

#ifndef __FITKMODELSURFACE_H__
#define __FITKMODELSURFACE_H__

#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModelComponent.h"

 // 前置声明。
namespace Interface
{
    class FITKModelSet;
}

namespace Interface
{
    //表面的几何信息，复合集合展开并过滤，仅包含最底层
    struct SurfaceAbsSetInfo
    {
        FITKAbstractModel* _model{};
        FITKModelSet* _set{};
        //面索引，单元表面特有
        int _surfaceIndex{ -1 };
        //点权重，节点表面特有
        double _nodeWidght{ 0 };
    };

    /**
     * @brief       模型表面抽象类型。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-28
     */
    class FITKInterfaceModelAPI FITKModelSurface : public FITKAbstractModelComponent
    {
        Q_OBJECT

    public:
        /**
         * @brief       模型表面类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-28
         */
        enum ModelSurfaceType
        {
            MST_None = -1,
            MST_Geom = 0,
            MST_MeshNode,
            MST_MeshEle
        };
        Q_ENUM(ModelSurfaceType);

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-28
         */
        explicit FITKModelSurface() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-28
         */
        virtual ~FITKModelSurface();

        /**
         * @brief       获取模型表面类型。[纯虚函数]
         * @return      模型表面类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-28
         */
        virtual ModelSurfaceType getModelSurfaceType() = 0;
        /**
         * @brief       获取集合数量。
         * @return      集合数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-29
         */
        int getSetCount() const;

        /**
         * @brief       根据索引获取集合ID。
         * @param[in]   index：集合索引
         * @return      集合ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-11
         */
        int getSetIDAt(const int index) const;

        /**
         * @brief       根据索引获取集合。
         * @param[in]   index：集合索引
         * @return      集合
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-29
         */
        FITKModelSet* getSetAt(const int index) const;

        /**
         * @brief 是否为内部面
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        bool isInternal() const;

        /**
         * @brief 设置是否为内部面
         * @param[i]  internal       是否内部面
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        void isInternal(bool internal);

        /**
         * @brief 获取第index个集合属于的模型，part instance assembly等
         * @param[i]  index         索引
         * @return FITKModelSet*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        FITKAbstractModel* getModelAt(const int index) const;

        /**
        * @brief 获取集合信息，将复合集合拆分，返回值不会包含复合集合
        * @return QList<SurfaceAbsSetInfo>
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-26
        */
        virtual QList<SurfaceAbsSetInfo> getAbsoluteSetInfo();

        /**
         * @brief       根据索引移除子集。
         * @param[in]   index：集合索引
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-16
         */
        virtual void removeSetAt(int index);

        /**
         * @brief       清空子集。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        virtual void clearSets();

    protected:
        /**
         * @brief 集合的索引
         * @param[i]  modelID        模型id
         * @param[i]  setID          集合ID
         * @return int               索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-18
         */
        int indexOf(int modelID, int setID);

    protected:
        /**
         * @brief 是否内部表面
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        bool _internal{ false };

        /**
         * @brief 集合ID, 第一个值为model，可以是instace等，第二值为集合id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        QList<QPair<int, int>> _modelSetIDs{};

    };
}

#endif // !__FITKMODELSURFACE_H__

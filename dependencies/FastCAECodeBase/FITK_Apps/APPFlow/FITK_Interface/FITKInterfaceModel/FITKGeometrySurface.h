/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKGeometrySurface.h
 * @brief       几何模型表面。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-07-28
 *********************************************************************/

#ifndef __FITKGEOMETRYSURFACE_H__
#define __FITKGEOMETRYSURFACE_H__

#include "FITKModelSurface.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    /**
     * @brief       几何模型表面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-28
     */
    class FITKInterfaceModelAPI FITKGeometrySurface : public FITKModelSurface
    {
        FITKCLASS(Interface, FITKGeometrySurface);
        Q_OBJECT

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-28
         */
        explicit FITKGeometrySurface() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-28
         */
        virtual ~FITKGeometrySurface() = default;

        /**
         * @brief       获取模型表面类型。[虚函数][重写]
         * @return      模型表面类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-28
         */
        virtual ModelSurfaceType getModelSurfaceType() override;

        /**
         * @brief 追加表面
         * @param[i]  modelID        set的父对象id，可以是装配、部件 装配实例
         * @param[i]  setID          集合ID
         * @param[i]  surfIndex      表面索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        void addGeometrySet(int modelID, int setID, int surfIndex);

        /**
         * @brief 追加表面
         * @param[i]  model          set的父对象，可以是装配、部件 装配实例
         * @param[i]  set            集合
         * @param[i]  surfIndex      面索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        void addGeometrySet(FITKAbstractModel* model, FITKModelSet* set, int surfIndex);

        /**
         * @brief  清除表面集合.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-11-14
         */
        void clearGeometrySet();

        /**
         * @brief 获取第index个面的索引
         * @param[i]  index          索引
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        int getSurfaceIndexAt(const int index) const;

        /**
         * @brief 设置第index个面的索引
         * @param[i] index        集合索引
         * @param[i]  surfIndex          面索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        void setSurfaceIndex(const int index, const int surfIndex);


        /**
        * @brief 获取集合信息，将复合集合拆分，返回值不会包含复合集合
        * @return QList<SurfaceAbsSetInfo>
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-26
        */
        virtual QList<SurfaceAbsSetInfo> getAbsoluteSetInfo() override;

    protected:
        /**
         * @brief       表面中集合的面索引列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        QList<int> m_surfIndice{};

    };
}

#endif // !__FITKGEOMETRYSURFACE_H__

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAbstractRadiossSurface.h
 * @brief  Radioss表面抽象类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-09-18
 * 
 */
#ifndef _FITK_ABSTRACT_RADIOSS_SURFACE_H__
#define _FITK_ABSTRACT_RADIOSS_SURFACE_H__

#include "FITKAbstractRadiossData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceElement.h"

namespace Radioss
{
    class FITKRadiossMeshModel;
    /**
     * @brief  Radioss表面抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-09-18
     */
    class FITKRadiossDataAPI FITKAbstractRadiossSurface : public Interface::FITKMeshSurfaceElement,
                                                public FITKAbstractRadiossData
    {
    public:
        /**
         * @brief Radioss表面类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        enum RadiossSurfaceType
        {
            RST_None = -1,
            RST_SEG = 0,        //所有类型都转换成/SURF/SEG
            RST_PART,   
            RST_PartAll,
            RST_PartExt,
            RST_SURF,
            RST_GRSHEL,
            RST_GRSH3N,
        };
        Q_ENUM(RadiossSurfaceType);

    public:
        /**
         * @brief Construct a new FITKAbstractRadiossSurface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        explicit FITKAbstractRadiossSurface() = default;
        /**
         * @brief Destroy the FITKAbstractRadiossSurface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        virtual ~FITKAbstractRadiossSurface() = 0;
        /**
         * @brief 获取Radioss表面类型
         * @return RadiossSurfaceType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        virtual RadiossSurfaceType getRadiossSurfaceType() = 0;
        /**
         * @brief  添加单元表面
         * @param[i]  element       单元ID
         * @param[i]  surfaceIndex  表面索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        void addElementSurface(int element, int surfaceIndex);
        /**
         * @brief 获取表面包含的全部节点ID列表
         * @return int
         * @author LiBaojun 
         * @date 2025-09-26
         */
        QList<int> getSurfaceNodeIDs();

        /**
         * @brief 获取指定单元ID的表面索引
         * @param[i]  elementID     单元ID
         * @return QList<int>  表面索引列表
         * @author LiBaojun
         * @date 2025-10-15
         */
        QList<int> getElementSurfaceIDs(const int &elementID ) const;

        /**
        * @brief 获取Radioss表面信息
        * @return RadIOInfo*  Radioss表面信息指针,key为表面索引列表，value为单元ID列表
        * @author wangning (2185896382@qq.com)
        * @date 2025-10-15
        */
        QHash<int, QList<int>> getRadiossSurfaceInfo() const;

    protected:
        /**
         * @brief 获取Radioss网格模型
         * @return FITKRadiossMeshModel*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-18
         */
        FITKRadiossMeshModel* getRadiossMeshModel();

    protected:
        QMultiHash<int,int> _elementSurface{};//单元表面，单元ID-表面索引

    };

} // namespace Radioss

#endif // _FITK_ABSTRACT_RADIOSS_SURFACE_H__
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAbaqus2RadiossDataMapper.h
 * @brief  Abaqus到Radioss数据映射器类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-10-21
 * 
 */
#ifndef _FITKAbaqus2RadiossDataMapper_H_
#define _FITKAbaqus2RadiossDataMapper_H_

#include "FITKAbaqusRadiossDataMapperAPI.h"

namespace AbaqusData
{
    class FITKDataCase;
}

namespace Radioss
{
    class FITKRadiossCase;
}

namespace AbaqusRadiossDataMapper
{
    /**
     * @brief  Abaqus到Radioss数据映射器类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-10-21
     */
    class FITKAbaqusRadiossDataMapperAPI FITKAbaqus2RadiossDataMapper
    {
    public:
        /**
         * @brief Construct a new FITKAbaqus2RadiossDataMapper object
         * @param[i]  abaqusCase     Abaqus数据对象指针
         * @param[i]  radiossCase    Radioss数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-21
         */
        explicit FITKAbaqus2RadiossDataMapper( AbaqusData::FITKDataCase* abaqusCase,
                                               Radioss::FITKRadiossCase* radiossCase );
        /**
         * @brief Destroy the FITKAbaqus2RadiossDataMapper object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-21
         */
        virtual ~FITKAbaqus2RadiossDataMapper()= default;
        /**
         * @brief 执行映射操作
         * @return true  映射成功
         * @return false  映射失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-21
         */
        bool map();

    private:
        /**
         * @brief 映射网格部件
         * @return true  映射成功
         * @return false 映射失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-28
         */
        bool mapMeshParts();

    private:
        /**
         * @brief Abaqus数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-21
         */
        AbaqusData::FITKDataCase* _abaqusCase{ nullptr };
        /**
         * @brief Radioss数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-21
         */
        Radioss::FITKRadiossCase* _radiossCase{ nullptr };
    };


} // namespace AbaqusRadiossDataMapper


#endif
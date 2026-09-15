/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAbstractBCS.h
 * @brief  Radioss边界条件基类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-06-05
 * 
 */
#ifndef FITK_ABSTRACT_BCS_H
#define FITK_ABSTRACT_BCS_H

#include "FITKRadiossDataAPI.h"
#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{
    /**
     * @brief   Radioss边界条件抽象基类
     * @details 此类定义了Radioss边界条件的通用接口，包括坐标系ID和组ID等属性。
     *          所有Radioss边界条件类型（如节点约束等）都需要继承此类。
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-06-05
     */
    class FITKRadiossDataAPI FITKAbstractBCS 
        : public Core::FITKAbstractNDataObject, public FITKAbstractRadiossData
    {
        Q_OBJECT
    public:
        enum BCSType
        {
            BCS= 1,       ///< Boundary Condition 限制节点6自由度
            BSSCyclic = 2, ///< Cyclic Boundary Condition
            BCSLAGMUL = 3  ///< Lagrange Multiplier Condition
        };

    public:
        explicit FITKAbstractBCS();
        virtual ~FITKAbstractBCS() = 0;

        /**
         * @brief 设置坐标系ID
         * @param[i]  id  坐标系ID, 0表示全局坐标系
         * @author LiBaojun (libaojunqd@foxmail.com)
         */
        void setCoordinateSystemID(int id); 
        /**
         * @brief 获取坐标系ID
         * @return int 坐标系ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         */
        int getCoordinateSystemID() const;
        /**
         * @brief 设置组ID
         * @param[i]  id1  组ID1
         * @param[i]  id2  组ID2
         * @author LiBaojun (libaojunqd@foxmail.com)
         */
        void setGroupID(int id1, int id2 = 0);

        /**
         * @brief 获取组ID1
         * @return int 组ID1
         * @author LiBaojun (libaojunqd@foxmail.com)
         */
        int getGroupID1() const;
        /**
         * @brief 获取组ID2
         * @return int 组ID2
         * @author LiBaojun (libaojunqd@foxmail.com)
         */
        int getGroupID2() const;

        /**
         * @brief 获取边界条件类型
         * @return BCSType 边界条件类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         */
        virtual BCSType getBCSType() const;

    private: 
        int _coordinateSystemID{0}; // Coordinate system ID
        int _groupID1{0}; // Group ID 1, default is 0
        int _groupID2{0}; // Group ID 2, default is 0
    };
    
    /**
     * @brief Radioss边界条件数据管理器
     * @details 此类用于管理Radioss边界条件数据对象的集合，提供添加、删除、查询等功能。
     *          继承自FITKAbstractDataManager，专门处理FITKAbstractBCS类型的数据。
     * @author LiBaojun (libaojunqd@foxmail.com)
     */
    class FITKRadiossDataAPI FITKRadiossBCSManager :
        public Core::FITKAbstractDataManager<FITKAbstractBCS>
    {
    public:
        explicit FITKRadiossBCSManager()= default;

        virtual ~FITKRadiossBCSManager() = default;

    };

} // namespace Radioss

#endif // FITK_ABSTRACT_BCS_H

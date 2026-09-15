/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaTransform.h
 * @brief  坐标系转换与管理器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-05-25
 *
 */
#ifndef  __FITLABATRANSFORM_H___
#define  __FITLABATRANSFORM_H___

#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Interface/FITKInterfaceModel/FITKCoordinateSystem.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Core
{
    class FITKPoint;
}

namespace Interface
{
    /**
     * @brief 坐标系转换定义
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-25
     */
    class FITKInterfaceStructuralAPI FITKAbaTransform : public FITKCoordinateSystem
    {
        FITKCLASS(Interface, FITKAbaTransform);
    public:
        /**
         * @brief 坐标系转换定义
         * @param[i]  sysType        坐标系类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        explicit FITKAbaTransform(FCSysType sysType);
        /**
         * @brief Destroy the FITKAbaTransform object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual ~FITKAbaTransform();
        /**
         * @brief 设置第一点和第二点
         * @param[i]  p1             第一点
         * @param[i]  p2             第二点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        void setTransform(double* p1, double* p2);
        
        /**
         * @brief 获取第一点和第二点
         * @param[o]  p1             第一点
         * @param[o]  p2             第二点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        void getTransform(double* p1, double* p2);
       


        /**
         * @brief 获取在某一点的坐标系的三个方向，默认是局部坐标系的三个方向
         * @param[i] pxyz      输入点坐标位置
         * @param[o]  dirs            三个轴线方向，9个分量，dx1,dy1,dz1,dx2,dy2,dz2,dx3,dx3,dz3
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual void get3DirectionAtPoint(const double* pxyz, double* dirs) override;

    private:
        /**
         * @brief 更新坐标系
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        void update();
        /**
         * @brief 创建笛卡尔坐标系
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        void createCartesianSystem();
        /**
         * @brief 创建柱坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        void createCylinderSystem();
        /**
         * @brief 创建球坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        void createSphereSystem();

    private:
        /**
         * @brief 第一点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        Core::FITKPoint* _point1{};
        /**
         * @brief 第二点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        Core::FITKPoint* _point2{};
 
    };
    /**
     * @brief 坐标变换管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-25
     */
    class FITKInterfaceStructuralAPI FITKAbaTransformManager :
        public Core::FITKAbstractDataManager<FITKAbaTransform>
    {
    public:
        /**
         * @brief Construct a new FITKAbaTransformManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        explicit FITKAbaTransformManager() = default;
        /**
         * @brief Destroy the FITKAbaTransformManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual ~FITKAbaTransformManager() = default;
    };

}

#endif

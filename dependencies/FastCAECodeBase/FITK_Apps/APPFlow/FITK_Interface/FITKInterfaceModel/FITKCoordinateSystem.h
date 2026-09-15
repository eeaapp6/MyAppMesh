/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKCoordinateSystem.h
 * @brief 坐标系定义
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-05-25
 *
 */
#ifndef  __FITKCOORDINATE_SYSTEM_H___
#define __FITKCOORDINATE_SYSTEM_H___

#include "FITKInterfaceModelAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Core
{
    class FITKPoint;
}

namespace Interface
{
    /**
     * @brief 定义坐标系
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-25
     */
    class FITKInterfaceModelAPI FITKCoordinateSystem : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKCoordinateSystem);
    public:
       /**
        * @brief 坐标系类型
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-05-25
        */
        enum FCSysType
        {
            FCSCartesian,  ///< 笛卡尔坐标系
            FCSCylinder,   ///< 圆柱坐标系
            FCSSphere,     ///< 球坐标系
        };
        Q_ENUM(FCSysType);
    public:
        /**
         * @brief Construct a new FITKCoordinateSystem object
         * @param[i]  sysType        坐标系类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        explicit FITKCoordinateSystem(FCSysType sysType);
        /**
         * @brief Destroy the FITKCoordinateSystem object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual ~FITKCoordinateSystem();
        /***
         * @brief 获取坐标系类型
         * @return FCSysType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        FCSysType getSysType();
        /**
         * @brief 设置原点
         * @param[i]  x              x 坐标
         * @param[i]  y              y 坐标
         * @param[i]  z              z 坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual void setOrigin(const double x, const double y, const double z);
        /**
         * @brief 设置第一点
         * @param[i]  x              x 坐标
         * @param[i]  y              y 坐标
         * @param[i]  z              z 坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual void setPoint1(const double x, const double y, const double z);
        /**
         * @brief 设置第二点
         * @param[i]  x              x 坐标
         * @param[i]  y              y 坐标
         * @param[i]  z              z 坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual void setPoint2(const double x, const double y, const double z);
        /**
         * @brief 获取原点坐标
         * @param[o]  origin         原点坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual void getOrigin(double* origin );
        /**
         * @brief 获取第一轴线方向
         * @param[o]  dir            第一轴线方向
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual void getDirection1(double* dir );
        /**
         * @brief 获取第二轴线方向
         * @param[o]  dir            第二轴线方向
         * @param[i]  vertialToD1    是否垂直于第一轴线
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual void getDirection2(double* dir, bool vertialToD1 = true );
        /**
         * @brief 获取第三轴线方向
         * @param[o]  dir            第三轴线方向
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual void getDirection3(double* dir );
        /**
         * @brief 获取在某一点的坐标系的三个方向，默认是局部坐标系的三个方向
         * @param[i] pxyz      输入点坐标位置
         * @param[o]  dirs            三个轴线方向，9个分量，dx1,dy1,dz1,dx2,dy2,dz2,dx3,dx3,dz3
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        virtual void get3DirectionAtPoint(const double* pxyz, double* dirs);
        /**
       * @brief 复制数据对象,深拷贝，数据id不拷贝
       * @param[i] obj 被拷贝的对象
       * @return true
       * @return false
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-05-30
       */
        virtual bool copy(FITKAbstractDataObject* obj) override;

    protected:
        /**
         * @brief 坐标轴类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        FCSysType _sysType{ FCSCartesian };
        /**
         * @brief 原点坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        Core::FITKPoint* _originPt{};
        /**
         * @brief 第一点坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        Core::FITKPoint* _xPt{};
        /**
         * @brief 第二点坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        Core::FITKPoint* _yPt{};
    };

    /**
     * @brief 定义坐标系管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-06
     */
    class FITKInterfaceModelAPI FITKCoordinateSystemManager :
        public Core::FITKAbstractDataManager<FITKCoordinateSystem>
    {
    public:
        /**
         * @brief Construct a new FITKCoordinateSystemManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        explicit FITKCoordinateSystemManager() = default;
        /**
         * @brief Destroy the FITKCoordinateSystemManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        virtual ~FITKCoordinateSystemManager() = default;

        template<class T>
         /**
          * @brief 根据ID获取坐标系，并转换坐标系类型
          * @param[i]  id             坐标系ID
          * @return T*
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-06-06
          */
        T* getCoordinateSystemT(const int id)
        {
            return dynamic_cast<T*>(this->getDataByID(id));
        }
        /**
         * @brief 根据类型获取全部坐标系
         * @param[i]  type           坐标系类型
         * @return QList<FITKCoordinateSystem*>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        QList<FITKCoordinateSystem*> getCoordinateSystems(FITKCoordinateSystem::FCSysType type);
    };

}


#endif

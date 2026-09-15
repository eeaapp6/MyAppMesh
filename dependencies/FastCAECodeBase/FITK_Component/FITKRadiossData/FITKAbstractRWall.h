/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKAbstractRWall.h
 * @brief Radioss 刚性壁（Rigid Wall）的抽象基类声明。
 * @author libaojun
 * @date 2025-08-25
 */
#ifndef FITK_ABSTRACT_R_WALL_H
#define FITK_ABSTRACT_R_WALL_H

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKAbstractRadiossData.h"


namespace Radioss
{
    /**
     * @brief Radioss 刚性壁（Rigid Wall）的抽象基类。
     * @details 提供刚性壁通用的属性与访问接口，包括节点 ID、滑移类型、搜索距离、摩擦系数与滤波设置等。
     * @author libaojun
     * @date 2025-08-25
     */
    class FITKRadiossDataAPI FITKAbstractRWall : public Core::FITKAbstractNDataObject,
        public FITKAbstractRadiossData
    {
        Q_OBJECT
    public:
        /**
         * @brief 刚性壁的几何类型。
         */
        enum RWallType
        {
            RWallPLANE = 1,   ///< 平面壁。
            RWallCYL,         ///< 圆柱壁。
            RWallSPHER,       ///< 球面壁。
            RWallPARAL,       ///< 平行平面。
        }; Q_ENUM(RWallType)

        /**
         * @brief 与刚性壁的接触/滑移行为。
         */
            enum RWallSliding
        {
            Sliding = 0,          ///< 无摩擦滑移。
            Tied,                 ///< 与刚性壁绑定。
            SlidingWithFriction,  ///< 有摩擦滑移。
        }; Q_ENUM(RWallSliding)

    public:
        /**
         * @brief 默认构造函数。
         * @author libaojun
         * @date 2025-08-25
         */
        explicit FITKAbstractRWall() = default;

        /**
         * @brief 虚析构函数。
         * @author libaojun
         * @date 2025-08-25
         */
        virtual ~FITKAbstractRWall() = 0;

        /**
         * @brief 获取派生实例的刚性壁几何类型。
         * @return 壁类型。
         * @author libaojun
         * @date 2025-08-25
         */
        virtual RWallType getType() const = 0;

        /**
         * @brief 获取参考节点 ID。
         * @return 节点 ID。
         * @author libaojun
         * @date 2025-08-25
         */
        int getNodeId() const;
        /**
         * @brief 设置参考节点 ID。
         * @param v 节点 ID。
         * @author libaojun
         * @date 2025-08-25
         */
        void setNodeId(int v);

        /**
         * @brief 获取配置的滑移行为。
         * @return 滑移行为枚举值。
         * @author libaojun
         * @date 2025-08-25
         */
        RWallSliding getSlidingType() const;
        /**
         * @brief 设置滑移行为。
         * @param v 滑移行为。
         * @author libaojun
         * @date 2025-08-25
         */
        void setSlidingType(RWallSliding v);

        /**
         * @brief 获取第一节点集合 ID。
         * @return 节点集合 ID 1。
         * @author libaojun
         * @date 2025-08-25
         */
        int getNodeGroupId1() const;
        /**
         * @brief 设置第一节点集合 ID。
         * @param v 节点集合 ID 1。
         * @author libaojun
         * @date 2025-08-25
         */
        void setNodeGroupId1(int v);

        /**
         * @brief 获取第二节点集合 ID。
         * @return 节点集合 ID 2。
         * @author libaojun
         * @date 2025-08-25
         */
        int getNodeGroupId2() const;
        /**
         * @brief 设置第二节点集合 ID。
         * @param v 节点集合 ID 2。
         * @author libaojun
         * @date 2025-08-25
         */
        void setNodeGroupId2(int v);

        /**
         * @brief 获取搜索距离/容差。
         * @return 搜索距离。
         * @author libaojun
         * @date 2025-08-25
         */
        double getDSearch() const;
        /**
         * @brief 设置搜索距离/容差。
         * @param v 搜索距离。
         * @author libaojun
         * @date 2025-08-25
         */
        void setDSearch(double v);

        /**
         * @brief 获取摩擦系数（mu）。
         * @return 摩擦系数。
         * @author libaojun
         * @date 2025-08-25
         */
        double getFrictionCoef() const;
        /**
         * @brief 设置摩擦系数（mu）。
         * @param v 摩擦系数。
         * @author libaojun
         * @date 2025-08-25
         */
        void setFrictionCoef(double v);

        /**
         * @brief 获取特征直径。
         * @return 直径。
         * author libaojun
         * @date 2025-08-25
         */
        double getDiameter() const;
        /**
         * @brief 设置特征直径。
         * @param v 直径。
         * author libaojun
         * @date 2025-08-25
         */
        void setDiameter(double v);

        /**
         * @brief 获取滤波因子。
         * @return 滤波因子。
         * author libaojun
         * @date 2025-08-25
         */
        double getFilteringFactor() const;
        /**
         * @brief 设置滤波因子。
         * @param v 滤波因子。
         * author libaojun
         * @date 2025-08-25
         */
        void setFilteringFactor(double v);

        /**
         * @brief 获取滤波标志。
         * @return 滤波标志（0/1）。
         * author libaojun
         * @date 2025-08-25
         */
        int getFilteringFlag() const;
        /**
         * @brief 设置滤波标志。
         * @param v 滤波标志（0/1）。
         * author libaojun
         * @date 2025-08-25
         */
        void setFilteringFlag(int v);

    protected:
        /**
         * @brief 设置 M 点坐标。COM/DOWNLOAD/DOWNLOAD.HTML
         * @param x X 坐标。
         * @param y Y 坐标。
         * @param z Z 坐标。
         * @note 写入 m_coordinatesM（长度为 3 的数组）。
         * @author libaojun
         * @date 2025-08-25
         */
        void setMCoordinates(double x, double y, double z);

        /**
         * @brief 获取 M 点坐标。
         * @param x 输出：X 坐标。
         * @param y 输出：Y 坐标。
         * @param z 输出：Z 坐标。
         * @note 读取 m_coordinatesM（长度为 3 的数组）。
         * @author libaojun
         * @date 2025-08-25
         */
        void getMCoordinates(double& x, double& y, double& z) const;

        /**
         * @brief 设置 M1 点坐标。
         * @param x X 坐标。
         * @param y Y 坐标。
         * @param z Z 坐标。
         * @note 写入 m_coordinatesM1（长度为 3 的数组）。
         * author libaojun
         * @date 2025-08-25
         */
        void setM1Coordinates(double x, double y, double z);

        /**
         * @brief 获取 M1 点坐标。
         * @param x 输出：X 坐标。
         * @param y 输出：Y 坐标。
         * @param z 输出：Z 坐标。
         * @note 读取 m_coordinatesM1（长度为 3 的数组）。
         * author libaojun
         * @date 2025-08-25
         */
        void getM1Coordinates(double& x, double& y, double& z) const;

    protected:
        int m_nodeId{ 0 };                 //!< 参考节点 ID。
        RWallSliding m_slidingType{ Sliding }; //!< 接触/滑移行为。
        int m_nodeGroupId1{ 0 };           //!< 第一节点集合 ID。
        int m_nodeGroupId2{ 0 };           //!< 第二节点集合 ID。

        double m_DSearch{ 0.0 };           //!< 搜索距离/容差。
        double m_frictionCoef{ 0.0 };      //!< 摩擦系数（mu）。
        double m_diameter{ 0.0 };          //!< 特征直径。
        double m_filteringFactor{ 0.0 };   //!< 滤波因子。

        int m_filteringFlag{ 0 };          //!< 滤波启用标志（0-5）。

        double m_coordinatesM[3]{ 0.0,0.0,0.0 };     //!< M点坐标（x, y, z）。
        double m_coordinatesM1[3]{ 0.0,0.0,0.0 };    //!< M1点坐标（x, y, z）。
        
    };


    /**
     * @brief RWall 管理器类。
     */
    class FITKRadiossDataAPI FITKRadiossRWallManager :
        public Core::FITKAbstractDataManager<FITKAbstractRWall>
    {
    public:
        explicit FITKRadiossRWallManager() = default;
        virtual ~FITKRadiossRWallManager() = default;
    };
}

#endif // FITK_ABSTRACT_R_WALL_H
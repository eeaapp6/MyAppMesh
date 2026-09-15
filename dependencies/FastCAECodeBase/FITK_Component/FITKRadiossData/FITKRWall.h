/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITK_R_WALL_H__
#define FITK_R_WALL_H__

#include "FITKAbstractRWall.h"

namespace Radioss
{
    /**
     * @brief 刚性平面墙（/RWALL/PLANE）数据对象。
     * @details 表示一个具有基点与法向的无限平面
     * @author libaojun
     * @date 2025-08-25
     */
    class FITKRadiossDataAPI FITKRWallPlane : public FITKAbstractRWall
    {
        FITKCLASS(Radioss, FITKRWallPlane); //!< FITK类标识
        RadiossKeyWord(FITKRWallPlane, /RWALL/PLANE) //!< Radioss关键字标识
    public:
        /**
         * @brief 默认构造函数。
         * @author libaojun
         * @date 2025-08-25
         */
        explicit FITKRWallPlane() = default;

        /**
         * @brief 析构函数。
         * @author libaojun
         * @date 2025-08-25
         */
        virtual ~FITKRWallPlane() = default;

        /**
         * @brief 获取刚性墙类型（平面）。
         * @return RWallType::RWallPLANE
         * @author libaojun
         * @date 2025-08-25
         */
        virtual RWallType getType() const override;

        /**
         * @brief 设置平面基点坐标。
         * @param x X 坐标
         * @param y Y 坐标
         * @param z Z 坐标
         * @note 基点用于定义平面在空间中的位置，与法向共同确定平面方程。Typically 对应 M 点或派生类自定义存储。 
         * @see getBasicPoint
         * @see setNormalVector
         * @author libaojun
         * @date 2025-08-25
         */
        void setBasicPoint(double x, double y, double z);

        /**
         * @brief 获取平面基点坐标。
         * @param x 输出：X 坐标
         * @param y 输出：Y 坐标
         * @param z 输出：Z 坐标
         * @see setBasicPoint
         * @see getNormalVector
         * @return 无（通过引用返回）
         * @author libaojun
         * @date 2025-08-25
         */
        void getBasicPoint(double& x, double& y, double& z) const;

        /**
         * @brief 设置平面法向量。
         * @param nx 法向 X 分量
         * @param ny 法向 Y 分量
         * @param nz 法向 Z 分量
         * @note 建议提供单位向量；若非单位，派生实现可进行归一化处理。
         * @see getNormalVector
         * @see setBasicPoint
         * @warning 法向零向量将导致几何无效，应由实现进行校验处理。 
         * @author libaojun
         * @date 2025-08-25
         */
        void setNormalVector(double nx, double ny, double nz);

        /**
         * @brief 获取平面法向量。
         * @param nx 输出：法向 X 分量
         * @param ny 输出：法向 Y 分量
         * @param nz 输出：法向 Z 分量
         * @see setNormalVector
         * @return 无（通过引用返回）
         * @author libaojun
         * @date 2025-08-25
         */
        void getNormalVector(double& nx, double& ny, double& nz) const;

    };

    class FITKRadiossDataAPI FITKRWallCylinder : public FITKAbstractRWall
    {
        FITKCLASS(Radioss, FITKRWallCylinder); //!< FITK类标识
        RadiossKeyWord(FITKRWallCylinder, /RWALL/CYL) //!< Radioss关键字标识
    public:
        /**
         * @brief 默认构造函数。
         * @author libaojun
         * @date 2025-08-25
         */
        explicit FITKRWallCylinder() = default;

        /**
         * @brief 析构函数。
         * @author libaojun
         * @date 2025-08-25
         */
        virtual ~FITKRWallCylinder() = default;

        /**
         * @brief 获取刚性墙类型（圆柱）。
         * @return RWallType::RWallCYL
         * @author libaojun
         * @date 2025-08-25
         */
        virtual RWallType getType() const override;

        /**
         * @brief 设置圆柱基点坐标。
         * @param x X 坐标
         * @param y Y 坐标
         * @param z Z 坐标
         * @note 基点用于定义圆柱在空间中的位置，与轴向量共同确定圆柱的中心轴线。
         * @see getBasicPoint
         * @see setAxisVector
         * @author libaojun
         * @date 2025-08-25
         */
        void setBasicPoint(double x, double y, double z);

        /**
         * @brief 获取圆柱基点坐标。
         * @param x 输出：X 坐标
         * @param y 输出：Y 坐标
         * @param z 输出：Z 坐标
         * @see setBasicPoint
         * @see getAxisVector
         * @return 无（通过引用返回）
         * @author libaojun
         * @date 2025-08-25
         */
        void getBasicPoint(double& x, double& y, double& z) const;

        /**
         * @brief 设置圆柱轴向量。
         * @param nx 轴向 X 分量
         * @param ny 轴向 Y 分量
         * @param nz 轴向 Z 分量
         * @note 建议提供单位向量；若非单位，派生实现可进行归一化处理。
         * @see getAxisVector
         * @see setBasicPoint
         * @warning 轴向零向量将导致几何无效，应由实现进行校验处理。
         * @author libaojun
         * @date 2025-08-25
         */
        void setAxisVector(double nx, double ny, double nz);

        /**
         * @brief 获取圆柱轴向量。
         * @param nx 输出：轴向 X 分量
         * @param ny 输出：轴向 Y 分量
         * @param nz 输出：轴向 Z 分量
         * @see setAxisVector
         * @return 无（通过引用返回）
         * @author libaojun
         * @date 2025-08-25
         */
        void getAxisVector(double& nx, double& ny, double& nz) const;
        /**
         * @brief    获取包围盒的交点
         * @param[o] coor1 交点1
         * @param[o] coor2 交点2
         * @param[i] boxP1 包围盒p1
         * @param[i] boxP2 包围盒p2
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-25
         */
        bool calcBoxInterPoint(double * coor1, double * coor2, double * boxP1, double * boxP2);
    };

    /**
     * @brief 刚性球面墙（/RWALL/SPHER）数据对象。
     * @details 表示一个具有球心和半径的球面
     * @author liuzhonghuaAI
     * @date 2025-08-25
     */
    class FITKRadiossDataAPI FITKRWallSpherical : public FITKAbstractRWall
    {
        FITKCLASS(Radioss, FITKRWallSpherical); //!< FITK类标识
        RadiossKeyWord(FITKRWallSpherical, /RWALL/SPHER) //!< Radioss关键字标识
    public:
        /**
         * @brief 默认构造函数。
         * @author liuzhonghuaAI
         * @date 2025-08-25
         */
        explicit FITKRWallSpherical() = default;

        /**
         * @brief 析构函数。
         * @author liuzhonghuaAI
         * @date 2025-08-25
         */
        virtual ~FITKRWallSpherical() = default;

        /**
         * @brief 获取刚性墙类型（球面）。
         * @return RWallType::RWallSPHER
         * @author liuzhonghuaAI
         * @date 2025-08-25
         */
        virtual RWallType getType() const override;

        /**
         * @brief 设置球心坐标。
         * @param x X 坐标
         * @param y Y 坐标
         * @param z Z 坐标
         * @note 球心用于定义球面在空间中的位置。
         * @see getCenter
         * @see setRadius
         * @author liuzhonghuaAI
         * @date 2025-08-25
         */
        void setCenter(double x, double y, double z);

        /**
         * @brief 获取球心坐标。
         * @param x 输出：X 坐标
         * @param y 输出：Y 坐标
         * @param z 输出：Z 坐标
         * @see setCenter
         * @see getRadius
         * @return 无（通过引用返回）
         * @author liuzhonghuaAI
         * @date 2025-08-25
         */
        void getCenter(double& x, double& y, double& z) const;

        /**
         * @brief 设置球面半径。
         * @param r 半径
         * @note 半径必须为正值。
         * @see getRadius
         * @see setCenter
         * @warning 零半径或负半径将导致几何无效，应由实现进行校验处理。
         * @author liuzhonghuaAI
         * @date 2025-08-25
         */
        void setRadius(double r);

        /**
         * @brief 获取球面半径。
         * @return 半径
         * @see setRadius
         * @return 无（通过引用返回）
         * @author liuzhonghuaAI
         * @date 2025-08-25
         */
        double getRadius() const;

    };
}

#endif // FITK_R_WALL_H__
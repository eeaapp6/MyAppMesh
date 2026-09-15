/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelSplitPart.h
 * @brief       模型分割抽象类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-13
 *********************************************************************/

#ifndef __FITKABSGEOMODELSPLITPART_H__
#define __FITKABSGEOMODELSPLITPART_H__

#include "FITKAbsGeoCommand.h"
#include "FITKGeoEnum.h"
#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       模型分割命令抽象类。
     * @details     平面默认为XY平面，可绕X轴与Y轴进行旋转；XZ平面可绕X轴与Z轴进行旋转；YZ平面可绕Y轴与Z轴进行旋转。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSplitPart : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelSplitPart);
        Q_OBJECT
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKAbsGeoModelSplitPart() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKAbsGeoModelSplitPart() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置源部件ID。
         * @param[in]   partId 部件ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setSourcePartId(int partId);

        /**
         * @brief       获取源部件ID。
         * @return      部件ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        int getSourcePartId() const;

        /**
         * @brief       设置分割平面中心坐标。
         * @param[in]   x：X坐标
         * @param[in]   y：Y坐标
         * @param[in]   z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setSplitOrigin(double x, double y, double z);

        /**
         * @brief       获取分割平面中心坐标。
         * @param[out]  x：X坐标
         * @param[out]  y：Y坐标
         * @param[out]  z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void getSplitOrigin(double& x, double& y, double& z) const;

        /**
         * @brief       设置分割平面位置类型。
         * @param[in]   type：平面位置类型（XY/XZ/YZ）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setSplitPlaneType(FITKGeoEnum::PlaneType type);

        /**
         * @brief       获取分割平面位置类型。
         * @return      平面位置类型（XY/XZ/YZ）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKGeoEnum::PlaneType getSplitPlaneType() const;

        /**
         * @brief       设置X方向旋转角度。
         * @param[in]   angle：旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setRotateX(double angle);

        /**
         * @brief       获取X方向旋转角度。
         * @return      旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double getRotateX() const;

        /**
         * @brief       设置Y方向旋转角度。
         * @param[in]   angle：旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setRotateY(double angle);

        /**
         * @brief       获取Y方向旋转角度。
         * @return      旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double getRotateY() const;

        /**
         * @brief       设置Z方向旋转角度。
         * @param[in]   angle 旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setRotateZ(double angle);

        /**
         * @brief       获取Z方向旋转角度。
         * @return      旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double getRotateZ() const;

    protected:
        /**
         * @brief       源部件ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        int m_sourcePartId{ -1 };

        /**
         * @brief       分割平面中心坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_origin[3]{ 0.0, 0.0, 0.0 };

        /**
         * @brief       分割平面位置类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKGeoEnum::PlaneType m_planeType{ FITKGeoEnum::PT_XY };

        /**
         * @brief       X方向旋转角度（度）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_rotateX{ 0.0 };

        /**
         * @brief       Y方向旋转角度（度）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_rotateY{ 0.0 };

        /**
         * @brief       Z方向旋转角度（度）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_rotateZ{ 0.0 };

    };
}

#endif

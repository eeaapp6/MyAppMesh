/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoFeature.h
 * @brief  几何特征抽象类.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-09-25
 */
#ifndef FITKABSGEOFEATURE_H
#define FITKABSGEOFEATURE_H

#include "FITKAbsGeoCommand.h"
#include "FITKAbsGeoDatum.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface {
    /**
     * @brief  肋特征抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoStiffener :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoStiffener);
    public:
        FITKAbsGeoStiffener() = default;
        virtual ~FITKAbsGeoStiffener() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        VirtualShape sourceShape() const { return m_SourceShape; }
        void setSourceShape(VirtualShape shape) { m_SourceShape = shape; }
        VirtualShape profileShape() const { return m_ProfileShape; }
        void setProfileShape(VirtualShape shape) { m_ProfileShape = shape; }
        double thickness1() const { return m_Thickness1; }
        void setThickness1(double thickness) { m_Thickness1 = thickness; }
        double thickness2() const { return m_Thickness2; }
        void setThickness2(double thickness) { m_Thickness2 = thickness; }

    protected:
        /**
         * @brief  要增加肋特征的源形状.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-25
         */
        VirtualShape m_SourceShape{};
        /**
         * @brief  截面（线）的形状.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-13
         */
        VirtualShape m_ProfileShape{};
        /**
         * @brief  拉伸方向1上的厚度.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-25
         */
        double m_Thickness1{};
        /**
         * @brief  拉伸方向2上的厚度.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-25
         */
        double m_Thickness2{};
    };
}
#endif // FITKABSGEOFEATURE_H

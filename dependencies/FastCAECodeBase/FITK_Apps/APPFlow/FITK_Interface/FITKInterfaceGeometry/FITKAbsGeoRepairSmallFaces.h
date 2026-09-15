/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoRepairSmallFaces.h
 * @brief  修复小面
 * @author Yanzhihui (chanyuantiandao@126.com)
 * @date   2025-03-27
 */
#ifndef FITKABSGEOREPAIRSMALLFACES_H
#define FITKABSGEOREPAIRSMALLFACES_H


#include "FITKAbsGeoCommand.h"
#include "FITKAbsGeoDatum.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface {
    /**
     * @brief  修复小面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoRepairSmallFaces :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoRepairSmallFaces);
    public:
        FITKAbsGeoRepairSmallFaces() = default;
        virtual ~FITKAbsGeoRepairSmallFaces() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        QList<VirtualShape> faces() const { return m_Faces; }
        void setFaces(QList<VirtualShape> faces){ m_Faces= faces;}

    protected:
        /**
         * @brief  要修复的面
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-03-25
         */
        QList<VirtualShape> m_Faces{};
    };
}
#endif // !FITKABSGEOREPAIRSMALLFACES_H

/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoReplaceFaces.h
 * @brief  替换面
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2025-05-14
 */
#ifndef FITKABSGEOREPLACEFACES_H
#define FITKABSGEOREPLACEFACES_H


#include "FITKAbsGeoCommand.h"
#include "FITKAbsGeoDatum.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface {
    /**
     * @brief  替换面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-05-14
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReplaceFaces :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoReplaceFaces);
    public:
        FITKAbsGeoReplaceFaces() = default;
        virtual ~FITKAbsGeoReplaceFaces() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        QList<VirtualShape> faces() const { return m_Faces; }
        void setFaces(QList<VirtualShape> faces) { m_Faces = faces; }
        bool isExtend() { return m_ExtendNeighboringFace; }
        void setExtend(bool extend) { m_ExtendNeighboringFace = extend; }
    protected:
        /**
         * @brief  要替换的面
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-05-14
         */
        QList<VirtualShape> m_Faces{};
        /**
         * @brief  是否扩展相邻面
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-05-14
         */
        bool m_ExtendNeighboringFace{};
    };
}
#endif // !FITKABSGEOREPLACEFACES_H

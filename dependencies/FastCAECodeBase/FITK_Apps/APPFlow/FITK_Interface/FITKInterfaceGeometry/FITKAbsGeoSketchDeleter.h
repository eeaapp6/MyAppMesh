/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoSketchDeleter.h
 * @brief  草图删除命令
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2025-04-27
 */
#ifndef FITKABSGEOSKETCHDELETER_H
#define FITKABSGEOSKETCHDELETER_H


#include "FITKAbsGeoCommand.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface {
    /**
     * @brief  分割基类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-04-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoSketchDeleter :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoSketchDeleter);
    public:
        FITKAbsGeoSketchDeleter() = default;
        ~FITKAbsGeoSketchDeleter() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        QList<VirtualShape> toBeRemovedEntities() const { return m_ToBeRemovedEntities; }
        void setToBeRemovedEntities(QList<VirtualShape> entities) { m_ToBeRemovedEntities = entities; }

    protected:
        /**
         * @brief  要移除的形状.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-04-27
         */
        QList<VirtualShape> m_ToBeRemovedEntities{};
    };
}
#endif // !FITKABSGEOSKETCHDELETER_H

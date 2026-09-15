/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoModelPartitionEdge.h
 * @brief  分割边抽象类
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2025-07-21
 */
#ifndef  FITKABSGEOMODELPARTITIONEDGE_H
#define  FITKABSGEOMODELPARTITIONEDGE_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include <QVector>
#include <array>

namespace Interface
{
    class FITKAbsGeoDatumPlane;
    /**
     * @brief  分割边抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-07-21
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionEdge :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionEdge);
    public:
        FITKAbsGeoModelPartitionEdge() = default;
        ~FITKAbsGeoModelPartitionEdge() override = default;

        void setEdges(QList<Interface::VirtualShape> edges) { m_Edges = edges; }
        QList<Interface::VirtualShape> edges() const { return m_Edges; }

    protected:
        QList<Interface::VirtualShape> m_Edges{};
    };


    /**
     * @brief  使用参数进行边分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-07-21
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionEdgeWithParameter :
        public FITKAbsGeoModelPartitionEdge
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionEdgeWithParameter);
    public:
        FITKAbsGeoModelPartitionEdgeWithParameter() = default;
        ~FITKAbsGeoModelPartitionEdgeWithParameter() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void setParameter(double p) { m_Parameter = p; }
        double parameter() const { return m_Parameter; }
    protected:
        /// 参数范围(0,1)
        double m_Parameter{};
    };



    /**
     * @brief  使用点进行边分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-07-21
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionEdgeWithPoint :
        public FITKAbsGeoModelPartitionEdge
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionEdgeWithPoint);
    public:
        FITKAbsGeoModelPartitionEdgeWithPoint() = default;
        ~FITKAbsGeoModelPartitionEdgeWithPoint() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void setPoint(QVector<double> p) { m_Point = p; }
        QVector<double> point() const { return m_Point; }
    protected:
        QVector<double> m_Point{};
    };



    /**
     * @brief  使用基准面进行边分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-07-21
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionEdgeWithDatumPlane :
        public FITKAbsGeoModelPartitionEdge
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionEdgeWithDatumPlane);
    public:
        FITKAbsGeoModelPartitionEdgeWithDatumPlane() = default;
        ~FITKAbsGeoModelPartitionEdgeWithDatumPlane() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        FITKAbsGeoDatumPlane* datumPlane() const { return m_DatumPlane; }
        void setDatumPlane(FITKAbsGeoDatumPlane* plane) { m_DatumPlane = plane; }
    protected:
        FITKAbsGeoDatumPlane* m_DatumPlane{};
    };


}

#endif // !FITKABSGEOMODELPARTITIONEDGE_H

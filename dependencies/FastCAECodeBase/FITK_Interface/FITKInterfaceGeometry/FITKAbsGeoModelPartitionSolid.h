/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoModelPartitionSolid.h
 * @brief  实体分块
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2025-06-06
 */
#ifndef  FITKABSGEOMODELPARTITIONSOLID_H
#define  FITKABSGEOMODELPARTITIONSOLID_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include <QVector>
#include <array>

namespace Interface
{

    class FITKAbsGeoSketch2D;

    /**
     * @brief  体分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-06-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionSolid :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionSolid);
    public:
        FITKAbsGeoModelPartitionSolid() = default;
        ~FITKAbsGeoModelPartitionSolid() override = default;

        void setSolids(QList<Interface::VirtualShape> solids) { m_Solids = solids; }
        QList<Interface::VirtualShape> solids() const { return m_Solids; }

    protected:
        QList<Interface::VirtualShape> m_Solids{};
    };
    /**
     * @brief  使用平面进行实体分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-06-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionSolidWithPlane :
        public FITKAbsGeoModelPartitionSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionSolidWithPlane);
    public:
        FITKAbsGeoModelPartitionSolidWithPlane() = default;
        ~FITKAbsGeoModelPartitionSolidWithPlane() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;


        void setPlane(QVector<double> origin, QVector<double> normal) { m_Origin = origin; m_Normal = normal; }
        QVector<double> planeOrigin() const { return m_Origin; }
        QVector<double> planeNormal() const { return m_Normal; }

    protected:
        QVector<double> m_Origin{};
        QVector<double> m_Normal{};


    };
    /**
     * @brief  使用扩展面进行实体分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-06-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionSolidWithExtendFace :
        public FITKAbsGeoModelPartitionSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionSolidWithExtendFace);
    public:
        FITKAbsGeoModelPartitionSolidWithExtendFace() = default;
        ~FITKAbsGeoModelPartitionSolidWithExtendFace() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void setExtendFace(VirtualShape face) { m_ExtendFace = face; }
        VirtualShape extendFace() const { return m_ExtendFace; }
    protected:
        VirtualShape m_ExtendFace{};
    };

    /**
     * @brief  使用扫掠边进行实体分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-06-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionSolidWithSweepEdge :
        public FITKAbsGeoModelPartitionSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionSolidWithSweepEdge);
    public:
        FITKAbsGeoModelPartitionSolidWithSweepEdge() = default;
        ~FITKAbsGeoModelPartitionSolidWithSweepEdge() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;


        void setEdges(QList<VirtualShape> edges) { m_Edges = edges; }
        QList<VirtualShape> edges() const { return m_Edges; }
        void setSweepPath(VirtualShape path) { m_SweepPath = path; m_SweepDirection.resize(0); }
        VirtualShape sweepPath() const { return m_SweepPath; }
        void setSweepDirection(QVector<double> direction) { m_SweepDirection = direction; m_SweepPath.reset(); }
        QVector<double> sweepDirection() const { return m_SweepDirection; }

    protected:
        QList<VirtualShape> m_Edges{};
        VirtualShape m_SweepPath{};
        QVector<double> m_SweepDirection{};
    };

    /**
     * @brief  使用区域补丁进行实体分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-06-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionSolidWithNSidedPatch :
        public FITKAbsGeoModelPartitionSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionSolidWithNSidedPatch);
    public:
        FITKAbsGeoModelPartitionSolidWithNSidedPatch() = default;
        ~FITKAbsGeoModelPartitionSolidWithNSidedPatch() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    };

    /**
     * @brief  使用草绘进行实体分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-06-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionSolidWithSketchPlanar :
        public FITKAbsGeoModelPartitionSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionSolidWithSketchPlanar);
    public:
        FITKAbsGeoModelPartitionSolidWithSketchPlanar() = default;
        ~FITKAbsGeoModelPartitionSolidWithSketchPlanar() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void setSketch(VirtualShape sketch) { m_Sketch = sketch; }
        VirtualShape sketch() const { return m_Sketch; }

    protected:
        VirtualShape m_Sketch{};
    };

}

#endif // !FITKABSGEOMODELPARTITIONSOLID_H

/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelCurve.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    std::array<double, 3> FITKAbsGeoModelLine::dir() const
    {
        return m_Dir;
    }

    void FITKAbsGeoModelLine::setDir(double x, double y, double z)
    {
        m_Dir[0] = x;
        m_Dir[1] = y;
        m_Dir[2] = z;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelLine::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTLine;
    }

    std::array<double, 3> FITKAbsGeoModelSegment::startPoint() const
    {
        return m_StartPoint;
    }

    void FITKAbsGeoModelSegment::setStartPoint(double x, double y, double z)
    {
        m_StartPoint[0] = x;
        m_StartPoint[1] = y;
        m_StartPoint[2] = z;
    }

    std::array<double, 3> FITKAbsGeoModelSegment::endPoint() const
    {
        return m_EndPoint;
    }

    void FITKAbsGeoModelSegment::setEndPoint(double x, double y, double z) {
        m_EndPoint[0] = x;
        m_EndPoint[1] = y;
        m_EndPoint[2] = z;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSegment::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTSegment;
    }

    double FITKAbsGeoModelCircle::radius() const
    {
        return m_Radius;
    }

    void FITKAbsGeoModelCircle::setRadius(double radius)
    {
        m_Radius = radius;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCircle::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTCircle;
    }

    double FITKAbsGeoModelEllipse::majorRadius() const
    {
        return m_MajorRadius;
    }

    void FITKAbsGeoModelEllipse::setMajorRadius(double radius)
    {
        m_MajorRadius = radius;
    }

    double FITKAbsGeoModelEllipse::minorRadius() const
    {
        return m_MinorRadius;
    }

    void FITKAbsGeoModelEllipse::setMinorRadius(double radius)
    {
        m_MinorRadius = radius;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelEllipse::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTEllipse;
    }

    double FITKAbsGeoModelHyperbola::majorRadius() const
    {
        return m_MajorRadius;
    }

    void FITKAbsGeoModelHyperbola::setMajorRadius(double radius)
    {
        m_MajorRadius = radius;
    }

    double FITKAbsGeoModelHyperbola::minorRadius() const
    {
        return m_MinorRadius;
    }

    void FITKAbsGeoModelHyperbola::setMinorRadius(double radius)
    {
        m_MinorRadius = radius;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelHyperbola::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTHyperbola;
    }
    double FITKAbsGeoModelParabola::focalLength() const
    {
        return m_FocalLength;
    }
    void FITKAbsGeoModelParabola::setFocalLength(double length)
    {
        m_FocalLength = length;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelParabola::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTParabola;
    }
    int FITKAbsGeoModelTrimmedCurve::sourceCurveId() const
    {
        return m_SourceCurveId;
    }
    void FITKAbsGeoModelTrimmedCurve::setSourceCurveId(int id)
    {
        m_SourceCurveId = id;
    }
    double FITKAbsGeoModelTrimmedCurve::startTrimmed() const
    {
        return m_StartTrimmed;
    }
    void FITKAbsGeoModelTrimmedCurve::setStartTrimmed(double val)
    {
        m_StartTrimmed = val;
    }
    double FITKAbsGeoModelTrimmedCurve::endTrimmed() const
    {
        return m_EndTrimmed;
    }
    void FITKAbsGeoModelTrimmedCurve::setEndTrimmed(double val)
    {
        m_EndTrimmed = val;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelTrimmedCurve::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTTrimmed;
    }
    void FITKAbsGeoModelWire::addPoint(double x, double y, double z)
    {
        m_Points.push_back({ x, y,z });
    }
    int FITKAbsGeoModelWire::count() const
    {
        return m_Points.count();
    }
    std::array<double, 3> FITKAbsGeoModelWire::point(int index) const
    {
        assert(index > -1 && index < m_Points.count());
        return m_Points.at(index);
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelWire::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTWire;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelOffsetCurve::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTOffsetCurve;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelThreePointsCircle::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTThreePointsCircle;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelThreePointsEllipse::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTThreePointsEllipse;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelThreePointsHyperbola::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTThreePointsHyperbola;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelThreePointsParabola::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTThreePointsParabola;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCurveProjectionOnSurface::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCurveProjectionOnSurface;
    }
    VirtualShape FITKAbsGeoModelCurveProjectionOnSurface::sourceCurve() const
    {
        return m_SourceCurve;
    }
    void FITKAbsGeoModelCurveProjectionOnSurface::setSourceCurve(VirtualShape curve)
    {
        m_SourceCurve = curve;
    }
    VirtualShape FITKAbsGeoModelCurveProjectionOnSurface::sourceSurface() const
    {
        return m_SourceSurface;
    }
    void FITKAbsGeoModelCurveProjectionOnSurface::setSourceSurface(VirtualShape surface)
    {
        m_SourceSurface = surface;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSurfaceIntersectionSurface::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSurfaceIntersectSurface;
    }
    VirtualShape FITKAbsGeoModelSurfaceIntersectionSurface::sourceSurface1() const
    {
        return m_SourceSurface1;
    }
    void FITKAbsGeoModelSurfaceIntersectionSurface::setSourceSurface1(VirtualShape surface)
    {
        m_SourceSurface1 = surface;
    }
    VirtualShape FITKAbsGeoModelSurfaceIntersectionSurface::sourceSurface2() const
    {
        return m_SourceSurface2;
    }
    void FITKAbsGeoModelSurfaceIntersectionSurface::setSourceSurface2(VirtualShape surface)
    {
        m_SourceSurface2 = surface;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelBridgeCurve::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTBridgeCurve;
    }

    VirtualShape FITKAbsGeoModelBridgeCurve::sourceCurve1() const { return m_SourceCurve1; }
    void FITKAbsGeoModelBridgeCurve::setSourceCurve1(VirtualShape curve) { m_SourceCurve1 = curve; }
    bool FITKAbsGeoModelBridgeCurve::useStartEnd1() const { return m_UseStartEnd1; }
    void FITKAbsGeoModelBridgeCurve::setUseStartEnd1(bool use) { m_UseStartEnd1 = use; }
    double FITKAbsGeoModelBridgeCurve::parameter1() const { return m_Parameter1; }
    void FITKAbsGeoModelBridgeCurve::setParameter1(double p) { m_Parameter1 = p; }
    VirtualShape FITKAbsGeoModelBridgeCurve::sourceCurve2() const { return m_SourceCurve2; }
    void FITKAbsGeoModelBridgeCurve::setSourceCurve2(VirtualShape curve) { m_SourceCurve2 = curve; }
    bool FITKAbsGeoModelBridgeCurve::useStartEnd2() const { return m_UseStartEnd2; }
    void FITKAbsGeoModelBridgeCurve::setUseStartEnd2(bool use) { m_UseStartEnd2 = use; }
    double FITKAbsGeoModelBridgeCurve::parameter2() const { return m_Parameter2; }
    void FITKAbsGeoModelBridgeCurve::setParameter2(double p) { m_Parameter2 = p; }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSurfaceEdge::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTCurveFromSurface;
    }
    VirtualShape FITKAbsGeoModelSurfaceEdge::sourceSurface() const { return m_SourceSurface; }
    void FITKAbsGeoModelSurfaceEdge::setSourceSurface(VirtualShape surface) { m_SourceSurface = surface; }
    int FITKAbsGeoModelSurfaceEdge::edgeVirtualTopoId() const { return m_EdgeVirtualTopoId; }
    void FITKAbsGeoModelSurfaceEdge::setEdgeVirtualTopoId(int id) { m_EdgeVirtualTopoId = id; }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCircleArc::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCircleArc;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelEllipseArc::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTEllipseArc;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelHyperbolaArc::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTHyperbolaArc;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelParabolaArc::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTParabolaArc;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelBeizeByControlPoints::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTBezierByControlPoints;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelBsplineByThroughPoints::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTBSplineByThroughPoints;
    }
}

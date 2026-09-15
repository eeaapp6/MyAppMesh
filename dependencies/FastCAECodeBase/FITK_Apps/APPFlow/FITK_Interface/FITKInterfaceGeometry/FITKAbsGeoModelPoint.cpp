/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelPoint.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKAbsGeoModelPoint::FITKAbsGeoModelPoint(double x, double y, double z)
        : m_X(x), m_Y(y), m_Z(z)
    {
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPoint::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPoint;
    }

    double FITKAbsGeoModelPoint::x() const { return m_X; }
    void FITKAbsGeoModelPoint::setX(double x) { m_X = x; }
    double FITKAbsGeoModelPoint::y() const { return m_Y; }
    void FITKAbsGeoModelPoint::setY(double y) { m_Y = y; }
    double FITKAbsGeoModelPoint::z() const { return m_Z; }
    void FITKAbsGeoModelPoint::setZ(double z) { m_Z = z; }
    void FITKAbsGeoModelPoint::setCoord(double x, double y, double z)
    {
        m_X = x; m_Y = y; m_Z = z;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelTwoCurveIntersection::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTTwoCurveIntersectPoint;
    }
    VirtualShape FITKAbsGeoModelTwoCurveIntersection::sourceCurve1() const
    {
        return m_SourceCurve1;
    }
    VirtualShape FITKAbsGeoModelTwoCurveIntersection::sourceCurve2() const
    {
        return m_SourceCurve2;
    }
    void FITKAbsGeoModelTwoCurveIntersection::setSourceCurve(VirtualShape curve1, VirtualShape curve2)
    {
        m_SourceCurve1 = curve1;
        m_SourceCurve2 = curve2;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPointProjectionOnCurve::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPointProjectionOnCurve;
    }
    VirtualShape FITKAbsGeoModelPointProjectionOnCurve::sourcePoint() const
    {
        return m_SourcePoint;
    }
    void FITKAbsGeoModelPointProjectionOnCurve::setSourcePoint(VirtualShape point)
    {
        m_SourcePoint = point;
    }
    VirtualShape FITKAbsGeoModelPointProjectionOnCurve::sourceCurve() const
    {
        return m_SourceCurve;
    }
    void FITKAbsGeoModelPointProjectionOnCurve::setSourceCurve(VirtualShape curve)
    {
        m_SourceCurve = curve;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPointProjectionOnSurface::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTPointProjectionOnSurface;
    }
    VirtualShape FITKAbsGeoModelPointProjectionOnSurface::sourcePoint() const {
        return m_SourcePoint;
    }
    void FITKAbsGeoModelPointProjectionOnSurface::setSourcePoint(VirtualShape point) {
        m_SourcePoint = point;
    }
    VirtualShape FITKAbsGeoModelPointProjectionOnSurface::sourceSurface() const {
        return m_SourceSurface;
    }
    void FITKAbsGeoModelPointProjectionOnSurface::setSourceSurface(VirtualShape surface) {
        m_SourceSurface = surface;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCurveIntersectionSurface::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTCurveIntersectSurface;
    }
    VirtualShape FITKAbsGeoModelCurveIntersectionSurface::sourceCurve() const {
        return m_SourceCurve;
    }
    void FITKAbsGeoModelCurveIntersectionSurface::setSourceCurve(VirtualShape curve) {
        m_SourceCurve = curve;
    }
    VirtualShape FITKAbsGeoModelCurveIntersectionSurface::sourceSurface() const {
        return m_SourceSurface;
    }
    void FITKAbsGeoModelCurveIntersectionSurface::setSourceSurface(VirtualShape surface) {
        m_SourceSurface = surface;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCurveEnd::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCurveEnd;
    }

    VirtualShape FITKAbsGeoModelCurveEnd::sourceCurve() const
    {
        return m_SourceCurve;
    }

    void FITKAbsGeoModelCurveEnd::setSourceCurve(VirtualShape curve)
    {
        m_SourceCurve = curve;
    }

    bool FITKAbsGeoModelCurveEnd::useStartEnd() const
    {
        return m_UseStartEnd;
    }

    void FITKAbsGeoModelCurveEnd::setUseStartEnd(bool use)
    {
        m_UseStartEnd = use;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCurveEndDistance::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCurveEndDistance;
    }

    VirtualShape FITKAbsGeoModelCurveEndDistance::sourceCurve() const
    {
        return m_SourceCurve;
    }

    void FITKAbsGeoModelCurveEndDistance::setSourceCurve(VirtualShape curve)
    {
        m_SourceCurve = curve;
    }

    bool FITKAbsGeoModelCurveEndDistance::useStartEnd() const
    {
        return m_UseStartEnd;
    }

    void FITKAbsGeoModelCurveEndDistance::setUseStartEnd(bool use)
    {
        m_UseStartEnd = use;
    }

    double FITKAbsGeoModelCurveEndDistance::distance() const
    {
        return m_Distance;
    }

    void FITKAbsGeoModelCurveEndDistance::setDistance(double distance)
    {
        m_Distance = distance;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCurveRadio::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTCurveRadio;
    }
    VirtualShape FITKAbsGeoModelCurveRadio::sourceCurve() const {
        return m_SourceCurve;
    }
    void FITKAbsGeoModelCurveRadio::setSourceCurve(VirtualShape curve) {
        m_SourceCurve = curve;
    }
    double FITKAbsGeoModelCurveRadio::radio() const {
        return m_Radio;
    }
    void FITKAbsGeoModelCurveRadio::setRadio(double radio)
    {
        m_Radio = radio;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelInteractionPoint::getGeometryCommandType() {
        return FITKGeoEnum::FITKGeometryComType::FGTInteractionPoint;
    }

}

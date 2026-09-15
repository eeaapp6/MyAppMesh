/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCAIS_Datum.h
 * @brief       The sub-class of interactive datums AIS_Plane, AIS_Line
 *              and AIS_Point.

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-30
 *********************************************************************/

#ifndef __FITKOCCAIS_PLANE_H__
#define __FITKOCCAIS_PLANE_H__

#include <AIS_Plane.hxx>
#include <AIS_ColoredShape.hxx>
#include <AIS_Line.hxx>
#include <AIS_Point.hxx>
#include "FITKAIS_ObjectBase.h"

#include <Geom_Plane.hxx>
#include <Geom_Line.hxx>
#include <Geom_Point.hxx>

#include <Geom_Axis2Placement.hxx>

// Datum plane.
//@{
class FITKOCCAIS_Plane;
DEFINE_STANDARD_HANDLE(FITKOCCAIS_Plane, AIS_Plane);

/**
 * @brief       The sub-class of AIS_Plane.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-30
 */
class FITKOCCAIS_Plane : public AIS_Plane, public FITKAIS_ObjectBase
{
public:
    /**
     * @brief       Constructor.
     * @param[in]   aComponent: The plane[quote]
     * @param[in]   aCurrentMode: Whether to use user define display mode[default]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    FITKOCCAIS_Plane(const Handle(Geom_Plane)& aComponent, const Standard_Boolean aCurrentMode = Standard_False);

    /**
     * @brief       Constructor.
     * @param[in]   aComponent: The plane[quote]
     * @param[in]   aCenter: The plane center[quote]
     * @param[in]   aCurrentMode: Whether to use user define display mode[default]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    FITKOCCAIS_Plane(const Handle(Geom_Plane)& aComponent, const gp_Pnt& aCenter, const Standard_Boolean aCurrentMode = Standard_False);

    /**
     * @brief       Constructor.
     * @param[in]   aComponent: The plane[quote]
     * @param[in]   aCenter: The plane center[quote]
     * @param[in]   aPmin: The point min[quote]
     * @param[in]   aPmax: The point max[quote]
     * @param[in]   aCurrentMode: Whether to use user define display mode[default]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    FITKOCCAIS_Plane(const Handle(Geom_Plane)& aComponent, const gp_Pnt& aCenter, const gp_Pnt& aPmin, const gp_Pnt& aPmax,
        const Standard_Boolean aCurrentMode = Standard_False);

    /**
     * @brief       Constructor.
     * @param[in]   aComponent: The plane[quote]
     * @param[in]   aPlaneType: The direction type of plane
     * @param[in]   aCurrentMode: Whether to use user define display mode[default]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    FITKOCCAIS_Plane(const Handle(Geom_Axis2Placement)& aComponent, const AIS_TypeOfPlane aPlaneType, const Standard_Boolean aCurrentMode = Standard_False);

    /**
     * @brief       Returns true if the display mode selected
     * @param[in]   aMode: The display mode[const]
     * @return      Is valid for planes
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    virtual Standard_Boolean AcceptDisplayMode(const Standard_Integer aMode) const Standard_OVERRIDE;

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-31
     */
    virtual ~FITKOCCAIS_Plane() = default;

private:
    /**
     * @brief       Initialize the properties.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    void initProperties();

};
//@}

// Custom datum plane.
//@{
class FITKOCCAIS_ShapePlane;
DEFINE_STANDARD_HANDLE(FITKOCCAIS_ShapePlane, AIS_ColoredShape);

/**
 * @brief       The sub-class of AIS_Shape.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-10
 */
class FITKOCCAIS_ShapePlane : public AIS_ColoredShape, public FITKAIS_ObjectBase
{
public:
    /**
     * @brief       Constructor.
     * @param[in]   aComponent: The plane[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-10
     */
    FITKOCCAIS_ShapePlane(const Handle(Geom_Plane)& aComponent);

    /**
     * @brief       Constructor.
     * @param[in]   shape: The plane shape[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-10-10
     */
    FITKOCCAIS_ShapePlane(TopoDS_Shape & shape);

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-10
     */
    virtual ~FITKOCCAIS_ShapePlane() = default;

    /**
     * @brief       Get the type of the object.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-10
     */
    virtual AIS_KindOfInteractive Type() const Standard_OVERRIDE;

    /**
     * @brief       Reset the plane for the datum.
     * @param[in]   Pl: The plane[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-10
     */
    void SetPln(const gp_Pln & Pl);

    /**
     * @brief       Get the plane component.
     * @return      The plane
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-10
     */
    const Handle(Geom_Plane)& Component();

    /**
     * @brief       Update the shape.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-20
     */
    void update();

private:
    /**
     * @brief       Initialize the properties.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-10
     */
    void initProperties();

    /**
     * @brief       Generate the plane shape.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-10
     */
    void generateShape();

private:
    /**
     * @brief       The plane of the datum.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-10
     */
    Handle(Geom_Plane) m_plane;

};
//@}

// Datum line.
//@{
class FITKOCCAIS_Line;
DEFINE_STANDARD_HANDLE(FITKOCCAIS_Line, AIS_Line);

/**
 * @brief       The sub-class of AIS_Line.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-04
 */
class FITKOCCAIS_Line : public AIS_Line, public FITKAIS_ObjectBase
{
public:
    /**
     * @brief       Constructor.
     * @param[in]   aLine: Initializes the line aLine.[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    FITKOCCAIS_Line(const Handle(Geom_Line)& aLine);

    /**
     * @brief       Constructor.
     * @param[in]   aStartPoint: Initializes a starting point aStartPoint for the line[quote]
     * @param[in]   aEndPoint: Initializes a finishing point aEndPoint for the line[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    FITKOCCAIS_Line(const Handle(Geom_Point)& aStartPoint, const Handle(Geom_Point)& aEndPoint);

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    virtual ~FITKOCCAIS_Line() = default;

private:
    /**
     * @brief       Initialize the properties.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    void initProperties();

};
//@}

// Custom datum line.
//@{
class FITKOCCAIS_ShapeLine;
DEFINE_STANDARD_HANDLE(FITKOCCAIS_ShapeLine, AIS_ColoredShape);

/**
 * @brief       The sub-class of AIS_Shape.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-19
 */
class FITKOCCAIS_ShapeLine : public AIS_ColoredShape, public FITKAIS_ObjectBase
{
public:
    /**
     * @brief       Constructor.
     * @param[in]   aStartPoint: Initializes a starting point aStartPoint for the line[quote]
     * @param[in]   aEndPoint: Initializes a finishing point aEndPoint for the line[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-19
     */
    FITKOCCAIS_ShapeLine(const Handle(Geom_Point)& aStartPoint, const Handle(Geom_Point)& aEndPoint);

    /**
     * @brief       Constructor.
     * @param[in]   shape: The edge shape[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-10-10
     */
    FITKOCCAIS_ShapeLine(TopoDS_Shape & shape);

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-19
     */
    virtual ~FITKOCCAIS_ShapeLine() = default;

    /**
     * @brief       Get the type of the object.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-19
     */
    virtual AIS_KindOfInteractive Type() const Standard_OVERRIDE;

    /**
     * @brief       Get the line points.
     * @param[in]   thePStart: The start point[quote]
     * @param[in]   thePEnd: The end point[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-19
     */
    void Points(Handle(Geom_Point)& thePStart, Handle(Geom_Point)& thePEnd) const;

    /**
     * @brief       Update the shape.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-20
     */
    void update();

private:
    /**
     * @brief       Initialize the properties.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-19
     */
    void initProperties();

    /**
     * @brief       Generate the line shape.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-19
     */
    void generateShape();

private:
    /**
     * @brief       The start point of the datum line.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-19
     */
    Handle(Geom_Point) m_start;

    /**
     * @brief       The end point of the datum line.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-19
     */
    Handle(Geom_Point) m_end;

};
//@}

// Datum point.
//@{
class FITKOCCAIS_Point;
DEFINE_STANDARD_HANDLE(FITKOCCAIS_Point, AIS_Point);

/**
 * @brief       The sub-class of AIS_Point.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-30
 */
class FITKOCCAIS_Point : public AIS_Point, public FITKAIS_ObjectBase
{
public:
    /**
     * @brief       Constructor.
     * @param[in]   aComponent: Initializes the point aComponent from which the point[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    FITKOCCAIS_Point(const Handle(Geom_Point)& aComponent);

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    virtual ~FITKOCCAIS_Point() = default;

private:
    /**
     * @brief       Initialize the properties.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-04
     */
    void initProperties();

};
//@}

// Custom datum point.
//@{
class FITKOCCAIS_ShapePoint;
DEFINE_STANDARD_HANDLE(FITKOCCAIS_ShapePoint, AIS_ColoredShape);

/**
 * @brief       The sub-class of AIS_Shape.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-20
 */
class FITKOCCAIS_ShapePoint : public AIS_ColoredShape, public FITKAIS_ObjectBase
{
public:
    /**
     * @brief       Constructor.
     * @param[in]   aComponent: Initializes the point aComponent from which the point[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-20
     */
    FITKOCCAIS_ShapePoint(const Handle(Geom_Point)& aComponent);

    /**
     * @brief       Constructor.
     * @param[in]   shape: The point shape[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-10-10
     */
    FITKOCCAIS_ShapePoint(TopoDS_Shape & shape);

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-20
     */
    virtual ~FITKOCCAIS_ShapePoint() = default;

    /**
     * @brief       Get the type of the object.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-20
     */
    virtual AIS_KindOfInteractive Type() const Standard_OVERRIDE;

    /**
     * @brief       Get the point handle of the datum point.
     * @return      The point handle
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-20
     */
    Handle(Geom_Point) Component();

    /**
     * @brief       Update the shape.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-20
     */
    void update();

private:
    /**
     * @brief       Initialize the properties.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-20
     */
    void initProperties();

    /**
     * @brief       Generate the point shape.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-20
     */
    void generateShape();

private:
    /**
     * @brief       The point handle.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-20
     */
    Handle(Geom_Point) m_point;

};
//@}

#endif // __FITKOCCAIS_PLANE_H__

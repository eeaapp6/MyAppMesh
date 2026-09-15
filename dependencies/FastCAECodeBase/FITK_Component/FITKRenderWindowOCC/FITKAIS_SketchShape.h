/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKAIS_SketchShape.h
 * @brief   Subclass of AIS_SketchShape, which can save sketch information.
 *
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-08-13
 *********************************************************************/

#ifndef __FITKAIS_SKETCHSHAPE_H__
#define __FITKAIS_SKETCHSHAPE_H__

#include "FITKRenderWindowOCCAPI.h"
#include "TypeDefOCCWindow.h"

#include <AIS_Shape.hxx>
#include <AIS_InteractiveObject.hxx>

#include <QObject>
#include <QList>

 // Forward declaration
//class TopoDS_Shape;
class Geom_Circle;
class AIS_InteractiveContext;

DEFINE_STANDARD_HANDLE(FITKAIS_SketchShape, AIS_Shape);

class FITKAIS_SketchShape : public QObject, public AIS_Shape
{
    Q_OBJECT
public:
    FITKAIS_SketchShape(AIS_InteractiveContext* m_context);
    ~FITKAIS_SketchShape() = default;

    void setShapeType(CommonOCC::SketchShapeType t);

    /**
     * @brief   add point to list
     * @param   p: Point selected by mouse interaction
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void addPoint(gp_Pnt& p);

    /**
     * @brief   get point to list
     * @return  Point selected by mouse interaction
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    const QList<double*> getPoint();

    /**
     * @brief   replace last point
     * @param   p: Point selected by mouse interaction
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void replaceLast(gp_Pnt& p);

    /**
     * @brief   sey init geometry parameters
     * @param   p: Point selected by mouse interaction
     * @param   direction: direction selected by mouse interaction
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void setInitParameter(const gp_Pnt& p, const gp_Dir& direction, const gp_Dir& upDir);

    /**
    * @brief   get drawing status
    * @author  fulipeng (fulipengqd@yeah.net)
    * @date    2024-08-13
    */
    bool drawStatus();

    /**
     * @brief   参数点是否可以继续添加
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-26
     */
    bool isAddParameters();

    /**
    * @brief   参数是否满足
    * @author  fulipeng (fulipengqd@yeah.net)
    * @date    2024-08-26
    */
    bool isParametersSatisfy();

    /**
     * @brief   Set drawing status
     * @param   status: drawing status
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void setDrawStatus(bool status);

    /**
     * @brief   refresh drawing
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void update();

signals:
    /**
     * @brief   parameters satisfy
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void sig_parametersSatisfy();

public slots:
    /**
     * @brief   Update selected objects
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-28
     */
    void slot_updateSelectdObject();

private:

    /**
     * @brief   initialization
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void init();

    /**
     * @brief   update draft line
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void updateDraftLine();
    /**
     * @brief   update draft wire
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void updateDraftWire();
    /**
     * @brief   update draft fillet
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void updateDraftFillet();
    /**
     * @brief   update draft arcTools
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void updateDraftArcTools();
    /**
     * @brief   update draft circle
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void updateDraftCircle();
    /**
     * @brief   update draft ellipse
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void updateDraftEllipse();
    /**
     * @brief   update draft rectangle
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void updateDraftRectangle();
    /**
     * @brief   update draft polygon
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void updateDraftPolygon();
    /**
     * @brief   update draft bspline
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void updateDraftBspline();
    /**
     * @brief   update draft bezierTools
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void updateDraftBezierTools();

    /**
     * @brief   update draft gyperbola
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-09-04
     */
    void updateDraftHyperbola();

    /**
     * @brief   update draft parabola
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-09-04
     */
    void updateDraftParabola();


private:

    /**
     * @brief   function pointer
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    void(FITKAIS_SketchShape::*updateDraft)() { nullptr };

    /**
     * @brief   for sketch mode picking
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-12
     */
    AIS_InteractiveContext* m_context{ nullptr };

    /**
     * @brief   sketch type
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    CommonOCC::SketchShapeType _ssType;

    /**
     * @brief   Input Point List
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    QList<gp_Pnt> _inputPointList;

    /**
     * @brief   calculate Point List
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-09-03
     */
    QList<gp_Pnt> _calculatePointList;
    /**
     * @brief   mouse move point
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    gp_Pnt _mouseMovePoint;
    gp_Pnt _p;

    /**
     * @brief   Z direction
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    gp_Dir _direction;

    /**
     * @brief   Y direction
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    gp_Dir _upDirection;

    /**
     * @brief   Linked list of temporary storage points
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-26
     */
    QList<double*> tempPList{};
    /**
     * @brief   default value,Create minimum value passed by sketch
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    double _defaultValue = 0.001;

    /**
     * @brief   sketch draw status
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-13
     */
    bool _drawStatus = false;
};

#endif
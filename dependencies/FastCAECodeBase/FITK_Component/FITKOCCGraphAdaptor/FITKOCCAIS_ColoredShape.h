/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCAIS_ColoredShape.h
 * @brief       Subclass of AIS_ColoredShape, which can save user information.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-02
 *********************************************************************/

#ifndef __FITKOCCAIS_COLOREDSHAPE_H__
#define __FITKOCCAIS_COLOREDSHAPE_H__

#include <AIS_ColoredShape.hxx>
#include <TopoDS_Shape.hxx>
#include "FITKAIS_ObjectBase.h"

 // Qt
#include <QList>
#include <QColor>

// Forward declaration
class FITKOCCAIS_ColoredShape;
DEFINE_STANDARD_HANDLE(FITKOCCAIS_ColoredShape, AIS_ColoredShape);

/**
 * @brief       Subclass of AIS_ColoredShape, which can save user information.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-02
 */
class FITKOCCAIS_ColoredShape : public AIS_ColoredShape, public FITKAIS_ObjectBase
{
public:
    /**
     * @brief       Copy constructor.
     * @param[in]   theShape: The TopoDS_Shape data[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-02
     */
    FITKOCCAIS_ColoredShape(const TopoDS_Shape& theShape);

    /**
     * @brief       Copy constructor.
     * @param[in]   theShape: The shape interactive object[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-02
     */
    FITKOCCAIS_ColoredShape(const Handle(FITKOCCAIS_ColoredShape)& theShape);

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-31
     */
    virtual ~FITKOCCAIS_ColoredShape() = default;

    /**
     * @brief       Set the sub-shape color by manual.[virtual]
     * @param[in]   shape: The shape[quote]
     * @param[in]   color: The shape color
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-07
     */
    virtual void setSubShapeColor(const TopoDS_Shape & shape, QColor color);

    /**
     * @brief       Set the sub-shape transparency.[virtual]
     * @param[in]   shape: The shape[quote]
     * @param[in]   trans: The transparency
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-07
     */
    virtual void setSubShapeTransparency(const TopoDS_Shape & shape, double trans);

    /**
     * @brief       Hide the sub-shape.[virtual]
     * @param[in]   shape: The shape[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-07
     */
    virtual void setSubShapeHidden(const TopoDS_Shape & shape);

    /**
     * @brief       Clear custom transparency of sub-shapes.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-07
     */
    virtual void clearSubShapeTransparency();

    /**
     * @brief       Clear custom color and transparency.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-07
     */
    virtual void clearSubShapeAspect();

protected:
    ///**
    // * @brief       The select computation method.
    // * @author      ChengHaotian (yeguangbaozi@foxmail.com)
    // * @date        2025-07-07
    // */
    //void ComputeSelection(const Handle(SelectMgr_Selection) & theSelection,
    //    const Standard_Integer theMode) Standard_OVERRIDE;

    ///**
    // * @brief       Add sub-shape to selection considering hidden state (recursively).
    // * @param[in]   theParentDrawer: Drawer of parent shape
    // * @param[in]   theShapeDrawerMap: Shapes map
    // * @param[in]   theShape: Shape to compute sensitive entities
    // * @param[in]   theOwner: Selectable owner object
    // * @param[in]   theSelection: Selection to append new sensitive entities
    // * @param[in]   theTypOfSel: Type of selection
    // * @param[in]   thePriority: The priority
    // * @param[in]   theDeflection: Linear deflection
    // * @param[in]   theDeflAngle: Angular deflection
    // * @author      ChengHaotian (yeguangbaozi@foxmail.com)
    // * @date        2025-07-07
    // */
    //Standard_EXPORT void computeSubshapeSelectionSkipInVis(const Handle(AIS_ColoredDrawer)& theParentDrawer,
    //    const AIS_DataMapOfShapeDrawer& theShapeDrawerMap,
    //    const TopoDS_Shape& theShape,
    //    const Handle(StdSelect_BRepOwner)& theOwner,
    //    const Handle(SelectMgr_Selection)& theSelection,
    //    const TopAbs_ShapeEnum theTypOfSel,
    //    const Standard_Integer thePriority,
    //    const Standard_Real theDeflection,
    //    const Standard_Real theDeflAngle);

protected:
    ///**
    // * @brief       The colored shape list.
    // * @author      ChengHaotian (yeguangbaozi@foxmail.com)
    // * @date        2025-07-07
    // */
    //QList<TopoDS_Shape> m_coloredShapes;

    ///**
    // * @brief       The transparent shape list.
    // * @author      ChengHaotian (yeguangbaozi@foxmail.com)
    // * @date        2025-07-07
    // */
    //QList<TopoDS_Shape> m_transShapes;

    ///**
    // * @brief       The hidden shape list.
    // * @author      ChengHaotian (yeguangbaozi@foxmail.com)
    // * @date        2025-07-07
    // */
    //QList<TopoDS_Shape> m_hiddenShapes;

};

#endif // __FITKOCCAIS_COLOREDSHAPE_H__

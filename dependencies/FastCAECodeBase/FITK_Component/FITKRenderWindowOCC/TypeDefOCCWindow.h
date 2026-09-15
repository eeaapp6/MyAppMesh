/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    TypeDefOCCWindow.h
 * @brief   Declaration of enums and structures
 *
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-08-13
 *********************************************************************/

#ifndef __TYPEDEFOCCWINDOW_H__
#define __TYPEDEFOCCWINDOW_H__

#include <QObject>

#include "FITKRenderWindowOCCAPI.h"

/**
 * @brief   Tool class for enums.
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-10
 */
class FITKRENDERWINDOWOCCAPI CommonOCC : public QObject
{
    Q_OBJECT;

public:
    /**
     * @brief   The shape selectable types.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-12
     */
    enum SelectMode
    {
        // For shape select.
        S_None = -1,

        // The start of base shape select modes.
        S_ShapeBaseModeStart = 0,

        // For sketch only.
        S_ShapeShape = 0,

        S_ShapeVertex = 1,
        S_ShapeEdge = 2,
        S_ShapeWire = 3,
        S_ShapeFace = 4,
        S_ShapeShell = 5,
        S_ShapeSolid = 6,

        // Not used for now.
        S_ShapeCompSolid = 7,

        // For sketch only.
        S_ShapeCompound = 8,

        // The end of base shape select modes.
        S_ShapeBaseModeEnd = S_ShapeCompound,

        // Composite selct modes.
        // S_ShapeEdgeAndWire,

        // For mesh select.
        S_MeshNode,
        S_MeshElement
    };
    Q_ENUM(SelectMode);
    Q_DECLARE_FLAGS(SelectModes, SelectMode)
    Q_FLAG(SelectModes)

    enum class SketchShapeType
    {
        None = -1,
        Draft_Line = 0,
        Draft_Wire,
        Draft_Fillet,
        Draft_ArcTools,
        Draft_Circle,
        Draft_Ellipse,
        Draft_Rectangle,
        Draft_Polygon,
        Draft_Bspline,
        Draft_BezierTools,
        Draft_Hyperbola,
        Draft_Parabola,
        EMax,
    };

    enum class SketchAxisType
    {
        None = -1,
        XOY = 0,
        ZOX = 1,
        YOZ = 2,
    };
};

#endif
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKV3d_Viewer.h
 * @brief   Subclass of V3d_Viewer and QObject.( For changing grid
 *          properties. )
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-13
 *********************************************************************/

#ifndef __FITKV3D_VIEWER_H__
#define __FITKV3D_VIEWER_H__

#include <QObject>
#include <V3d_Viewer.hxx>
#include <Graphic3d_AspectMarker3d.hxx>

// Forward declaration

//class V3d_Viewer;
DEFINE_STANDARD_HANDLE(FITKV3d_Viewer, V3d_Viewer);

/**
 * @brief   The sub-class of V3d_Viewer.
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-13
 */
class FITKV3d_Viewer : public QObject, public V3d_Viewer
{
    Q_OBJECT
public:
    /**
     * @brief   Constructor.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-13
     */
    FITKV3d_Viewer(const Handle(Graphic3d_GraphicDriver) & theDriver);

    /**
     * @brief   Destructor.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-13
     */
    ~FITKV3d_Viewer() = default;

    /**
     * @brief   Enable the user defined grid.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-13
     */
    void EnableCustomGrid();

    /**
     * @brief   Disable the user defined grid.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-13
     */
    void DisbaleCustomGrid();

private:
    /**
     * @brief   Create the user defined grid.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-13
     */
    void createGrid();

private:
    /**
     * @brief   The user defined grid.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-13
     */
    Handle(Aspect_Grid) m_grid;

    /**
     * @brief       The echo aspect.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-10-29
     */
    Handle(Graphic3d_AspectMarker3d) m_hitAsp;

};

#endif // __FITKV3D_VIEWER_H__
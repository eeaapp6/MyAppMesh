/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKV3d_View.h
 * @brief   Subclass of V3d_View and QObject.( For detecting redraw. )
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-13
 *********************************************************************/

#ifndef __FITKV3D_VIEW_H__
#define __FITKV3D_VIEW_H__

#include <QObject>
#include <V3d_View.hxx>

 // Forward declaration

//class V3d_View;
DEFINE_STANDARD_HANDLE(FITKV3d_View, V3d_View);

/**
 * @brief   The V3d_View with signals.
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-16
 */
class FITKV3d_View : public QObject, public V3d_View
{
    Q_OBJECT
public:
    /**
     * @brief   Constructor.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-16
     */
    FITKV3d_View(const Handle(V3d_Viewer)& theViewer, const V3d_TypeOfView theType = V3d_ORTHOGRAPHIC);

    /**
     * @brief   Constructor.
     * @param   theViewer
     * @param   theView
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-16
     */
    FITKV3d_View(const Handle(V3d_Viewer)& theViewer, const Handle(V3d_View)& theView);

    /**
     * @brief   Destructor.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-16
     */
    ~FITKV3d_View() = default;

    /**
     * @brief   Redisplays the view even if there has not been any modification.[override]
     *          P.S. Must be called if the view is shown.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-16
     */
    void Redraw() const override;

    /**
     * @brief   Updates layer of immediate presentations.[override]
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-16
     */
    void RedrawImmediate() const override;

signals:
    /**
     * @brief   Signal - V3d_View redrawed.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-16
     */
    void sig_viewRedrawed() const;

};

#endif // __FITKV3D_VIEW_H__
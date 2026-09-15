/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKAIS_ViewController.h
 * @brief   Subclass of AIS_ViewController and QObject.( For disabling select. )
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-22
 *********************************************************************/

#ifndef __FITKAIS_VIEWCONTROLLER_H__
#define __FITKAIS_VIEWCONTROLLER_H__

#include <QObject>
#include <AIS_ViewController.hxx>

 // Forward declaration

//class AIS_ViewController;
DEFINE_STANDARD_HANDLE(FITKAIS_ViewController, AIS_ViewController);

/**
 * @brief   The V3d_View with signals.
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-22
 */
class FITKAIS_ViewController : public QObject, public AIS_ViewController
{
    Q_OBJECT
public:
    /**
     * @brief   Constructor.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-22
     */
    FITKAIS_ViewController() = default;

    /**
     * @brief   Destructor.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-22
     */
    ~FITKAIS_ViewController() = default;

    /**
     * @brief   Enable or disable the select mode in AIS_ViewController.
     * @param   flag: Enable or not
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-22
     */
    void EnableSelectInView(bool flag);

    /**
     * @brief   Callback called by handleMoveTo() on Selection in 3D Viewer.
     *          (This method is expected to be called from rendering thread.)[virtual][override]
     * @param   theCtx
     * @param   theView
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-22
     */
    virtual void OnSelectionChanged(const Handle(AIS_InteractiveContext)& theCtx,
         const Handle(V3d_View)& theView) override;

signals:
    /**
     * @brief   Signal - The mouse click tried to perform a select.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-22
     */
    void sig_pickPerformed();

private:
    /**
     * @brief   Wether to select in view while the left mouse button clicked.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-22
     */
    bool m_enableSelect = true;

};

#endif // __FITKAIS_VIEWCONTROLLER_H__
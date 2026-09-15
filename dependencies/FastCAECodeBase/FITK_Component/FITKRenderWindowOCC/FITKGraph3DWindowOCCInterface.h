/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKGraph3DWindowOCCInterface.h
 * @brief   The component interface of graph 3D window for OCC.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-11
 *********************************************************************/

#ifndef __FITKGRAPH3DWINDOWOCCINTERFACE_H__
#define __FITKGRAPH3DWINDOWOCCINTERFACE_H__

#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

#include "FITKRenderWindowOCCAPI.h"

#include <QWidget>

namespace Render
{
    /**
     * @brief   The component interface of graph 3D window for OCC.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-11
     */
    class FITKRENDERWINDOWOCCAPI FITKGraph3DWindowOCCInterface : public AppFrame::FITKComponentInterface
    {
    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        FITKGraph3DWindowOCCInterface() = default;

        /**
         * @brief   Destructor.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        virtual ~FITKGraph3DWindowOCCInterface() = default;

        /**
         * @brief   Get the indexed render window.
         * @param   port: The port of the widget
         * @return  The render widget
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        QWidget* getWidget(const int port) override;

        /**
         * @brief   Get the component name.
         * @return  
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        QString getComponentName();

    };
}

#endif // __FITKGRAPH3DWINDOWOCCINTERFACE_H__

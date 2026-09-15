/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectTopoBase.h
 * @brief       Base graph object for previewing the virtual topo information.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-06
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTTOPOBASE_H__
#define __FITKOCCGRAPHOBJECTTOPOBASE_H__

#include "FITKOCCGraphObject3D.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

class AIS_ColoredShape;

namespace Interface
{
    class FITKAbsVirtualTopo;
}

namespace Exchange
{
    /**
     * @brief       Base graph object for previewing the virtual topo information.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-06
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectTopoBase : public FITKOCCGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectTopoBase, FITKOCCGraphObject3D);
        FITKCLASS(Exchange, FITKOCCGraphObjectTopoBase);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   dataObj: The data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKOCCGraphObjectTopoBase(Core::FITKAbstractDataObject* dataObj);

        /**
         * @brief       Destructor.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual ~FITKOCCGraphObjectTopoBase() = default;

        /**
         * @brief       Update the interactive objects in graph object.[override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        void updateInteractiveObjs() override;

        /**
         * @brief       Set the color.[override]
         * @param[in]   color: The color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        void setColor(QColor color) override;

        /**
         * @brief       Enable or disable transparency.[override]
         * @param[in]   isOn: Enable or not
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        void setEnableTransparency(bool isOn) override;

        /**
         * @brief       Hide or show the graph object's interactive objects.[override]
         * @param[in]   visibility: The visibility.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        void setVisible(bool visibility) override;

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKOCCGraphObjectTopoBase() = default;

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTTOPOBASE_H__

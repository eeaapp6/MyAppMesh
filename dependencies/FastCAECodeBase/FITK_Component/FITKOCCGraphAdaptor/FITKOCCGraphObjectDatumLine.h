/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectDatumLine.h
 * @brief       Graph object for datum line.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-30
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTDATUMLINE_H__
#define __FITKOCCGRAPHOBJECTDATUMLINE_H__

#include "FITKOCCGraphObject3D.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

namespace Interface
{
    class FITKAbsGeoDatumLine;
}

namespace Exchange
{
    /**
     * @brief       Graph object for datum line.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-30
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectDatumLine : public FITKOCCGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectDatumLine, FITKOCCGraphObject3D);
        FITKCLASS(Exchange, FITKOCCGraphObjectDatumLine);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   datumData: The datum data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        FITKOCCGraphObjectDatumLine(Interface::FITKAbsGeoDatumLine* datumData);

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-06
         */
        ~FITKOCCGraphObjectDatumLine();

        /**
         * @brief       Update the graph object.[override]
         * @param[in]   forceUpdate: Force update[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-06
         */
        void update(bool forceUpdate = false) override;

        /**
         * @brief       Update the interactive objects in graph object.[override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-09
         */
        void updateInteractiveObjs() override;

        /**
         * @brief       Set the color to all vertice, edges and faces.[override]
         * @param[in]   color: The color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        void setColor(QColor color) override;

        /**
         * @brief       Enable or disable transparency.[override]
         * @param[in]   isOn: Enable or not
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-05
         */
        void setEnableTransparency(bool isOn) override;

        /**
         * @brief       Set the line width of the edges( wire ).[override]
         * @param[in]   size: The line width
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        void setLineWidth(double size) override;

        /**
         * @brief       Highlight the whole object.[override]
         * @param[in]   update: Need to update the view[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-07
         */
        void highlight(bool update = true) override;

        /**
         * @brief       Dis-highlight the object.[override]
         * @param[in]   update: Need to update the view[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-07
         */
        void disHighlight(bool update = true) override;

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-06
         */
        FITKOCCGraphObjectDatumLine() = default;

    private:
        /**
         * @brief       Generate the AIS_Shape with the TopoDS_Shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        void generateShape();

    private:
        /**
         * @brief       The interactive object of the datum.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-30
         */
        AIS_InteractiveObject* m_aisObj{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTDATUMLINE_H__

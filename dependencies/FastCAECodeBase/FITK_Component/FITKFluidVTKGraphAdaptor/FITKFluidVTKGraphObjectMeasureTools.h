/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObjectMeasureTools.h
 * @brief   Graph object for calculating distance.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-26
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECTMEASURETOOLS_H__
#define __FITKFLUIDVTKGRAPHOBJECTMEASURETOOLS_H__

#include "FITKFluidVTKGraphObject3D.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

// Forward declaration
class QVariant;

class vtkAbstractWidget;
class vtkDistanceWidget;
class vtkAngleWidget;

namespace Core
{
    class FITKAbstractDataObject;
}

/**
 * @brief   The measure data type.
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-29
 */
enum MeasureType
{
    MT_None = -1,
    MT_Distance = 0,
    MT_Angle
};

namespace Exchange
{
    /**
     * @brief   Graph object for calculating distance.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-26
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObjectMeasureTools : public FITKFluidVTKGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObjectMeasureTools, FITKFluidVTKGraphObject3D);
        FITKCLASS(Exchange, FITKFluidVTKGraphObjectMeasureTools);

    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        FITKFluidVTKGraphObjectMeasureTools();

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        ~FITKFluidVTKGraphObjectMeasureTools();

        /**
         * @brief   Hide or show the graph object's actor.[override]
         * @param   visibility: The visibility.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void setVisible(bool visibility) override;

        /**
         * @brief   Set the measure type.
         * @param   type: The measure type
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-29
         */
        void setMeasureType(MeasureType type);

        /**
         * @brief   Set the position for the widget representation.
         * @param   index: The index of the point
         * @param   pos: The position of the point
         * 
         *          P.S.
         *          For distance widget:
         *          1. The 0th point is the begin point.
         *          2. The 1st point is the end point.
         * 
         *          For angle widget:
         *          1. The 0th point is the begin point.
         *          2. The 1st point is the center point.
         *          3. The 2nd point is the end point.
         * 
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-29
         */
        void setDisplayPosition(int index, double* pos);

    private:
        /**
         * @brief   Initialize the pointers.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void init();
 
    private:
        /**
         * @brief   The distance widget.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-29
         */
        vtkDistanceWidget* m_distWidget{ nullptr };

        /**
         * @brief   The angle widget.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-29
         */
        vtkAngleWidget* m_angleWidget{ nullptr };

        /**
         * @brief   Current widget of current measure type.( Maybe nullptr. )
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-29
         */
        vtkAbstractWidget* m_currentWidget{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECTMEASURETOOLS_H__

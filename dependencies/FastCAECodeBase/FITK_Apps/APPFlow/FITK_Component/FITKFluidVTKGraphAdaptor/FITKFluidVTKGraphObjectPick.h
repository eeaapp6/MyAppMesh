/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObjectPick.h
 * @brief   Graph object for picking.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-19
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECTPICK_H__
#define __FITKFLUIDVTKGRAPHOBJECTPICK_H__

#include "FITKFluidVTKGraphObject3D.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

// Forward declaration
class QVariant;

class vtkDataSet;
class vtkDataSetSurfaceFilter;

class FITKGraphActor;
class FITKHighOrderCellFilter;

namespace Core
{
    class FITKAbstractDataObject;
}

namespace Exchange
{
    /**
     * @brief   Graph object for picking.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-19
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObjectPick : public FITKFluidVTKGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObjectPick, FITKFluidVTKGraphObject3D);
        FITKCLASS(Exchange, FITKFluidVTKGraphObjectPick);

    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        FITKFluidVTKGraphObjectPick();

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        ~FITKFluidVTKGraphObjectPick();

        /**
         * @brief   Hide or show the graph object's actor.[override]
         * @param   visibility: The visibility.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void setVisible(bool visibility) override;

        /**
         * @brief   Set the dataset for visualization.
         * @param   grid: The picked data
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void setPickedData(vtkDataSet* grid);

        /**
         * @brief   Set the data set for visualization.
         * @param   type: The type of picked data( 1: Points, 2: Cells, 3: Mixed)
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void setPickedType(int type);

        /**
        * @brief   Set the color.[override]
        * @param   color: The color
        * @author  ChengHaotian (yeguangbaozi@foxmail.com)
        * @date    2024-07-19
        */
        void setColor(QColor color) override;

    private:
        /**
         * @brief   Initialize the pointers.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        void init();

        /**
         * @brief   Initialize the actor's visualization properties.[override]
         * @param   actor: The actor
         * @param   details: No used
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void initActorProperties(vtkProp* actor, QVariant details = QVariant()) override;
 
    private:
        /**
         * @brief   The actor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        FITKGraphActor* m_fActor{ nullptr };

        /**
         * @brief   The surface filter for high-order element.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        vtkDataSetSurfaceFilter* m_surfaceFilter{ nullptr };

        /**
         * @brief   The high-order element dealer.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        FITKHighOrderCellFilter* m_highOrderFilter{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECTPICK_H__

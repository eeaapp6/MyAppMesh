/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObjectPickPreview.h
 * @brief   Graph object for picking preview.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-19
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECTPICKPREVIEW_H__
#define __FITKFLUIDVTKGRAPHOBJECTPICKPREVIEW_H__

#include "FITKFluidVTKGraphObject3D.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

// Forward declaration
class QVariant;

class vtkDataSet;
class vtkSelectionNode;
class vtkSelection;
class vtkExtractSelection;
class vtkIdTypeArray;
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
     * @brief   Graph object for picking preview.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-19
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObjectPickPreview : public FITKFluidVTKGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObjectPickPreview, FITKFluidVTKGraphObject3D);
        FITKCLASS(Exchange, FITKFluidVTKGraphObjectPickPreview);

    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        FITKFluidVTKGraphObjectPickPreview();

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        ~FITKFluidVTKGraphObjectPickPreview();

        /**
         * @brief   Hide or show the graph object's actor.[override]
         * @param   visibility: The visibility.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void setVisible(bool visibility) override;

        /**
         * @brief   Set the picked type( Point or Cell ) and the picked index.
         * @param   actor: The picked actor.
         * @param   type: The picked type( 1: Point, 2: Cell, 3: Mixed )
         * @param   index: The picked index
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void setPickedData(vtkActor* actor, int type, int index);

        /**
         * @brief   Set the dataset for visualization.
         * @param   grid: The picked data
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void setPickedData(vtkDataSet* grid);

        /**
         * @brief   Set the picked type( Point or Cell ) and the picked index.
         * @param   dataSet: The picked dataSet.
         * @param   type: The picked type( 1: Point, 2: Cell, 3: Mixed )
         * @param   index: The picked index
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void setPickedData(vtkDataSet* dataSet, int type, int index);

        /**
         * @brief   Set the dataset for visualization.
         * @param   type: The type of picked data( 1: Points, 2: Cells, 3: Mixed )
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void setPickedType(int type);

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
         * @brief   The selection node for extract mesh grid.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        vtkSelectionNode* m_selectNode{ nullptr };

        /**
         * @brief   The selection for vtkSelectionNode.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        vtkSelection* m_section{ nullptr };

        /**
         * @brief   The mesh grid extractor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        vtkExtractSelection* m_extractSelection{ nullptr };

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

        /**
         * @brief   The indice array of extract id.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        vtkIdTypeArray* m_idArray{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECTPICKPREVIEW_H__

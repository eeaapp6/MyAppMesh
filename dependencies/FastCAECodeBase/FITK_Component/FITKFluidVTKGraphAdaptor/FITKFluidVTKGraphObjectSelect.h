/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObjectSelect.h
 * @brief   Internal graph object for highlighting part of the model or
 *          mesh graph for VTK graph widget.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-06
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECTSELECT_H__
#define __FITKFLUIDVTKGRAPHOBJECTSELECT_H__

#include "FITKFluidVTKGraphObject3D.h"
#include "FITKFluidVTKGraphAdaptorAPI.h"

// Forward declaration
class QVariant;

class vtkDataSet;
class vtkSelectionNode;
class vtkSelection;
class vtkIdTypeArray;
class vtkExtractSelection;

class FITKGraphActor;

namespace Core
{
    class FITKAbstractNDataObject;
}

namespace Exchange
{
    /**
     * @brief   Internal graph object for highlighting part of the model or mesh graph.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-06
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObjectSelect : public FITKFluidVTKGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObjectSelect, FITKFluidVTKGraphObject3D);
        FITKCLASS(Exchange, FITKFluidVTKGraphObjectSelect);

    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        FITKFluidVTKGraphObjectSelect();

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        ~FITKFluidVTKGraphObjectSelect();

        /**
         * @brief   Hide or show the graph object's actor.[override]
         * @param   visibility: The visibility.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        void setVisible(bool visibility) override;

        /**
         * @brief   Enable or disable transparent.[override]
         * @param   isOn: Enable or disable
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        void setTransparent(bool isOn) override;

        /**
         * @brief   Change the view mode of the graph actors.[override]
         * @param   type: The shape type
         * @param   visible: The visibility
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        void setViewMode(FITKFluidVTKCommons::ShapeMeshViewMode type, bool visible) override;

        /**
         * @brief   Set the data set and the mesh type for visualization.
         * @param   grid: The picked data
         * @param   indice: The indice of cells need to be extracted[quote]
         * @param   type: The type of mesh shape data
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        void setSelectData(vtkDataSet* grid, QVector<int> & indice, FITKFluidVTKCommons::ShapeType type);

        /**
        * @brief   Set the color.[override]
        * @param   color: The color
        * @author  ChengHaotian (yeguangbaozi@foxmail.com)
        * @date    2024-06-06
        */
        void setColor(QColor color) override;

        /**
         * @brief   Clear the select data indice.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void clearData();

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
         * @date    2024-06-06
         */
        void initActorProperties(vtkProp* actor, QVariant details = QVariant()) override;
 
    private:
        /**
         * @brief   The actor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        FITKGraphActor* m_fActor{ nullptr };

        /**
         * @brief   The selection node for extract mesh grid.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        vtkSelectionNode* m_selectNode{ nullptr };

        /**
         * @brief   The selection for vtkSelectionNode.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        vtkSelection* m_section{ nullptr };

        /**
         * @brief   The mesh grid extractor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        vtkExtractSelection* m_extractSelection{ nullptr };

        /**
         * @brief   The indice array of extract id.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        vtkIdTypeArray* m_idArray{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECTSELECT_H__

/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObjectBoundMesh.h
 * @brief   Graph object for 'boundary mesh' data.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-23
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECTBOUNDMESH_H__
#define __FITKFLUIDVTKGRAPHOBJECTBOUNDMESH_H__

#include "FITKFluidVTKGraphObject3D.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

// Forward declaration
class QVariant;

class FITKGraphActor;

namespace Interface
{
    class FITKBoundaryMeshVTK;
}

namespace Exchange
{
    class FITKFluidVTKGraphObjectSelect;
}

namespace Exchange
{
    /**
     * @brief   Graph object for 'boundary mesh' data.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-23
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObjectBoundMesh : public FITKFluidVTKGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObjectBoundMesh, FITKFluidVTKGraphObject3D);
        FITKCLASS(Exchange, FITKFluidVTKGraphObjectBoundMesh);

    public:
        /**
         * @brief   Constructor.
         * @param   meshData: The boundary mesh data
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        FITKFluidVTKGraphObjectBoundMesh(Interface::FITKBoundaryMeshVTK* meshData);

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        ~FITKFluidVTKGraphObjectBoundMesh() = default;

        /**
         * @brief   Update all graph actors.[override]
         * @param   forceUpdate: Force update[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-06
         */
        void update(bool forceUpdate = false) override;

        /**
         * @brief   Hide or show the graph object's actor.[override]
         * @param   visibility: The visibility.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        void setVisible(bool visibility) override;

        /**
        * @brief   Set the color.[override]
        * @param   color: The color
        * @author  ChengHaotian (yeguangbaozi@foxmail.com)
        * @date    2024-07-23
        */
        void setColor(QColor color) override;

        /**
         * @brief   Set the mesh view mode by the given type and flag.[override]
         * @param   type: The shape type
         * @param   visible: The visibility
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void setViewMode(FITKFluidVTKCommons::ShapeMeshViewMode type, bool visible) override;

        /**
         * @brief   Enable or disable transparent.[override]
         * @param   isOn: Enable or disable
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-30
         */
        void setTransparent(bool isOn) override;

        /**
         * @brief   Highlight the graph actors.[override]
         * @param   type: The shape type need to be highlighted[default]
         * @param   color: The given color for highlighting this time[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void highlight(FITKFluidVTKCommons::ShapeType type = FITKFluidVTKCommons::ShapeTypeNone, QColor color = QColor()) override;

        /**
         * @brief   Dis-highlight the graph actors.[override]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void disHighlight() override;

        /**
         * @brief   Advanced highlight mode for highlighting part of the graph actors.[override]
         * @param   type: The type of the model need to be highlighted
         * @param   indice: The given indice of the model component need to be highlight
         * @param   color: The given color for highlighting this time[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void advanceHighlight(FITKFluidVTKCommons::ShapeType type, QVector<int> indice, QColor color = QColor()) override;

        /**
         * @brief   Dis-highlight the graph actors if the graph object is in advanced highlighting mode.[override]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void disAdvanceHighlight() override;

        /**
         * @brief   Get the VTK mesh grid by the given mesh type.[override]
         * @param   type: The shape's VTK mesh type
         * @return  The vtkDataSet object of the given type of mesh
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        vtkDataSet* getMesh(FITKFluidVTKCommons::ShapeType type) override;

    private:
        /**
         * @brief   Initialize the pointer.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        void init();

        /**
         * @brief   Generate the mesh with the vtkDataSet.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-24
         */
        void generateGraph();
 
    private:
        /**
         * @brief   The model extractor for normal or advanced highlight.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        FITKFluidVTKGraphObjectSelect* m_highlightSelector{ nullptr };

        /**
         * @brief   The actor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        FITKGraphActor* m_fActor{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECTBOUNDMESH_H__

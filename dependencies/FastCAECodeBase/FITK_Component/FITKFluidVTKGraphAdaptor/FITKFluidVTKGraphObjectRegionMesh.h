/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObjectRegionMesh.h
 * @brief   Graph object for previewing 'region mesh' data.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-25
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECTREGIONMESH_H__
#define __FITKFLUIDVTKGRAPHOBJECTREGIONMESH_H__

#include "FITKFluidVTKGraphObject3D.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

// Forward declaration
class QVariant;

class vtkUnstructuredGrid;

class FITKGraphActor;

namespace Interface
{
    class FITKAbstractRegionMeshSize;
}

namespace Exchange
{
    class FITKFluidVTKGraphObjectSelect;
}

namespace Exchange
{
    /**
     * @brief   Graph object for previewing 'region mesh' data.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-25
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObjectRegionMesh : public FITKFluidVTKGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObjectRegionMesh, FITKFluidVTKGraphObject3D);
        FITKCLASS(Exchange, FITKFluidVTKGraphObjectRegionMesh);

    public:
        /**
         * @brief   Constructor.
         * @param   meshData: The region mesh data
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        FITKFluidVTKGraphObjectRegionMesh(Interface::FITKAbstractRegionMeshSize* meshData);

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        ~FITKFluidVTKGraphObjectRegionMesh();

        /**
         * @brief   Hide or show the graph object's actor.[override]
         * @param   visibility: The visibility.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        void setVisible(bool visibility) override;

        /**
        * @brief   Set the color.[override]
        * @param   color: The color
        * @author  ChengHaotian (yeguangbaozi@foxmail.com)
        * @date    2024-07-25
        */
        void setColor(QColor color) override;

        /**
         * @brief   Update all graph actors.[override]
         * @param   forceUpdate: Force update[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        void update(bool forceUpdate = false) override;

        /**
         * @brief   Advanced highlight mode for highlighting part of the graph actors.[override]
         * @param   type: The type of the model need to be highlighted
         * @param   indice: The given indice of the model component need to be highlight
         * @param   color: The given color for highlighting this time[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        void advanceHighlight(FITKFluidVTKCommons::ShapeType type, QVector<int> indice, QColor color = QColor()) override;

        /**
         * @brief   Dis-highlight the graph actors if the graph object is in advanced highlighting mode.[override]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        void disAdvanceHighlight() override;

    private:
        /**
         * @brief   Initialize the pointer.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        void init();

        /**
         * @brief   Generate the data set by different type of the region shape data.
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        bool generateDataSet();

        /**
         * @brief   Generate the mesh actor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        void generateGraph();

        /**
         * @brief   Clear all data set.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        void clearData();
 
    private:
        /**
         * @brief   The model extractor for normal or advanced highlight.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        FITKFluidVTKGraphObjectSelect* m_highlightSelector{ nullptr };
#ifdef Q_OS_WIN32
        /**
         * @brief   The actor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        FITKGraphActor* m_fActor{ nullptr };
#endif
#ifdef Q_OS_LINUX
        /**
         * @brief  演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-02-06
         */
        vtkActor* m_fActor = nullptr;
#endif

        /**
         * @brief   The geometry grid data of the region shape.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        vtkUnstructuredGrid* m_ugrid{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECTREGIONMESH_H__

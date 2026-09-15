/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObjectMaterialPoints.h
 * @brief   Graph object for previewing all material points.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-26
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECTMATERIALPOINTS_H__
#define __FITKFLUIDVTKGRAPHOBJECTMATERIALPOINTS_H__

#include "FITKFluidVTKGraphObjectFixedSizeBase.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

// Forward declaration
class QVariant;

class vtkUnstructuredGrid;
class vtkPoints;
class vtkArrowSource;
class vtkTransformFilter;
class vtkDoubleArray;

class FITKTransformRepresentation;
class FITKTransformWidget;

namespace Interface
{
    class FITKZonePointManager;
}

namespace Exchange
{
    /**
     * @brief   Graph object for previewing all material points.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-26
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObjectMaterialPoints : public FITKFluidVTKGraphObjectFixedSizeBase
    {
        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObjectMaterialPoints, FITKFluidVTKGraphObjectFixedSizeBase);
        FITKCLASS(Exchange, FITKFluidVTKGraphObjectMaterialPoints);

    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        FITKFluidVTKGraphObjectMaterialPoints(Interface::FITKZonePointManager* mPtsMgr);

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        ~FITKFluidVTKGraphObjectMaterialPoints();

        /**
         * @brief   Hide or show the graph object's actor.[override]
         * @param   visibility: The visibility.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void setVisible(bool visibility) override;

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
         * @brief   Update all graph actors.[override]
         * @param   forceUpdate: Force update[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void update(bool forceUpdate = false) override;

    private:
        /**
         * @brief   Initialize the pointers.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void init();

        /**
         * @brief   Generate the data set by the points manager.
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        bool generateDataSet();

        /**
         * @brief   Generate the actor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void generateGraph();

        /**
         * @brief   Clear all data set.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void clearData();

        /**
         * @brief   Clear marker data set.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        void clearMarkerData();

    private:
        /**
         * @brief   The glyph source for point marker.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        vtkArrowSource* m_arrowSource{ nullptr };

        /**
         * @brief   The transform filter for the arrow source.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        vtkTransformFilter* m_transFilter{ nullptr };

        /**
         * @brief   The grid data of the material points.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        vtkUnstructuredGrid* m_ugrid{ nullptr };

        /**
         * @brief   The points data.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        vtkPoints* m_points{ nullptr };

        /**
         * @brief   The grid data of the material points.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        vtkUnstructuredGrid* m_ugridMarker{ nullptr };

        /**
         * @brief   The points data.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        vtkPoints* m_pointsMarker{ nullptr };

        /**
         * @brief   The points actor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        FITKGraphActor* m_fActor{ nullptr };

        /**
         * @brief   The point mark actor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        FITKGraphActor* m_fActorMarker{ nullptr };

        /**
         * @brief   The marker widget for moving the point.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-31
         */
        FITKTransformWidget* m_moveWidget{ nullptr };

        /**
         * @brief   The representation for moving widget.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-31
         */
        FITKTransformRepresentation* m_moveRep{ nullptr };

        /**
         * @brief   The marker glyph.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        vtkGlyph3D* m_glyph{ nullptr };

        /**
         * @brief   The normals array for glyph 3D.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        vtkDoubleArray* m_normals{ nullptr };

        /**
         * @brief   The size of the vertex.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-30
         */
        double m_pointSize = 10.;

        /**
         * @brief   Internal flag for the advance highlight marker.
         *          The movable style: Three arrow point to ( 1, 0, 0),
         *          ( 0, 1, 0 ) and ( 0, 0, 1 ).
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-30
         */
        bool m_useMovableMarkerStyle = false;

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECTMATERIALPOINTS_H__

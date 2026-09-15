/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKCommons.h
 * @brief   Shape graph object tool functions class.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-05
 *********************************************************************/

#ifndef __FITKFLUIDVTKCOMMONS_H__
#define __FITKFLUIDVTKCOMMONS_H__

#include "FITKFluidVTKGraphAdaptorAPI.h"

#include <QColor>

// QString to char*
#ifndef QStringToCharA
#define QStringToCharA(str) \
    str.toUtf8().data()
#endif // !QStringToCharA

namespace Exchange
{
    /**
     * @brief   Shape graph object tool functions.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-05
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKCommons
    {
    public:
        /**
         * @brief   The same as TopAbs_ShapeEnum.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        enum ShapeAbsEnum
        {
            STA_COMPOUND,
            STA_COMPSOLID,
            STA_SOLID,
            STA_SHELL,
            STA_FACE,
            STA_WIRE,
            STA_EDGE,
            STA_VERTEX,
            STA_SHAPE
        };

        /**
         * @brief   The view mode for shape and mesh.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-14
         */
        enum ShapeMeshViewMode
        {
            SMVM_Shade = 0,
            SMVM_Wireframe,
            SMVM_Vertex,
        };

        /**
         * @brief   The pick( select ) mode.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        enum ShapePickMode
        {
            PickNone = -1,
            PickVertex = 0,
            PickEdge,
            PickFace,
            PickSolid,
            PickAll
        };

        /**
         * @brief   The data type.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        enum ShapeDataType
        {
            OtherShape = -1,
            ModelShape = 0,
            MeshShape = 1
        };

        /**
         * @brief   The shape's mesh type.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        enum ShapeType
        {
            ShapeTypeNone = -1,

            // Shape model type.
            ModelVertex = 0,
            ModelEdge,
            ModelFace,

            // The solid is the same as ModelFace in vtk graph object.
            ModelSolid,

            // Shape mesh type.
            MeshNode,
            MeshElement,

            // Other graph.
            Others
        };

        /**
         * @brief   Save the data information for graph object.
         *          ( This struct is mainly prepared for getting picked information,
         *          if the graph object is not a model, than the struct will only save
         *          the data object id. )
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        struct ShapeInfo
        {
            // The type of the graph object.
            ShapeDataType Type = OtherShape;

            // The data object id.
            int DataObjId = -1;
        };

    public:
        /**
         * @brief   Get the color by the given index from default color list.[static]
         * @param   index: The index of color
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        static QColor GetColorByIndex(int index);

        /**
         * @brief   Get color randomly.[static]
         * @return  The color
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        static QColor GetRandomColor();

        /**
         * @brief   Get the color's rgbf.[static]
         * @param   color: The color
         * @param   rgbf: The RGB float values
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        static void  QColorToDouble3(QColor color, double* rgbf);

        /**
         * @brief   Get the color's rgb.[static]
         * @param   color: The color
         * @param   rgb: The RGB int values.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        static void QColorToInt3(QColor color, int* rgb);

    public:
        // For actors color.
        //@{
        /**
         * @brief   Default color list.[static]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        static QList<QColor> s_colorList;

        /**
         * @brief   The color for geometry vertex.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        static QColor s_geoVertexColor;

        /**
         * @brief   The color for geometry edge.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        static QColor s_geoEdgeColor;

        /**
         * @brief   The color for geometry face.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        static QColor s_geoFaceColor;

        /**
         * @brief   The color for mesh face.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-13
         */
        static QColor s_meshFaceColor;

        /**
         * @brief   The color for block mesh face.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-24
         */
        static QColor s_blockMeshFaceColor;

        /**
         * @brief   The color for region mesh face.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        static QColor s_regionMeshFaceColor;

        /**
         * @brief   The color for region mesh edge.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        static QColor s_regionMeshEdgeColor;

        /**
         * @brief   The color for material points.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        static QColor s_matPointsColor;

        /**
         * @brief   The color for material point marker.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        static QColor s_matPointMarkerColor;
        //@}

        // Size.
        //@{
        /**
         * @brief   The point size for highlighting.[static]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        static double s_highlightPointSize;

        /**
         * @brief   The line width for highlighting.[static]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        static double s_highlightLineWidth;
        //@}

        // Properties.
        //@{
        /**
         * @brief   The default transparency for some of the graph object.[static]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        static double s_transparency;
        //@}

        // Advanced.
        //@{
        /**
         * @brief   The default polygon offset of the geometry model.[static]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-28
         */
        static double s_polygonOffsetGeom;

        /**
         * @brief   The default polygon offset of the mesh model.[static]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-28
         */
        static double s_polygonOffsetMesh;
        //@}
    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKCOMMONS_H__

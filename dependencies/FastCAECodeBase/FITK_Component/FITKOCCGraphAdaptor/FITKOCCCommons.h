/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCCommons.h
 * @brief       Shape graph object tool functions class.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-06-05
 *********************************************************************/

#ifndef __FITKOCCCOMMONS_H__
#define __FITKOCCCOMMONS_H__

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// The system defined data role
#ifndef ROLE_INIT
#define ROLE_INIT

// Default role
#define ROLE_DEFAULT_INPUT 1000
#define ROLE_DEFAULT_OUTPUT 2000
#define ROLE_DEFAULT_DEFAULT 3000

// Input roles for sub-class
#define ROLE_INPUT_MEASURE_IDS 1001
#define ROLE_INPUT_PICK_DIRECTION_REVERSE 1002
#define ROLE_INPUT_PICK_DIRECTION_CENTER 1003
#define ROLE_INPUT_PICK_DIRECTION_ANCHOR_MAIN_ID 1004
#define ROLE_INPUT_PICK_DIRECTION_ANCHOR_SUB_ID 1005
#define ROLE_INPUT_PICK_DIRECTION_USER_DIRECTION 1006
#define ROLE_INPUT_PICK_PROJ_POSITION_TARGET_CMD_ID 1007
#define ROLE_INPUT_PICK_PROJ_POSITION_TARGET_TOPO_ID 1008

// Output roles for sub-class
#define ROLE_OUTPUT_PICK_DIRECTION_DIRECTION 2001
#define ROLE_OUTPUT_PICK_DIRECTION_POSITION 2002
#define ROLE_OUTPUT_PICK_PROJ_POSITION 2003
#define ROLE_OUTPUT_TOPO_DIRECTION 2004

// Temp roles for sub-class
#define ROLE_TEMP_PICK_DIRECTION_DIRECTION 3001

#endif // !ROLE_INIT

// Forward declaration
class Quantity_Color;
class gp_Pnt;
class gp_Vec;
class gp_Dir;
class TopoDS_Shape;
class V3d_View;

template <class TheKeyType> 
class NCollection_DefaultHasher;

template <class TheKeyType, class TheItemType, class Hasher>
class NCollection_DataMap;

// QString to char*
#ifndef QStringToCharA
#define QStringToCharA(str) \
    str.toUtf8().data()
#endif // !QStringToCharA

namespace Exchange
{
    /**
     * @brief       Shape graph object tool functions.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-05
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCCommons
    {
    public:
        /**
         * @brief       The view mode for shape and mesh.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        enum ShapeMeshViewMode
        {
            SMVM_Shade = 0,
            SMVM_Wireframe,
            SMVM_Vertex,
        };

        /**
         * @brief       The pick( select ) mode.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-06
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
         * @brief       The data type.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        enum GraphDataType
        {
            OtherGraph = -1,
            ModelGraph = 0,
            MeshGraph,
            DatumPointGraph,
            DatumLineGraph,
            DatumPlaneGraph,
            SketchGraph,
            PickToolDirection,
            PickToolEdgePos
        };

        /**
         * @brief       The shape and mesh type.( for visualization )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-06
         */
        enum ShapeMeshType
        {
            SMT_TypeNone = -1,

            // Shape type.
            SMT_ShapeVertex = 0,
            SMT_ShapeEdge,
            SMT_ShapeFace,
            SMT_ShapeSolid,

            // Mesh type.
            SMT_MeshNode,
            SMT_MeshElement
        };

        /**
         * @brief       Save the data information for graph object.
         *              ( This struct is mainly prepared for getting picked information,
         *              if the graph object is not a model, than the struct will only save
         *              the data object id. )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        struct GraphInfo
        {
            // The type of the graph object.
            GraphDataType Type = OtherGraph;

            // The data object id.
            int DataObjId = -1;
        };

        /**
         * @brief       The shape type of the point mark.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        enum PointMarkShape
        {
            PMS_POINT = 0,
            PMS_PLUS,
            PMS_STAR,
            PMS_X,
            PMS_O,
            PMS_O_POINT,
            PMS_O_PLUS,
            PMS_O_STAR,
            PMS_O_X,
            PMS_RING1,
            PMS_RING2,
            PMS_RING3,
            PMS_BALL
        };

    public:
        /**
         * @brief       Get the color by the given index from default color list.[static]
         * @param[in]   index: The index of color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        static QColor GetColorByIndex(int index);

        /**
         * @brief       Get color randomly.[static]
         * @return      The color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-12
         */
        static QColor GetRandomColor();

        /**
         * @brief       Get the color's rgbf.[static]
         * @param[in]   color: The color
         * @param[i/o]  rgbf: The RGB float values
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        static void  QColorToDouble3(QColor color, double* rgbf);

        /**
         * @brief       Get the color's rgb.[static]
         * @param[in]   color: The color
         * @param[i/o]  rgb: The RGB int values.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        static void QColorToInt3(QColor color, int* rgb);

        /**
         * @brief       Convert QColor to OCC Quantity_Color.[static]
         * @param[in]   color: The QColor
         * @return      The Quantity_Color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        static Quantity_Color QColorToQuan(QColor color);

        /**
         * @brief       Double 3 to gp_Pnt.[static]
         * @param[in]   vec3: The double 3 array
         * @return      The gp_Pnt
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        static gp_Pnt Double3ToPnt(double* vec3);

        /**
         * @brief       Double 3 to gp_Vec.[static]
         * @param[in]   vec3: The double 3 array
         * @return      The gp_Vec
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        static gp_Vec Double3ToVec(double* vec3);

        /**
         * @brief       Double 3 to gp_Dir.[static]
         * @param[in]   vec3: The double 3 array
         * @return      The gp_Dir
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        static gp_Dir Double3ToDir(double* vec3);

        /**
         * @brief       Generate a color map( MeshVS_DataMapOfIntegerColor ) by the given values.[static]
         * @param[in]   values: The values[quote]
         * @param[in]   max: The max value
         * @param[in]   min: The min value
         * @return      The color map( MeshVS_DataMapOfIntegerColor )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        static NCollection_DataMap<int, Quantity_Color, NCollection_DefaultHasher<int>> DataToColorMap(QVector<double> & values, double max, double min);

        /**
         * @brief       Generate a fixed-length color map( MeshVS_DataMapOfIntegerColor ) by the given color and size.[static]
         * @param[in]   color: The color
         * @param[in]   size: The size of the color map
         * @return      The color map( MeshVS_DataMapOfIntegerColor )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        static NCollection_DataMap<int, Quantity_Color, NCollection_DefaultHasher<int>> QColorToColorMap(QColor color, int size);

        /**
         * @brief       Get the shape center.[static]
         * @param[in]   shape: The given shape( edge or face )[quote]
         * @param[in]   pt: The mid point of the shape[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-14
         */
        static void GetShapeCenter(TopoDS_Shape & shape, gp_Pnt & pt);

        /**
         * @brief       Get the shape direction.[static]
         * @param[in]   iShape: The given shape( edge or face )[quote]
         * @param[out]  oDir: The result direction[quote]
         * @param[out]  oPt: The mid point or the start point of the shape[quote]
         * @return      Wether the shape has the direction
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        static bool GetShapeDirection(TopoDS_Shape & iShape, gp_Vec & oDir, gp_Pnt & oPt);

        /**
         * @brief       Get the shape direction by the given position.[static]
         * @param[in]   iShape: The given shape( edge or face )[quote]
         * @param[out]  oDir: The result direction[quote]
         * @param[out]  oPt: The given point of the shape[quote]
         * @param[out]  oPpt: The point projected to the shape[quote]
         *          
         *              P.S.
         *              If the given position is not on the shape, this function 
         *              will project the point to the shape and get the U, V
         *              parameters to calculate the direction.( without any 
         *              distance limit )
         * 
         * @return      Wether the shape has the direction
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        static bool GetShapeDirectionByPos(TopoDS_Shape & iShape, gp_Vec & oDir, gp_Pnt & oPt, gp_Pnt & oPpt);

        /**
         * @brief       Create a cone and a cylinder with the given position and direction.[static]
         * @param[in]   pos: The position[quote]
         * @param[in]   dir: The direction[quote]
         * @param[in]   scaleFactor: The scale factor[default]
         * @return      The arrow shape( compound )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        static TopoDS_Shape CreateArrow(gp_Pnt & pos, gp_Dir & dir, double scaleFactor = 1.5);

        /**
         * @brief       Convert the screen position to the world position with the given face.[static]
         * @param[in]   sx: The screen x
         * @param[in]   sy: The screen y
         * @param[in]   face: The surface's face[quote]
         * @param[in]   view: The active view
         * @param[out]  ret: The result point[quote]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        static bool ScreenToWorldWithSurf(int sx, int sy, TopoDS_Shape & face, V3d_View* view, gp_Pnt & ret);

        /**
         * @brief       Convert the screen position to the world position with the given edge.[static]
         * @param[in]   sx: The screen x
         * @param[in]   sy: The screen y
         * @param[in]   edge: The curve's edge[quote]
         * @param[in]   view: The active view
         * @param[out]  ret: The result point[quote]
         * @return      Is OK
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        static bool ScreenToWorldWithCurve(int sx, int sy, TopoDS_Shape & edge, V3d_View* view, gp_Pnt & ret);

        /**
         * @brief       Get the edge mid point.[static]
         * @param[in]   edge: The given edge[quote]
         * @param[out]  ret: Wether the shape has mid point
         * @return      Has mid point
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        static bool GetEdgeMidPoint(TopoDS_Shape & edge, gp_Pnt & ret);

        /**
         * @brief       Get the face mid point.[static]
         * @param[in]   face: The given face[quote]
         * @param[out]  ret: Wether the shape has mid point
         * @return      Has mid point
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-26
         */
        static bool GetFaceMidPoint(TopoDS_Shape & face, gp_Pnt & ret);

    public:
        // For actors color.
        //@{
        /**
         * @brief       Default color list.[static]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-05
         */
        static QList<QColor> s_colorList;

        /**
         * @brief       The color for shape vertex.[static]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-11
         */
        static QColor s_vertexColor;

        /**
         * @brief       The color for shape edge.[static]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-11
         */
        static QColor s_edgeColor;

        /**
         * @brief       The color for shape face.[static]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-11
         */
        static QColor s_faceColor;

        /**
         * @brief       The color for mesh face.[static]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        static QColor s_meshFaceColor;

        /**
         * @brief       The color for datum.[static]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-10
         */
        static QColor s_datumColor;

        /**
         * @brief       The color for previewing model.[static]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-28
         */
        static QColor s_previewColor;

        /**
         * @brief       The color for marks.[static]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-04
         */
        static QColor s_markColor;

        /**
         * @brief       The color for the previewing shape with mark.[static]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        static QColor s_markPreviewShapeColor;
        //@}
        
        // Properties.
        //@{
        /**
         * @brief       The default transparency.[static]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-05
         */
        static double s_transpacency;

        /**
         * @brief       The line width for model objects.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-08
         */
        static double s_modelLineWidth;

        /**
         * @brief       The line width for datum objects.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-10
         */
        static double s_datumLineWidth;

        /**
         * @brief       The line width for tool objects.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-08
         */
        static double s_toolLineWidth;
        //@}
    };
}   // namespace Exchange

#endif // __FITKOCCCOMMONS_H__

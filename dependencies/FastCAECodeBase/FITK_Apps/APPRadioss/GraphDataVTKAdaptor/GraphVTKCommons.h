/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKCommons.h
 * @brief  形状图形工具类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-26
 *********************************************************************/
#ifndef GraphVTKCommons_H_
#define GraphVTKCommons_H_

#include "GraphDataVTKAdaptorAPI.h"
#include <QColor>
#include <cmath>

// QString to char*
#ifndef QStringToCharA
#define QStringToCharA(str) \
    str.toUtf8().data()
#endif // !QStringToCharA

namespace Exchange
{
    // 定义一个结构体来表示 RGBA 颜色
    struct RGBA {
        RGBA(double rr, double gg, double bb, double aa) {
            r = rr; g = gg; b = bb; a = aa;
        }
        RGBA() {};
        bool operator==(const RGBA& other) const {
            return (fabs(r-other.r)<0.001) && (fabs(g-other.g)<0.001)&& (fabs(b -other.b)<0.001)&& (fabs(a -other.a)<0.001);
        }
        double r, g, b, a = 1;
    };
    /**
     * @brief  形状图形工具类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-12-26
     */
    class GraphDataVTKAdaptorAPI GraphVTKCommons
    {
    public:
        /**
         * @brief  拾取模式
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-26
         */
        enum ShapePickMode
        {
            PickNone = 0,
            PickGeoVertex = 1,                 //拾取几何节点
            PickGeoEdge = 2,                   //拾取几何线
            PickGeoFace = 3,                   //拾取几何面
            PickGeoSolid = 4,                  //拾取几何体
            PickMeshVertex = 5,                //拾取网格节点
            PickMeshVertexSurf = 6,            //拾取网格表面节点
            PickMeshCell = 7,                  //拾取网格单元
            PickMeshCellSurf = 8,              //拾取网格表面单元
        };

        /**
         * @brief  形状数据类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-26
         */
        enum ShapeDataType
        {
            OtherShape = -1,
            ModelShape = 0,
            MeshShape = 1
        };
        /**
         * @brief  形状信息结构体
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-26
         */
        struct ShapeInfo
        {
            //形状数据类型
            ShapeDataType Type = OtherShape;

            //数据id
            int DataObjId = -1;
        };
    public:
        /**
         * @brief    获取VTK默认颜色
         * @return   QColor             颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-14
         */
        static QColor getVTKDefaultColor();
        /**
         * @brief    颜色线性差值颜色获取,默认蓝-绿-红
         * @param[i] num                              颜色段数
         * @param[i] start:起始rbga (底部颜色),end:结束rbga（顶部颜色）
         * @return   std::vector<RGBA>                差值后的颜色
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        static std::vector<RGBA> calculateColorTransition(int num, double* start = nullptr, double* end=nullptr);
    public:
        /**
         * @brief  几何点颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-14
         */
        static QColor _geoVertexColor;
        /**
         * @brief  几何线颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-14
         */
        static QColor _geoEdgeColor;
        /**
         * @brief  几何面颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-14
         */
        static QColor _geoFaceColor;
        /**
         * @brief  集中力载荷颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-02
         */
        static QColor _concentratedForceColor;
        static QColor s_highlightColor;
        /**
         * @brief  压力载荷颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-02
         */
        static QColor _pressureForceColor;
        /**
         * @brief  点高亮大小
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-14
         */
        static double _highlightPointSize;
        /**
         * @brief  线高亮大小
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-14
         */
        static double _highlightLineWidth;
        /**
         * @brief  拾取对象不透明度
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-14
         */
        static double _pickTransparency;
        /**
         * @brief  点ID存储数据名称
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-04-21
         */
        static QString _dataPointIDArratName;
        /**
         * @brief  单元ID存储数据名称
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-04-21
         */
        static QString _dataCellIDArratName;
        /**
         * @brief  表面提取过滤器原始单元id与提取后单元id映射数组名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-25
         */
        static QString _vtkSurFilterCellIdMapperName;
        /**
         * @brief  表面提取过滤器原始点id与提取后点id映射数组名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-25
         */
        static QString _vtkSurFilterPointIdMapperName;
        /**
         * @brief  几何渲染偏移参数
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-15
         */
        static double _geoGraphOffset;
        /**
         * @brief  2D网格(面网格)渲染偏移参数
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-15
         */
        static double _mesh2DGraphOffset;
        /**
         * @brief  刚性墙渲染偏移参数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-08
         */
        static double _rigidWallGraphOffset;
        /**
         * @brief  3D网格(面网格)渲染偏移参数
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-15
         */
        static double _mesh3DGraphOffset;

        static int  _unusedNodesPartId;
    };
}

#endif // __GraphVTKCommons_H__

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKCommons.h"

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkProperty.h>

namespace Exchange
{
    QString GraphVTKCommons::_dataPointIDArratName = "dataPointIDArrayName";
    QString GraphVTKCommons::_dataCellIDArratName = "dataCellIDArrayName";
    QString GraphVTKCommons::_vtkSurFilterCellIdMapperName = "vtkOriginalCellIds";
    QString GraphVTKCommons::_vtkSurFilterPointIdMapperName = "vtkOriginalPointIds";

    //color
    QColor GraphVTKCommons::_geoVertexColor = getVTKDefaultColor();
    QColor GraphVTKCommons::_geoEdgeColor = getVTKDefaultColor();
    QColor GraphVTKCommons::_geoFaceColor = getVTKDefaultColor();
    QColor GraphVTKCommons::_concentratedForceColor = Qt::yellow;
    QColor GraphVTKCommons::s_highlightColor = Qt::red;
    QColor GraphVTKCommons::_pressureForceColor = QColor(238, 130, 238);

    //size
    double GraphVTKCommons::_highlightPointSize = 10.;
    double GraphVTKCommons::_highlightLineWidth = 3.;

    //transparency
    double GraphVTKCommons::_pickTransparency = 0.6;

    //偏移参数
    double GraphVTKCommons::_geoGraphOffset = 2;
    double GraphVTKCommons::_mesh3DGraphOffset = 0;
    double GraphVTKCommons::_mesh2DGraphOffset = 3;
    double GraphVTKCommons::_rigidWallGraphOffset = 1;

    //未使用节点的PartId
    int GraphVTKCommons::_unusedNodesPartId = -10;

    QColor GraphVTKCommons::getVTKDefaultColor()
    {
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkProperty> property = vtkSmartPointer<vtkProperty>::New();
        actor->SetProperty(property);
        double VTKColor[3] = { 0,0,0 };
        property->GetColor(VTKColor);

        return QColor::fromRgbF(VTKColor[0], VTKColor[1], VTKColor[2]);
    }
    std::vector<RGBA> GraphVTKCommons::calculateColorTransition(int num, double* start, double* end)
    {
        std::vector<RGBA> colors;
        RGBA startColor{ start[0], start[1], start[2], start[3]};

        if (num < 1) {
            return colors;
        }
        if (num == 1) {
            colors.push_back(startColor);
            return colors;
        }

        RGBA endColor{ end[0], end[1], end[2], 1.0 };
        // 绿色
        RGBA green;
        green.r = 0;
        green.g = 1;
        green.b = 0;
        green.a = 1;

        //计算从起始颜色到结束颜色的线性插值
        auto interpolate = [](const RGBA& start, const RGBA& end, double t)->RGBA {
            RGBA result;
            result.r = start.r + t * (end.r - start.r);
            result.g = start.g + t * (end.g - start.g);
            result.b = start.b + t * (end.b - start.b);
            result.a = 1;
            return result;
            };
        //计算每个阶段的段数
        int halfNum = num / 2;
        //第一阶段：从起始颜色到中间颜色
        for (int i = 0; i < halfNum; ++i) {
            double t = static_cast<double>(i) / halfNum;
            colors.push_back(interpolate(startColor, green, t));
        }
        //第二阶段：从中间颜色到结束颜色
        for (int i = 0; i < num - halfNum; ++i) {
            double t = static_cast<double>(i) / (num - halfNum);
            colors.push_back(interpolate(green, endColor, t));
        }

        return colors;
    }
}
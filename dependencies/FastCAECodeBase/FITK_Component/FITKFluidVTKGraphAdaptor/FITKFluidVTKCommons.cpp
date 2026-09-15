/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKCommons.h"

namespace Exchange
{
    // Initialize color list.
    //@{
    QList<QColor> FITKFluidVTKCommons::s_colorList =
    {
        QColor(69, 139, 116), QColor(245, 245, 220), QColor(187, 58, 58),
        QColor(0, 94, 157), QColor(193, 205, 205), QColor(222, 184, 135),
        QColor(177, 198, 237), QColor(139, 136, 120), QColor(233, 150, 122),
        QColor(189, 183, 107), QColor(139, 10, 80), QColor(49, 58, 151),
        QColor(202, 217, 187), QColor(238, 201, 0), QColor(173, 216, 230),
        QColor(110, 123, 139), QColor(139, 71, 137), QColor(238, 232, 170),
        QColor(184, 206, 198), QColor(184, 184, 219), QColor(206, 164, 107),
        QColor(106, 90, 205), QColor(238, 233, 233), QColor(0, 51, 102)
    };
    //@}

    // Initialize colors.
    //@{
    QColor FITKFluidVTKCommons::s_geoVertexColor = Qt::blue;
    QColor FITKFluidVTKCommons::s_geoEdgeColor = Qt::black;
    QColor FITKFluidVTKCommons::s_geoFaceColor = QColor(230, 255, 0);
    QColor FITKFluidVTKCommons::s_meshFaceColor = Qt::gray; // Unused.
    QColor FITKFluidVTKCommons::s_blockMeshFaceColor = Qt::red; // Unused.
    QColor FITKFluidVTKCommons::s_regionMeshFaceColor = Qt::gray;
    QColor FITKFluidVTKCommons::s_regionMeshEdgeColor = Qt::yellow;
    QColor FITKFluidVTKCommons::s_matPointsColor = Qt::yellow;
    QColor FITKFluidVTKCommons::s_matPointMarkerColor = QColor(255, 130, 40);
    //@}

    // Initialize size.
    //@{
    double FITKFluidVTKCommons::s_highlightPointSize = 10.;
    double FITKFluidVTKCommons::s_highlightLineWidth = 3.;
    //@}

    // Initialize properties.
    //@{
    double FITKFluidVTKCommons::s_transparency = 0.6;
    //@}

    // Advanced.
    //@{
    double FITKFluidVTKCommons::s_polygonOffsetGeom = 0.;
    double FITKFluidVTKCommons::s_polygonOffsetMesh = 2.;
    //@}

    QColor FITKFluidVTKCommons::GetColorByIndex(int index)
    {
        // Default color list.
        return s_colorList[index % s_colorList.count()];
    }

    QColor FITKFluidVTKCommons::GetRandomColor()
    {
        //int index = rand() % s_colorList.count();
        //return s_colorList[index];

        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        return QColor(r, g, b);
    }

    void FITKFluidVTKCommons::QColorToDouble3(QColor color, double* rgbf)
    {
        rgbf[0] = color.redF();
        rgbf[1] = color.greenF();
        rgbf[2] = color.blueF();
    }

    void FITKFluidVTKCommons::QColorToInt3(QColor color, int* rgb)
    {
        rgb[0] = color.red();
        rgb[1] = color.green();
        rgb[2] = color.blue();
    }
}   // namespace Exchange
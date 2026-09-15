/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPlotWindowsInterface.h"
#include "FITKXYPlotWindow.h"
#include "FITKBarChartWindow.h"
#include "FITKPolarPlotWindow.h"
#include "FITKSpectrogramWindow.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

FITKLIBINFOREGISTER(FITKPlotWindow, FITKPLOTWINDOWVERSION);

namespace Plot {
    FITKPlotWindowsInterface::FITKPlotWindowsInterface()
    {
    }

    FITKPlotWindowsInterface::~FITKPlotWindowsInterface()
    {
    }

    QString FITKPlotWindowsInterface::getComponentName()
    {
        return "FITKPlotWindowsInterface";
    }

    QWidget * FITKPlotWindowsInterface::getWidget(const int indexPort)
    {
        enum PlotType
        {
            Normal,     //0普通坐标系      Plot::FITKXYPlotWindow
            BarChart,   //1柱状图          Plot::FITKBarChartWindow
            Polar,      //2极坐标          Plot::FITKPolarPlotWindow
            Spectrogram //3云图            Plot::FITKSpectrogramWindow
        };
        switch (indexPort) {
            case Normal:
                return new FITKXYPlotWindow();
            case BarChart:
                return new FITKBarChartWindow();
            case Polar:
                return new FITKPolarPlotWindow();
            case Spectrogram:
                return new FITKSpectrogramWindow();
            default:
                break;
        }
        return nullptr;
    }
}
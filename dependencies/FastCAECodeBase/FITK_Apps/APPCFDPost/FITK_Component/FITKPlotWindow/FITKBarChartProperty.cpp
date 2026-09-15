/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKBarChartProperty.h"
#include "FITKBarChartWindow.h"
#include <qwt_plot.h>

namespace Plot
{
    FITKBarChartProperty::FITKBarChartProperty(FITKBarChartWindow * Window):_window(Window)
    {
        _plotWidget = _window->getPlotWidget();
    }
    FITKBarChartProperty::~FITKBarChartProperty()
    {
    }

    void FITKBarChartProperty::setTitle(const QString & name)
    {
        if (!_plotWidget)return;
        if (name.isEmpty())return;
        _plotWidget->setTitle(name);
    }

    void FITKBarChartProperty::setAxisName(const QString & yName, const QString & xName)
    {
        if (!_plotWidget)return;
    //允许省略轴标题
        _plotWidget->setAxisTitle(QwtPlot::yLeft, yName);
        if(!xName.isEmpty())
        _plotWidget->setAxisTitle(QwtPlot::xBottom, xName);
    }


}
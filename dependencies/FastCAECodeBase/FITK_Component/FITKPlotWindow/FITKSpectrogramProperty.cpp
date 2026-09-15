/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKSpectrogramProperty.h"
#include "FITKSpectrogramWindow.h"
#include <qwt_plot.h>
#include <qwt_text.h>

namespace Plot {
    FITKSpectrogramProperty::FITKSpectrogramProperty(FITKSpectrogramWindow * Window):_window(Window)
    {
    }
    FITKSpectrogramProperty::~FITKSpectrogramProperty()
    {
    }
    void FITKSpectrogramProperty::setTitle(const QString & name)
    {
        if (!_window)return;
        if (name.isEmpty())return;
        _window->getPlotWidget()->setTitle(name);
    }
    void FITKSpectrogramProperty::setAxisScale(int axis, double min, double max)
    {
        if (!_window)return;
        _window->getPlotWidget()->setAxisScale((QwtPlot::Axis)axis, min, max, 0);
    }

    void FITKSpectrogramProperty::setPlotMagnifierAndPanner(bool isUse)
    {
        if (!_window)return;
        _window->_isUseMagnifierAndPanner = isUse;
        QWidget* canvas = _window->getPlotWidget()->canvas();
        if (!canvas)return;
        QObjectList filters = canvas->findChildren<QObject*>();
        for (QObject* filter : filters) {
            if(!isUse)
                canvas->removeEventFilter(filter);  // 移除画布上所有事件过滤器（包括QwtPlotZoomer安装的）
            else
                canvas->installEventFilter(filter);
        }

    }

    void FITKSpectrogramProperty::setAxisName(const QString & xName, const QString & yName)
    {
        if (!_window)return;
        _window->getPlotWidget()->setAxisTitle(QwtPlot::Axis::xBottom, xName);
        _window->getPlotWidget()->setAxisTitle(QwtPlot::Axis::yLeft, yName);
    }

    void FITKSpectrogramProperty::getAxisName(QString & xName, QString & yName)
    {
        if (!_window)return;
        xName = _window->getPlotWidget()->axisTitle(QwtPlot::Axis::xBottom).text();
        yName = _window->getPlotWidget()->axisTitle(QwtPlot::Axis::yLeft).text();
    }

    void FITKSpectrogramProperty::setAxisRatioFixed(bool isFixed)
    {
        if (!_window)return;
        _window->_isAxisRatioFixed = isFixed;
    }
}


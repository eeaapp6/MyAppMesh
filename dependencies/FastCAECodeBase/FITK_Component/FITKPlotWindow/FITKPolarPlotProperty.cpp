/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPolarPlotProperty.h"
#include "FITKPolarPlotWindow.h"
#include <qwt_polar_plot.h>

namespace Plot
{
    FITKPolarPlotProperty::FITKPolarPlotProperty(FITKPolarPlotWindow * Window):_window(Window)
    {
    }
    FITKPolarPlotProperty::~FITKPolarPlotProperty()
    {
    }
    void FITKPolarPlotProperty::setTitle(const QString & name)
    {
        if (!_window)return;
        if (name.isEmpty())return;
        _window->getPolarWidget()->setTitle(name);
    }
}
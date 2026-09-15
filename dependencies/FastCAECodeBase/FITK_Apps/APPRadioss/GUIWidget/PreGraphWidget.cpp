/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PreGraphWidget.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

namespace GUI
{
    PreGraphWidget::PreGraphWidget(QWidget* parent)
        :GraphArea(parent)
    {
        if(_renderWindow)
             _renderWindow->showScaleLegend(false);
    }

    GUI::GraphAreaType PreGraphWidget::getGraphAreaType()
    {
        return GATPre;
    }

}

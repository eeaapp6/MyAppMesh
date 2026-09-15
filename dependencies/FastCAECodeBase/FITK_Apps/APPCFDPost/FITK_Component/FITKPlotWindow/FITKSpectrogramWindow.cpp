/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKSpectrogramWindow.h"
#include"ui_FITKSpectrogramWindow.h"
#include "FITKSpectrogramProperty.h"
#include "FITKSpectrogramManager.h"
#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_widget.h>

namespace Plot {

    FITKSpectrogramWindow::FITKSpectrogramWindow(QWidget* parent/*= nullptr*/)
        :Core::FITKAbstractGraph2DWidget(parent)
    {
        _ui = new Ui::FITKSpectrogramWindow;
        _ui->setupUi(this);

        _plotWidget = new QwtPlot;
        _ui->gridLayout->addWidget(_plotWidget);

        _spectrogramProperty = new FITKSpectrogramProperty(this);
        _spectrogramManager = new FITKSpectrogramManager(this);

    //添加右轴
        _plotWidget->enableAxis(QwtPlot::yRight);

    //画布端点和轴对齐
        _plotWidget->plotLayout()->setAlignCanvasToScales(true);

    //鼠标中键平移画布
        QwtPlotPanner *panner = new QwtPlotPanner(_plotWidget->canvas());
        panner->setAxisEnabled(QwtPlot::yRight, false);
        panner->setMouseButton(Qt::MidButton);

        _plotWidget->setAutoReplot(false);

     //添加到图上的所有图元项在坐标图回收时，将自动析构
        _plotWidget->setAutoDelete(true);
    }

    FITKSpectrogramWindow::~FITKSpectrogramWindow()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
        if (_spectrogramManager) delete _spectrogramManager;
        _spectrogramManager = nullptr;
        if (_spectrogramProperty) delete _spectrogramProperty;
        _spectrogramProperty = nullptr;
    //最后析构Plot
        if (_plotWidget) delete _plotWidget;
        _plotWidget = nullptr;
    }

    QwtPlot * FITKSpectrogramWindow::getPlotWidget()
    {
        return _plotWidget;
    }

    FITKSpectrogramProperty * FITKSpectrogramWindow::getPlotProperty()
    {
        return _spectrogramProperty;
    }

    FITKSpectrogramManager * FITKSpectrogramWindow::getSpectrogramManager()
    {
        return _spectrogramManager;
    }

}


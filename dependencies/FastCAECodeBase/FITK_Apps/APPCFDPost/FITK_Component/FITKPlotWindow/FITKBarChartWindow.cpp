/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKBarChartWindow.h"
#include "ui_FITKBarChartWindow.h"
#include "FITKBarChartProperty.h"
#include "FITKBarChartDrawManager.h"
#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_column_symbol.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>

namespace Plot 
{
    FITKBarChartWindow::FITKBarChartWindow(QWidget*parent/*=nullptr*/) 
        :Core::FITKAbstractGraph2DWidget(parent)
    {
        _ui = new Ui::FITKBarChartWindow;
        _ui->setupUi(this);

        _plotWidget = new QwtPlot;
        _ui->gridLayout->addWidget(_plotWidget);

        _barChartProperty = new FITKBarChartProperty(this);
        _barChartManager = new FITKBarChartDrawManager(this);

    //背景色
        QwtPlotCanvas *canvas = new QwtPlotCanvas();
        canvas->setLineWidth(1);
        canvas->setBorderRadius(3);
        canvas->setPalette(QColor(245,240,235));
        _plotWidget->setCanvas(canvas);

    //刻度字体
        QFont font;
        font.setPointSize(12);
        font.setBold(false);  //是否加粗
        _plotWidget->setAxisFont(QwtPlot::Axis::yLeft, font);
        _plotWidget->setAxisFont(QwtPlot::Axis::xBottom, font); 

    //添加图例
        _plotWidget->insertLegend(new QwtLegend());   

        _plotWidget->setAutoReplot(false);
        _plotWidget->replot();

    //添加到QwtPlot上的所有图元项在QwtPlot回收时,将自动析构
        _plotWidget->setAutoDelete(true);
    }

    FITKBarChartWindow::~FITKBarChartWindow()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
        if (_barChartManager) delete _barChartManager;
        _barChartManager = nullptr;
        if (_barChartProperty) delete _barChartProperty;
        _barChartProperty = nullptr;   
    //最后析构QwtPlot
        if (_plotWidget) delete _plotWidget;
        _plotWidget = nullptr;
    }

    QwtPlot * FITKBarChartWindow::getPlotWidget()
    {
        return _plotWidget;
    }

    FITKBarChartProperty * FITKBarChartWindow::getPlotProperty()
    {
        return _barChartProperty;
    }

    FITKBarChartDrawManager * FITKBarChartWindow::getBarChartManager()
    {
        return _barChartManager;
    }

    void FITKBarChartWindow::updataCanvas()
    {
        _plotWidget->replot();
    }
}
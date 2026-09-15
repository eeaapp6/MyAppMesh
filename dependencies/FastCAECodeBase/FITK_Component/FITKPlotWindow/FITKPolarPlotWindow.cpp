/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPolarPlotWindow.h"
#include "ui_FITKPolarPlotWindow.h"
#include "FITKPolarPlotProperty.h"
#include "FITKPolarCurveDrawManager.h"
#include <qwt_polar_plot.h>
#include <qwt_polar_grid.h>
#include <qwt_polar_curve.h>
#include <qwt_legend.h>
#include <QToolBar>

namespace Plot
{
    FITKPolarPlotWindow::FITKPolarPlotWindow(QWidget * parent/*=nullptr*/)
        :Core::FITKAbstractGraph2DWidget(parent)
    {
        _ui = new Ui::FITKPolarPlotWindow;
        _ui->setupUi(this);

        _polarWidget = new QwtPolarPlot();
        _ui->gridLayout->addWidget(_polarWidget);

        _polarPlotProperty = new FITKPolarPlotProperty(this);
        _polarCurveDrawManager = new FITKPolarCurveDrawManager(this);

    // 设置背景颜色
        _polarWidget->setPlotBackground(QBrush(Qt::white, Qt::SolidPattern));

    // 添加图例，右侧
        _polarWidget->insertLegend(new QwtLegend(), QwtPolarPlot::RightLegend);

    // 设置网格效果属性
        QwtPolarGrid *grid = new QwtPolarGrid();
        grid->setFont(QFont("Microsoft YaHei UI Light", 10));
        grid->setPen(QPen(QColor(0,0,0), 1, Qt::SolidLine));
        grid->setMajorGridPen(QPen(QColor(0, 0, 0,120), 1, Qt::SolidLine));

    // 设置坐标数值样式
        grid->setAxisPen(QwtPolar::AxisTop, QPen(Qt::black));
        grid->setAxisPen(QwtPolar::AxisRight, QPen(Qt::black));

        grid->attach(_polarWidget);

    //添加工具栏
        _renderToolBar = new QToolBar();
        _renderToolBar->setContentsMargins(0, 0, 0, 0);
        _renderToolBar->setIconSize(QSize(40, 40));
        _renderToolBar->setOrientation(Qt::Vertical);
        _ui->gridLayout_2->addWidget(_renderToolBar, 0, 1, Qt::AlignRight | Qt::AlignTop);

    // 添加数值(不设置网格，看不到效果)
        _polarWidget->setScale(QwtPolar::Azimuth, 0, 360,30);  
        //_polarWidget->setScale(QwtPolar::Radius, 0, 40);     

        grid->showGrid(QwtPolar::AxisRight, true);
        grid->showAxis(QwtPolar::AxisLeft, false);
        grid->showAxis(QwtPolar::AxisTop, true);
        grid->showAxis(QwtPolar::AxisBottom, false);
 
    //添加到极坐标图上的所有图元项在坐标图回收时，将自动析构
        _polarWidget->setAutoDelete(true);
    }
    FITKPolarPlotWindow::~FITKPolarPlotWindow()
    {
        if (_ui) delete _ui;
            _ui = nullptr;
        if (_polarPlotProperty) delete _polarPlotProperty;
            _polarPlotProperty = nullptr;
        if (_polarCurveDrawManager) delete _polarCurveDrawManager;
            _polarCurveDrawManager = nullptr;
    //最后析构Plot
        if (_polarWidget) delete _polarWidget;
            _polarWidget = nullptr;
    }

    QwtPolarPlot * FITKPolarPlotWindow::getPolarWidget()
    {
        return _polarWidget;
    }

    FITKPolarPlotProperty * FITKPolarPlotWindow::getPlotProperty()
    {
        return _polarPlotProperty;
    }

    QToolBar * FITKPolarPlotWindow::getToolBar()
    {
        return _renderToolBar;
    }

    FITKPolarCurveDrawManager * FITKPolarPlotWindow::getPolarCurveManager()
    {
        return _polarCurveDrawManager;
    }

    void FITKPolarPlotWindow::updataCanvas()
    {
        _polarWidget->replot();
    }
}
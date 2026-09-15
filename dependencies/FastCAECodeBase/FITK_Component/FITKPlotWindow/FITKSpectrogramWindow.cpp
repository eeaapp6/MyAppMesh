/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKSpectrogramWindow.h"
#include "ui_FITKSpectrogramWindow.h"
#include "FITKSpectrogramProperty.h"
#include "FITKSpectrogramManager.h"
#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_widget.h>
#include <QResizeEvent>
#include <cmath>

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

     //重置尺寸
        _plotWidget->setMinimumSize(QSize(1, 1));
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

    void FITKSpectrogramWindow::resizeEvent(QResizeEvent * event)
    {
        QWidget::resizeEvent(event);
        if (!_isAxisRatioFixed )
            return; 

        double xMin = _plotWidget->axisScaleDiv(QwtAxis::XBottom).lowerBound();
        double xMax = _plotWidget->axisScaleDiv(QwtAxis::XBottom).upperBound();
        double yMin = _plotWidget->axisScaleDiv(QwtAxis::YLeft).lowerBound();
        double yMax = _plotWidget->axisScaleDiv(QwtAxis::YLeft).upperBound();

        //长/高  比例
        double ratio = fabs(xMax-xMin) / fabs(yMax - yMin);

        QSize plotSize = _plotWidget->size();//坐标轴及刻度标签的总尺寸
        QSize canvasSize = _plotWidget->canvas()->size();//坐标轴尺寸

        int y_LabelSize = plotSize.height() - canvasSize.height();//标签尺寸
        int x_LabelSize = plotSize.width() - canvasSize.width();

        //最外部窗口尺寸
        QSize newSize = event->size();
        // 假设x和y轴都能恰好铺满，此时的最大轴长：pixel_x，pixel_y
        int pixel_x = newSize.width() - x_LabelSize;
        int pixel_y = newSize.height() - y_LabelSize;

        //假设让y方向长度=pixel_y，计算对应比例的x方向长度
        int benchmark_x = (double)pixel_y * ratio;
        //若计算的x轴长度超出范围，则让x轴长度=pixel_x,计算对应比例的y轴长度
        if (benchmark_x > pixel_x)
        {
            pixel_y = (double)pixel_x * (1.0 / ratio);
        }
        else{
            pixel_x = benchmark_x;
        }
        //此时的坐标轴及刻度标签的总尺寸应为
        int plot_x = pixel_x + x_LabelSize;
        int plot_y = pixel_y + y_LabelSize;

        //如果x方向铺满
        if (abs(plot_x - newSize.width())<=1)
        {
            int blank = (newSize.height() - plot_y) / 2;
            int oldBlank = _ui->gridLayout->contentsMargins().top();
            if (abs(oldBlank - blank) <= 1)
            {
                return;
            }
            _ui->gridLayout->setContentsMargins(0, blank, 0, newSize.height() - plot_y - blank);
        }
        //如果y方向铺满
        else if (abs(plot_y - newSize.height())<=1)
        {
            int blank = (newSize.width() - plot_x) / 2;
            int oldBlank = _ui->gridLayout->contentsMargins().left();
            if (abs(oldBlank - blank) <= 1)
            {
                return;
            }
            _ui->gridLayout->setContentsMargins(blank, 0, newSize.width() - plot_x - blank, 0);
        }
        return;
    }

}


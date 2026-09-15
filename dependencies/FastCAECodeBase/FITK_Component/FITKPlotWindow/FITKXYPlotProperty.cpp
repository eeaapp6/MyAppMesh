/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKXYPlotProperty.h"
#include "FITKXYPlotWindow.h"
#include <qwt_plot.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_grid.h>
#include <QToolBar>
#include <qwt_legend.h>
#include <QPen>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_text.h>

namespace Plot 
{
    FITKXYPlotProperty::FITKXYPlotProperty(FITKXYPlotWindow * Window):_window(Window)
    {
        _plotWidget = _window->getPlotWidget();

    //绘制背景网格虚线,默认显示
        _grid = new QwtPlotGrid();
        QPen pen;
        QVector<qreal>dashes;
        qreal space = 3;
        dashes << 2 << space;
        pen.setDashPattern(dashes);
        _grid->setPen(pen);
        _grid->attach(_plotWidget);
    }

    FITKXYPlotProperty::~FITKXYPlotProperty()
    {
        if (_grid) {
            if (_grid->plot())_grid->detach();
            delete _grid;
            _grid = nullptr;
        }   
    }

    void FITKXYPlotProperty::setTitle(const QString & name)
    {
        if (name.isEmpty()||!_plotWidget)return;
        _plotWidget->setTitle(name);
    }

    void FITKXYPlotProperty::setLegendVisible(bool is)
    {
        if(_plotWidget)
            _plotWidget->legend()->setVisible(is);
    }

    void FITKXYPlotProperty::setLegendClickable(bool is)
    {
        if (!_plotWidget)return;
        QwtLegend* legend = dynamic_cast<QwtLegend*>(_plotWidget->legend());
        if (!legend)return;
        legend->setDefaultItemMode(is ? QwtLegendData::Mode::Clickable : QwtLegendData::Mode::ReadOnly);
    }

    void FITKXYPlotProperty::setAxisName(const QString & xName, const QString & yName)
    {
        if (!_plotWidget)return;
        _plotWidget->setAxisTitle(QwtPlot::Axis::xBottom, xName);
        _plotWidget->setAxisTitle(QwtPlot::Axis::yLeft, yName);
    }

    void FITKXYPlotProperty::getAxisName(QString & xName, QString & yName)
    {
        xName = _plotWidget->axisTitle(QwtPlot::Axis::xBottom).text();
        yName = _plotWidget->axisTitle(QwtPlot::Axis::yLeft).text();
    }

    void FITKXYPlotProperty::setAxisScaleEngine(int Axis,bool isLog)
    {
        if (Axis != 0 && Axis != 2) return;
        if (!_plotWidget)return;
        QwtScaleEngine* scaleEngine = new QwtLinearScaleEngine();
        if (isLog) {
            delete scaleEngine;
            scaleEngine = new QwtLogScaleEngine();
        }
        //在设置为指数轴的方向上，曲线最值将贴在画布边界
        scaleEngine->setAttribute(QwtScaleEngine::Floating, isLog);
        _plotWidget->setAxisScaleEngine((QwtPlot::Axis)Axis, scaleEngine);
    }

    bool FITKXYPlotProperty::isAxisScaleEngine(int axis)
    {
        if (!_plotWidget)return false;
        return _plotWidget->axisScaleEngine((QwtPlot::Axis)axis)->testAttribute(QwtScaleEngine::Floating);
    }

    void FITKXYPlotProperty::setAxisMaxMajor(int axis, int num)
    {
        if (!_plotWidget)return;
        _plotWidget->setAxisMaxMajor((QwtPlot::Axis)axis, num);
        //_plotWidget->setAxisAutoScale(true);//MaxMajor的优先级高于AutoScale
    }

    void FITKXYPlotProperty::setLegendPlace(int place)
    {
        if (!_plotWidget)return;
        if (!(_plotWidget->legend())) return;
        if (place != 1 && place != 2) return;

        _plotWidget->insertLegend(_plotWidget->legend(), (QwtPlot::LegendPosition)place);
        if (place == 1)
            dynamic_cast<QwtLegend*>(_plotWidget->legend())->setMaxColumns(1);
        else
            dynamic_cast<QwtLegend*>(_plotWidget->legend())->setMaxColumns(0);
    }

    void FITKXYPlotProperty::setTitleSize(int size)
    {
        if (!_plotWidget)return;
        if (size <=0) return;
        //设置标题字体大小
        QwtText title = _plotWidget->title();
        QFont font = title.font();
        font.setPointSize(size);
        title.setFont(font);
        _plotWidget->setTitle(title);
    }

    int FITKXYPlotProperty::getTitleSize() const
    {
        if (!_plotWidget)return 0;
        //获取标题字体大小
        QwtText title = _plotWidget->title();
        QFont font = title.font();
        return font.pointSize();
    }

    void FITKXYPlotProperty::setAxisTitleSize(int Xsize, int Ysize)
    {
        if (!_plotWidget)return;
        //设置坐标轴标题字体大小
        if (Xsize > 0) {
            QwtText xTitle = _plotWidget->axisTitle(QwtPlot::Axis::xBottom);
            QFont font = xTitle.font();
            font.setPointSize(Xsize);
            xTitle.setFont(font);
            _plotWidget->setAxisTitle(QwtPlot::Axis::xBottom, xTitle);
        }
        if (Ysize > 0) {
            QwtText yTitle = _plotWidget->axisTitle(QwtPlot::Axis::yLeft);
            QFont font = yTitle.font();
            font.setPointSize(Ysize);
            yTitle.setFont(font);
            _plotWidget->setAxisTitle(QwtPlot::Axis::yLeft, yTitle);
        }
    }

    int FITKXYPlotProperty::getAxisTitleSize(int axis) const
    {
        if (!_plotWidget)return 0;
        if (axis != 0 && axis != 2) return 0;
        //获取坐标轴标题字体大小
        QwtText title = _plotWidget->axisTitle((QwtPlot::Axis)axis);
        QFont font = title.font();
        return font.pointSize();
    }

    void FITKXYPlotProperty::disToolBar()
    {
        if (!_window)return;
        QList <QAction*> actions= _window->getToolBar()->actions();
        for (auto i: actions) {
            _window->getToolBar()->removeAction(i);
        }
    }

    void FITKXYPlotProperty::setPlotGrid(bool is)
    {
        if (!_plotWidget)return;
        if(is)
            _grid->attach(_plotWidget);
        else
            _grid->detach();
    }

    void FITKXYPlotProperty::setAxisScale(int axis, double min, double max)
    {
        if (!_plotWidget)return;
        if (axis == 0){
            _window->_userYmin = min;
            _window->_userYmax = max;
        }
        else if (axis == 2) {
            _window->_userXmin = min;
            _window->_userXmax = max;
        }
    }

    void FITKXYPlotProperty::getAxisScale(int axis, double &min, double &max)
    {
        if (!_plotWidget)return;
        const QwtScaleDiv &xDiv = _plotWidget->axisScaleDiv((QwtPlot::Axis)axis);
        min= xDiv.lowerBound();
        max= xDiv.upperBound();
    }

    void FITKXYPlotProperty::setPlotMagnifier(bool isUse)
    {
        if (!_window|| !_window->getPlotMagnifier())return;
        _window->getPlotMagnifier()->setEnabled(isUse);
    }

    void FITKXYPlotProperty::setPlotPanner(bool isUse)
    {
        if (!_window || !_window->getPlotPanner())return;
        _window->getPlotPanner()->setEnabled(isUse);
    }

    void FITKXYPlotProperty::setLogAxisOrderOfMagnitudeEndpoint(int type, bool isUse)
    {
        if (type == 0)
            isOrderOfMagnitude_y = isUse;
        else if(type==2)
            isOrderOfMagnitude_x = isUse;
    }

    bool FITKXYPlotProperty::getLogAxisOrderOfMagnitudeEndpoint(int type)
    {
        if (type == 0)
            return isOrderOfMagnitude_y;
        else if (type == 2)
            return isOrderOfMagnitude_x;
    }
}
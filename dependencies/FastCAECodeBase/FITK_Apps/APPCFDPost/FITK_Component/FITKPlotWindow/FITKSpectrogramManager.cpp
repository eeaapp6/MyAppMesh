/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKSpectrogramManager.h"
#include "FITKSpectrogramWindow.h"
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_plot.h>
#include <qwt_color_map.h>
#include <qwt_interval.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_plot_zoomer.h>
#include <QPen>
#include <QMouseEvent>
/**
 * @brief 云图数据类
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-31
 */
class CounterData :public QwtMatrixRasterData
{
public:
    CounterData(QVector<double> xcoord, QVector<double> ycoord, QVector <QVector<double>> zcoord)
        :_xcoord(xcoord), _ycoord(ycoord), _zcoord(zcoord)
    {
        double xx = _xcoord[_xcoord.size() - 1];
        double yy = _ycoord[_ycoord.size() - 1];
        double min = 1e64, max = -1e64;
        for (int i = 0; i < _zcoord.size(); i++) {
            for (int j = 0; j < _zcoord[0].size(); j++) {
                if (min > _zcoord[i][j])min = _zcoord[i][j];
                if (max < _zcoord[i][j])max = _zcoord[i][j];
            }
        }
        length = _xcoord.size() - 1;
        width = _ycoord.size() - 1;

        //设置云图范围
        setInterval(Qt::XAxis, QwtInterval(_xcoord[0], xx));
        setInterval(Qt::YAxis, QwtInterval(_ycoord[0], yy));
        setInterval(Qt::ZAxis, QwtInterval(min, max));
    }
    virtual double value(double x, double y) const
    {
        QList<QPointF> pointCoors;
        QList<double> pointValue;
        int xIndex = 0, yIndex = 0;

        //查找渲染像素区周围的四个插值点
            //获取横纵坐标索引
        for (; yIndex < width; ++yIndex) {
            if (y - _ycoord.value(yIndex) >= 0 && y - _ycoord.value(yIndex + 1) <= 0)
                break;
        }
        for (; xIndex < length; ++xIndex) {
            if (x - _xcoord.value(xIndex) >= 0 && x - _xcoord.value(xIndex + 1) <= 0)
                break;
        }
        //根据索引添加对应插值点和插值
        pointCoors.append(QPointF(_xcoord.value(xIndex), _ycoord.value(yIndex)));
        pointCoors.append(QPointF(_xcoord.value(xIndex + 1), _ycoord.value(yIndex)));
        pointCoors.append(QPointF(_xcoord.value(xIndex), _ycoord.value(yIndex + 1)));
        pointCoors.append(QPointF(_xcoord.value(xIndex + 1), _ycoord.value(yIndex + 1)));
        pointValue.append(_zcoord.value(yIndex).value(xIndex));
        pointValue.append(_zcoord.value(yIndex).value(xIndex + 1));
        pointValue.append(_zcoord.value(yIndex + 1).value(xIndex));
        pointValue.append(_zcoord.value(yIndex + 1).value(xIndex + 1));

        //像素插值计算
        const double dx = pointCoors[1].x() - pointCoors[0].x();
        const double v4 = pointValue[0] + (x - pointCoors[0].x())*(pointValue[1] - pointValue[0]) / dx;
        const double v5 = pointValue[2] + (x - pointCoors[2].x())*(pointValue[3] - pointValue[2]) / dx;
        //const double v55=((pointCoors[1].x()-x)*pointValue[2]+(x-pointCoors[0].x())*pointValue[3])/ dx;
        const double dy = pointCoors[2].y() - pointCoors[0].y();
        double v6 = v4 + (y - pointCoors[0].y())*(v5 - v4) / dy;
        return v6;
    }

private:
    QVector <double> _xcoord;
    QVector <double> _ycoord;
    QVector <QVector<double>> _zcoord;
    int length = 0, width = 0;
};

/**
 * @brief 云图颜色渐变类
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-31
 */
class LinearColorMapRGB : public QwtLinearColorMap
{
public:
    LinearColorMapRGB() :
        QwtLinearColorMap(Qt::darkCyan, Qt::red, QwtColorMap::RGB)
    {
        addColorStop(0.15, Qt::cyan);
        addColorStop(0.55, Qt::green);
        addColorStop(0.9, Qt::yellow);
    }
};

/**
 * @brief 放大镜
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-28
 */
class MyZoomer : public QwtPlotZoomer
{
public:
    MyZoomer(QWidget *canvas) :
        QwtPlotZoomer(canvas)
    {
        setTrackerMode(AlwaysOn);
    }

    //右键返回和ctrl+右键还原时的刷新界面函数
    bool eventFilter(QObject *object, QEvent *event) override
    {
        if (object != canvas())  return QwtPlotZoomer::eventFilter(object, event);
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (event->type() == QEvent::MouseButtonRelease && mouseEvent->button() == Qt::RightButton)
        {
            rescale();
        }
        return QwtPlotZoomer::eventFilter(object, event);
    }

    virtual QwtText trackerTextF(const QPointF &pos) const
    {
        QColor bg(Qt::white);
        bg.setAlpha(200);
        QwtText text = QwtPlotZoomer::trackerTextF(pos);
        text.setBackgroundBrush(QBrush(bg));
        return text;
    }
};

namespace Plot {
    FITKSpectrogramDrawPro::FITKSpectrogramDrawPro() :Core::FITKAbstractGraphObject(nullptr)
    {
    }
    FITKSpectrogramDrawPro::~FITKSpectrogramDrawPro()
    {
    }

    bool FITKSpectrogramDrawPro::setData(QVector<double>& x, QVector<double>& y, QVector<QVector<double>> &value)
    {
        if (value.isEmpty())return false;
        if (x.size() != value[0].size() && y.size() != value.size())return false;
        _x.clear();
        _y.clear();
        _value.clear();
        _x += x;
        _y += y;
        _value += value;
        return true;
    }


    FITKSpectrogramManager::FITKSpectrogramManager(FITKSpectrogramWindow *Window) :_window(Window)
    {
        _spectrogram = new QwtPlotSpectrogram();

        //根据系统使用特定的线程数
        _spectrogram->setRenderThreadCount(0);
        _spectrogram->setCachePolicy(QwtPlotRasterItem::PaintCache);

        //设置颜色划分等级
        QList<double> contourLevels;
        for (double level = 0.5; level < 10.0; level += 1.0)
            contourLevels += level;
        _spectrogram->setContourLevels(contourLevels);
    }

    FITKSpectrogramManager::~FITKSpectrogramManager()
    {
        if (_spectrogram) {
            if (_spectrogram->plot())_spectrogram->detach();
            delete _spectrogram;
            _spectrogram = nullptr;
        }
    }

    void FITKSpectrogramManager::appendGraphObj(Core::FITKAbstractGraphObject * item)
    {
        FITKSpectrogramDrawPro* spectrogramData = dynamic_cast<Plot::FITKSpectrogramDrawPro*>(item);
        if (!spectrogramData)return;
        if (spectrogramData->_value.isEmpty())return;

        //设置数据并关联到图表
        _spectrogram->setData(new CounterData(spectrogramData->_x, spectrogramData->_y, spectrogramData->_value));
        _spectrogram->attach(_window->getPlotWidget());

        //设置yRight轴作颜色对比
        const QwtInterval zInterval = _spectrogram->data()->interval(Qt::ZAxis);
        QwtScaleWidget *rightAxis = _window->getPlotWidget()->axisWidget(QwtPlot::yRight);
        //rightAxis->setTitle("Intensity");
        rightAxis->setColorBarEnabled(true);
        _window->getPlotWidget()->setAxisScale(QwtPlot::yRight, zInterval.minValue(), zInterval.maxValue());
        _spectrogram->setColorMap(new LinearColorMapRGB());
        rightAxis->setColorMap(zInterval, new LinearColorMapRGB());

        QWidget* canvas = _window->getPlotWidget()->canvas();
        if (!canvas)return;

        // 左键拖动放大，右键返回，Ctrl+右键 还原
        QwtPlotZoomer* zoomer = new MyZoomer(canvas);
        zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
            Qt::RightButton, Qt::ControlModifier);
        zoomer->setMousePattern(QwtEventPattern::MouseSelect3,
            Qt::RightButton);
        zoomer->setRubberBandPen(QColor(Qt::darkBlue));
        zoomer->setTrackerPen(QColor(Qt::darkBlue));

        // 移除画布上所有事件过滤器
        if (!_window->_isUseMagnifierAndPanner)
        {
            QObjectList filters = canvas->findChildren<QObject*>();
            for (QObject* filter : filters) {
                canvas->removeEventFilter(filter);
            }
        }

        // Avoid jumping when labels with more/less digits
        // appear/disappear when scrolling vertically
        const QFontMetrics fm(_window->getPlotWidget()->axisWidget(QwtPlot::yLeft)->font());
        QwtScaleDraw *sd = _window->getPlotWidget()->axisScaleDraw(QwtPlot::yLeft);
        sd->setMinimumExtent(fm.width("100.00"));

    }
}


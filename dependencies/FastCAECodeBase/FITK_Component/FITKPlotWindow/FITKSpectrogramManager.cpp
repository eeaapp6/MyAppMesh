/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
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
#include <iterator>
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
        QwtLinearColorMap(Qt::blue, Qt::red, QwtColorMap::RGB)
    {
        addColorStop(0.1, QColor(0, 104, 255));
        addColorStop(0.25, QColor(0, 247, 255));
        addColorStop(0.5, Qt::green);
        addColorStop(0.75, Qt::yellow);
        addColorStop(0.9, QColor(255, 105, 0));
    }
    LinearColorMapRGB(QColor start, QColor end, QMap<double, QColor> colorStops) :
        QwtLinearColorMap(start, end, QwtColorMap::RGB)
    {
        for (auto it = colorStops.begin(); it != colorStops.end(); ++it) {
            if(it.key() < 1e-5 || it.key() > 0.99999) continue; // Ensure the key is within [0, 1]
            addColorStop(it.key(), it.value());
        }
    }
    LinearColorMapRGB(QMap<double, QColor> colorStops) :
        QwtLinearColorMap()
    {
        if (colorStops.isEmpty() || colorStops.size() < 2) return;
        setColorInterval(colorStops.first(), colorStops.last());
        if(colorStops.size() < 3) return;
        for (auto it = std::next(colorStops.begin()); it != std::prev(colorStops.end()); ++it) {
            addColorStop(it.key(), it.value());
        }
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

    void FITKSpectrogramDrawPro::setColorRange(double min, double max)
    {
        if (min >= max)
            return;
        _useCustomColorRange = true;
        _customColorMin = min;
        _customColorMax = max;
    }

    void FITKSpectrogramDrawPro::autoColorRange()
    {
        _useCustomColorRange = false;
    }

    void FITKSpectrogramDrawPro::setColorMap(const QMap<double, QColor>& colorStops)
    {
        _customColorStops = colorStops;
    }

    void FITKSpectrogramDrawPro::resetColorMap()
    {
        _customColorStops.clear();
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
        CounterData* rasterData = new CounterData(spectrogramData->_x, spectrogramData->_y, spectrogramData->_value);
        QwtInterval zInterval = rasterData->interval(Qt::ZAxis);
        if (spectrogramData->_useCustomColorRange) {
            zInterval = QwtInterval(spectrogramData->_customColorMin, spectrogramData->_customColorMax);
            rasterData->setInterval(Qt::ZAxis, zInterval);
        }
        _spectrogram->setData(rasterData);

        QwtPlot* plot = _window->getPlotWidget();
        if (!plot) return;
        _spectrogram->attach(plot);

        //设置yRight轴作颜色对比（从数据对象读取颜色设置）
        QwtScaleWidget* rightAxis = plot->axisWidget(QwtPlot::yRight);
        rightAxis->setColorBarEnabled(true);
        plot->setAxisScale(QwtPlot::yRight, zInterval.minValue(), zInterval.maxValue());
    

        bool isColorMap = !spectrogramData->_customColorStops.isEmpty();
        if (isColorMap) {
            _spectrogram->setColorMap(new LinearColorMapRGB(spectrogramData->_customColorStops));
            rightAxis->setColorMap(zInterval, new LinearColorMapRGB(spectrogramData->_customColorStops));
        }
        else {
            _spectrogram->setColorMap(new LinearColorMapRGB());
            rightAxis->setColorMap(zInterval, new LinearColorMapRGB());
        }

        QWidget* canvas = plot->canvas();
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
        const QFontMetrics fm(plot->axisWidget(QwtPlot::yLeft)->font());
        QwtScaleDraw *sd = plot->axisScaleDraw(QwtPlot::yLeft);
        sd->setMinimumExtent(fm.width("100.00"));

        //初始化轴比例
        if (_window->_isAxisRatioFixed)
        {
            QSize oldSize = _window->size();
            _window->resize(oldSize.width() - 1, oldSize.height() - 1);
            _window->resize(oldSize);
        }

        QList<Core::FITKAbstractGraphObject *>::iterator it;
        for (it = _graphObjList.begin(); it != _graphObjList.end(); ++it) {
            if (*it == item) return;
        }
        _graphObjList.append(item);
        connect(item, SIGNAL(graphObjDestroyedSig(FITKAbstractGraphObject*)),
            this, SLOT(removeGraphObjSlot(FITKAbstractGraphObject*)));
    }

    void FITKSpectrogramManager::clear()
    {
        // 清理管理的图形对象（基类 clear 仅清空列表，不负责 delete）
        for (auto* obj : _graphObjList) {
            disconnect(obj, nullptr, this, nullptr);
            delete obj;
        }
        this->FITKGraphObjManager::clear();

        _spectrogram->setData(nullptr);
        _spectrogram->detach();
    }
}


/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKBarChartDrawManager.h"
#include "FITKBarChartWindow.h"
#include <QRandomGenerator>
#include <QVector>
#include <qwt_text.h>
#include <qwt_plot_multi_barchart.h>
#include <qwt_plot_barchart.h>
#include <qwt_column_symbol.h>
#include <qwt_plot.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_div.h>
#include <qwt_scale_widget.h>

/**
 * @brief 自定义坐标轴
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-23
 */
class DistroScaleDraw : public QwtScaleDraw
{
public:
    DistroScaleDraw() 
    {
        enableComponent(QwtScaleDraw::Backbone, false);
        enableComponent(QwtScaleDraw::Ticks, false);
    }
    DistroScaleDraw(const QList<QString> &labels,int type=0)
    {
        setTickLength(QwtScaleDiv::MinorTick, 0);
        setTickLength(QwtScaleDiv::MediumTick, 0);
        setTickLength(QwtScaleDiv::MajorTick, 5);
        enableComponent(QwtScaleDraw::Backbone, false);

        for (int i = 0; i < labels.size(); i++) {
            _labelMap.insert(i, labels[i]);
        }
        if (type != 0) {
            setLabelAlignment(Qt::AlignHCenter | Qt::AlignBottom);
            //enableComponent(QwtScaleDraw::Ticks, false);
        }else {
            setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            setLabelRotation(-45.0);
        }     
    }
    virtual QwtText label(double value) const
    {
        QwtText lbl{};
        if (_labelMap.isEmpty()) return lbl;
        
        const int index = qRound(value);       
        if (index >= 0 && index < _labelMap.size())
        {
            //if (index - value<1e-2 && index - value>-1e-2)
                lbl = _labelMap[index];
        }
        return lbl;
    }

private:
    /**
     * @brief 映射，将坐标轴对应的刻度值换成相应字符串
     * @author wangning (2185896382@qq.com)
     * @date   2024-10-20
     */
    QMap< double, QString > _labelMap{};
};

/**
 * @brief 自定义条型柱状图
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-25
 */
class DistroPlotBarChart: public QwtPlotBarChart
{
public:
    DistroPlotBarChart(QList<QString> distros, QList<QColor> colors) 
        :QwtPlotBarChart(),_colors(colors), _distros(distros)
    {
        setLegendMode(QwtPlotBarChart::LegendBarTitles);
        itemChanged();
    }

    virtual QwtColumnSymbol *specialSymbol(int index, const QPointF&) const
    {
        if (index >= 0 && index < _colors.size()) {
            QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
            symbol->setLineWidth(1);
            symbol->setFrameStyle(QwtColumnSymbol::Raised);
            symbol->setPalette(_colors[index]);
            return symbol;
        }
        return nullptr;
    }

    virtual QwtText barTitle(int sampleIndex) const
    {
        QwtText title;
        if (sampleIndex >= 0 && sampleIndex < _distros.size())
            title = _distros[sampleIndex];
        return title;
    }

private:
    QList<QColor> _colors{};
    QList<QString> _distros{};
};

namespace Plot 
{

    FITKBarChartDrawProp::FITKBarChartDrawProp() :Core::FITKAbstractGraphObject(nullptr)
    {
    }
    FITKBarChartDrawProp::~FITKBarChartDrawProp()
    {
    }

    int FITKBarChartDrawProp::setBarNameGroupNum(QList<QString>& barNames, int groupNum)
    {
        if (barNames.isEmpty()|| groupNum<=0) return -1;
        _barNames.clear();
        _barNames.append(barNames);
        _groupNum = groupNum;
        _dataSize = _barNames.size()*_groupNum;
        return _dataSize;
    }

    void FITKBarChartDrawProp::setColor(QList<QColor> &colors)
    {
        if (colors.isEmpty())return;
        _colors.clear();
        _colors.append(colors);
    }

    bool FITKBarChartDrawProp::setData(QVector<double>& data)
    {
        if (data.size() != _dataSize) return false;
        //_data->reserve(_dataSize);
        _data.clear();
        _data.append(data);
        return true;
    }

    void FITKBarChartDrawProp::setXLabel(QList<QString>& xLabel)
    {
        if (xLabel.isEmpty()) return;
        _xLabel.clear();
        _xLabel.append( xLabel);
    }



    FITKBarChartDrawManager::FITKBarChartDrawManager(FITKBarChartWindow *Plot):_window(Plot)
    {
    }

    FITKBarChartDrawManager::~FITKBarChartDrawManager()
    {
        if (_barChartItem) {
            if (_barChartItem->plot())_barChartItem->detach();
            delete _barChartItem;
            _barChartItem = nullptr;
        }
        if (_barChartUnit) {
            if (_barChartUnit->plot())_barChartUnit->detach();
            delete _barChartUnit;
            _barChartUnit = nullptr;
        }   
    }

    void FITKBarChartDrawManager::appendGraphObj(Core::FITKAbstractGraphObject * item)
    {
        FITKBarChartDrawProp* barChart = dynamic_cast<Plot::FITKBarChartDrawProp*>(item);
        if (!barChart)return;
        if (barChart->_barNames.isEmpty()||barChart->_colors.size()< barChart->_barNames.size())return;
        if (barChart->_data.isEmpty())return;
        bool is = true;

    //根据_groupNum判断柱状图是条组型还是条型
        if (barChart->_groupNum != 1) {
            if(!_barChartItem)
                _barChartItem = new QwtPlotMultiBarChart();
            is = appendMultiBarChart(barChart);
        }else {
            if (!_barChartUnit)
                _barChartUnit = new DistroPlotBarChart(barChart->_barNames, barChart->_colors);
            is = appendUnitBarChart(barChart);
        }
    //添加到显示对象列表
        if (!is)return;
        _graphObjList.append(item);
        connect(item, SIGNAL(graphObjDestroyedSig(FITKAbstractGraphObject*)),
            this, SLOT(removeGraphObjSlot(FITKAbstractGraphObject*)));
    }

    void FITKBarChartDrawManager::removeGraphObj()
    {
        if(_barChartItem)
            _barChartItem->detach();
        if (_barChartUnit)
            _barChartUnit->detach();
        _window->getPlotWidget()->replot();
    }

    void FITKBarChartDrawManager::clear()
    {
        for (auto obj : _graphObjList)
        {
            Core::FITKAbstractGraphObject * curve = obj;
            if (curve) delete curve;
        }
        _graphObjList.clear();
    }

    bool FITKBarChartDrawManager::appendMultiBarChart(FITKBarChartDrawProp * barChart)
    {
        if(!_barChartItem) return false;

    //添加柱形条名字
        int barNum = barChart->_barNames.size();
        QList<QwtText> titles{};
        for (int i = 0; i < barNum; i++)
        {
            titles += (barChart->_barNames)[i];
        }
        _barChartItem->setBarTitles(titles);

    //柱形条上色
        for (int i = 0; i < barNum; i++)
        {
            QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
            symbol->setLineWidth(1);
            symbol->setFrameStyle(QwtColumnSymbol::Raised);
            symbol->setPalette(QPalette(barChart->_colors[i]));
            _barChartItem->setSymbol(i, symbol);
        }
    //添加数据
        QVector< QVector<double> > series;
        for (int i = 0; i < barChart->_groupNum; i++)
        {
            QVector<double> values;
            for (int j = 0; j < barNum; j++)
                values += barChart->_data[barNum*i + j];
            series += values;
        }
        _barChartItem->setSamples(series);

    //设置图例尺寸
        _barChartItem->setLegendIconSize(QSize(10, 14));

    //关联到坐标图
        _barChartItem->attach(_window->getPlotWidget());

    //柱状图布局
        _barChartItem->setLayoutPolicy(QwtPlotMultiBarChart::AutoAdjustSamples);
        _barChartItem->setSpacing(400 / (barChart->_groupNum + 2 + barNum));
        _barChartItem->setMargin(400 / (barChart->_groupNum + 1 + barNum));

        _barChartItem->setOrientation(Qt::Vertical);
        QwtPlot::Axis axis1, axis2;
        axis1 = QwtPlot::xBottom;
        axis2 = QwtPlot::yLeft;

        _window->getPlotWidget()->setAxisScale(axis1, 0, barChart->_groupNum - 1, 1.0);

    //判断自定义x轴字符串是否小于条组的个数
        if (barChart->_xLabel.isEmpty() || barChart->_xLabel.size() < barChart->_groupNum) {
            QList<QString>list;
            for (int i = 0; i < barChart->_groupNum; i++)
               list.append(QString("%1").arg(i+1));
            _window->getPlotWidget()->setAxisScaleDraw(axis1, new DistroScaleDraw(list,1));
        }else {
            _window->getPlotWidget()->setAxisScaleDraw(axis1, new DistroScaleDraw(barChart->_xLabel,1));
        }
        _window->getPlotWidget()->setAxisAutoScale(axis2);

        _window->getPlotWidget()->plotLayout()->setAlignCanvasToScale(axis1, true);
        _window->getPlotWidget()->plotLayout()->setAlignCanvasToScale(axis2, false);

    //画布与轴的间隔
        //_window->getPlotWidget()->plotLayout()->setCanvasMargin(1, axis1);
    //刻度值与轴的间隔
        //_window->getPlotWidget()->axisWidget(axis1)->setMargin(10);

        _window->getPlotWidget()->replot();
        return true;
    }


    bool FITKBarChartDrawManager::appendUnitBarChart(FITKBarChartDrawProp * barChart)
    {
        if (!_barChartUnit) return false;

    //添加数据
        _barChartUnit->setSamples(barChart->_data);

    //关联到坐标图
        _barChartUnit->attach(_window->getPlotWidget());

    //设置图例尺寸
        _barChartUnit->setLegendIconSize(QSize(10, 14));

    //设置布局
        _barChartUnit->setLayoutPolicy(QwtPlotMultiBarChart::AutoAdjustSamples);
        _barChartUnit->setLayoutHint(6.0); 
        _barChartUnit->setSpacing(400 / (barChart->_barNames.size()+3) );
        _barChartUnit->setMargin(360 / (barChart->_barNames.size()+1) );

        _barChartUnit->setOrientation(Qt::Vertical);
        QwtPlot::Axis axis1, axis2;
        axis1 = QwtPlot::xBottom;
        axis2 = QwtPlot::yLeft;
    
        if (barChart->_barNames.size() != 1) {
            _window->getPlotWidget()->setAxisScale(axis1, 0, barChart->_barNames.size() - 1, 1.0);
        }else{ 
            _window->getPlotWidget()->setAxisScale(axis1, -0.9,  0.9, 1.0);
            _barChartUnit->setMargin(1);
        }
        _window->getPlotWidget()->setAxisScaleDraw(axis1, new DistroScaleDraw(barChart->_barNames));
        _window->getPlotWidget()->setAxisAutoScale(axis2);

        _window->getPlotWidget()->plotLayout()->setAlignCanvasToScale(axis1, true);
        _window->getPlotWidget()->plotLayout()->setAlignCanvasToScale(axis2, false);

        _window->getPlotWidget()->replot();
        return true;
    }
}


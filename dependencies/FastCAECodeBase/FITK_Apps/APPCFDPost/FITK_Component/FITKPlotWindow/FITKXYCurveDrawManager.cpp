/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKXYCurveDrawManager.h"
#include "FITKXYPlotWindow.h"
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_symbol.h>
#include <qwt_text.h>

namespace Plot 
{
    FITKXYCurveDrawProp::FITKXYCurveDrawProp():Core::FITKAbstractGraphObject(nullptr)
    {
        _curve = new QwtPlotCurve();
    }

    FITKXYCurveDrawProp::~FITKXYCurveDrawProp()
    {
        if (_curve) {
            if (_curve->plot())_curve->detach();
            delete _curve;
            _curve = nullptr;
        }
    }

    void FITKXYCurveDrawProp::setColor(QColor color)
    {
        if (!_curve)return;
    //默认线宽为1
        _curve->setPen(QPen(color, 1));
        _isSetColor = true;
    }

    void FITKXYCurveDrawProp::setLineType(int style)
    {
        if (!_curve)return;
        if (style<0||style>5)return;
        QPen pen = _curve->pen();
        pen.setStyle((Qt::PenStyle)style);
        _curve->setPen(pen);
    }

    void FITKXYCurveDrawProp::setSymbol(int style)
    {
        if (!_curve)return;
        if (style < -1 || style>4) return;
        QwtSymbol* sym = new QwtSymbol((QwtSymbol::Style)style);
        QPen pen = _curve->pen();
        QColor color = pen.color();
        int width = pen.width();
        QPen p(color, width+4);
        sym->setPen(p);
        _curve->setSymbol(sym);
    }

    void FITKXYCurveDrawProp::setVisible(bool is)
    {
        if (!_curve)return;
        _curve->setVisible(is);
    }

    bool FITKXYCurveDrawProp::getVisible()
    {
        if (!_curve)return false;
        return _curve->isVisible();
    }

    bool FITKXYCurveDrawProp::setData(const QString &name, QVector<double> &xData, QVector<double> &yData)
    {
        if (!_curve)return false;
        if (xData.size() == 0 || xData.size() != yData.size())return false;
    //曲线名
        _curve->setTitle(name);
    //抗锯齿
        _curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    //填坐标
        _curve->setSamples(xData,yData);
        return true;
    }

    QString FITKXYCurveDrawProp::getCurveTitle()
    {
        return _curve->title().text();
    }
    QList<QPointF> FITKXYCurveDrawProp::getPointList()
    {
        int num = _curve->dataSize();
        QList<QPointF> pList{};
        if (num < 1)
            return pList;
        for (int i = 0; i < num; ++i)
        {
            pList.append(_curve->sample(i));
        }
        return pList;
    }


/*******************************************************************************************************************/


    FITKXYCurveDrawManager::FITKXYCurveDrawManager(FITKXYPlotWindow* Plot):_window(Plot)
    {
    }

    FITKXYCurveDrawManager::~FITKXYCurveDrawManager()
    {
    }

    void FITKXYCurveDrawManager::appendGraphObj(Core::FITKAbstractGraphObject * item)
    {
        FITKXYCurveDrawProp* curve = dynamic_cast<Plot::FITKXYCurveDrawProp*>(item);
        if (!curve)return;
        if (curve->_curve->dataSize() == 0)return;

    //添加曲线到图表窗口
        curve->_curve->attach(_window->getPlotWidget());

        QList<Core::FITKAbstractGraphObject *>::iterator it;
        for (it = _graphObjList.begin(); it != _graphObjList.end(); ++it) {
            if (*it == item) return;
        }
        _graphObjList.append(item);

    //设置默认颜色
        int colorIndex = _graphObjList.size()%_colors.size();
        if (!curve->_isSetColor && _graphObjList.size()>0)
            curve->setColor(_colors[colorIndex]);

        connect(item, SIGNAL(graphObjDestroyedSig(FITKAbstractGraphObject*)),
            this, SLOT(removeGraphObjSlot(FITKAbstractGraphObject*)));
    }

    void FITKXYCurveDrawManager::removeGraphObj(Core::FITKAbstractGraphObject * item)
    {
        FITKXYCurveDrawProp* curve = dynamic_cast<Plot::FITKXYCurveDrawProp*>(item);
        if (!curve)return;

    //解除关联
        curve->_curve->detach();
        _window->getPlotWidget()->replot();

        if (!_graphObjList.contains(item)) return;
        _graphObjList.removeOne(item);
        disconnect(item, SIGNAL(graphObjDestroyedSig(FITKAbstractGraphObject*)),
            this, SLOT(removeGraphObjSlot(FITKAbstractGraphObject*)));
    }

    void FITKXYCurveDrawManager::clear()
    {
        for (auto obj: _graphObjList) 
        {
            Core::FITKAbstractGraphObject * curve= obj;
            if (curve) delete curve;
        }
        _graphObjList.clear();
    }
}
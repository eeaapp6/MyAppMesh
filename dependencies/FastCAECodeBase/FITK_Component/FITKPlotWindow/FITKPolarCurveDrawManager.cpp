/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPolarCurveDrawManager.h"
#include "FITKPolarPlotWindow.h"
#include <qwt_polar_curve.h>
#include <qwt_polar_plot.h>
#include <QColor>
#include <qwt_text.h>

/**
 * @brief 极坐标类
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-25
 */
class PolarData :public QwtSeriesData<QwtPointPolar>
{
public:
    PolarData(QVector<double> &azs, QVector<double> &rads)
    {
        _azs.append(azs);
        _rads.append(rads);
    }
private:
    virtual size_t size() const
    {
        return _azs.size();
    }

    virtual QwtPointPolar sample(size_t i) const
    {
        const double rad = _rads.at(i);
        const double az = _azs.at(i);

        return QwtPointPolar(az, rad);
    }

    virtual QRectF boundingRect() const
    {
        if (cachedBoundingRect.width() < 0.0)
            cachedBoundingRect = qwtBoundingRect(*this);
        return cachedBoundingRect;
    }
private:
    /**
     * @brief 极径，角度
     * @author wangning (2185896382@qq.com)
     * @date   2024-10-25
     */
    QVector<double> _azs{}, _rads{};
};


namespace Plot
{
    FITKPolarCurveDrawProp::FITKPolarCurveDrawProp() :Core::FITKAbstractGraphObject(nullptr)
    {
        _polarCurve = new QwtPolarCurve();
    }

    FITKPolarCurveDrawProp::~FITKPolarCurveDrawProp()
    {
        if (_polarCurve) {
            if(_polarCurve->plot())_polarCurve->detach();
            delete _polarCurve;
            _polarCurve = nullptr;
        }
    }

    void FITKPolarCurveDrawProp::setColor(QColor color)
    {
        if (!_polarCurve)return;
        _polarCurve->setPen(QPen(color, 2));
    }

    bool FITKPolarCurveDrawProp::setData(const QString & name, QVector<double>& azs, QVector<double>& rads)
    {
        if (!_polarCurve)return false;
        if(name.isEmpty()|| azs.isEmpty()|| azs.size()!= rads.size())  return false;
        _polarCurve->setTitle(name);
        PolarData *data = new PolarData(azs, rads);
        _polarCurve->setData(data);

    //极径最大值
        //auto max = std::max_element(azs.begin(), azs.end());
        //polarPlotMax = *max;

        return true;
    }

    QString FITKPolarCurveDrawProp::getCurveTitle()
    {
        return _polarCurve->title().text();
    }

    QList<QPointF> FITKPolarCurveDrawProp::getPointList()
    {
        int num = _polarCurve->dataSize();
        QList<QPointF> pList{};
        if (num < 1)
            return pList;
        for (int i = 0; i < num; ++i)
        {
            const QwtPointPolar& pp = _polarCurve->sample(i);
            pList.append(QPointF(pp.radius(), pp.azimuth()));
        }
        return pList;
    }




    FITKPolarCurveDrawManager::FITKPolarCurveDrawManager(FITKPolarPlotWindow * Window):_window(Window)
    {
    }
    FITKPolarCurveDrawManager::~FITKPolarCurveDrawManager()
    {
        this->clear();
    }

    void FITKPolarCurveDrawManager::appendGraphObj(Core::FITKAbstractGraphObject * item)
    {
        FITKPolarCurveDrawProp* curve = dynamic_cast<Plot::FITKPolarCurveDrawProp*>(item);
        if (!curve)return;
        if (curve->_polarCurve->dataSize() == 0)return;

    //添加极坐标曲线到图表窗口
        curve->_polarCurve->attach(_window->getPolarWidget());

        _window->updataCanvas();

        QList<Core::FITKAbstractGraphObject *>::iterator it;
        for (it = _graphObjList.begin(); it != _graphObjList.end(); ++it) {
            if (*it == item) return;
        }
        _graphObjList.append(item);
        connect(item, SIGNAL(graphObjDestroyedSig(FITKAbstractGraphObject*)),
            this, SLOT(removeGraphObjSlot(FITKAbstractGraphObject*)));
    }

    void FITKPolarCurveDrawManager::removeGraphObj(Core::FITKAbstractGraphObject * item)
    {
        FITKPolarCurveDrawProp* curve = dynamic_cast<Plot::FITKPolarCurveDrawProp*>(item);
        if (!curve)return;

        //解除关联
        curve->_polarCurve->detach();
        _window->getPolarWidget()->replot();

        if (!_graphObjList.contains(item)) return;
        _graphObjList.removeOne(item);
        disconnect(item, SIGNAL(graphObjDestroyedSig(FITKAbstractGraphObject*)),
            this, SLOT(removeGraphObjSlot(FITKAbstractGraphObject*)));
    }
    void FITKPolarCurveDrawManager::clear()
    {
        for (auto obj : _graphObjList)
        {
            Core::FITKAbstractGraphObject * curve = obj;
            if (curve) delete curve;
        }
        _graphObjList.clear();
    }
}
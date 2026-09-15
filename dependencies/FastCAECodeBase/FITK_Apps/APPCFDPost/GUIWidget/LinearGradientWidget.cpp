/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "LinearGradientWidget.h"

#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostColorLibaryData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostColorLibaryDataManager.h"

#include <QLinearGradient>
#include <QPainter>
#include <QLabel>
#include <QBrush>
#include <QDebug>

namespace GUI
{
    LinearGradientWidget::LinearGradientWidget(Interface::FITKCFDPostColorLibaryData * colorLibData, QWidget * parent):
        GUIWidgetBase(parent)
    {
        if (colorLibData == nullptr)return;
        _currentDataID = colorLibData->getDataObjectID();
        _colorLibData = colorLibData;
    }

    LinearGradientWidget::~LinearGradientWidget()
    {
        
    }

    void LinearGradientWidget::updateWidget()
    {
        update();
    }

    void LinearGradientWidget::paintEvent(QPaintEvent * event)
    {
        if (_colorLibData) {
            QLinearGradient linearGradient(0, height() % 2, width(), height());

            QMap<double, Interface::FITKPostColorStr> colorPoints = _colorLibData->getColorPoints();
            for (double pos : colorPoints.keys()) {
               Interface::FITKPostColorStr color = colorPoints.value(pos);
               linearGradient.setColorAt(pos, QColor::fromRgbF(color._R, color._G, color._B));
            }

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setBrush(linearGradient);
            painter.setPen(Qt::transparent);
            // 绘制一个填充矩形，覆盖整个 widget
            painter.drawRect(0, height() % 2, width(), height());
        }

        GUIWidgetBase::paintEvent(event);
    }
}
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWidgetHBox.h"
#include "ui_FITKWidgetHBox.h"

#include <QRadioButton>

namespace Core
{
    FITKWidgetHBox::FITKWidgetHBox(QList<QWidget*> widgetList, QWidget * parent) :
        QWidget(parent), _subWidgets(widgetList)
    {
        _ui = new Ui::FITKWidgetHBox();
        _ui->setupUi(this);

        for (int i = 0; i < widgetList.size(); i++) {
            auto w = widgetList[i];
            if (w == nullptr)continue;
            _ui->horizontalLayout->addWidget(w);
        }
    }

    FITKWidgetHBox::~FITKWidgetHBox()
    {
        if (_ui)delete _ui;
    }

    void FITKWidgetHBox::setSubWidgetEnable(bool enable, bool isFirst)
    {
        for (int i = 0; i < _subWidgets.size(); i++) {
            auto w = _subWidgets[i];
            if (w == nullptr)continue;
            if (i == 0 && isFirst == true) {
                continue;
            }
            w->setEnabled(enable);
        }
    }

    QList<QWidget*> FITKWidgetHBox::getSubWidget()
    {
        return _subWidgets;
    }
}


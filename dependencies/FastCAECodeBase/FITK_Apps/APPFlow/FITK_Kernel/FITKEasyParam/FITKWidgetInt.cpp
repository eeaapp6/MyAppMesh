/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWidgetInt.h"
#include "FITKAbstractEasyParam.h"
#include "FITKParamInt.h"

namespace Core
{
    FITKWidgetInt::FITKWidgetInt(FITKAbstractEasyParam * data, QWidget * parent):
        QSpinBox(parent)
    {
        _value = dynamic_cast<FITKParamInt*>(data);
        init();
        connect(this, SIGNAL(valueChanged(int)), this, SLOT(slotDataChange()));
    }

    FITKWidgetInt::~FITKWidgetInt()
    {

    }

    void FITKWidgetInt::init()
    {
        if (_value == nullptr)return;
        int value = _value->getValue();
        int range[2] = { 0,0 };
        _value->getRange(range);

        this->setRange(range[0], range[1]);
        this->setValue(value);
    }

    void FITKWidgetInt::wheelEvent(QWheelEvent * event)
    {
        Q_UNUSED(event);
    }

    void FITKWidgetInt::slotDataChange()
    {
        if (_value == nullptr)return;
        _value->setValue(value());
    }
}

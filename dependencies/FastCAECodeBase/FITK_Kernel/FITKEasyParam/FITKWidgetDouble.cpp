/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWidgetDouble.h"
#include "FITKAbstractEasyParam.h"
#include "FITKParamDouble.h"

namespace Core
{
    FITKWidgetDouble::FITKWidgetDouble(FITKAbstractEasyParam * data, QWidget * parent):
        FITKWidgetSciNotation(parent)
    {
        _value = dynamic_cast<FITKParamDouble*>(data);
        init();
    }

    FITKWidgetDouble::~FITKWidgetDouble()
    {

    }

    void FITKWidgetDouble::init()
    {
        if (_value == nullptr)return;
        double value = _value->getValue();
        double range[2] = { 0,0 };
        _value->getRange(range);
        this->setRange(range);
        this->setCurrentValidValue(value);

        connect(this, SIGNAL(dataChanged()), this, SLOT(slotDataChangeFinished()));
    }

    void FITKWidgetDouble::wheelEvent(QWheelEvent * event)
    {
        Q_UNUSED(event);
    }

    void FITKWidgetDouble::slotDataChangeFinished()
    {
        if (_value == nullptr)return;

        double value = 0.0;
        getCurrentValidValue(value);
        _value->setValue(value);
    }
}

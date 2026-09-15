/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWidgetString.h"
#include "FITKAbstractEasyParam.h"
#include "FITKParamString.h"

namespace Core
{
    FITKWidgetString::FITKWidgetString(FITKAbstractEasyParam * data, QWidget * parent) :
        QLineEdit(parent)
    {
        _value = dynamic_cast<FITKParamString*>(data);
        init();
        connect(this, SIGNAL(textChanged(QString)), this, SLOT(slotDataChange()));
    }

    FITKWidgetString::~FITKWidgetString()
    {

    }

    void FITKWidgetString::init()
    {
        if (_value == nullptr)return;
        QString value = _value->getValue();

        this->setText(value);
    }

    void FITKWidgetString::slotDataChange()
    {
        if (_value == nullptr)return;
        _value->setValue(text());
    }
}


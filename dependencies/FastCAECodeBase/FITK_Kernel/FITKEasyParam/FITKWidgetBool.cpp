/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWidgetBool.h"

#include "FITKAbstractEasyParam.h"
#include "FITKParamBool.h"

namespace Core
{
    FITKWidgetBool::FITKWidgetBool(FITKAbstractEasyParam * data, QWidget * parent) :
        QCheckBox(parent)
    {
        _value = dynamic_cast<FITKParamBool*>(data);
        init();
        connect(this, SIGNAL(stateChanged(int)), this, SLOT(slotDataChange()));
    }

    FITKWidgetBool::~FITKWidgetBool()
    {

    }

    void FITKWidgetBool::init()
    {
        if (_value == nullptr)return;
        setChecked(_value->getValue());
    }

    void FITKWidgetBool::slotDataChange()
    {
        if (_value == nullptr)return;
        _value->setValue(isChecked());
    }
}


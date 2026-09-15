/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ClipSliceWidgetBase.h"

namespace GUI
{
    ClipSliceWidgetBase::ClipSliceWidgetBase(ClipSlice widgetType, QWidget * parent) :
        GUIWidgetBase(parent), _widgetType(widgetType)
    {

    }

    ClipSliceWidgetBase::~ClipSliceWidgetBase()
    {

    }
}


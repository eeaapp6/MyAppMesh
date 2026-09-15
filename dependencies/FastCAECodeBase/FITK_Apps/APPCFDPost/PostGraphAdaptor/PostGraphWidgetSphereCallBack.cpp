/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphWidgetSphereCallBack.h"

#include <vtkSphereWidget.h>
#include <vtkWidgetRepresentation.h>
#include <vtkSphereRepresentation.h>

namespace Interface
{
    void PostGraphWidgetSphereCallBack::Execute(vtkObject * caller, unsigned long eventId, void * callData)
    {
        Q_UNUSED(eventId);
        Q_UNUSED(callData);

        vtkSphereWidget* widget = vtkSphereWidget::SafeDownCast(caller);
        if (widget == nullptr)return;
        double origin[3] = { 0,0,0 };
        double radius = 0.0;
        widget->GetCenter(origin);
        radius = widget->GetRadius();
        emit sigValueChange(origin, radius);
    }
}

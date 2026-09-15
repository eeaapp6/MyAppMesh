/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphWidgetCylinderCallBack.h"

#include <vtkImplicitCylinderWidget.h>
#include <vtkImplicitCylinderRepresentation.h>

namespace Interface
{
    void PostGraphWidgetCylinderCallBack::Execute(vtkObject * caller, unsigned long eventId, void * callData)
    {
        vtkImplicitCylinderWidget* widget = vtkImplicitCylinderWidget::SafeDownCast(caller);
        if (widget == nullptr)return;
        vtkImplicitCylinderRepresentation* rep = widget->GetCylinderRepresentation();
        if (rep == nullptr)return;

        double center[3] = { 0, 0, 0 }, axis[3] = { 0, 0, 0 }, radius = 0.0;

        rep->GetCenter(center);
        rep->GetAxis(axis);
        radius = rep->GetRadius();
        emit sigValueChange(center, axis, radius);
    }
}
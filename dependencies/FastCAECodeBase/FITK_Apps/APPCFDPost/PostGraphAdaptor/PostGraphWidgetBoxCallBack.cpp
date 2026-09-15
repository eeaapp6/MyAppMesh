/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphWidgetBoxCallBack.h"

#include <vtkBoxWidget2.h>
#include <vtkBoxRepresentation.h>

namespace Interface
{
    void PostGraphWidgetBoxCallBack::Execute(vtkObject * caller, unsigned long eventId, void * callData)
    {
        vtkBoxWidget2* widget = vtkBoxWidget2::SafeDownCast(caller);
        if (widget == nullptr)return;
        vtkBoxRepresentation* rep = vtkBoxRepresentation::SafeDownCast(widget->GetRepresentation());
        if (rep == nullptr)return;
        double* bounds;
        bounds = rep->GetBounds();
        emit sigValueChange(bounds);
    }
}

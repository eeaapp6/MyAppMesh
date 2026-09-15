/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphWidgetLineCallBack.h"

#include <vtkLineWidget2.h>
#include <vtkLineRepresentation.h>

namespace Interface
{
    void PostGraphWidgetLineCallBack::Execute(vtkObject * caller, unsigned long eventId, void * callData)
    {
        Q_UNUSED(eventId);
        Q_UNUSED(callData);

        vtkLineWidget2* widget = vtkLineWidget2::SafeDownCast(caller);
        if (widget == nullptr)return;
        vtkLineRepresentation* lineRep = vtkLineRepresentation::SafeDownCast(widget->GetRepresentation());
        if (lineRep == nullptr)return;
        double* startPoint = lineRep->GetPoint1WorldPosition();
        double* endPoint = lineRep->GetPoint2WorldPosition();
        emit sigValueChange(startPoint, endPoint);
    }
}
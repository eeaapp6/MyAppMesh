/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphWidgetCylinder.h"
#include "PostGraphWidgetCylinderCallBack.h"

#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

#include <vtkRenderWindow.h>
#include <vtkImplicitCylinderWidget.h>
#include <vtkImplicitCylinderRepresentation.h>
#include <vtkLineRepresentation.h>
#include <vtkCommand.h>

namespace Interface
{
    PostGraphWidgetCylinder::PostGraphWidgetCylinder(QWidget * widget)
    {
        Comp::FITKGraph3DWindowVTK* graph3DWidget = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(widget);
        if (graph3DWidget == nullptr)return;

        _widget = vtkImplicitCylinderWidget::New();
        _widget->CreateDefaultRepresentation();
        //设置交互器
        _widget->SetInteractor(graph3DWidget->getVTKRenderWindow()->GetInteractor());
        //代理类获取
        _representation = _widget->GetCylinderRepresentation();

        _callBack = new PostGraphWidgetCylinderCallBack();
        connect(_callBack, SIGNAL(sigValueChange(double*, double*, double)), this, SIGNAL(sigValueChange(double*, double*, double)));
        _widget->AddObserver(vtkCommand::InteractionEvent, _callBack);
    }

    PostGraphWidgetCylinder::~PostGraphWidgetCylinder()
    {
        if (_widget) {
            _widget->Off();
            _widget->Delete();
            _widget = nullptr;
        }

        if (_callBack) {
            delete _callBack;
            _callBack = nullptr;
        }
    }

    void PostGraphWidgetCylinder::setBounds(double * bounds)
    {
        if (_representation == nullptr)return;
        _representation->SetWidgetBounds(bounds);
    }

    void PostGraphWidgetCylinder::setValue(double * center, double * axis, double radius)
    {
        if (_representation == nullptr)return;
        _representation->SetCenter(center);
        _representation->SetAxis(axis);
        _representation->SetRadius(radius);
    }
    void PostGraphWidgetCylinder::setIsShow(bool isShow)
    {
        if (_widget == nullptr)return;
        if (isShow) {
            _widget->On();
        }
        else {
            _widget->Off();
        }
    }
}

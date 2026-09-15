/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphWidgetSphere.h"
#include "PostGraphWidgetSphereCallBack.h"

#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

#include <vtkSphereWidget.h>
#include <vtkRenderWindow.h>

namespace Interface
{
    PostGraphWidgetSphere::PostGraphWidgetSphere(QWidget* widget)
    {
        Comp::FITKGraph3DWindowVTK* graph3DWidget = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(widget);
        if (graph3DWidget == nullptr)return;
        //创建裁切平面
        _widget = vtkSphereWidget::New();
        //设置交互器
        _widget->SetInteractor(graph3DWidget->getVTKRenderWindow()->GetInteractor());

        _callBack = new PostGraphWidgetSphereCallBack();
        connect(_callBack, SIGNAL(sigValueChange(double*, double)), this, SIGNAL(sigValueChange(double*, double)));
        //设置回调类
        _widget->AddObserver(vtkCommand::InteractionEvent, _callBack);
    }

    PostGraphWidgetSphere::~PostGraphWidgetSphere()
    {
        if (_widget) {
            _widget->Off();
            _widget->Delete();
            _widget = nullptr;
        }
    }

    void PostGraphWidgetSphere::setIsShow(bool isShow)
    {
        if (isShow)_widget->On();
        else _widget->Off();
    }

    void PostGraphWidgetSphere::setValue(double * center, double radius)
    {
        if (_widget == nullptr)return;
        _widget->SetCenter(center);
        _widget->SetRadius(radius);
    }

    void PostGraphWidgetSphere::getValue(double * center, double & radius)
    {
        if (_widget == nullptr)return;
        _widget->GetCenter(center);
        radius = _widget->GetRadius();
    }
}


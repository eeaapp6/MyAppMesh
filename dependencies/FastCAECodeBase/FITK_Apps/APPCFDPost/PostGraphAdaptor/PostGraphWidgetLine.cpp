/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphWidgetLine.h"
#include "PostGraphWidgetLineCallBack.h"

#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

#include <vtkRenderWindow.h>
#include <vtkLineWidget2.h>
#include <vtkLineRepresentation.h>
#include <vtkCommand.h>

namespace Interface
{
    PostGraphWidgetLine::PostGraphWidgetLine(QWidget * widget)
    {
        Comp::FITKGraph3DWindowVTK* graph3DWidget = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(widget);
        if (graph3DWidget == nullptr)return;

        _widget = vtkLineWidget2::New();
        _widget->CreateDefaultRepresentation();
        //设置交互器
        _widget->SetInteractor(graph3DWidget->getVTKRenderWindow()->GetInteractor());
        //代理类获取
        _representation = _widget->GetLineRepresentation();

        _callBack = new PostGraphWidgetLineCallBack();
        connect(_callBack, SIGNAL(sigValueChange(double*, double*)), this, SIGNAL(sigValueChange(double*, double*)));
        _widget->AddObserver(vtkCommand::InteractionEvent, _callBack);
    }

    PostGraphWidgetLine::~PostGraphWidgetLine()
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

    void PostGraphWidgetLine::setLine(double * startPoint, double * endPoint)
    {
        if (_representation) {
            _representation->SetPoint1WorldPosition(startPoint);
            _representation->SetPoint2WorldPosition(endPoint);
        }
    }

    void PostGraphWidgetLine::setIsShow(bool isShow)
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
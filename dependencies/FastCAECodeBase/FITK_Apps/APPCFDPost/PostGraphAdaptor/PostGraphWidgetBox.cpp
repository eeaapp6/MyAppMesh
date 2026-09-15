/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphWidgetBox.h"
#include "PostGraphWidgetBoxCallBack.h"
#include "FITKVKTBoxRepresentaion.h"

#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

#include <vtkRenderWindow.h>
#include <vtkBoxWidget2.h>
#include <vtkBoxRepresentation.h>
#include <vtkCommand.h>
#include <vtkPolyData.h>

namespace Interface
{
    PostGraphWidgetBox::PostGraphWidgetBox(QWidget* widget)
    {
        Comp::FITKGraph3DWindowVTK* graph3DWidget = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(widget);
        if (graph3DWidget == nullptr)return;

        _widget = vtkBoxWidget2::New();
        _widget->CreateDefaultRepresentation();
        //设置交互器
        _widget->SetInteractor(graph3DWidget->getVTKRenderWindow()->GetInteractor());
        //代理类获取
        _representation = FITKVKTBoxRepresentaion::New();
        _widget->SetRepresentation(_representation);

        _callBack = new PostGraphWidgetBoxCallBack();
        connect(_callBack, SIGNAL(sigValueChange(double*)), this, SIGNAL(sigValueChange(double*)));
        _widget->AddObserver(vtkCommand::InteractionEvent, _callBack);
    }

    PostGraphWidgetBox::~PostGraphWidgetBox()
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

    void PostGraphWidgetBox::setValue(double * bounds)
    {
        if (_representation == nullptr)return;
        _representation->setBounds(bounds);
    }

    void PostGraphWidgetBox::setIsShow(bool isShow)
    {
        if (_widget == nullptr)return;
        if (isShow)_widget->On();
        else _widget->Off();
    }
}

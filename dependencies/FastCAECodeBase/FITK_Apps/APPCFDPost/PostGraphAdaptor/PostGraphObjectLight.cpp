/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphObjectLight.h"

#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostLight.h"

#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkLight.h>

namespace Interface
{
    PostGraphObjectLight::PostGraphObjectLight(FITKCFDPostLight* lightData, Comp::FITKGraph3DWindowVTK* graph3DWidget) :
        _lightData(lightData), _graph3DWidget(graph3DWidget)
    {
        if (_graph3DWidget == nullptr || _lightData == nullptr)return;
        vtkRenderer* render = _graph3DWidget->getVTKRenderer(0);
        if (render == nullptr)return;
        _VTKLight = vtkLight::New();
        _VTKLight->SwitchOn();
        _VTKLight->SetIntensity(1);
        render->AddLight(_VTKLight);
    }

    PostGraphObjectLight::~PostGraphObjectLight()
    {
        if (_graph3DWidget) {
            vtkRenderer* render = _graph3DWidget->getVTKRenderer(0);
            if (render)render->RemoveLight(_VTKLight);
        }

        if (_VTKLight) {
            _VTKLight->Delete();
            _VTKLight = nullptr;
        }
    }

    void PostGraphObjectLight::update()
    {
        if (_VTKLight == nullptr || _lightData == nullptr)return;
        switch (_lightData->getLightType()) {
        case Interface::FITKPostLightType::Post_Positional: {
            _VTKLight->SetPositional(true);
            break;
        }
        case Interface::FITKPostLightType::Post_Direction: {
            _VTKLight->SetPositional(false);
            break;
        }
        }

        double position[3] = { 0,0,0 }, focalPoint[3] = { 0,0,0 }, color[3] = { 0,0,0 }, coneAngle = 0.0;
        _lightData->getPosition(position);
        _VTKLight->SetPosition(position);

        _lightData->getFocalPoint(focalPoint);
        _VTKLight->SetFocalPoint(focalPoint);

        coneAngle = _lightData->getConeAngle();
        _VTKLight->SetConeAngle(coneAngle);

        _lightData->getColor(color);
        _VTKLight->SetColor(color[0], color[1], color[2]);
    }

    FITKCFDPostLight* PostGraphObjectLight::getLightData()
    {
        return _lightData;
    }
}
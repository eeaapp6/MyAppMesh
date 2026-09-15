/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostLight.h"

namespace Interface
{
    void FITKCFDPostLight::setLightType(FITKPostLightType type)
    {
        _lightType = type;
    }

    FITKPostLightType FITKCFDPostLight::getLightType()
    {
        return _lightType;
    }

    void FITKCFDPostLight::setColor(double * color)
    {
        _color[0] = color[0];
        _color[1] = color[1];
        _color[2] = color[2];
    }

    void FITKCFDPostLight::getColor(double * color)
    {
        color[0] = _color[0];
        color[1] = _color[1];
        color[2] = _color[2];
    }

    void FITKCFDPostLight::setPosition(double * position)
    {
        _position[0] = position[0];
        _position[1] = position[1];
        _position[2] = position[2];
    }

    void FITKCFDPostLight::getPosition(double * position)
    {
        position[0] = _position[0];
        position[1] = _position[1];
        position[2] = _position[2];
    }

    void FITKCFDPostLight::setFocalPoint(double * focalPoint)
    {
        _focalPoint[0] = focalPoint[0];
        _focalPoint[1] = focalPoint[1];
        _focalPoint[2] = focalPoint[2];
    }

    void FITKCFDPostLight::getFocalPoint(double * focalPoint)
    {
        focalPoint[0] = _focalPoint[0];
        focalPoint[1] = _focalPoint[1];
        focalPoint[2] = _focalPoint[2];
    }

    void FITKCFDPostLight::setConeAngle(double value)
    {
        _coneAngle = value;
    }

    double FITKCFDPostLight::getConeAngle()
    {
        return _coneAngle;
    }
}

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractRadiossData.h"

Radioss::FITKAbstractRadiossData::~FITKAbstractRadiossData()
{
}

void Radioss::FITKAbstractRadiossData::setVisible(bool isVisible)
{
    _isVisible = isVisible;
}

bool Radioss::FITKAbstractRadiossData::getVisible()
{
    return _isVisible;
}

//QString Radioss::FITKAbstractRadiossData::getRadiossFormatKeyWords() const
//{
//    return QString();
//}

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractMaterial.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
FITKLIBINFOREGISTER(FITKInterfacePhysics, FITKInterfacePhysicsVersion);

Interface::FITKAbstractMaterial::FITKAbstractMaterial()
{
    //构造
    static int ABSMATERALID = 0;
    _materialID = ++ABSMATERALID;
}

Interface::FITKAbstractMaterial::~FITKAbstractMaterial()
{
    //析构
}

int Interface::FITKAbstractMaterial::getMaterialID() const
{
    //返回材料ID
    return _materialID;
}

QString Interface::FITKAbstractMaterial::getMaterialDescribe() const
{
    return _describe;
}

void Interface::FITKAbstractMaterial::setMaterialDescribe(const QString& des)
{
    _describe = des;
}

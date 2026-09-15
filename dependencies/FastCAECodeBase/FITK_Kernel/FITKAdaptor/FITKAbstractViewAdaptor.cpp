/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractViewAdaptor.h"


namespace Adaptor
{
    FITKAbstractViewAdaptor::~FITKAbstractViewAdaptor()
    {
    }


    QString FITKAbstractViewAdaptor::getAdaptorClass()
    {
        //返回错误值
        return QString();
    }

    Adaptor::FITKAdaptorType FITKAbstractViewAdaptor::getAdaptorType()
    {
        //返回视图类
        return FITKAdaptorType::AdaTView;
    }

}


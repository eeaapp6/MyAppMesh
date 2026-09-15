/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractIOAdaptor.h"

namespace Adaptor
{
    /**
     * @brief Destroy the FITKAbstractIOAdaptor::FITKAbstractIOAdaptor object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-03
     */
    FITKAbstractIOAdaptor::~FITKAbstractIOAdaptor()
    {
    }

    QString FITKAbstractIOAdaptor::getAdaptorClass()
    {
        return QString();
    }

    FITKAdaptorType FITKAbstractIOAdaptor::getAdaptorType()
    {
        //返回错误值
        return FITKAdaptorType::AdaTIO;
    }

    bool FITKAbstractIOAdaptor::adaptR()
    {
        return false;
    }

    bool FITKAbstractIOAdaptor::adaptW()
    {
        return false;
    }

}
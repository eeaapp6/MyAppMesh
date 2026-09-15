/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

namespace Adaptor
{
   FITKLIBINFOREGISTER(FITKAdaptor, FITKAdaptorVersion);

    /**
     * @brief Destroy the FITKAbstractAdaptor::FITKAbstractAdaptor object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-03
     */
    FITKAbstractAdaptor::~FITKAbstractAdaptor()
    {

    }

    QString FITKAbstractAdaptor::getAdaptorClass()
    {
        return QString();
    }

    FITKAdaptorType FITKAbstractAdaptor::getAdaptorType()
    {
        //返回错误值
        return FITKAdaptorType::AdaTNone;
    }

    void FITKAbstractAdaptor::setDataObject(Core::FITKAbstractDataObject* data)
    {
        _dataObj = data;
    }

    bool FITKAbstractAdaptor::adapt()
    {
        return false;
    }

}




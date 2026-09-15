/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractFactory.h"

namespace Core
{
    /**
     * @brief Destroy the FITKAbstractFactory::FITKAbstractFactory object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    FITKAbstractFactory::~FITKAbstractFactory()
    {

    }

    Core::FITKAbstractObject* FITKAbstractFactory::create(const QString& key)
    {
        return nullptr;
    }

    Core::FITKAbstractObject* FITKAbstractFactory::create(const int key)
    {
        return nullptr;
    }

}

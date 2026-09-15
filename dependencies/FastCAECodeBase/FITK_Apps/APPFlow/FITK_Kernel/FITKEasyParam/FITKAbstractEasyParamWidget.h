/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITKABSTRACT_PAEAM_WIDGET_H___
#define _FITKABSTRACT_PAEAM_WIDGET_H___
 
#include "FITKEasyParamAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace Core
{
    class FITKEasyParamAPI FITKAbstractEasyParamWidget
    {
    public:
        explicit FITKAbstractEasyParamWidget();
        virtual ~FITKAbstractEasyParamWidget() = default;
    };
}
#endif

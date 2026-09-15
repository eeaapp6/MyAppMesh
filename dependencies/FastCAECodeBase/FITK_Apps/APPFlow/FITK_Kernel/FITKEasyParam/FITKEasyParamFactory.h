/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_EASY_PARAM_FACTORY_H___
#define _FITK_EASY_PARAM_FACTORY_H___

#include "FITKEasyParamAPI.h" 
#include "FITKAbstractEasyParam.h"

namespace Core
{
    class FITKAbstractEasyParam;

    class FITKEasyParamAPI FITKEasyParamFactory

    {
    public:
        static FITKAbstractEasyParam* createParam(FITKAbstractEasyParam::FITKEasyParamType type);
   
    private:
        explicit FITKEasyParamFactory() = default;
        virtual ~FITKEasyParamFactory() = default;
    };
 
}


#endif

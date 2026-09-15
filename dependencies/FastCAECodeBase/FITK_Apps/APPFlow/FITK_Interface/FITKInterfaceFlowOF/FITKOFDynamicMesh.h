/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFDynamicMesh.h
 * @brief  动态网格配置接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-04
 *********************************************************************/
#ifndef _FITK_OF_DYNAMICMESH__H___
#define _FITK_OF_DYNAMICMESH__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{

    class FITKInterfaceFlowOFAPI FITKOFDynamicMesh : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFDynamicMesh();
        virtual ~FITKOFDynamicMesh();

       
    };
}


#endif

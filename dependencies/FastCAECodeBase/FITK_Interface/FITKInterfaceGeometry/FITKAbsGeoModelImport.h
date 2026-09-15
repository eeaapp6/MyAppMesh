/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_GEOMODEL_IMPORT_H___
#define _FITK_GEOMODEL_IMPORT_H___
 
#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoImportBase.h"
#include <QString>

namespace Interface
{
    class FITKInterfaceGeometryAPI FITKAbsGeoModelImport :
        public  FITKAbsGeoImportBase
    {
        FITKCLASS(Interface, FITKAbsGeoModelImport);
    public:

        explicit FITKAbsGeoModelImport() = default;

        virtual ~FITKAbsGeoModelImport() = default;;

        /**
         * @brief 获取几何命令类型
         * @return FITKGeoEnum::FITKGeometryComType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-01
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;


    };
}


#endif

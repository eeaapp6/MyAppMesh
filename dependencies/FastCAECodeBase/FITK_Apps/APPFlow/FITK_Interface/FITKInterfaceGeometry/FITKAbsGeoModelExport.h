/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_GEO_MODEL_EXPORT__H___
#define _FITK_GEO_MODEL_EXPORT__H___
 
#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <QString>

namespace Interface
{
    class FITKInterfaceGeometryAPI FITKAbsGeoModelExport :
        public  FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelExport);
    public:

        explicit FITKAbsGeoModelExport() = default;

        virtual ~FITKAbsGeoModelExport();

        void setFileName(const QString& fileName);

        QString getFileName() const;

        /**
         * @brief       添加需要导出的几何命令ID。
         * @param[in]   id：几何命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        void addExportCommandID(int id);

        /**
         * @brief       移除需要导出的几何命令ID。
         * @param[in]   id：几何命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        void removeExportCommandID(int id);

        /**
            * @brief 获取几何命令类型
            * @return FITKGeoEnum::FITKGeometryComType
            * @author LiBaojun (libaojunqd@foxmail.com)
            * @date 2024-08-01
            */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;


    protected:
        QString _exportFileName{};

        /**
         * @brief       导出几何命令ID列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-24
         */
        QList<int> _exportIds;

    };
    
}


#endif

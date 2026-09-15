/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoImportedPart.h
 * @brief       导入的模型部件命令数据抽象类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-11-27
 *********************************************************************/

#ifndef  FITKABSGEOIMPORTEDPART_H
#define  FITKABSGEOIMPORTEDPART_H

#include "FITKAbsGeoCommand.h"

#include "FITKGeoEnum.h"

#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       导入的模型部件命令数据抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-11-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoImportedPart : public Interface::FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoImportedPart);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        FITKAbsGeoImportedPart() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        virtual ~FITKAbsGeoImportedPart() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    };
}

#endif // !FITKABSGEOIMPORTEDPART_H

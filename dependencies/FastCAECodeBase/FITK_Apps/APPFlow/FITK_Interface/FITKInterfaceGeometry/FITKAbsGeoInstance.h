/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoInstance.h
 * @brief       模型装配实例命令数据抽象类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-11-27
 *********************************************************************/

#ifndef  FITKABSGEOINSTANCE_H
#define  FITKABSGEOINSTANCE_H

#include "FITKAbsGeoCommand.h"

#include "FITKGeoEnum.h"

#include "FITKInterfaceGeometryAPI.h"

// 前置声明
namespace Interface
{
    class FITKAbsGeoPart;
}

namespace Interface
{
    /**
     * @brief       模型装配实例命令数据抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-11-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoInstance : public Interface::FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoInstance);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        FITKAbsGeoInstance() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        virtual ~FITKAbsGeoInstance() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       更新命令。[虚函数][重写]
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        virtual bool update() override;

        /**
         * @brief       设置被引用部件命令ID。
         * @param[in]   id：命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        void setPartCommandID(int id);

        /**
         * @brief       获取被引用部件命令ID。
         * @return      命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        int getPartCommandID();

        /**
         * @brief       获取被引用部件命令。
         * @return      部件命令。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        FITKAbsGeoPart* getPartCommand();

        // TODO
        // 维护部件装配信息。

    protected:
        /**
         * @brief       被引用部件命令ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-27
         */
        int m_partCmdId = -1;

    };
}

#endif // !FITKABSGEOINSTANCE_H

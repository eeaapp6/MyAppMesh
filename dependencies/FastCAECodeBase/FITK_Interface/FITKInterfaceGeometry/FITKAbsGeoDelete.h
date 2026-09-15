/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoDelete.h
 * @brief       模型删除命令数据抽象类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-06
 *********************************************************************/

#ifndef  FITKABSGEODELETE_H
#define  FITKABSGEODELETE_H

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

#include "FITKGeoEnum.h"
#include "FITKAbsGeoCommand.h"

#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       模型删除命令数据抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-03
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoDelete : public Interface::FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoDelete);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKAbsGeoDelete() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual ~FITKAbsGeoDelete() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       更新命令。[虚函数][重写]
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual bool update() override;

        /**
         * @brief       执行撤销逻辑。（非必须）[虚函数][重写]
         * @return      是否执行成功。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual bool undo() override;

        /**
         * @brief       执行重做逻辑。（非必须）[虚函数][重写]
         * @return      是否执行成功。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        virtual bool redo() override;

        /**
         * @brief       设置被移除命令ID。
         * @param[in]   id：命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        void setDeleteCommandID(int id);

        /**
         * @brief       获取被移除命令ID。
         * @return      命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        int getDeleteCommandID();

    protected:
        /**
         * @brief       被删除命令ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-06
         */
        int m_deletedCmdId = -1;

    };
}

#endif // !FITKABSGEODELETE_H

/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoMultiDelete.h
 * @brief       多选模型删除命令数据抽象类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-10-21
 *********************************************************************/

#ifndef  FITKABSGEOMULTIDELETE_H
#define  FITKABSGEOMULTIDELETE_H

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

#include "FITKGeoEnum.h"
#include "FITKAbsGeoCommand.h"

#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       多选模型删除命令数据抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-10-21
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoMultiDelete : public Interface::FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoMultiDelete);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-21
         */
        FITKAbsGeoMultiDelete() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-21
         */
        virtual ~FITKAbsGeoMultiDelete() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-21
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       更新命令。[虚函数][重写]
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-21
         */
        virtual bool update() override;

        /**
         * @brief       执行撤销逻辑。（非必须）[虚函数][重写]
         * @return      是否执行成功。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-21
         */
        virtual bool undo() override;

        /**
         * @brief       执行重做逻辑。（非必须）[虚函数][重写]
         * @return      是否执行成功。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-21
         */
        virtual bool redo() override;

        /**
         * @brief       设置被移除命令ID列表。
         * @param[in]   ids：命令ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-21
         */
        void setDeleteCommandIDs(QList<int> ids);

        /**
         * @brief       获取被移除命令ID列表。
         * @return      命令ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-21
         */
        QList<int> getDeleteCommandIDs();

    protected:
        /**
         * @brief       被删除命令ID列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-21
         */
        QList<int> m_deletedCmdIds;

    };
}

#endif // !FITKABSGEOMULTIDELETE_H

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperExportManager.h
 * @brief   导出几何文件操作器。

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-27
 *********************************************************************/

#ifndef OPEREXPORTMANAGER_H
#define OPEREXPORTMANAGER_H

#include "OperManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /**
     * @brief   导出几何文件操作器。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-27
     */
    class OperatorsModelAPI OperExportManager : public OperManagerBase
    {
        Q_OBJECT
    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-27
         */
        explicit OperExportManager() = default;

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-27
         */
        ~OperExportManager() = default;

    private:
        /**
         * @brief   界面逻辑执行。[重写]
         * @return  是否执行成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-27
         */
        bool execGUI() override;

        /**
         * @brief   业务逻辑执行。[重写]
         * @return  是否执行成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-27
         */
        bool execProfession() override;
    };

    // 注册操作器
    Register2FITKOperatorRepo(OperExportManager, OperExportManager);
    Register2FITKOperatorRepo(actionExport, OperExportManager);
} // namespace ModelOper

#endif // OPEREXPORTMANAGER_H

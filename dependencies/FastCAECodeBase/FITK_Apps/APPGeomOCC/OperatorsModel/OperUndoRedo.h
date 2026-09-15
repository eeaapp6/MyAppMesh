/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperUndoRedo.h
 * @brief   撤销重做操作器。

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-07
 *********************************************************************/

#ifndef OPERATORSUNDOREDO_H
#define OPERATORSUNDOREDO_H

#include "OperatorsModelAPI.h"

#include "OperManagerBase.h"

namespace ModelOper
{
    /**
     * @brief   撤销操作器。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-07
     */
    class OperatorsModelAPI OperUndo : public OperManagerBase
    {
        Q_OBJECT
    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        explicit OperUndo() = default;

        /**
         * @brief   析构函数。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        virtual ~OperUndo() = default;

    private:
        /**
         * @brief   执行界面操作。[重写]
         * @return  是否执行成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        bool execGUI() override;

        /**
         * @brief   执行业务逻辑。[重写]
         * @return  是否执行成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        bool execProfession() override;

    };

    // 注册撤销操作器
    Register2FITKOperatorRepo(OperUndo, OperUndo);
    Register2FITKOperatorRepo(actionUndo, OperUndo);

    /**
     * @brief   重做操作器。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-07
     */
    class OperatorsModelAPI OperRedo : public OperManagerBase
    {
        Q_OBJECT
    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        explicit OperRedo() = default;

        /**
         * @brief   析构函数。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        virtual ~OperRedo() = default;

    private:
        /**
         * @brief   执行界面操作。[重写]
         * @return  是否执行成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        bool execGUI() override;

        /**
         * @brief   执行业务逻辑。[重写]
         * @return  是否执行成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        bool execProfession() override;

    };

    // 注册重做操作器
    Register2FITKOperatorRepo(OperRedo, OperRedo);
    Register2FITKOperatorRepo(actionRedo, OperRedo);

} // namespace ModelOper
#endif // OPERATORSUNDOREDO_H

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        OperFaceHidden.h
 * @brief       隐藏几何面操作器。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-07-07
 *********************************************************************/

#ifndef __OPERFACEHIDDEN_H__
#define __OPERFACEHIDDEN_H__

#include "OperManagerBase.h"

#include "OperatorsModelAPI.h"

// Qt 
#include <QDialog>

namespace ModelOper
{
    /**
     * @brief       隐藏几何面操作器。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-07
     */
    class OperatorsModelAPI OperFaceHidden : public OperManagerBase
    {
        Q_OBJECT
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        OperFaceHidden() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual ~OperFaceHidden() = default;

        /**
         * @brief       执行界面相关功能。[虚函数][重写]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual bool execGUI() override;

        /**
         * @brief       执行业务逻辑。[虚函数][重写]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual bool execProfession() override;

        /**
         * @brief       结束操作。[虚函数][重写]
         * @param       updateCommand：是否更新命令[缺省]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual void finalize(bool updateCommand = false) override;

    protected:

    };

    // 注册操作器。
    Register2FITKOperatorRepo(OperFaceHidden, OperFaceHidden);
    Register2FITKOperatorRepo(actionHideFaces, OperFaceHidden);
} // namespace ModelOper
#endif //!OperFaceHidden_H

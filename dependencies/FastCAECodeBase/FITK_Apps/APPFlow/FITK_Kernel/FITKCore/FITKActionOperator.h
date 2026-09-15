/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file ActionEventOperator.h
 * @brief action点击事件处理基
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-25
 *
 */

#ifndef __FITKACTIONOPERATOR_H___
#define __FITKACTIONOPERATOR_H___

#include "FITKAbstractOperator.h"

namespace Core
{
    /**
    * @brief Action点击事件处理器，实现事件的处理逻辑，抽象类
    * @author LiBaojun (libaojunqd@foxmail.com)
    * @date 2024-03-25
    */
    class FITKCoreAPI FITKActionOperator : public FITKAbstractOperator
    {
    public:
        explicit FITKActionOperator() = default;
        virtual ~FITKActionOperator() = 0;


        /**
         * @brief 点击事件处理方法
         * @return true      执行成功
         * @return false     执行失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-25
         */
        virtual bool actionTriggered();

        /**
         * @brief 界面逻辑，生成UI交互
         * @return true             执行成功
         * @return false            执行失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-25
         */
        virtual bool execGUI();
        /**
         * @brief 业务处理逻辑，在execGUI后执行
         * @return true             执行成功
         * @return false            执行失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-25
         */
        virtual bool execProfession();


    protected:
        /**
         * @brief 参数预处理
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-25
         */
        virtual void preArgs();
    };

}



#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperLicense.h
 * @brief   License窗口显示操作器。

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-02
 *********************************************************************/

#ifndef __OPERLICENSE_H__
#define __OPERLICENSE_H__

// Operator base
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUIOper
{
    /**
     * @brief   License窗口显示操作器。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-02
     */
    class OperLicense : public  Core::FITKActionOperator
    {
    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-02
         */
        explicit OperLicense() = default;

        /**
         * @brief   析构函数。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-02
         */
        virtual ~OperLicense() = default;

    protected:
        /**
         * @brief   
         * @return  
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-02
         */
        bool execGUI() override;

    };

    // 注册操作器。
    Register2FITKOperatorRepo(actionLicense, OperLicense);
}


#endif //__OPERLICENSE_H__

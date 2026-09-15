/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _OPERGUI_INTERFACE__H___
#define _OPERGUI_INTERFACE__H___

#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "OperatorsGUIAPI.h"

namespace GUIOper
{
    /* @brief 模型相关操作器注册，主要用于加载dll
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-14
     */
    class OperatorsGUIAPI OperGUIInterface :
        public AppFrame::FITKComponentInterface
    {
    public:
        /**
         * @brief Construct a new Oper G U I Interface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        explicit OperGUIInterface() = default;
        virtual ~OperGUIInterface() = default;
        /**
         * @brief 获取部件名称，不能重复
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual QString getComponentName();

    };
}


#endif

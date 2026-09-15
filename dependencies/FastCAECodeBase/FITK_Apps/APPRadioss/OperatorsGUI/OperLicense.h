/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file OperLicense.h
 * @brief License operator for GUI
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-10-14
 * 
 */
#ifndef OPERLICENSE_H__
#define OPERLICENSE_H__

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUIOper
{
    /**
     * @brief License operator for GUI
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-10-14
     */
    class OperLicense : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new Oper License object
         * @author LiBaojun (libaojunqd@foxmail.com)N
         * @date 2025-10-14
         */
        explicit OperLicense() = default;
        /**
         * @brief Destroy the Oper License object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-14
         */
        virtual ~OperLicense() = default;
        /**
         * @brief Check if the action is triggered
         * @return true 
         * @return false 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-14
         */
        bool execGUI() override;
 
    };

    Register2FITKOperatorRepo(actionLicense, OperLicense);
}

#endif // OPERLICENSE_H__
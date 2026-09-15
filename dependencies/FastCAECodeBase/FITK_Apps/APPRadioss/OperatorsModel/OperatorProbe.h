/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _OperatorProbe_H_
#define _OperatorProbe_H_


#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
namespace Radioss
{
    class FITKAbstractProbe;
}

namespace ModelOper
{
    class  OperatorProbe: public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        explicit OperatorProbe() = default;
        virtual ~OperatorProbe() = default;
        /**
         * @brief    执行界面操作
         * @return true     执行成功
         * @return false     执行失败
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-17
         */
        virtual bool execGUI() override;
        /**
         * @brief    执行写出操作
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-17
         */
        virtual bool execProfession() override;

    private:
        /**
         * @brief    获取探针数据
         * @return   Radioss::FITKAbstractProbe *
         */
        Radioss::FITKAbstractProbe* getProbeData();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionCreateAcceleration, OperatorProbe);
    Register2FITKOperatorRepo(actionEditAcceleration, OperatorProbe);
    Register2FITKOperatorRepo(actionCreateSection, OperatorProbe);
    Register2FITKOperatorRepo(actionEditSection, OperatorProbe);
}

#endif
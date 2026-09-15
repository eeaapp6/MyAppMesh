/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorRBody.h
 * @brief  刚体连接操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-06
 *********************************************************************/
#ifndef __OPERATORRBODY_H__
#define __OPERATORRBODY_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace Radioss
{
    class FITKAbstractConnection;
}

namespace OperModel
{
    /**
     * @brief  刚体连接操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-11-06
     */
    class OperatorRBody :public Core::FITKActionOperator
    {
    public:
        explicit OperatorRBody() = default;
        virtual ~OperatorRBody() = default;
        /**
         * @brief    执行界面操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        virtual bool execGUI();
        /**
         * @brief    执行业务处理逻辑
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        virtual bool execProfession();
    private:
        /**
         * @brief    创建刚体连接
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        bool createRigidBodyOper();
        /**
         * @brief    编辑刚体连接
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        bool editRigidBodyOper();
        /**
         * @brief    获取连接数据
         * @return   Radioss::FITKAbstractConnection *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        Radioss::FITKAbstractConnection* getConnectionData();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionRBodyCreate, OperatorRBody);
    Register2FITKOperatorRepo(actionRBodyEdit, OperatorRBody);
}

#endif

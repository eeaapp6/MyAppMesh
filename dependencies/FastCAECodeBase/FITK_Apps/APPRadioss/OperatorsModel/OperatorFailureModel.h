/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorFailureModel.h
 * @brief  失效模型操作
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-10
 *********************************************************************/
#ifndef __OperatorFailureModel_H__
#define __OperatorFailureModel_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace Radioss
{
    class FITKAbstractFailureModel;
    class FITKRadiossFailureModelManager;
}
namespace OperModel
{
    /**
     * @brief  失效模型操作
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-10-30
     */
    class OperatorFailureModel :public Core::FITKActionOperator
    {
    public:
        explicit OperatorFailureModel() = default;
        virtual ~OperatorFailureModel() = default;
        /**
         * @brief  执行界面操作
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-30
         */
        virtual bool execGUI();
        /**
         * @brief  执行业务处理逻辑
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-30
         */
        virtual bool execProfession();
    private:
        /**
         * @brief  获取失效模型对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-30
         */
        Radioss::FITKAbstractFailureModel* getSelectedFailureModel();
        /**
         * @brief  获取失效模型管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-30
         */
        Radioss::FITKRadiossFailureModelManager* getFailureModelManager();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionFailureModelCreate, OperatorFailureModel);
    Register2FITKOperatorRepo(actionFailureModelEdit, OperatorFailureModel);
}

#endif

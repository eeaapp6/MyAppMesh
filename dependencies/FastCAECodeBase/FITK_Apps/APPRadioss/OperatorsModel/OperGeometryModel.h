/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperGeometryModel.h
 * @brief  几何模型操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-15
 *********************************************************************/
#ifndef __OperGeometryModel_H__
#define __OperGeometryModel_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"


namespace OperModel
{
    /**
     * @brief  几何模型操作器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-17
     */
    class OperGeometryModel :public Core::FITKActionOperator
    {
    public:
        explicit OperGeometryModel() = default;
        virtual ~OperGeometryModel() = default;
        /**
         * @brief    执行界面操作
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-15
         */
        virtual bool execGUI();
        /**
         * @brief    执行业务处理逻辑
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-15
         */
        virtual bool execProfession();
    private:

    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionCreateBox, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateCylinder, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateSphere, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateBool, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateChamfer, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateFillet, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateDefeature, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateMirror, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateRectangularPattern, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateCircularPattern, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateScale, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateRotate, OperGeometryModel);
    Register2FITKOperatorRepo(actionCreateTrans, OperGeometryModel);

}

#endif

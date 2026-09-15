/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file OperatorMaterialProp.h
 * @brief 材料属性操作器接口
 * @author wangning (2185896382@qq.com)
 * @date 2025-05-23
 *
 */
#ifndef __OPERATORMATERIALPROP_H__
#define __OPERATORMATERIALPROP_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace Interface
{
    class FITKAbstractMaterial;
    class FITKMaterialManager;
}
namespace OperModel
{
    /**
     * @brief 材料属性操作器
     * @author wangning (2185896382@qq.com)
     * @date 2025-05-23
     */
    class OperatorMaterialProp :public Core::FITKActionOperator
    {
    public:
        explicit OperatorMaterialProp() = default;
        virtual ~OperatorMaterialProp() = default;
        /**
         * @brief    执行界面操作
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-22
         */
        virtual bool execGUI();
        /**
         * @brief    执行业务处理逻辑
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-22
         */
        virtual bool execProfession();
    private:
        /**
         * @brief    获取材料对象
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-22
         */
        Interface::FITKAbstractMaterial* getSelectedMaterial();
        /**
         * @brief    获取材料管理器
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-22
         */
        Interface::FITKMaterialManager* getMaterialManager();
        /**
         * @brief    重命名材料
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-22
         */
        bool materialPropRenameOper();
        /**
         * @brief    删除材料
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-22
         */
        bool deleteMaterial();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionMaterialPropCreate, OperatorMaterialProp);
    Register2FITKOperatorRepo(actionMaterialPropRename, OperatorMaterialProp);
    Register2FITKOperatorRepo(actionMaterialPropEdit, OperatorMaterialProp);
    Register2FITKOperatorRepo(actionMaterialPropDelete, OperatorMaterialProp);
}

#endif

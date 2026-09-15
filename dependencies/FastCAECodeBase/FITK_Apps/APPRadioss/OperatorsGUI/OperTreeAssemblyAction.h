/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    OperTreeAssemblyAction.h
 * @brief   装配树形菜单动作操作器
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-19
 *********************************************************************/
#ifndef __OPER_TREE_ASSEMBLY_ACTION_H__
#define __OPER_TREE_ASSEMBLY_ACTION_H__

#include "OperatorsGUI/OperTreeAction.h"

namespace Interface { class FITKGeoCommandList; }
namespace Radioss { class FITKRadiossPartManager; }

namespace GUIOper
{
    /**
     * @brief   装配树形菜单动作操作器
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-19
     */
    class  OperTreeAssemblyAction : public OperTreeAction
    {
    public:
        /**
         * @brief   构造函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-19
         */
        explicit OperTreeAssemblyAction() = default;
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-19
         */
        virtual ~OperTreeAssemblyAction();
        /**
         * @brief   界面逻辑，生成UI交互
         * @return  执行结果
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-19
         */
        virtual bool execGUI();
    protected:
        /**
         * @brief   参数预处理
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-19
         */
        virtual void preArgs() override;
    private:
        void setColor();
        void setMaterialProperty(int model);
        /**
         * @brief    SPH转化
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-04
         */
        void modelSPHTransformation();
        /**
         * @brief     显示/隐藏模型
         * @param[i]  isShow 是否显示模型
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-19
         */
        void showModel(bool isShow);
        /**
         * @brief   重命名模型
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-19
         */
        void renameModel();
        /**
         * @brief   删除模型
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-19
         */
        void deleteModel();
    private:
        /**
         * @brief   获取几何模型管理器
         * @return  Interface::FITKGeoCommandList* 几何管理器
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-19
         */
        Interface::FITKGeoCommandList* getGeometryManager();
        /**
         * @brief   获取模型部件管理器
         * @return  Radioss::FITKRadiossPartManager* 部件管理器
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-19
         */
        Radioss::FITKRadiossPartManager* getPartManager();
    private:
        /**
         * @brief   选中的几何ID列表
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-19
         */
        QList<int> _selectedGeometryIDs{};
        /**
         * @brief   选中的部件ID列表
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-19
         */
        QList<int> _selectedPartIDs{};
    };
    //注册事件操作器
    Register2FITKOperatorRepo(actionAssemblyTreeCreateBox, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeCreateCylinder, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeCreateSphere, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeCreatePart, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeSetColor, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeSetMaterial, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeSetProperty, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeSPHTrans, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeShowModel, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeHideModel, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeRename, OperTreeAssemblyAction);
    Register2FITKOperatorRepo(actionAssemblyTreeDelete, OperTreeAssemblyAction);
}
#endif  // __OPER_TREE_ASSEMBLY_ACTION_H__
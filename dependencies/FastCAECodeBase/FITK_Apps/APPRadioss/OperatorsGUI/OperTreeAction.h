/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    OperTreeAction.h
 * @brief   树形菜单动作操作器基类
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-18
 *********************************************************************/
#ifndef __OPER_TREE_ACTION_H__
#define __OPER_TREE_ACTION_H__

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "GUIDialog/GUIToolDialog/GUIRenameDialog.h"

namespace Core { class FITKAbstractDataManagerPrivate; }
namespace Radioss { class FITKRadiossCase; }

namespace GUIOper
{
    /**
     * @brief   树形菜单动作操作器基类
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-18
     */
    class  OperTreeAction : public Core::FITKActionOperator
    {
    public:
        /**
         * @brief   构造函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        explicit OperTreeAction() = default;
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        virtual ~OperTreeAction()= default;
    protected:
        /**
         * @brief     调用指定名称的操作器
         * @param[i]  registerName 注册的操作器名称
         * @param[i]  hashArg 参数键值对，默认为空
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-18
         */
        void callOperator(QString registerName, QHash<QString, QVariant> hashArg = QHash<QString, QVariant>());
        /**
         * @brief     改变激活状态模版
         * @param[i]  manager 数据管理器
         * @param[i]  id 数据ID
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-23
         */
        template<typename T>
        void changeActivationStatus(T manager, int id)
        {
            // 检查参数
            if (manager == nullptr && id < 1) return;

            // 获取数据对象
            Core::FITKAbstractNDataObject* data = manager->getDataByID(id);
            if (data == nullptr) return;

            // 反转激活状态
            data->enable(!data->isEnable());

            // 更新树
            updateTree();
        }
        /**
         * @brief     重命名数据模版
         * @param[i]  oper 当前操作器
         * @param[i]  manager 数据管理器
         * @param[i]  id 数据ID
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-18
         */
        template<typename T>
        void renameData(Core::FITKActionOperator* oper, T manager, int id)
        {
            // 检查参数
            if (oper == nullptr || manager == nullptr && id < 1) return;

            // 获取重命名对话框
            GUI::GUIRenameDialog dialog(oper, manager->getDataByID(id), manager);
            // 对话框模态显示
            dialog.exec();
            // 更新树
            updateTree();
        }
        /**
         * @brief     删除数据对象
         * @param[i]  manager 数据管理器
         * @param[i]  id 数据ID
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-18
         */
        void deleteData(Core::FITKAbstractDataManagerPrivate* manager, int id);
        /**
         * @brief   获取Radioss算例数据对象
         * @return  Radioss::FITKRadiossCase* Radioss算例数据对象
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        Radioss::FITKRadiossCase* getRadiossCase();
        /**
         * @brief   更新树形菜单（全部刷新）
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        void updateTree();
        /**
         * @brief     界面显示/隐藏数据对象
         * @param[i]  id 数据ID
         * @param[i]  isShow 是否显示
         * @param[i]  is​Adaptive 是否自适应
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-22
         */
        void show(int id, bool isShow, bool is​Adaptive);
        /**
         * @brief   清除除全部高亮
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        void clearAllHighLight();
    };
}
#endif  // __OPER_TREE_ACTION_H__
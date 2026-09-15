/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperTreeAction.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManagerPrivate.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QAction>

namespace GUIOper
{
    void OperTreeAction::callOperator(QString registerName, QHash<QString, QVariant> hashArg)
    {
        // 从操作符仓库中获取指定名称的动作操作符实例
        // 如果获取失败则直接返回
        Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>(registerName);
        if (oper == nullptr) return;

        // 创建新的QAction对象并设置对象名称和发射器
        QAction* action = new QAction();
        action->setObjectName(registerName);
        oper->setEmitter(action);

        // 遍历参数哈希表，将所有参数设置到操作符中
        for (QString key : hashArg.keys())
            oper->setArgs(key, hashArg[key]);

        // 触发操作符的动作执行
        oper->actionTriggered();

        if (action) delete action;
        action = nullptr;
    }

    void OperTreeAction::deleteData(Core::FITKAbstractDataManagerPrivate* manager, int id)
    {
        // 从管理器中删除数据
        if (manager != nullptr && id > 0) manager->removeDataByID(id);
        // 清除高亮
        clearAllHighLight();
        // 更新树
        updateTree();
    }

    Radioss::FITKRadiossCase* OperTreeAction::getRadiossCase()
    {
        // 获取Radioss算例数据对象
        return FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    }

    void OperTreeAction::updateTree()
    {
        // 通过树形菜单事件操作器刷新树
        
        // 初始化树形菜单事件操作器列表
        QStringList list = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent","ResultTreeEvent" };

        // 遍历树形菜单事件操作器列表
        for (QString name : list)
        {
            // 获取并转换树形菜单事件操作器，如果获取成功则更新树形菜单
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(name));
            if (oper != nullptr) oper->updateTree();
        }
    }

    void OperTreeAction::show(int id, bool isShow, bool is​Adaptive)
    {
        // 获取图形事件操作器实例
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!oper) return;

        // 调用显示方法
        oper->setModelVisible(id, isShow, is​Adaptive);
    }

    void OperTreeAction::clearAllHighLight()
    {
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!oper) return;
        oper->clearAllHight();
        oper->render();
    }
}

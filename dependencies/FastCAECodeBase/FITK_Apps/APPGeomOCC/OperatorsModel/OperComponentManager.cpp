/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperComponentManager.h"

#include "GUIDialog/ComponentInfoDialog.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{
    bool OperComponentManager::execGUI()
    {
        // 先退出草图模式。
        exitSketchMode();

        // 尝试获取模型数据。
        Interface::FITKGlobalGeoComponent* group = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>()->getGlobalGeoCompManager()->getDataByID(m_Id);

        switch (m_OperType)
        {
        case Create:
        {
            // 显示模型参数窗口。
            showDialog();
            break;
        }
        case Edit:
        {
            // 显示移除特征窗口并加载历史数据。
            showDialog();
            break;
        }
        case Delete:
        {
            // 移除数据。
            auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
            auto compManageer = geoCmdList->getGlobalGeoCompManager();
            compManageer->removeDataByID(m_Id);
            // 刷新模型树。
            updateTree();
            break;
        }
        case Show:
        {
            if (!group)
            {
                return false;
            }

            // 启用数据。
            group->enable(true);

            // 更新可视化对象。
            EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (graphOper)
            {
                graphOper->updateGraph(m_Id);
            }

            break;
        }
        case Hide:
        {
            if (!group)
            {
                return false;
            }

            // 禁用数据。
            group->enable(false);

            // 更新可视化对象。
            EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (operGraph)
            {
                operGraph->updateGraph(m_Id);
            }

            break;
        }
        default:
            return false;
        }

        return false;
    }

    bool OperComponentManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        QString name;
        Interface::FITKModelEnum::FITKModelSetType type;
        QList<Interface::VirtualShape> members;
        bool flag = argValue("Name", name);
        flag &= argValue("Type", type);
        flag &= argValue("Members", members);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        auto compManageer = geoCmdList->getGlobalGeoCompManager();

        // 创建逻辑
        auto comp = compManageer->getDataByID(originCmdId);
        if (comp == nullptr) {
            comp = new Interface::FITKGlobalGeoComponent(type);
            comp->setDataObjectName(name);
            compManageer->appendDataObj(comp);
        }
        comp->clear();

        QHash<int, Interface::FITKGeoComponent*> mems{};

        for (auto m : members) {
            if (!mems.contains(m.CmdId)) {
                auto cmdComp = new Interface::FITKGeoComponent(type);
                auto cmd = geoCmdList->getDataByID(m.CmdId);
                if (cmd == nullptr) false;
                cmdComp->setModel(cmd->getShapeAgent()->getDataObjectID());
                mems.insert(m.CmdId, cmdComp);
            }
            auto c = mems.value(m.CmdId);
            c->appendMember(m.VirtualTopoIndex);
        }

        for (auto m : mems.values()) {
            comp->appendDataObj(m);
        }

        // 清除高亮。
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->clearHighlight();
        }

        // 暂存数据ID。
        setArgs("Id", comp->getDataObjectID());

        // 刷新模型树。
        updateTree();

        return true;
    }

    void OperComponentManager::showDialog()
    {
        Interface::FITKGlobalGeoComponent *obj{};
        if (m_OperType == Edit)
        {
            obj = dynamic_cast<Interface::FITKGlobalGeoComponent*>(FITKDATAREPO->getDataByID(m_Id));
            if (obj == nullptr) {
                printLog(3, tr("System Error."));
                return;
            }
        }

        QList<QDialog*> dlgs = FITKGUIREPO->getGUIObjects<QDialog>();
        for (QDialog* iDlg : dlgs)
        {
            if (iDlg && !dynamic_cast<GUI::ComponentInfoDialog*>(iDlg))
            {
                iDlg->close();
            }
        }


        // 彻底关闭拾取状态。
        EventOper::GraphInteractionOperator* operPick = FITKOPERREPO->getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");
        if (operPick)
        {
            operPick->clearPickMode();
        }

        GUI::ComponentInfoDialog* dlg = GUI::ComponentInfoDialog::New(m_MainWindow, this, obj);

        // 显示模型参数窗口。
        dlg->show();
    }

} // namespace ModelOper

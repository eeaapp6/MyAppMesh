/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperUndoRedo.h"

// Qt
#include <QDialog>

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKGUIRepo.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImport.h"

// Operator
#include "OperatorsInterface/GraphEventOperator.h"

namespace ModelOper
{
    // 撤销。
    //@{
    bool OperUndo::execGUI()
    {
        // 撤销无需界面操作。
        return true;
    }

    bool OperUndo::execProfession()
    {
        // 判断是否为草绘模式。
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return false;
        }

        // 判断是否为建模窗口弹出状态。
        QList<QDialog*> dlgs = FITKGUIREPO->getGUIObjects<QDialog>();
        for (QDialog* dlg : dlgs)
        {
            if (!dlg)
            {
                continue;
            }

            if (dlg->isVisible())
            {
                return false;
            }
        }

        // 执行撤销。
        Interface::FITKAbsGeoCommand* cmd = cmdList->getDataByIndex(cmdList->getDataCount() - 1);
        if (!cmd)
        {
            return false;
        }

        bool flag = cmdList->undo();
        if (!flag)
        {
            return false;
        }

        // 刷新被操作的对象及上游对象。
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateGraphRecursively(cmd->getDataObjectID(), -1, false);
        }

        // 同步移除几何分组。 2025/09/02
        //@{
        // 获取全局几何分组管理器。
        Interface::FITKGlobalGeoComponentManager* globalGeoCompMgr = cmdList->getGlobalGeoCompManager();
        if (globalGeoCompMgr)
        {
            QList<int> cmdIds;
            QList<Interface::FITKAbsGeoCommand*> rootCmds = cmdList->getRootCommandList();
            for (Interface::FITKAbsGeoCommand* cmd : rootCmds)
            {
                cmdIds.push_back(cmd->getDataObjectID());
            }

            int nComps = globalGeoCompMgr->getDataCount();
            for (int i = nComps - 1; i >= 0; i--)
            {
                // 检查子分组。
                Interface::FITKGlobalGeoComponent* globalComp = globalGeoCompMgr->getDataByIndex(i);
                if (!globalComp)
                {
                    continue;
                }

                // 获取子分组数量。
                int nSubComps = globalComp->getDataCount();
                if (nSubComps == 0)
                {
                    continue;
                }

                for (int j = nSubComps - 1; j >= 0; j--)
                {
                    Interface::FITKGeoComponent* subComp = globalComp->getDataByIndex(j);
                    if (!subComp)
                    {
                        continue;
                    }

                    Interface::FITKAbsGeoShapeAgent* shapeAgent = subComp->getAbstractModelAs<Interface::FITKAbsGeoShapeAgent>();
                    if (!shapeAgent)
                    {
                        continue;
                    }

                    Interface::FITKAbsGeoCommand* compCmd = shapeAgent->getGeoCommand();
                    if (cmdIds.contains(compCmd->getDataObjectID()))
                    {
                        continue;
                    }

                    // 移除失效子分组。
                    globalComp->removeDataByIndex(j);
                }

                // 移除失效全局分组。
                if (globalComp->getDataCount() == 0)
                {
                    globalGeoCompMgr->removeDataByIndex(i);
                }
            }
        }
        //@}

        // 刷新模型树。
        if (m_MainWindow)
        {
            m_MainWindow->updateGeometryTree();
        }

        return true;
    }
    //@}

    // 重做。
    //@{
    bool OperRedo::execGUI()
    {
        // 重做无需界面操作。
        return true;
    }

    bool OperRedo::execProfession()
    {
        // 判断是否为草绘模式。
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return false;
        }

        // 判断是否为建模窗口弹出状态。
        QList<QDialog*> dlgs = FITKGUIREPO->getGUIObjects<QDialog>();
        for (QDialog* dlg : dlgs)
        {
            if (!dlg)
            {
                continue;
            }

            if (dlg->isVisible())
            {
                return false;
            }
        }

        // 执行重做。
        bool flag = cmdList->redo();
        if (!flag)
        {
            return false;
        }

        Interface::FITKAbsGeoCommand* cmd = cmdList->getDataByIndex(cmdList->getDataCount() - 1);
        if (!cmd)
        {
            return false;
        }

        // 刷新被操作的对象及上游对象。
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateGraphRecursively(cmd->getDataObjectID(), -1, false);
        }

        // 刷新模型树。
        if (m_MainWindow)
        {
            m_MainWindow->updateGeometryTree();
        }

        return true;
    }
    //@}
} // namespace ModelOper

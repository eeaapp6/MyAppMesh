/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperCreateGeoManagerBase.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDelete.h"

// Operator
#include "OperatorsInterface/GraphEventOperator.h"

// GUI
#include "GUIFrame/MainWindow.h"

namespace ModelOper
{
    bool OperCreateGeoManagerBase::execGUI()
    {
        // 先退出草图模式。
        exitSketchMode();

        // 尝试获取模型数据。
        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>
            (FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>()->getDataByID(m_Id));

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
            deleteCmd(m_Id);

            break;
        }
        case Show:
        {
            if (!cmd)
            {
                return false;
            }

            // 启用数据。
            cmd->enable(true);

            // 更新可视化对象。
            EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (operGraph)
            {
                operGraph->updateGraph(m_Id);
                operGraph->clearHighlight();
            }

            break;
        }
        case Hide:
        {
            if (!cmd)
            {
                return false;
            }

            // 禁用数据。
            cmd->enable(false);

            // 更新可视化对象。
            EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (operGraph)
            {
                operGraph->updateGraph(m_Id);
                operGraph->clearHighlight();
            }

            break;
        }
        default:
            return false;
        }

        return false;
    }

    bool OperCreateGeoManagerBase::execProfession()
    {
        updateTree();

        return true;
    }

    void OperCreateGeoManagerBase::finalize(bool updateCommand)
    {
        int histId = -1;
        bool flag = argValue("Id", histId);
        if (!flag)
        {
            return;
        }

        // 尝试获取命令数据。
        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>
            (FITKDATAREPO->getDataByID(histId));

        // 编辑状态还原引用计数并重新启用数据。
        if (!cmd)
        {
            return;
        }

        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");

        // 基准元素仅启用并刷新自己即可。
        if (cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTDatum)
        {
            cmd->enable(true);

            if (operGraph)
            {
                operGraph->updateGraphRecursively(histId, 0);
            }
        }
        else
        {
            // 先还原引用关系。
            if (!cmd->getEnableReferenceList())
            {
                cmd->enableReferenceList(true);
            }

            // 递归更新命令。
            if (updateCommand)
            {
                updateCommandRecursively(cmd);
            }

            // 递归启用数据。
            setRefEnableRecursively(cmd, true, true);

            // 向上向下刷新可视化对象。
            if (operGraph)
            {
                operGraph->updateGraphRecursively(histId, -1, false);
                operGraph->updateGraphRecursively(histId, 1, true);
            }
        }

        // 刷新模型树。
        updateTree();
    }

    void OperCreateGeoManagerBase::setRefEnableRecursively(Interface::FITKAbsGeoCommand* cmd, bool flag, bool recDown)
    {
        if (!cmd)
        {
            return;
        }

        cmd->enable(flag);

        // 获取引用对象数据。
        if (recDown)
        {
            int nRefed = cmd->getReferencedCmdCount();
            for (int i = 0; i < nRefed; i++)
            {
                Interface::FITKAbsGeoCommand* subCmd = cmd->getReferencedCmdByIndex(i);
                if (!subCmd)
                {
                    continue;
                }

                // 递归刷新属性。
                setRefEnableRecursively(subCmd, flag, recDown);
            }
        }
        else
        {
            int nRef = cmd->getReferenceCmdCount();
            for (int i = 0; i < nRef; i++)
            {
                Interface::FITKAbsGeoCommand* subCmd = cmd->getReferenceCmdByIndex(i);
                if (!subCmd)
                {
                    continue;
                }

                // 递归刷新属性。
                setRefEnableRecursively(subCmd, flag, recDown);
            }
        }
    }

    void OperCreateGeoManagerBase::updateCommandRecursively(Interface::FITKAbsGeoCommand* cmd)
    {
        if (!cmd)
        {
            return;
        }

        cmd->update();

        int nRefed = cmd->getReferencedCmdCount();
        for (int i = 0; i < nRefed; i++)
        {
            Interface::FITKAbsGeoCommand* subCmd = cmd->getReferencedCmdByIndex(i);
            if (!subCmd)
            {
                continue;
            }

            // 递归刷新属性。
            updateCommandRecursively(subCmd);
        }
    }
} // namespace ModelOper
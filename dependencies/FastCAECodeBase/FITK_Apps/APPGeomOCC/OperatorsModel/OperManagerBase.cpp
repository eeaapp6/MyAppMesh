/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperManagerBase.h"
#include "GUIFrame/MainWindow.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

// Qt
#include <QAction>

// App
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoMultiDelete.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

// Operator
#include "OperatorsInterface/GraphEventOperator.h"

namespace ModelOper
{
    /**
     * @brief OperManagerBase的构造函数
     *
     * 该构造函数初始化OperManagerBase类的一个实例。它通过动态类型转换从FITKAPP的全局数据中获取GUI::MainWindow的指针，并将其赋值给_mainWindow成员变量。
     *
     * @param args 构造函数参数列表（当前未列出具体参数，表示可能有可选的参数传递给构造函数）
     */
    OperManagerBase::OperManagerBase(/* args */)
    {
        m_MainWindow = dynamic_cast<GUI::MainWindow *>(FITKAPP->getGlobalData()->getMainWindow());
    }

    /**
     * @brief OperManagerBase的析构函数
     *
     * 该析构函数释放OperManagerBase类实例的资源，是一个空函数体。
     */
    OperManagerBase::~OperManagerBase() = default;

    void OperManagerBase::preArgs()
    {
        preEmitter();
        preItemArgs();
    }

    /**
     * @brief 准备发射器操作前的初始化工作。
     * 该函数根据发射器对象的名称后缀，来判断操作的类型，并设置操作类型变量。
     * 如果发射器对象为空，则不执行任何操作。
     */
    void OperManagerBase::preEmitter()
    {
        // 初始化操作类型为未知
        m_OperType = None;
        if (_emitter == nullptr)
            return;
        QString name = _emitter->objectName();

        // 根据对象名的后缀，判断并设置操作类型
        if (name.endsWith("Edit"))
            m_OperType = Edit;
        else if (name.endsWith("Create"))
            m_OperType = Create;
        else if (name.endsWith("Delete"))
            m_OperType = Delete;
        else if (name.endsWith("Rename"))
            m_OperType = Rename;
        else if (name.endsWith("Copy"))
            m_OperType = Copy;
        else if (name.endsWith("Hide"))
            m_OperType = Hide;
        else if (name.endsWith("Show"))
            m_OperType = Show;

    }

    /**
     * 准备操作项参数
     * 本函数用于根据传入的参数预先设置操作的类型及相关ID，同时收集额外的信息。
     * 该函数不接受参数，也不返回任何值。
     */
    void OperManagerBase::preItemArgs()
    {
        bool ok = false;
        // 尝试获取案例ID和对象ID
        int objID = -1;
        ok = argValue<int>("ID", objID);
        if (ok)
            m_Id = objID;

        // 清除并更新额外信息
        m_AddtionalInfo.clear();
        this->argValue<QStringList>("info", m_AddtionalInfo);
    }

    // 打印日志 added by ChengHaotian 2024/08/27
    void OperManagerBase::printLog(int type, QString msg)
    {
        AppFrame::FITKSignalTransfer* sigTrans = FITKAPP->getSignalTransfer();
        if (sigTrans)
        {
            sigTrans->outputMessageSig(type, msg);
        }
    }
    
    void OperManagerBase::deleteCmd(int id)
    {
        // 移除数据。
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 获取被移除命令以及引用该命令列表一并移除。
        Interface::FITKAbsGeoCommand* cmd = cmdList->getDataByID(m_Id);
        if (!cmd)
        {
            return;
        }

        // 创建移除指令。
        Interface::FITKAbsGeoMultiDelete* deleteCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT
            <Interface::FITKAbsGeoMultiDelete>(Interface::FITKGeoEnum::FGTMultiDelete);
        if (!deleteCmd)
        {
            return;
        }

        deleteCmd->setDataObjectName(QString("Delete-%1").arg(cmdList->getDataCount()));
       
        QList<Interface::FITKAbsGeoCommand*> refedCmdList = cmd->getDownStreamCommands();

        QList<int> ids;
        for (Interface::FITKAbsGeoCommand* refedCmd : refedCmdList)
        {
            if (refedCmd)
            {
                ids.push_back(refedCmd->getDataObjectID());
            }
        }

        // 设置被移除命令ID列表并添加至命令列表。
        deleteCmd->setDeleteCommandIDs(ids);
        deleteCmd->update();
        cmdList->appendDataObj(deleteCmd);

        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");

        if (operGraph)
        {
            // 刷新被移除对象可见性。
            for (const int & id : ids)
            {
                operGraph->updateGraphRecursively(id, 0);
            }

            // 向上刷新一级可视化对象。
            operGraph->updateGraphRecursively(m_Id, 1, false);
        }

        // 同步移除几何分组。 2025/06/16
        //@{
        // 获取全局几何分组管理器。
        Interface::FITKGlobalGeoComponentManager* globalGeoCompMgr = cmdList->getGlobalGeoCompManager();
        if (globalGeoCompMgr)
        {
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
                    if (!ids.contains(compCmd->getDataObjectID()))
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
        updateTree();
    }

    void OperManagerBase::exitSketchMode()
    {
        if (!m_MainWindow)
        {
            return;
        }

        // 获取草图按钮。
        QAction* action =  m_MainWindow->getSketchAction();
        if (!action)
        {
            return;
        }

        // 检查是否在草图状态。
        bool isSketchMode = action->isChecked();
        if (!isSketchMode)
        {
            return;
        }

        // 取消勾选，发送信号。
        action->setChecked(false);
        action->triggered();
    }

    void OperManagerBase::updateTree()
    {
        // 刷新模型树。
        if (m_MainWindow)
        {
            m_MainWindow->updateGeometryTree();
        }
    }
} // namespace ModelOper
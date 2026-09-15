/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIDialogBase.h"
#include "GUIFrame/MainWindow.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDelete.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

namespace GUI
{
    GUIDialogBase::GUIDialogBase(QWidget* parent) :
        Core::FITKDialog(parent)
    {
        // 从全局获取主窗口
        m_MainWindow = dynamic_cast<MainWindow*>(FITKGLODATA->getMainWindow());
        // 禁止使用该标志
        //setAttribute(Qt::WA_DeleteOnClose);

        // 初始化前处理操作器。
        m_GraphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");

        // 初始化前处理工具可视化操作器。
        m_GraphToolOper = FITKOPERREPO->getOperatorT<EventOper::GraphToolOperator>("GraphTool");

        // 初始化前处理拾取功能操作器。
        m_GraphPickOper = FITKOPERREPO->getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");

        // 初始化几何命令列表。
        m_CmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (m_CmdList)
        {
            // 初始化基准元素命令列表。
            m_DatumList = m_CmdList->getDatumManager();
        }

        // 去除对话框右上角问号。
        setWindowFlags(windowFlags() &~ Qt::WindowContextHelpButtonHint);
    }

    GUIDialogBase::~GUIDialogBase() = default;

    bool GUIDialogBase::execProfession() {
        // 如果操作器不为空则回调操作器的方法
        if (m_Operator == nullptr) return false;
        return m_Operator->execProfession();
    }

    void GUIDialogBase::showEvent(QShowEvent *event)
    {
        Core::FITKDialog::showEvent(event);
    }

    void GUIDialogBase::closeEvent(QCloseEvent * e)
    {
        // 窗口关闭前释放资源
        if (this->isVisible())
        {
            finalize();

            // 清除缓存。
            if (m_Operator)
            {
                m_Operator->finalize();
            }
        }

        Core::FITKDialog::closeEvent(e);
    }

    void GUIDialogBase::hideEvent(QHideEvent * event)
    {
        if (this->isVisible())
        {
            finalize();

            // 清除缓存。
            if (m_Operator)
            {
                m_Operator->finalize();
            }
        }

        Core::FITKDialog::hideEvent(event);
    }

    void GUIDialogBase::finalize()
    {
    }

    // 打印日志 added by ChengHaotian 2024/08/27
    void GUIDialogBase::printLog(int type, QString msg)
    {
        AppFrame::FITKSignalTransfer* sigTrans = FITKAPP->getSignalTransfer();
        if (sigTrans)
        {
            sigTrans->outputMessageSig(type, msg);
        }
    }

    void GUIDialogBase::updateGraph(int id)
    {
        // 更新可视化对象并刷新可视化窗口。
        if (m_GraphOper)
        {
            m_GraphOper->updateGraph(id);
        }
    }

    void GUIDialogBase::previewGraph(int id, EventOper::PreviewType type, QColor color)
    {
        // 创建或更新预览对象并刷新可视化窗口。
        if (m_GraphOper)
        {
            m_GraphOper->preview(id, type, color);
        }
    }

    void GUIDialogBase::clearPreview()
    {
        // 清除当前所有预览对象。
        if (m_GraphOper)
        {
            m_GraphOper->clearPreview();
        }
    }

    void GUIDialogBase::accept()
    {
        QDialog::accept();

        //// 初始化前处理操作器。
        //EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        //if (!operGraph)
        //{
        //    return;
        //}

        //// 检查数据ID。
        //Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(m_dataId);
        //if (!cmd)
        //{
        //    return;
        //}

        //// 更新可视化对象。
        //operGraph->updateGraph(m_dataId);

        //// 更新引用数据对象。
        //int nRefCmd = cmd->getReferenceCmdCount();
        //for (int i = 0; i < nRefCmd; i++)
        //{
        //    Interface::FITKAbsGeoCommand* cmdRef = cmd->getReferenceCmdByIndex(i);
        //    if (!cmdRef)
        //    {
        //        continue;
        //    }

        //    operGraph->updateGraph(cmdRef->getDataObjectID());
        //}
    }

    void GUIDialogBase::reject()
    {
        //// 清除缓存。
        //if (m_Operator)
        //{
        //    m_Operator->finalize();
        //}

        QDialog::reject();
    }
}


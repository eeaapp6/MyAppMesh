/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ChamferInfoDialog.h"
#include "ui_ChamferInfoDialog.h"

// Qt
#include <QDoubleValidator>
#include <QVariant>
#include <QVariantList>

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperSolid.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

// Pick
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"

namespace GUI 
{
    ChamferInfoDialog::ChamferInfoDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper) :
        GUIDialogBase(parent),
        ui(new Ui::ChamferInfoDialog)
    {
        ui->setupUi(this);

        // 暂存操作器。
        m_Operator = oper;

        // 初始化界面。
        init();
    }

    ChamferInfoDialog::~ChamferInfoDialog()
    {
        // 移除预览数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }

        delete ui;
    }

    ChamferInfoDialog* ChamferInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, 
        Interface::FITKAbsGeoCommand* cmd)
    {
        static ChamferInfoDialog* w;
        if (!w)
        {
            w = new ChamferInfoDialog(parent, oper);
        }

        // 读取历史数据，刷新界面。
        Interface::FITKAbsGeoModelChamferSolid* cmdCha = dynamic_cast<Interface::FITKAbsGeoModelChamferSolid*>(cmd);
        if (cmdCha)
        {
            w->m_dataId = cmdCha->getDataObjectID();
            w->m_tempCmdId = cmdCha->getInputCmdId();
            w->m_tempEdgeTopos = cmdCha->getEdgeTopos();
            w->m_tempDist = cmdCha->getDistance();
        }
        else
        {
            w->m_dataId = -1;
            w->m_tempCmdId = -1;
            w->m_tempEdgeTopos.clear();
            w->m_tempDist = 1.;
        }

        // 刷新界面。
        w->reload();

        return w;
    }

    void ChamferInfoDialog::init()
    {
        // 绑定界面控件信号。
        //@{
        connect(ui->cmbType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChamferInfoDialog::slot_typeChanged);
        connect(ui->btnOK, &QPushButton::clicked, this, &ChamferInfoDialog::slot_ok);
        connect(ui->btnPreview, &QPushButton::clicked, this, &ChamferInfoDialog::slot_preview);
        connect(ui->btnCancel, &QPushButton::clicked, this, &ChamferInfoDialog::slot_cancel);
        connect(ui->btnPick, &QPushButton::clicked, this, &ChamferInfoDialog::slot_select);
        connect(ui->btnPickOK, &QPushButton::clicked, this, &ChamferInfoDialog::slot_deselect);
        //@}

        // 初始化创建类型。
        ui->cmbType->setCurrentIndex(0);
        ui->btnPickOK->hide();

        // 初始化默认参数。
        ui->editLength->setValidator(new QDoubleValidator);
        ui->editLength->setText("1.0");
    }

    void ChamferInfoDialog::reload()
    {
        // 刷新界面。
        ui->editLength->setText(QString::number(m_tempDist));

        // 刷新拾取信息。
        updateEdgesLabel();

        // 检测是否有数据被删除。
        if (!isInputValid())
        {
            clearSelectData();
        }
    }

    void ChamferInfoDialog::disconnectSelectSigs()
    {
        // 断开拾取边信号。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &ChamferInfoDialog::slot_shapeSelected);
    }

    void ChamferInfoDialog::updateEdgesLabel()
    {
        int nEdges = m_tempEdgeTopos.count();
        ui->labelEdges->setText(tr("Picked (%1)").arg(nEdges));
    }

    void ChamferInfoDialog::clearSelectData()
    {
        m_tempCmdId = -1;
        m_tempEdgeTopos.clear();

        updateEdgesLabel();
    }

    bool ChamferInfoDialog::isInputValid()
    {
        // 检测数据ID及参数。
        double dist = ui->editLength->text().toDouble();
        if (dist <= 0)
        {
            return false;
        }

        if (m_tempCmdId == -1 || m_tempEdgeTopos.count() == 0)
        {
            return false;
        }

        // 检查边数据是否存在。
        Interface::FITKAbsGeoCommand* cmd = m_CmdList->getDataByID(m_tempCmdId);
        if (!cmd)
        {
            return false;
        }

        QList<int> edgeIndice;
        for (const Interface::VirtualShape & vEdge : m_tempEdgeTopos)
        {
            edgeIndice.push_back(vEdge.VirtualTopoIndex);
        }

        if (cmd->getVirtualToposByIndice(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edgeIndice).count() !=
            m_tempEdgeTopos.count())
        {
            return false;
        }

        return true;
    }

    void ChamferInfoDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 断开拾取处理信号。
        disconnectSelectSigs();

        // 退出拾取状态。
        slot_deselect();

        // 清空拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->clearPickedData();

        // 还原界面。
        ui->btnPick->show();
        ui->btnPickOK->hide();

        // 删除预览模型数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }
    }

    void ChamferInfoDialog::slot_typeChanged(int index)
    {
        if (index < 0 || index >= ui->stackedWidget->count())
        {
            return;
        }

        // 切换页面。
        ui->stackedWidget->setCurrentIndex(index);
    }

    void ChamferInfoDialog::slot_ok()
    {
        if (!isInputValid() || !m_Operator)
        {
            return;
        }

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("InputCmdId", m_tempCmdId);
        m_Operator->setArgs("InputEdgeIndice", QVariant::fromValue(m_tempEdgeTopos));
        m_Operator->setArgs("InputDistance", ui->editLength->text().toDouble());

        // 执行功能。
        bool flag = m_Operator->execProfession();
        if (!flag)
        {
            return;
        }

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);

        // 结束创建模型。
        finalize();

        GUIDialogBase::accept();
    }

    void ChamferInfoDialog::slot_preview()
    {
        if (!isInputValid())
        {
            return;
        }

        if (!m_previewCmd)
        {
            m_previewCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->
                createCommandT<Interface::FITKAbsGeoModelChamferSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTChamferSolid);
        }

        // 设置参数生成数据。
        m_previewCmd->setInputCmdId(m_tempCmdId);
        m_previewCmd->setEdgeTopos(m_tempEdgeTopos);
        m_previewCmd->setDistance(ui->editLength->text().toDouble());
        bool flag = m_previewCmd->update();
        if (!flag)
        {
            return;
        }

        // 预览对象并刷新可视化窗口。
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper)
        {
            graphOper->preview(m_previewCmd->getDataObjectID());
        }
    }

    void ChamferInfoDialog::slot_cancel()
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::reject();
    }

    void ChamferInfoDialog::slot_select()
    {
        // 多选边模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJEdge;

        GUIPickInfo::SetPickInfo(pickInfo);
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &ChamferInfoDialog::slot_shapeSelected, Qt::UniqueConnection);

        ui->btnPickOK->show();
        ui->btnPick->hide();
    }

    void ChamferInfoDialog::slot_deselect()
    {
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &ChamferInfoDialog::slot_shapeSelected);

        ui->btnPickOK->hide();
        ui->btnPick->show();
    }

    void ChamferInfoDialog::slot_shapeSelected()
    {
        // 清除拾取数据重新进行数据提取。
        m_tempCmdId = -1;
        m_tempEdgeTopos.clear();

        // 获取拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        QList<GraphData::GraphPickedData*> pickList = pickDataProvider->getPickedList();

        for (GraphData::GraphPickedData* pickData : pickList)
        {          
            // 获取模型相关数据。           
            Interface::FITKAbsGeoShapeAgent* shapeAgent = pickData->getGeoShapeAgent();
            QList<Interface::FITKAbsVirtualTopo*> vTopos = pickData->getVirtualTopoAll();
            if (vTopos.count() == 0 || !shapeAgent)
            {
                continue;
            }

            Interface::FITKAbsGeoCommand* geoCmd = shapeAgent->getGeoCommand();
            int newCmdId = geoCmd->getDataObjectID();

            if (m_tempCmdId == -1)
            {
                m_tempCmdId = newCmdId;
            }
            else
            {
                // 拾取到不同模型数据，清空历史拾取。
                if (m_tempCmdId != newCmdId)
                {
                    pickDataProvider->clearPickedData();
                    clearSelectData();
                    return;
                }
            }

            // 保存拾取到的索引。
            m_tempEdgeTopos = pickData->getVirtualShapes();
        }

        // 刷新拾取边计数。
        updateEdgesLabel();
    }
}
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "NBodyUniteInfoDialog.h"
#include "ui_NBodyUniteInfoDialog.h"

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
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperNBodyUnite.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

// Pick
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"

namespace GUI 
{
    NBodyUniteInfoDialog::NBodyUniteInfoDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper) :
        GUIDialogBase(parent),
        ui(new Ui::NBodyUniteInfoDialog)
    {
        ui->setupUi(this);

        // 暂存操作器。
        m_Operator = oper;

        // 初始化界面。
        init();
    }

    NBodyUniteInfoDialog::~NBodyUniteInfoDialog()
    {
        // 移除预览数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }

        delete ui;
    }

    NBodyUniteInfoDialog* NBodyUniteInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, 
        Interface::FITKAbsGeoCommand* cmd)
    {
        static NBodyUniteInfoDialog* w;
        if (!w)
        {
            w = new NBodyUniteInfoDialog(parent, oper);
        }

        // 读取历史数据，刷新界面。
        Interface::FITKAbsGeoOperNBodyUnite* cmdNBUnite = dynamic_cast<Interface::FITKAbsGeoOperNBodyUnite*>(cmd);
        if (cmdNBUnite)
        {
            w->m_dataId = cmdNBUnite->getDataObjectID();
            w->m_tempSolidTopos = cmdNBUnite->getShapes();
        }
        else
        {
            w->m_dataId = -1;
            w->m_tempSolidTopos.clear();
        }

        // 刷新界面。
        w->reload();

        return w;
    }

    void NBodyUniteInfoDialog::init()
    {
        // 绑定界面控件信号。
        //@{
        connect(ui->cmbType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NBodyUniteInfoDialog::slot_typeChanged);
        connect(ui->btnOK, &QPushButton::clicked, this, &NBodyUniteInfoDialog::slot_ok);
        connect(ui->btnPreview, &QPushButton::clicked, this, &NBodyUniteInfoDialog::slot_preview);
        connect(ui->btnCancel, &QPushButton::clicked, this, &NBodyUniteInfoDialog::slot_cancel);
        connect(ui->btnPick, &QPushButton::clicked, this, &NBodyUniteInfoDialog::slot_select);
        connect(ui->btnPickOK, &QPushButton::clicked, this, &NBodyUniteInfoDialog::slot_deselect);
        //@}

        // 初始化创建类型。
        ui->cmbType->setCurrentIndex(0);
        ui->btnPickOK->hide();
    }

    void NBodyUniteInfoDialog::reload()
    {
        // 刷新拾取信息。
        updateSolidsLabel();

        // 检测是否有数据被删除。
        if (!isInputValid())
        {
            clearSelectData();
        }
    }

    void NBodyUniteInfoDialog::disconnectSelectSigs()
    {
        // 断开拾取边信号。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &NBodyUniteInfoDialog::slot_shapeSelected);
    }

    void NBodyUniteInfoDialog::updateSolidsLabel()
    {
        int nSolids = m_tempSolidTopos.count();
        ui->labelSolids->setText(tr("Picked (%1)").arg(nSolids));
    }

    void NBodyUniteInfoDialog::clearSelectData()
    {
        m_tempSolidTopos.clear();

        updateSolidsLabel();
    }

    bool NBodyUniteInfoDialog::isInputValid()
    {
        // 检测数据ID及参数。
        if (m_tempSolidTopos.count() < 2)
        {
            return false;
        }

        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return false;
        }

        for (const Interface::VirtualShape & vSolid : m_tempSolidTopos)
        {
            Interface::FITKAbsGeoCommand* cmd = cmdList->getDataByID(vSolid.CmdId);
            if (!cmd)
            {
                return false;
            }

            Interface::FITKShapeVirtualTopoManager* vSolidMgr = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSSolid);
            if (!vSolidMgr)
            {
                return false;
            }

            if (!vSolidMgr->getDataByIndex(vSolid.VirtualTopoIndex))
            {
                return false;
            }
        }

        return true;
    }

    void NBodyUniteInfoDialog::finalize()
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

    void NBodyUniteInfoDialog::slot_typeChanged(int index)
    {
        if (index < 0 || index >= ui->stackedWidget->count())
        {
            return;
        }

        // 切换页面。
        ui->stackedWidget->setCurrentIndex(index);
    }

    void NBodyUniteInfoDialog::slot_ok()
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
        m_Operator->setArgs("InputSolidIndice", QVariant::fromValue(m_tempSolidTopos));

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

    void NBodyUniteInfoDialog::slot_preview()
    {
        if (!isInputValid())
        {
            return;
        }

        if (!m_previewCmd)
        {
            m_previewCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->
                createCommandT<Interface::FITKAbsGeoOperNBodyUnite>(Interface::FITKGeoEnum::FITKGeometryComType::FGTNBodyUnite);
        }

        // 设置参数生成数据。
        for (Interface::VirtualShape & vSolid : m_tempSolidTopos)
        {
            m_previewCmd->addShape(vSolid);
        }

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

    void NBodyUniteInfoDialog::slot_cancel()
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::reject();
    }

    void NBodyUniteInfoDialog::slot_select()
    {
        // 多选实体模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJSolid;

        GUIPickInfo::SetPickInfo(pickInfo);
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &NBodyUniteInfoDialog::slot_shapeSelected, Qt::UniqueConnection);

        ui->btnPickOK->show();
        ui->btnPick->hide();
    }

    void NBodyUniteInfoDialog::slot_deselect()
    {
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &NBodyUniteInfoDialog::slot_shapeSelected);

        ui->btnPickOK->hide();
        ui->btnPick->show();
    }

    void NBodyUniteInfoDialog::slot_shapeSelected()
    {
        // 清除拾取数据重新进行数据提取。
        m_tempSolidTopos.clear();

        // 获取拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        QList<GraphData::GraphPickedData*> pickList = pickDataProvider->getPickedList();

        for (GraphData::GraphPickedData* pickData : pickList)
        {          
            // 保存拾取到的索引。
            m_tempSolidTopos << pickData->getVirtualShapes();
        }

        // 刷新拾取实体计数。
        updateSolidsLabel();
    }
}
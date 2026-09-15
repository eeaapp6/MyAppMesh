/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "DefeatureInfoDialog.h"
#include "ui_DefeatureInfoDialog.h"

// Qt
#include <QDoubleValidator>

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
    DefeatureInfoDialog::DefeatureInfoDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper) :
        GUIDialogBase(parent),
        ui(new Ui::DefeatureInfoDialog)
    {
        ui->setupUi(this);

        // 暂存操作器。
        m_Operator = oper;

        // 初始化界面。
        init();
    }

    DefeatureInfoDialog::~DefeatureInfoDialog()
    {
        // 移除预览数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }

        delete ui;
    }

    DefeatureInfoDialog* DefeatureInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, 
        Interface::FITKAbsGeoCommand* cmd)
    {
        static DefeatureInfoDialog* w;
        if (!w)
        {
            w = new DefeatureInfoDialog(parent, oper);
        }

        // 保存数据类型，刷新界面文本。
        w->m_geoType = Interface::FITKGeoEnum::FITKGeometryComType::FGTDefeature;

        // 读取历史数据，刷新界面。
        Interface::FITKAbsGeoModelDefeatureSolid* cmdDef = dynamic_cast<Interface::FITKAbsGeoModelDefeatureSolid*>(cmd);
        if (cmdDef)
        {
            w->m_dataId = cmdDef->getDataObjectID();
            w->m_tempCmdId = cmdDef->getInputCmdId();
            w->m_tempFaceTopos = cmdDef->getFaceTopos();
        }
        else
        {
            w->m_dataId = -1;
            w->m_tempCmdId = -1;
            w->m_tempFaceTopos.clear();
        }

        w->reload();

        return w;
    }

    void DefeatureInfoDialog::init()
    {
        // 绑定界面控件信号。
        //@{
        connect(ui->cmbType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DefeatureInfoDialog::slot_typeChanged);
        connect(ui->btnOK, &QPushButton::clicked, this, &DefeatureInfoDialog::slot_ok);
        connect(ui->btnPreview, &QPushButton::clicked, this, &DefeatureInfoDialog::slot_preview);
        connect(ui->btnCancel, &QPushButton::clicked, this, &DefeatureInfoDialog::slot_cancel);
        connect(ui->btnPick, &QPushButton::clicked, this, &DefeatureInfoDialog::slot_select);
        connect(ui->btnPickOK, &QPushButton::clicked, this, &DefeatureInfoDialog::slot_deselect);
        //@}

        // 初始化创建类型。
        ui->cmbType->setCurrentIndex(0);
        ui->btnPickOK->hide();
    }

    void DefeatureInfoDialog::reload()
    {
        // 刷新类型名。
        ui->cmbType->clear();

        // 刷新界面文本。
        switch (m_geoType)
        {
        case Interface::FITKGeoEnum::FGTDefeature:
            ui->cmbType->addItem(tr("Remove Feature"));
            break;
        case Interface::FITKGeoEnum::FGTRemoveChamfer:
            ui->cmbType->addItem(tr("Remove Chamfer"));
            break;
        case Interface::FITKGeoEnum::FGTRemoveFillet:
            ui->cmbType->addItem(tr("Remove Fillet"));
            break;
        case Interface::FITKGeoEnum::FGTFillHole:
            ui->cmbType->addItem(tr("Fill Hole"));
            break;
        default:
            return;
        }

        // 刷新拾取信息。
        updateFacesLabel();

        // 检测是否有数据被删除。
        if (!isInputValid())
        {
            clearSelectData();
        }
    }

    void DefeatureInfoDialog::disconnectSelectSigs()
    {
        // 断开拾取边信号。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &DefeatureInfoDialog::slot_shapeSelected);
    }

    void DefeatureInfoDialog::updateFacesLabel()
    {
        int nFaces = m_tempFaceTopos.count();
        ui->labelFace->setText(tr("Picked (%1)").arg(nFaces));
    }

    void DefeatureInfoDialog::clearSelectData()
    {
        m_tempCmdId = -1;
        m_tempFaceTopos.clear();

        updateFacesLabel();
    }

    bool DefeatureInfoDialog::isInputValid()
    {
        // 检测数据ID。
        if (m_tempCmdId == -1 || m_tempFaceTopos.isEmpty())
        {
            return false;
        }

        // 检查面数据是否存在。
        Interface::FITKAbsGeoCommand* cmd = m_CmdList->getDataByID(m_tempCmdId);
        if (!cmd)
        {
            return false;
        }

        QList<int> faceIndice;
        for (const Interface::VirtualShape & vFace : m_tempFaceTopos)
        {
            faceIndice.push_back(vFace.VirtualTopoIndex);
        }

        if (cmd->getVirtualToposByIndice(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, faceIndice).count() !=
            m_tempFaceTopos.count())
        {
            return false;
        }

        return true;
    }

    void DefeatureInfoDialog::finalize()
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

    void DefeatureInfoDialog::slot_typeChanged(int index)
    {
        if (index < 0 || index >= ui->stackedWidget->count())
        {
            return;
        }

        // 切换页面。
        ui->stackedWidget->setCurrentIndex(index);
    }

    void DefeatureInfoDialog::slot_ok()
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
        m_Operator->setArgs("InputFaceIndice", QVariant::fromValue(m_tempFaceTopos));
        m_Operator->setArgs("InputGeoCmdType", QVariant::fromValue(m_geoType));

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

        // 结束创建模型。
        finalize();

        GUIDialogBase::accept();
    }

    void DefeatureInfoDialog::slot_preview()
    {
        if (!isInputValid())
        {
            return;
        }

        // 判断类型是否发生变化。
        if (m_previewCmd)
        {
            if (m_previewCmd->getGeometryCommandType() != m_geoType)
            {
                delete m_previewCmd;
                m_previewCmd = nullptr;
            }
        }

        if (!m_previewCmd)
        {
            m_previewCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->
                createCommandT<Interface::FITKAbsGeoModelDefeatureSolid>(m_geoType);
        }

        // 设置参数生成数据。
        m_previewCmd->setInputCmdId(m_tempCmdId);
        m_previewCmd->setFaceTopos(m_tempFaceTopos);
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

    void DefeatureInfoDialog::slot_cancel()
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::reject();
    }

    void DefeatureInfoDialog::slot_select()
    {
        // 多选边模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJFace;

        GUIPickInfo::SetPickInfo(pickInfo);
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &DefeatureInfoDialog::slot_shapeSelected, Qt::UniqueConnection);

        ui->btnPickOK->show();
        ui->btnPick->hide();
    }

    void DefeatureInfoDialog::slot_deselect()
    {
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &DefeatureInfoDialog::slot_shapeSelected);

        ui->btnPickOK->hide();
        ui->btnPick->show();
    }

    void DefeatureInfoDialog::slot_shapeSelected()
    {
        // 清除拾取数据重新进行数据提取。
        m_tempCmdId = -1;
        m_tempFaceTopos.clear();

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
            m_tempFaceTopos = pickData->getVirtualShapes();
        }

        // 刷新拾取面计数。
        updateFacesLabel();
    }
}
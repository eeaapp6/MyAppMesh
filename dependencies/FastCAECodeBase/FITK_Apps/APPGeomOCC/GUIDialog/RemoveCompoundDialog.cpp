/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "RemoveCompoundDialog.h"
#include "ui_RemoveCompoundDialog.h"

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
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperCompound.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

// Pick
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"



namespace GUI
{
    RemoveCompoundDialog::RemoveCompoundDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper) :
        GUIDialogBase(parent),
        ui(new Ui::RemoveCompoundDialog)
    {
        ui->setupUi(this);

        // 暂存操作器。
        m_Operator = oper;

        // 初始化界面。
        init();
    }

    RemoveCompoundDialog::~RemoveCompoundDialog()
    {
        // 移除预览数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }

        delete ui;
    }

    void RemoveCompoundDialog::closeEvent(QCloseEvent* event)
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::closeEvent(event);
    }

    RemoveCompoundDialog* RemoveCompoundDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelOperCompound*>(cmd);

        static RemoveCompoundDialog* w;
        if (!w)
        {
            w = new RemoveCompoundDialog(parent, oper);
        }

        // 读取历史数据，刷新界面。
        if (command)
        {
            w->m_dataId = command->getDataObjectID();
            //w->m_tempAngle = cmd->getAngle();
            w->m_tempCompoundVShape = command->getInputCmdId();
            w->m_tempVShapes = command->getShapeCmdIds();
        }
        else
        {
            w->m_dataId = -1;
            w->clearSelectData();
        }
        w->reload();
        w->updateShapes();

        return w;
    }

    void RemoveCompoundDialog::init()
    {
        // 绑定界面控件信号。
        //@{
        connect(ui->cmbType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RemoveCompoundDialog::slot_typeChanged);
        connect(ui->btnOK, &QPushButton::clicked, this, &RemoveCompoundDialog::slot_ok);
        connect(ui->btnApply, &QPushButton::clicked, this, &RemoveCompoundDialog::slot_apply);
        connect(ui->btnCancel, &QPushButton::clicked, this, &RemoveCompoundDialog::slot_cancel);
        connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_switchShapeIndex(int)));

        connect(ui->btnSelect_MainFace, &QPushButton::clicked, this, &RemoveCompoundDialog::slot_selectCompound);
        connect(ui->btnSelectOK_MainFace, &QPushButton::clicked, this, &RemoveCompoundDialog::slot_deselectCompound);
        //@}

        // 初始化创建类型。
        ui->cmbType->setCurrentIndex(0);
        ui->btnSelectOK_MainFace->hide();
        // 初始化默认参数。
        //ui->editLength->setValidator(new QDoubleValidator);
        //ui->editLength->setText("1.0");
    }

    void RemoveCompoundDialog::reload()
    {
        // 刷新界面。
        //ui->editLength->setText(QString::number(m_tempAngle));

        // 检测是否有数据被删除。
        if (!isInputValid())
        {
            clearSelectData();
        }
    }

    void RemoveCompoundDialog::disconnectSelectSigs()
    {
        // 断开拾取边信号。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &RemoveCompoundDialog::slot_shapesSelected);
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &RemoveCompoundDialog::slot_shapeCompoundSelected);
    }

    void RemoveCompoundDialog::updateShapes()
    {
        int compound = m_tempCompoundVShape.CmdId != 0 ? 1 : 0;
        ui->labelEdges_MainFace->setText(tr("Picked Compound (%1)").arg(compound));
    }

    void RemoveCompoundDialog::clearSelectData()
    {
        m_tempVShapes.clear();
        m_tempCompoundVShape = Interface::VirtualShape();
        ui->comboBox->clear();
        updateShapes();
    }

    bool RemoveCompoundDialog::isInputValid()
    {
        // 检查是否已选择至少一条边。
        if (m_tempCompoundVShape.CmdId == 0) return false;
        if (m_tempVShapes.count() == 0) return false;

        return true;
    }

    void RemoveCompoundDialog::finalize()
    {
        // 断开拾取处理信号。
        disconnectSelectSigs();

        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 退出拾取状态。
        slot_deselectCompound();

        // 还原界面。
        ui->btnSelect_MainFace->show();
        ui->btnSelectOK_MainFace->hide();
        ui->comboBox->clear();
        // 取消高亮。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->clearPickedData();
        m_GraphOper->clearPreview();


        // 删除预览模型数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }
    }

    void RemoveCompoundDialog::slot_typeChanged(int index)
    {
        if (index < 0 || index >= ui->stackedWidget_MainFace->count()) return;

        // 切换页面。
        ui->stackedWidget_MainFace->setCurrentIndex(index);
    }

    void RemoveCompoundDialog::slot_ok()
    {
        if (!isInputValid() || !m_Operator) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("CompoundVShape", QVariant::fromValue(m_tempCompoundVShape));
        m_Operator->setArgs("VShapes", QVariant::fromValue(m_tempVShapes));

        // 执行功能。
        if (!m_Operator->execProfession())
        {
            printLog(3, tr("Parameter Error!"));
            return;
        } 

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);

        // 结束创建模型。
        finalize();

        GUIDialogBase::accept();
    }

    void RemoveCompoundDialog::slot_apply()
    {
        if (!isInputValid())
        {
            return;
        }

        if (!m_previewCmd)
        {
            m_previewCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperCompound>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCompoundRemoveShape);
        }
        if (m_previewCmd == nullptr || m_tempCompoundVShape.CmdId == 0) return;
        // 设置参数生成数据。
        m_previewCmd->setInputCmdId(m_tempCompoundVShape);
        m_previewCmd->setShapeCmdIds(m_tempVShapes);
        if (!m_previewCmd->update()) return;

        // 预览对象并刷新可视化窗口。
        m_GraphOper->preview(m_previewCmd->getDataObjectID());
    }

    void RemoveCompoundDialog::slot_cancel()
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::reject();
    }

    void RemoveCompoundDialog::slot_selectCompound()
    {
        // 多选边模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJAssembly;

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &RemoveCompoundDialog::slot_shapeCompoundSelected, Qt::UniqueConnection);
        GUIPickInfo::SetPickInfo(pickInfo);

        ui->btnSelectOK_MainFace->show();
        ui->btnSelect_MainFace->hide();
    }

    void RemoveCompoundDialog::slot_deselectCompound()
    {
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &RemoveCompoundDialog::slot_shapeCompoundSelected);

        ui->btnSelectOK_MainFace->hide();
        ui->btnSelect_MainFace->show();
    }

    void RemoveCompoundDialog::slot_shapeCompoundSelected()
    {
        ui->comboBox->clear();
        // 获取拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        QList<GraphData::GraphPickedData*> pickList = pickDataProvider->getPickedList();
        for (GraphData::GraphPickedData* pickData : pickList)
        {
            // 获取模型相关数据。
            auto vTopos = pickData->getVirtualTopoAll();
            auto vTopoIndexs = pickData->getVirtualTopoIndice();
            if (vTopos.size() < 1) return;
            Interface::FITKAbsGeoShapeAgent* shapeAgent = pickData->getGeoShapeAgent();

            if (vTopos.last()->getShapeType() != Interface::FITKGeoEnum::VTopoShapeType::VSAssembly) return;
            if (vTopos.size() != 1 || !shapeAgent) continue;
            Interface::FITKAbsGeoCommand* geoCmd = shapeAgent->getGeoCommand();
            if (!geoCmd) continue;
            m_tempCompoundVShape.CmdId = geoCmd->getDataObjectID();
            m_tempCompoundVShape.VirtualTopoIndex = vTopoIndexs[0];

            int subShapeCount = vTopos.last()->getSubTopoCount();
            for (int i = 0; i < subShapeCount; i++)
            {
                ui->comboBox->addItem(QString::number(i));
            }
        }
        // 刷新拾取边计数。
        updateShapes();
    }

    void RemoveCompoundDialog::slot_switchShapeIndex(int index)
    {
        m_tempVShapes.clear();
        if (m_tempCompoundVShape.CmdId == 0 || index < 0) return;

        // 创建或获取历史操作数据。
        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        Interface::FITKAbsGeoCommand* cmdHist = dynamic_cast<Interface::FITKAbsGeoCommand*>(geoData->getDataByID(m_tempCompoundVShape.CmdId));
        if (cmdHist == nullptr) return;
        auto vtopoManager = cmdHist->getVirtualTopoManager();
        if (vtopoManager == nullptr) return;
        auto shapeManager = vtopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSAssembly);
        if (shapeManager == nullptr) return;

        auto vTopo = shapeManager->getDataByIndex(m_tempCompoundVShape.VirtualTopoIndex);
        if (vTopo->getSubTopoCount() < index) return;
        auto vtID = vTopo->getSubTopo(index);
        if (vtID == nullptr) return;
        Interface::VirtualShape vShape;
        vShape.CmdId = m_tempCompoundVShape.CmdId;
        vShape.VirtualTopoId = vtID->getDataObjectID();
        vShape.VirtualTopoIndex = index;
        m_tempVShapes.append(vShape);

        // 获取拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->clearPickedData();

        m_GraphOper->clearPreview();
        previewGraph(vShape.VirtualTopoId);
    }

    void RemoveCompoundDialog::slot_shapesSelected()
    {
        // 清除拾取数据重新进行数据提取。
        m_tempVShapes.clear();

        // 获取拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        QList<GraphData::GraphPickedData*> pickList = pickDataProvider->getPickedList();
        // 刷新拾取边计数。
        updateShapes();
    }
}

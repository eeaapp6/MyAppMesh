/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ImprintSolidDialog.h"
#include "ui_ImprintSolidDialog.h"

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
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperImprintSolid.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

// Pick
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"



namespace GUI
{
    ImprintSolidDialog::ImprintSolidDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper) :
        GUIDialogBase(parent),
        ui(new Ui::ImprintSolidDialog)
    {
        ui->setupUi(this);

        // 暂存操作器。
        m_Operator = oper;

        // 初始化界面。
        init();
    }

    ImprintSolidDialog::~ImprintSolidDialog()
    {
        // 移除预览数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }

        delete ui;
    }

    void ImprintSolidDialog::closeEvent(QCloseEvent* event)
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::closeEvent(event);
    }

    ImprintSolidDialog* ImprintSolidDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelOperImprintSolid*>(cmd);

        static ImprintSolidDialog* w;
        if (!w)
        {
            w = new ImprintSolidDialog(parent, oper);
        }

        // 读取历史数据，刷新界面。
        if (command)
        {
            w->m_dataId = command->getDataObjectID();
            w->m_tempVShapes = command->getVShapes();
            w->reload();
        }
        else
        {
            w->m_dataId = -1;
            w->m_tempVShapes.clear();
            w->reload();
        }
        w->updateSolidsLabel();
        return w;
    }

    void ImprintSolidDialog::init()
    {
        // 绑定界面控件信号。
        //@{
        connect(ui->cmbType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ImprintSolidDialog::slot_typeChanged);
        connect(ui->btnOK, &QPushButton::clicked, this, &ImprintSolidDialog::slot_ok);
        connect(ui->btnApply, &QPushButton::clicked, this, &ImprintSolidDialog::slot_apply);
        connect(ui->btnCancel, &QPushButton::clicked, this, &ImprintSolidDialog::slot_cancel);

        connect(ui->btnSelect_Solid, &QPushButton::clicked, this, &ImprintSolidDialog::slot_selectSolid);
        connect(ui->btnSelectOK_Solid, &QPushButton::clicked, this, &ImprintSolidDialog::slot_deselectSolid);
        //@}

        // 初始化创建类型。
        ui->cmbType->setCurrentIndex(0);
        ui->btnSelectOK_Solid->hide();
        // 初始化默认参数。
        //ui->editLength->setValidator(new QDoubleValidator);
        //ui->editLength->setText("1.0");
    }

    void ImprintSolidDialog::reload()
    {
        // 刷新界面。
        //ui->editLength->setText(QString::number(m_tempAngle));

        // 检测是否有数据被删除。
        if (!isInputValid())
        {
            clearSelectData();
        }
    }

    void ImprintSolidDialog::disconnectSelectSigs()
    {
        // 断开拾取信号。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &ImprintSolidDialog::slot_shapeSolidSelected);
    }

    void ImprintSolidDialog::updateSolidsLabel()
    {
        int nShells = m_tempVShapes.count();
        ui->labelEdges_Solid->setText(tr("Picked Solid (%1)").arg(nShells));
    }

    void ImprintSolidDialog::clearSelectData()
    {
        m_tempVShapes.clear();

        updateSolidsLabel();
    }

    bool ImprintSolidDialog::isInputValid()
    {
        // 检查是否已选择至少一条边。
        if (m_tempVShapes.count() == 0) return false;

        return true;
    }

    void ImprintSolidDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 断开拾取处理信号。
        disconnectSelectSigs();

        // 退出拾取状态。
        slot_deselectSolid();

        // 还原界面。
        ui->btnSelect_Solid->show();
        ui->btnSelectOK_Solid->hide();

        // 删除预览模型数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }
    }

    void ImprintSolidDialog::slot_typeChanged(int index)
    {
        if (index < 0 || index >= ui->stackedWidget_Solid->count())
        {
            return;
        }

        // 切换页面。
        ui->stackedWidget_Solid->setCurrentIndex(index);
    }

    void ImprintSolidDialog::slot_ok()
    {
        if (!isInputValid() || !m_Operator) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
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

    void ImprintSolidDialog::slot_apply()
    {
        if (!isInputValid())
        {
            return;
        }

        if (!m_previewCmd)
        {
            m_previewCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperImprintSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTImprintSolid);
        }
        if (m_previewCmd == nullptr) return;
        m_previewCmd->setType(Interface::FITKAbsGeoModelOperImprintSolid::GBTImprintSolid);
        m_previewCmd->add(m_tempVShapes);
        // 设置参数生成数据。
        if (!m_previewCmd->update()) return;

        // 预览对象并刷新可视化窗口。
        EventOper::GraphEventOperator* graphOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper)
        {
            graphOper->preview(m_previewCmd->getDataObjectID());
        }
    }

    void ImprintSolidDialog::slot_cancel()
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::reject();
    }

    void ImprintSolidDialog::slot_selectSolid()
    {
        // 多选边模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJSolid;

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &ImprintSolidDialog::slot_shapeSolidSelected, Qt::UniqueConnection);
        GUIPickInfo::SetPickInfo(pickInfo);

        ui->btnSelectOK_Solid->show();
        ui->btnSelect_Solid->hide();
    }

    void ImprintSolidDialog::slot_deselectSolid()
    {
        //退出拾取
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMNone;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJNone;
        GUIPickInfo::SetPickInfo(pickInfo);

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &ImprintSolidDialog::slot_shapeSolidSelected);

        ui->btnSelectOK_Solid->hide();
        ui->btnSelect_Solid->show();
    }

    void ImprintSolidDialog::slot_shapeSolidSelected()
    {
        // 清除拾取数据重新进行数据提取。
        m_tempVShapes.clear();

        // 获取拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        QList<GraphData::GraphPickedData*> pickList = pickDataProvider->getPickedList();
        for (GraphData::GraphPickedData* pickData : pickList)
        {
            // 获取模型相关数据。
            QList<int> vTopoIndexs = pickData->getVirtualTopoIndice();
            Interface::FITKAbsGeoShapeAgent* shapeAgent = pickData->getGeoShapeAgent();

            if (vTopoIndexs.size() < 1) continue;
            m_tempVShapes.append(pickData->getVirtualShapes());
        }
        // 刷新拾取边计数。
        updateSolidsLabel();
    }
}

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SplitEdgeMidpointDialog.h"
#include "ui_SplitEdgeMidpointDialog.h"

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
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperLine.h"
// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

// Pick
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"



namespace GUI
{
    SplitEdgeMidpointDialog::SplitEdgeMidpointDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper) :
        GUIDialogBase(parent),
        ui(new Ui::SplitEdgeMidpointDialog)
    {
        ui->setupUi(this);

        // 暂存操作器。
        m_Operator = oper;

        // 初始化界面。
        init();
    }

    SplitEdgeMidpointDialog::~SplitEdgeMidpointDialog()
    {
        // 移除预览数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }

        delete ui;
    }

    void SplitEdgeMidpointDialog::closeEvent(QCloseEvent* event)
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::closeEvent(event);
    }

    SplitEdgeMidpointDialog* SplitEdgeMidpointDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelOperLine*>(cmd);

        static SplitEdgeMidpointDialog* w;
        if (!w)
        {
            w = new SplitEdgeMidpointDialog(parent, oper);
        }

        // 读取历史数据，刷新界面。
        if (command)
        {
            w->m_dataId = command->getDataObjectID();
            w->m_tempAngle = command->getAngle();
            w->m_tempVShapes = command->getVShapes();
            w->reload();
        }
        else
        {
            w->m_dataId = -1;
            w->m_tempAngle = 1;
            w->m_tempVShapes.clear();
            w->reload();
        }
        w->updateEdgesLabel();

        return w;
    }

    void SplitEdgeMidpointDialog::init()
    {
        // 绑定界面控件信号。
        //@{
        connect(ui->cmbType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SplitEdgeMidpointDialog::slot_typeChanged);
        connect(ui->btnOK, &QPushButton::clicked, this, &SplitEdgeMidpointDialog::slot_ok);
        connect(ui->btnApply, &QPushButton::clicked, this, &SplitEdgeMidpointDialog::slot_apply);
        connect(ui->btnCancel, &QPushButton::clicked, this, &SplitEdgeMidpointDialog::slot_cancel);
        connect(ui->btnSelect, &QPushButton::clicked, this, &SplitEdgeMidpointDialog::slot_select);
        connect(ui->btnSelectOK, &QPushButton::clicked, this, &SplitEdgeMidpointDialog::slot_deselect);
        //@}

        // 初始化创建类型。
        ui->cmbType->setCurrentIndex(0);
        ui->btnSelectOK->hide();

        // 初始化默认参数。
        //ui->editLength->setValidator(new QDoubleValidator);
        //ui->editLength->setText("1.0");
    }

    void SplitEdgeMidpointDialog::reload()
    {
        // 刷新界面。
        //ui->editLength->setText(QString::number(m_tempAngle));

        // 检测是否有数据被删除。
        if (!isInputValid())
        {
            clearSelectData();
        }
    }

    void SplitEdgeMidpointDialog::disconnectSelectSigs()
    {
        // 断开拾取边信号。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &SplitEdgeMidpointDialog::slot_shapeSelected);
    }

    void SplitEdgeMidpointDialog::updateEdgesLabel()
    {
        int nEdges = m_tempVShapes.count();
        ui->labelEdges->setText(tr("Picked Line (%1)").arg(nEdges));
    }

    void SplitEdgeMidpointDialog::clearSelectData()
    {
        m_tempVShapes.clear();
        updateEdgesLabel();
    }

    bool SplitEdgeMidpointDialog::isInputValid()
    {
        // 检查是否已选择至少一条边。
        if (m_tempVShapes.count() == 0) return false;

        return true;
    }

    void SplitEdgeMidpointDialog::finalize()
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

        // 还原界面。
        ui->btnSelect->show();
        ui->btnSelectOK->hide();

        // 删除预览模型数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }
    }

    void SplitEdgeMidpointDialog::slot_typeChanged(int index)
    {
        if (index < 0 || index >= ui->stackedWidget->count())
        {
            return;
        }

        // 切换页面。
        ui->stackedWidget->setCurrentIndex(index);
    }

    void SplitEdgeMidpointDialog::slot_ok()
    {
        if (!isInputValid())
        {
            return;
        }

        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建或获取历史线操作数据。
        Interface::FITKAbsGeoModelMiddleOperLine* cmdHist = dynamic_cast<Interface::FITKAbsGeoModelMiddleOperLine*>(geoData->getDataByID(m_dataId));
        Interface::FITKAbsGeoModelMiddleOperLine* operLineCmd = cmdHist == nullptr ?
            Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelMiddleOperLine>
            (Interface::FITKGeoEnum::FITKGeometryComType::FGTSplitByMiddlePositionLine) : cmdHist;

        if (operLineCmd == nullptr) return;
        operLineCmd->setType(Interface::FITKAbsGeoModelOperLine::GeoLineOperType::GBTSplitByMiddlePosition);
        m_previewCmd->clear();
        operLineCmd->add(m_tempVShapes);
        // 设置参数生成数据。
        if (!operLineCmd->update()) return;

        // 添加至数据仓库。
        if (!cmdHist)
        {
            operLineCmd->setDataObjectName(geoData->checkName(QString("SplitEdge-%1").arg(geoData->getDataCount() + 1)));
            geoData->appendDataObj(operLineCmd);
        }

        // 刷新可视化窗口。
        EventOper::GraphEventOperator* graphOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper)
        {
            graphOper->updateGraph(operLineCmd->getDataObjectID());
        }

        execProfession();

        // 保存数据ID。
        m_dataId = operLineCmd->getDataObjectID();
        m_tempAngle = operLineCmd->getAngle();

        // 结束创建模型。
        finalize();

        GUIDialogBase::accept();
    }

    void SplitEdgeMidpointDialog::slot_apply()
    {
        if (!isInputValid())
        {
            return;
        }

        if (!m_previewCmd)
        {
            m_previewCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperLine>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSplitByMiddlePositionLine);
        }
        if (m_previewCmd == nullptr) return;
        // 设置参数生成数据。
        m_previewCmd->setType(Interface::FITKAbsGeoModelOperLine::GeoLineOperType::GBTSplitByMiddlePosition);
        m_previewCmd->clear();
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

    void SplitEdgeMidpointDialog::slot_cancel()
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::reject();
    }

    void SplitEdgeMidpointDialog::slot_select()
    {
        // 多选边模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJEdge;

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &SplitEdgeMidpointDialog::slot_shapeSelected, Qt::UniqueConnection);
        GUIPickInfo::SetPickInfo(pickInfo);

        ui->btnSelectOK->show();
        ui->btnSelect->hide();
    }

    void SplitEdgeMidpointDialog::slot_deselect()
    {

        //退出拾取
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMNone;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJNone;
        GUIPickInfo::SetPickInfo(pickInfo);

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &SplitEdgeMidpointDialog::slot_shapeSelected);

        ui->btnSelectOK->hide();
        ui->btnSelect->show();
    }

    void SplitEdgeMidpointDialog::slot_shapeSelected()
    {
        // 清除拾取数据重新进行数据提取。
        m_tempVShapes.clear();

        // 获取拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        QList<GraphData::GraphPickedData*> pickList = pickDataProvider->getPickedList();
        for (GraphData::GraphPickedData* pickData : pickList)
        {
            // 获取模型相关数据。
            QList<Interface::FITKAbsVirtualTopo*> vTopos = pickData->getVirtualTopoAll();
            Interface::FITKAbsGeoShapeAgent* shapeAgent = pickData->getGeoShapeAgent();

            if (vTopos.size() < 1 || !shapeAgent) continue;
            Interface::FITKAbsGeoCommand* geoCmd = shapeAgent->getGeoCommand();
            if (!geoCmd) continue;

            for (int i = 0; i < vTopos.size(); i++)
            {
                Interface::VirtualShape v;
                v.CmdId = geoCmd->getDataObjectID();
                v.VirtualTopoId = vTopos[i]->getDataObjectID();
                m_tempVShapes.append(v);
            }
        }
        // 刷新拾取边计数。
        updateEdgesLabel();
    }
}

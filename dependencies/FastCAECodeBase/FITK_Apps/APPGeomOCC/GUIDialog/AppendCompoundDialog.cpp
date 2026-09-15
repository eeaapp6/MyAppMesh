/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "AppendCompoundDialog.h"
#include "ui_AppendCompoundDialog.h"

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
    AppendCompoundDialog::AppendCompoundDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper) :
        GUIDialogBase(parent),
        ui(new Ui::AppendCompoundDialog)
    {
        ui->setupUi(this);

        // 暂存操作器。
        m_Operator = oper;

        // 初始化界面。
        init();
    }

    AppendCompoundDialog::~AppendCompoundDialog()
    {
        // 移除预览数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }

        delete ui;
    }

    void AppendCompoundDialog::closeEvent(QCloseEvent* event)
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::closeEvent(event);
    }

    AppendCompoundDialog* AppendCompoundDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelOperCompound*>(cmd);

        static AppendCompoundDialog* w;
        if (!w)
        {
            w = new AppendCompoundDialog(parent, oper);
        }

        // 读取历史数据，刷新界面。
        if (command)
        {
            w->m_dataId = command->getDataObjectID();
            w->m_tempCompoundVShape = command->getInputCmdId();
            w->m_tempVShapes = command->getShapeCmdIds();
        }
        else
        {
            w->m_dataId = -1;
            w->clearSelectData();
        }

        w->reload();
        w->updateShapesLabel();

        return w;
    }

    void AppendCompoundDialog::init()
    {
        // 绑定界面控件信号。
        //@{
        connect(ui->cmbType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AppendCompoundDialog::slot_typeChanged);
        connect(ui->btnOK, &QPushButton::clicked, this, &AppendCompoundDialog::slot_ok);
        connect(ui->btnApply, &QPushButton::clicked, this, &AppendCompoundDialog::slot_apply);
        connect(ui->btnCancel, &QPushButton::clicked, this, &AppendCompoundDialog::slot_cancel);
        connect(ui->btnSelect_Faces, &QPushButton::clicked, this, &AppendCompoundDialog::slot_selectShapes);
        connect(ui->btnSelectOK_Faces, &QPushButton::clicked, this, &AppendCompoundDialog::slot_deselectShapes);

        connect(ui->btnSelect_MainFace, &QPushButton::clicked, this, &AppendCompoundDialog::slot_selectCompound);
        connect(ui->btnSelectOK_MainFace, &QPushButton::clicked, this, &AppendCompoundDialog::slot_deselectCompound);
        //@}

        // 初始化创建类型。
        ui->cmbType->setCurrentIndex(0);
        ui->btnSelectOK_Faces->hide();
        ui->btnSelectOK_MainFace->hide();
        // 初始化默认参数。
        //ui->editLength->setValidator(new QDoubleValidator);
        //ui->editLength->setText("1.0");
    }

    void AppendCompoundDialog::reload()
    {
        // 刷新界面。
        //ui->editLength->setText(QString::number(m_tempAngle));

        // 检测是否有数据被删除。
        if (!isInputValid())
        {
            clearSelectData();
        }
    }

    void AppendCompoundDialog::disconnectSelectSigs()
    {
        // 断开拾取边信号。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &AppendCompoundDialog::slot_shapesSelected);
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &AppendCompoundDialog::slot_shapeCompoundSelected);
    }

    void AppendCompoundDialog::updateShapesLabel()
    {
        int nShells = m_tempVShapes.count();
        ui->labelEdges_MainFace->setText(tr("Picked Compound (%1)").arg(0));
        if (m_tempCompoundVShape.CmdId != 0)
            ui->labelEdges_MainFace->setText(tr("Picked Compound (%1)").arg(1));

        ui->labelEdges_faces->setText(tr("Picked Shapes (%1)").arg(nShells));
    }

    void AppendCompoundDialog::clearSelectData()
    {
        m_tempVShapes.clear();
        m_tempCompoundVShape = Interface::VirtualShape();
        updateShapesLabel();
    }

    bool AppendCompoundDialog::isInputValid()
    {
        // 检查是否已选择至少一条边。
        if (m_tempVShapes.count() == 0) return false;

        return true;
    }

    void AppendCompoundDialog::finalize()
    {
        // 断开拾取处理信号。
        disconnectSelectSigs();

        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 清空拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->clearPickedData();

        // 退出拾取状态。
        slot_deselectCompound();
        slot_deselectShapes();

        // 还原界面。
        ui->btnSelect_Faces->show();
        ui->btnSelectOK_Faces->hide();
        ui->btnSelect_MainFace->show();
        ui->btnSelectOK_MainFace->hide();

        // 删除预览模型数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }
    }

    void AppendCompoundDialog::slot_typeChanged(int index)
    {
        if (index < 0 || index >= ui->stackedWidget_MainFace->count()) return;

        // 切换页面。
        ui->stackedWidget_MainFace->setCurrentIndex(index);
    }

    void AppendCompoundDialog::slot_ok()
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

    void AppendCompoundDialog::slot_apply()
    {
        if (!isInputValid()) return;

        if (!m_previewCmd)
        {
            m_previewCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperCompound>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCompoundAppendShape);
        }
        if (m_previewCmd == nullptr) return;
        // 设置参数生成数据。
        m_previewCmd->setInputCmdId(m_tempCompoundVShape);
        m_previewCmd->setShapeCmdIds(m_tempVShapes);
        if (!m_previewCmd->update())
        {
            printLog(3, tr("Parameter Error!"));
            return;
        }

        // 预览对象并刷新可视化窗口。
        m_GraphOper->preview(m_previewCmd->getDataObjectID());
    }

    void AppendCompoundDialog::slot_cancel()
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::reject();
    }

    void AppendCompoundDialog::slot_selectCompound()
    {
        slot_deselectShapes();
        // 多选边模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJAssembly;

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &AppendCompoundDialog::slot_shapeCompoundSelected, Qt::UniqueConnection);
        GUIPickInfo::SetPickInfo(pickInfo);

        ui->btnSelectOK_MainFace->show();
        ui->btnSelect_MainFace->hide();
    }

    void AppendCompoundDialog::slot_selectShapes()
    {
        slot_deselectCompound();
        // 多选模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJShape;

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &AppendCompoundDialog::slot_shapesSelected, Qt::UniqueConnection);
        GUIPickInfo::SetPickInfo(pickInfo);

        ui->btnSelectOK_Faces->show();
        ui->btnSelect_Faces->hide();
    }

    void AppendCompoundDialog::slot_deselectShapes()
    {
        //退出拾取
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMNone;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJNone;
        GUIPickInfo::SetPickInfo(pickInfo);
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &AppendCompoundDialog::slot_shapesSelected);

        ui->btnSelectOK_Faces->hide();
        ui->btnSelect_Faces->show();
    }

    void AppendCompoundDialog::slot_deselectCompound()
    {
        //退出拾取
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMNone;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJNone;
        GUIPickInfo::SetPickInfo(pickInfo);
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &AppendCompoundDialog::slot_shapeCompoundSelected);

        ui->btnSelectOK_MainFace->hide();
        ui->btnSelect_MainFace->show();
    }

    void AppendCompoundDialog::slot_shapeCompoundSelected()
    {
        // 获取拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        QList<GraphData::GraphPickedData*> pickList = pickDataProvider->getPickedList();
        for (GraphData::GraphPickedData* pickData : pickList)
        {
            // 获取模型相关数据。
            QList<int> vTopoIndexs = pickData->getVirtualTopoIndice();
            Interface::FITKAbsGeoShapeAgent* shapeAgent = pickData->getGeoShapeAgent();

            if (vTopoIndexs.size() != 1 || !shapeAgent) continue;
            Interface::FITKAbsGeoCommand* geoCmd = shapeAgent->getGeoCommand();
            if (!geoCmd) continue;

            //主对象不能与添加对象重复
            if (m_tempVShapes.size() == 1 && m_tempVShapes[0].CmdId == geoCmd->getDataObjectID())
                m_tempVShapes.clear();

            m_tempCompoundVShape.CmdId = geoCmd->getDataObjectID();
            m_tempCompoundVShape.VirtualTopoIndex = vTopoIndexs[0];
        }

        // 刷新拾取计数。
        updateShapesLabel();
    }

    void AppendCompoundDialog::slot_shapesSelected()
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
            if (vTopoIndexs.size() != 1 || !shapeAgent) continue;
            Interface::FITKAbsGeoCommand* geoCmd = shapeAgent->getGeoCommand();
            if (!geoCmd) continue;

            Interface::VirtualShape v;
            v.CmdId = geoCmd->getDataObjectID();
            m_tempVShapes.append(v);

            //主对象不能与添加对象重复
            if (m_tempVShapes.size() == 1 && m_tempVShapes[0].CmdId == m_tempCompoundVShape.CmdId)
            {
                m_tempCompoundVShape.CmdId = 0;
                m_tempCompoundVShape.VirtualTopoIndex = -1;
            }
        }

        // 刷新拾取计数。
        updateShapesLabel();
    }
}

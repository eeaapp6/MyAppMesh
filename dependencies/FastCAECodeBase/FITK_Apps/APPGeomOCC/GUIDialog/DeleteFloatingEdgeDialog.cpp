/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "DeleteFloatingEdgeDialog.h"
#include "ui_DeleteFloatingEdgeDialog.h"

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
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperFace.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

// Pick
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"



namespace GUI
{
    DeleteFloatingEdgeDialog::DeleteFloatingEdgeDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper) :
        GUIDialogBase(parent),
        ui(new Ui::DeleteFloatingEdgeDialog)
    {
        ui->setupUi(this);

        // 暂存操作器。
        m_Operator = oper;

        // 初始化界面。
        init();
    }

    DeleteFloatingEdgeDialog::~DeleteFloatingEdgeDialog()
    {
        // 移除预览数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }

        delete ui;
    }

    void DeleteFloatingEdgeDialog::closeEvent(QCloseEvent* event)
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::closeEvent(event);
    }

    DeleteFloatingEdgeDialog* DeleteFloatingEdgeDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge*>(cmd);

        static DeleteFloatingEdgeDialog* w;
        if (!w)
        {
            w = new DeleteFloatingEdgeDialog(parent, oper);
        }

        // 读取历史数据，刷新界面。
        if (command)
        {
            w->m_dataId = command->getDataObjectID();
            w->m_tempVShapes = command->getVShapes();
        }
        else
        {
            w->m_dataId = -1;
            w->m_tempVShapes.clear();
        }
        w->reload();
        w->updateFacesLabel();

        return w;
    }

    void DeleteFloatingEdgeDialog::init()
    {
        // 绑定界面控件信号。
        //@{
        connect(ui->cmbType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DeleteFloatingEdgeDialog::slot_typeChanged);
        connect(ui->btnOK, &QPushButton::clicked, this, &DeleteFloatingEdgeDialog::slot_ok);
        connect(ui->btnApply, &QPushButton::clicked, this, &DeleteFloatingEdgeDialog::slot_apply);
        connect(ui->btnCancel, &QPushButton::clicked, this, &DeleteFloatingEdgeDialog::slot_cancel);
        connect(ui->btnSelect_Faces, &QPushButton::clicked, this, &DeleteFloatingEdgeDialog::slot_selectFaces);
        connect(ui->btnSelectOK_Faces, &QPushButton::clicked, this, &DeleteFloatingEdgeDialog::slot_deselectFaces);

        connect(ui->btnSelect_MainFace, &QPushButton::clicked, this, &DeleteFloatingEdgeDialog::slot_selectMainFace);
        connect(ui->btnSelectOK_MainFace, &QPushButton::clicked, this, &DeleteFloatingEdgeDialog::slot_deselectMainFace);
        //@}

        // 初始化创建类型。
        ui->cmbType->setCurrentIndex(0);
        ui->btnSelectOK_Faces->hide();
        ui->btnSelectOK_MainFace->hide();
        // 初始化默认参数。
        //ui->editLength->setValidator(new QDoubleValidator);
        //ui->editLength->setText("1.0");
    }

    void DeleteFloatingEdgeDialog::reload()
    {
        // 刷新界面。
        //ui->editLength->setText(QString::number(m_tempAngle));

        // 检测是否有数据被删除。
        if (!isInputValid())
        {
            clearSelectData();
        }
    }

    void DeleteFloatingEdgeDialog::disconnectSelectSigs()
    {
        // 断开拾取边信号。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &DeleteFloatingEdgeDialog::slot_shapeFacesSelected);
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &DeleteFloatingEdgeDialog::slot_shapeMainFaceSelected);
    }

    void DeleteFloatingEdgeDialog::updateFacesLabel()
    {
        int nShells = m_tempVShapes.count();
        ui->labelEdges_MainFace->setText(tr("Picked MainFace (%1)").arg(0));
        if (m_tempMainFaceVShape.CmdId != 0)
            ui->labelEdges_MainFace->setText(tr("Picked MainFace (%1)").arg(1));
        ui->labelEdges_faces->setText(tr("Picked Faces (%1)").arg(nShells));
    }

    void DeleteFloatingEdgeDialog::clearSelectData()
    {
        m_tempVShapes.clear();
        m_tempMainFaceVShape = Interface::VirtualShape();
        updateFacesLabel();
    }

    bool DeleteFloatingEdgeDialog::isInputValid()
    {
        // 检查是否已选择至少一条边。
        if (m_tempVShapes.count() == 0) return false;

        return true;
    }

    void DeleteFloatingEdgeDialog::finalize()
    {
        // 断开拾取处理信号。
        disconnectSelectSigs();

        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 退出拾取状态。
        slot_deselectMainFace();
        slot_deselectFaces();

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

    void DeleteFloatingEdgeDialog::slot_typeChanged(int index)
    {
        if (index < 0 || index >= ui->stackedWidget_MainFace->count()) return;

        // 切换页面。
        ui->stackedWidget_MainFace->setCurrentIndex(index);
    }

    void DeleteFloatingEdgeDialog::slot_ok()
    {
        if (!isInputValid() || !m_Operator) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);
        
        //临时合并链表
        QList<Interface::VirtualShape> m_temp;
        m_temp.append(m_tempMainFaceVShape);
        m_temp.append(m_tempVShapes);

        // 传递输入参数。
        m_Operator->setArgs("VShapes", QVariant::fromValue(m_temp));

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

    void DeleteFloatingEdgeDialog::slot_apply()
    {
        if (!isInputValid()) return;

        if (!m_previewCmd)
        {
            m_previewCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge>(Interface::FITKGeoEnum::FITKGeometryComType::FGTDeleteFloatingEdge);
        }
        if (m_previewCmd == nullptr) return;
        m_previewCmd->add(m_tempMainFaceVShape);
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

    void DeleteFloatingEdgeDialog::slot_cancel()
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::reject();
    }

    void DeleteFloatingEdgeDialog::slot_selectMainFace()
    {
        slot_deselectFaces();
        // 多选边模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJFace;

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &DeleteFloatingEdgeDialog::slot_shapeMainFaceSelected, Qt::UniqueConnection);
        GUIPickInfo::SetPickInfo(pickInfo);

        ui->btnSelectOK_MainFace->show();
        ui->btnSelect_MainFace->hide();
    }

    void DeleteFloatingEdgeDialog::slot_selectFaces()
    {
        slot_deselectMainFace();
        // 多选边模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJFace;

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &DeleteFloatingEdgeDialog::slot_shapeFacesSelected, Qt::UniqueConnection);
        GUIPickInfo::SetPickInfo(pickInfo);

        ui->btnSelectOK_Faces->show();
        ui->btnSelect_Faces->hide();
    }

    void DeleteFloatingEdgeDialog::slot_deselectFaces()
    {
        //退出拾取
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMNone;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJNone;
        GUIPickInfo::SetPickInfo(pickInfo);

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &DeleteFloatingEdgeDialog::slot_shapeFacesSelected);

        ui->btnSelectOK_Faces->hide();
        ui->btnSelect_Faces->show();
    }


    void DeleteFloatingEdgeDialog::slot_deselectMainFace()
    {
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &DeleteFloatingEdgeDialog::slot_shapeMainFaceSelected);

        ui->btnSelectOK_MainFace->hide();
        ui->btnSelect_MainFace->show();
    }

    void DeleteFloatingEdgeDialog::slot_shapeMainFaceSelected()
    {
        // 清除拾取数据重新进行数据提取。
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
            m_tempMainFaceVShape.CmdId = geoCmd->getDataObjectID();
            m_tempMainFaceVShape.VirtualTopoId = vTopoIndexs[0];
        }

        // 刷新拾取边计数。
        updateFacesLabel();
    }

    void DeleteFloatingEdgeDialog::slot_shapeFacesSelected()
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

            if (vTopoIndexs.size() < 1 || !shapeAgent) continue;
            Interface::FITKAbsGeoCommand* geoCmd = shapeAgent->getGeoCommand();
            if (!geoCmd) continue;

            for (int i = 0; i < vTopoIndexs.size(); i++)
            {
                Interface::VirtualShape v;
                v.CmdId = geoCmd->getDataObjectID();
                v.VirtualTopoId = vTopoIndexs[i];
                m_tempVShapes.append(v);
            }
        }
        // 刷新拾取边计数。
        updateFacesLabel();
    }
}

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FillFaceInfoDialog.h"
#include "ui_FillFaceInfoDialog.h"

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

#include <QDebug>

namespace GUI
{
    FillFaceInfoDialog::FillFaceInfoDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper) :
        GUIDialogBase(parent),
        ui(new Ui::FillFaceInfoDialog)
    {
        ui->setupUi(this);

        // 暂存操作器。
        m_Operator = oper;

        // 初始化界面。
        init();
    }

    FillFaceInfoDialog::~FillFaceInfoDialog()
    {
        // 移除预览数据。
        if (m_previewCmd)
        {
            delete m_previewCmd;
            m_previewCmd = nullptr;
        }

        delete ui;
    }

    void FillFaceInfoDialog::closeEvent(QCloseEvent* event)
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::closeEvent(event);
    }

    FillFaceInfoDialog* FillFaceInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge*>(cmd);

        static FillFaceInfoDialog* w;
        if (!w)
        {
            w = new FillFaceInfoDialog(parent, oper);
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
        w->updateEdgesLabel();

        return w;
    }

    void FillFaceInfoDialog::init()
    {
        // 绑定界面控件信号。
        //@{
        connect(ui->cmbType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FillFaceInfoDialog::slot_typeChanged);
        connect(ui->btnOK, &QPushButton::clicked, this, &FillFaceInfoDialog::slot_ok);
        connect(ui->btnApply, &QPushButton::clicked, this, &FillFaceInfoDialog::slot_apply);
        connect(ui->btnCancel, &QPushButton::clicked, this, &FillFaceInfoDialog::slot_cancel);
        connect(ui->btnSelect, &QPushButton::clicked, this, &FillFaceInfoDialog::slot_select);
        connect(ui->btnSelectOK, &QPushButton::clicked, this, &FillFaceInfoDialog::slot_deselect);
        //@}

        // 初始化创建类型。
        ui->cmbType->setCurrentIndex(0);
        ui->btnSelectOK->hide();

        // 初始化默认参数。
        //ui->editLength->setValidator(new QDoubleValidator);
        //ui->editLength->setText("1.0");
    }

    void FillFaceInfoDialog::reload()
    {
        // 刷新界面。
        //ui->editLength->setText(QString::number(m_tempAngle));

        // 检测是否有数据被删除。
        if (!isInputValid())
        {
            clearSelectData();
        }
    }

    void FillFaceInfoDialog::disconnectSelectSigs()
    {
        // 断开拾取边信号。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &FillFaceInfoDialog::slot_shapeSelected);
    }

    void FillFaceInfoDialog::updateEdgesLabel()
    {
        int nShells = m_tempVShapes.count();
        ui->labelEdges->setText(tr("Picked Line (%1)").arg(nShells));
    }

    void FillFaceInfoDialog::clearSelectData()
    {
        m_tempVShapes.clear();

        updateEdgesLabel();
    }

    bool FillFaceInfoDialog::isInputValid()
    {
        // 检查是否已选择至少一条边。
        if (m_tempVShapes.count() == 0) return false;

        return true;
    }

    void FillFaceInfoDialog::finalize()
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

    bool FillFaceInfoDialog::isParentSolid(Interface::FITKAbsVirtualTopo* vTopo)
    {
        if (vTopo == nullptr) return false;
        bool isInSolid = false;
        //查询父拓扑
        int parentCount = vTopo->getParentTopoCount();
        for (int j = 0; j < parentCount; j++)
        {
            if (vTopo->getParentTopo(j)->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSSolid) return true;
            isInSolid |= isParentSolid(vTopo->getParentTopo(j));
        }
        return isInSolid;
    }

    bool FillFaceInfoDialog::isParentFace(Interface::FITKAbsVirtualTopo * vTopo)
    {
        if (vTopo == nullptr) return false;
        bool isInFace = false;
        //查询父拓扑
        int parentCount = vTopo->getParentTopoCount();
        for (int j = 0; j < parentCount; j++)
        {
            if (vTopo->getParentTopo(j)->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSFace) return true;
            isInFace |= isParentSolid(vTopo->getParentTopo(j));
        }
        return isInFace;
    }

    void FillFaceInfoDialog::clearShapeList()
    {
        m_tempVShapes.clear();
    }

    void FillFaceInfoDialog::slot_typeChanged(int index)
    {
        if (index < 0 || index >= ui->stackedWidget->count()) return;

        // 切换页面。
        ui->stackedWidget->setCurrentIndex(index);
    }

    void FillFaceInfoDialog::slot_ok()
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

    void FillFaceInfoDialog::slot_apply()
    {
        if (!isInputValid()) return;

        if (!m_previewCmd) m_previewCmd = Interface::FITKInterfaceGeometryFactory::getInstance()->createCommandT<Interface::FITKAbsGeoModelOperFace>(Interface::FITKGeoEnum::FITKGeometryComType::FGTFillGapsFace);
        if (m_previewCmd == nullptr) return;
        m_previewCmd->setType(Interface::FITKAbsGeoModelOperFace::GeoFaceOperType::GBTFillGaps);
        m_previewCmd->add(m_tempVShapes);

        // 设置参数生成数据。
        if (!m_previewCmd->update()) return;

        // 预览对象并刷新可视化窗口。
        EventOper::GraphEventOperator* graphOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper) graphOper->preview(m_previewCmd->getDataObjectID());
    }

    void FillFaceInfoDialog::slot_cancel()
    {
        // 结束创建模型。
        finalize();

        GUIDialogBase::reject();
    }

    void FillFaceInfoDialog::slot_select()
    {
        // 多选边模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJEdge;

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &FillFaceInfoDialog::slot_shapeSelected, Qt::UniqueConnection);
        GUIPickInfo::SetPickInfo(pickInfo);

        ui->btnSelectOK->show();
        ui->btnSelect->hide();
    }

    void FillFaceInfoDialog::slot_deselect()
    {
        //退出拾取
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUIPickInfo::PickMethod::PMNone;
        pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJNone;
        GUIPickInfo::SetPickInfo(pickInfo);

        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, this, &FillFaceInfoDialog::slot_shapeSelected);

        ui->btnSelectOK->hide();
        ui->btnSelect->show();
    }

    void FillFaceInfoDialog::slot_shapeSelected()
    {
        // 清除拾取数据重新进行数据提取。
        clearShapeList();

        // 获取拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        QList<GraphData::GraphPickedData*> pickList = pickDataProvider->getPickedList();
        //提取主面
        for (GraphData::GraphPickedData* pickData : pickList)
        {
            // 获取模型相关数据。
            QList<int> vTopoIndexs = pickData->getVirtualTopoIndice();
            auto vTopos = pickData->getVirtualTopoAll();
            if (vTopoIndexs.size() != vTopos.size()) return;

            Interface::FITKAbsGeoShapeAgent* shapeAgent = pickData->getGeoShapeAgent();

            if (vTopoIndexs.size() < 1 || !shapeAgent) continue;
            Interface::FITKAbsGeoCommand* geoCmd = shapeAgent->getGeoCommand();
            if (!geoCmd) continue;
            for (int i = 0; i < vTopoIndexs.size(); i++)
            {
                //查询父拓扑
                if (isParentSolid(vTopos[i]))
                {
                    clearShapeList();
                    pickDataProvider->clearPickedData();
                    return;
                }
                Interface::VirtualShape v;
                v.CmdId = geoCmd->getDataObjectID();
                v.VirtualTopoIndex = vTopoIndexs[i];
                m_tempVShapes.append(v);
            }

            //// 向上查找所选边对应体数据。
            //Interface::FITKVirtualTopoMapper* vMapper = new Interface::FITKVirtualTopoMapper;
            //vMapper->mapTopo(vTopo, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
            //
            //bool hasSameSolid = true;
            //
            //QList<int> faceWeight;
            //// 获取查询结果。
            //int nRet = vMapper->length();
            //for (int i = 0; i < nRet; i++)
            //{
            //    Interface::FITKAbsVirtualTopo* vFace = vMapper->virtualTopo(i);
            //    int newFaceId = vFace->getDataObjectID();
            //    //长度不够则添加长度
            //    if (faceWeight.size() < i + 1)
            //        faceWeight.append(0);
            //
            //    faceWeight[i] += 1;
            //    temp->CmdId = geoCmd->getDataObjectID();
            //    temp->VirtualTopoId = newFaceId;
            //}
        }
        // 刷新拾取边计数。
        updateEdgesLabel();
        //if (temp->CmdId != 0 && temp->VirtualTopoId != 0)
        //    m_tempVShapes.insert(0, temp);
    }
}

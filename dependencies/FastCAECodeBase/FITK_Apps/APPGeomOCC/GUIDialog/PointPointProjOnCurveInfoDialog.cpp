/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PointPointProjOnCurveInfoDialog.h"
#include "ui_PointPointProjOnCurveInfoDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"

#include <QMessageBox>
#include "PointPointProjOnCurveInfoDialog.h"

namespace GUI {

    PointPointProjOnCurveInfoDialog::PointPointProjOnCurveInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::PointPointProjOnCurveInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    PointPointProjOnCurveInfoDialog::~PointPointProjOnCurveInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    PointPointProjOnCurveInfoDialog* PointPointProjOnCurveInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelPointProjectionOnCurve*>(cmd);

        static PointPointProjOnCurveInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new PointPointProjOnCurveInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_pointprojoncurve_pointpicked->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->lb_pointprojoncurve_curvepicked->setText(tr("Picked (0)"));
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_PointProjOnCurvePoint = command->sourcePoint();
            uniqueObj->m_PointProjOnCurveCurve = command->sourceCurve();
            uniqueObj->m_Ui->lb_pointprojoncurve_pointpicked->setText(uniqueObj->m_PointProjOnCurvePoint.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            uniqueObj->m_Ui->lb_pointprojoncurve_curvepicked->setText(uniqueObj->m_PointProjOnCurveCurve.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        }
        /*@}*/

        return uniqueObj;
    }

    bool PointPointProjOnCurveInfoDialog::checkDialogData()
    {
        // 数据检查
        if (m_PointProjOnCurvePoint.isNull() || m_PointProjOnCurveCurve.isNull()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto vPoint = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_PointProjOnCurvePoint.VirtualTopoId);
        auto vCurve = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_PointProjOnCurveCurve.VirtualTopoId);
        if (vPoint == nullptr || vCurve == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }
        return true;
    }

    void PointPointProjOnCurveInfoDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 断开拾取信号
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->disconnect(this);

        // 重置窗口数据
        delete m_PreviewCmd;
        m_PreviewCmd = nullptr;
        m_dataId = -1;
    }

    void PointPointProjOnCurveInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void PointPointProjOnCurveInfoDialog::on_pb_pointprojoncurve_pointpick_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJVert;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();

            if (pickedList.size() == 1) {
                auto pickData = pickedList.at(0);
                if (pickData == nullptr) return;

                // 获取虚拓扑形状
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_PointProjOnCurvePoint = virtualShapes.at(0);
            }
            else {
                m_PointProjOnCurvePoint.reset();
            }
            m_Ui->lb_pointprojoncurve_pointpicked->setText(m_PointProjOnCurvePoint.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void PointPointProjOnCurveInfoDialog::on_pb_pointprojoncurve_curvepick_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJEdge;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            if (pickedList.size() == 1)
            {
                auto pickData = pickedList.at(0);
                if (pickData == nullptr) return;

                // 获取虚拓扑形状
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_PointProjOnCurveCurve = virtualShapes.at(0);
            }
            else {
                m_PointProjOnCurveCurve.reset();
            }
            m_Ui->lb_pointprojoncurve_curvepicked->setText(m_PointProjOnCurveCurve.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void PointPointProjOnCurveInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("SourcePoint", QVariant::fromValue(m_PointProjOnCurvePoint));
        m_Operator->setArgs("SourceCurve", QVariant::fromValue(m_PointProjOnCurveCurve));

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void PointPointProjOnCurveInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelPointProjectionOnCurve>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPointProjectionOnCurve);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourcePoint(m_PointProjOnCurvePoint);
        m_PreviewCmd->setSourceCurve(m_PointProjOnCurveCurve);
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void PointPointProjOnCurveInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}

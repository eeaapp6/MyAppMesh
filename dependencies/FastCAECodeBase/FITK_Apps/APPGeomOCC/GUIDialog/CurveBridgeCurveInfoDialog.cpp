/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CurveBridgeCurveInfoDialog.h"
#include "ui_CurveBridgeCurveInfoDialog.h"
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

namespace GUI {

    CurveBridgeCurveInfoDialog::CurveBridgeCurveInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::CurveBridgeCurveInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    CurveBridgeCurveInfoDialog::~CurveBridgeCurveInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    CurveBridgeCurveInfoDialog* CurveBridgeCurveInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelBridgeCurve*>(cmd);

        static CurveBridgeCurveInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new CurveBridgeCurveInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_bridge_curve1picked->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->le_bridge_parameter1->setValue(1);
            uniqueObj->m_Ui->lb_bridge_curve2picked->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->le_bridge_parameter2->setValue(1);
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_SourceCurve1 = command->sourceCurve1();
            uniqueObj->m_UseStartEnd1 = command->useStartEnd1();
            uniqueObj->m_SourceCurve2 = command->sourceCurve2();
            uniqueObj->m_UseStartEnd2 = command->useStartEnd2();
            uniqueObj->m_Ui->lb_bridge_curve1picked->setText(uniqueObj->m_SourceCurve1.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            uniqueObj->m_Ui->lb_bridge_curve1end->setText(uniqueObj->m_UseStartEnd1 ? tr("Start Point") : tr("End Point"));
            uniqueObj->m_Ui->le_bridge_parameter1->setValue(command->parameter1());
            uniqueObj->m_Ui->lb_bridge_curve2picked->setText(uniqueObj->m_SourceCurve2.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            uniqueObj->m_Ui->lb_bridge_curve2end->setText(uniqueObj->m_UseStartEnd2 ? tr("Start Point") : tr("End Point"));
            uniqueObj->m_Ui->le_bridge_parameter2->setValue(command->parameter2());

        }
        /*@}*/

        return uniqueObj;
    }

    bool CurveBridgeCurveInfoDialog::checkDialogData()
    {
        // 数据检查
        if (m_SourceCurve1.isNull() || m_SourceCurve2.isNull() || m_Ui->le_bridge_parameter1->text().isEmpty() || m_Ui->le_bridge_parameter2->text().isEmpty()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto vCurve1 = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceCurve1.VirtualTopoId);
        auto vCurve2 = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceCurve2.VirtualTopoId);
        if (vCurve1 == nullptr || vCurve2 == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }
        return true;
    }

    void CurveBridgeCurveInfoDialog::finalize()
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

        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!m_SourceCurve1.isNull() && graphPreprocessOper != nullptr) {
            graphPreprocessOper->clearPreview(m_SourceCurve1.VirtualTopoId);
        }
        if (!m_SourceCurve2.isNull() && graphPreprocessOper != nullptr) {
            graphPreprocessOper->clearPreview(m_SourceCurve2.VirtualTopoId);
        }

        m_SourceCurve1.reset();
        m_SourceCurve2.reset();
    }

    void CurveBridgeCurveInfoDialog::on_pb_bridge_curve1pick_clicked()
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
            auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (!m_SourceCurve1.isNull() && graphPreprocessOper != nullptr) {
                graphPreprocessOper->clearPreview(m_SourceCurve1.VirtualTopoId);
            }
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();

            if (pickedList.size() == 1) {
                auto pickData = pickedList.at(0);
                if (pickData == nullptr) return;

                // 获取虚拓扑
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_SourceCurve1 = virtualShapes.at(0);
                if (graphPreprocessOper != nullptr) {
                    graphPreprocessOper->preview(m_SourceCurve1.VirtualTopoId, EventOper::PreviewType::PT_IndexLabels, Qt::red);
                }
            }
            else {
                m_SourceCurve1.reset();
            }
            m_Ui->lb_bridge_curve1picked->setText(m_SourceCurve1.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void CurveBridgeCurveInfoDialog::on_pb_bridge_curve1toogle_clicked()
    {
        m_UseStartEnd1 = !m_UseStartEnd1;
        m_Ui->lb_bridge_curve1end->setText(m_UseStartEnd1 ? tr("Start Point") : tr("End Point"));
    }

    void CurveBridgeCurveInfoDialog::on_pb_bridge_curve2pick_clicked()
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
            auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (!m_SourceCurve2.isNull() && graphPreprocessOper != nullptr) {
                graphPreprocessOper->clearPreview(m_SourceCurve2.VirtualTopoId);
            }
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();

            if (pickedList.size() == 1) {
                auto pickData = pickedList.at(0);
                if (pickData == nullptr) return;

                // 获取虚拓扑
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_SourceCurve2 = virtualShapes.at(0);

                if (graphPreprocessOper != nullptr) {
                    graphPreprocessOper->preview(m_SourceCurve2.VirtualTopoId, EventOper::PreviewType::PT_IndexLabels, Qt::blue);
                }
            }
            else {
                m_SourceCurve2.reset();
            }
            m_Ui->lb_bridge_curve2picked->setText(m_SourceCurve2.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void CurveBridgeCurveInfoDialog::on_pb_bridge_curve2toogle_clicked()
    {
        m_UseStartEnd2 = !m_UseStartEnd2;
        m_Ui->lb_bridge_curve2end->setText(m_UseStartEnd2 ? tr("Start Point") : tr("End Point"));
    }

    void CurveBridgeCurveInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void CurveBridgeCurveInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("SourceCurve1", QVariant::fromValue(m_SourceCurve1));
        m_Operator->setArgs("SourceCurve2", QVariant::fromValue(m_SourceCurve2));
        m_Operator->setArgs("UseStartEnd1", QVariant::fromValue(m_UseStartEnd1));
        m_Operator->setArgs("UseStartEnd2", QVariant::fromValue(m_UseStartEnd2));
        m_Operator->setArgs("Parameter1", QVariant::fromValue(m_Ui->le_bridge_parameter1->value()));
        m_Operator->setArgs("Parameter2", QVariant::fromValue(m_Ui->le_bridge_parameter2->value()));

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void CurveBridgeCurveInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelBridgeCurve>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBridgeCurve);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourceCurve1(m_SourceCurve1);
        m_PreviewCmd->setUseStartEnd1(m_UseStartEnd1);
        m_PreviewCmd->setParameter1(m_Ui->le_bridge_parameter1->value());
        m_PreviewCmd->setSourceCurve2(m_SourceCurve2);
        m_PreviewCmd->setUseStartEnd2(m_UseStartEnd2);
        m_PreviewCmd->setParameter2(m_Ui->le_bridge_parameter2->value());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void CurveBridgeCurveInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}

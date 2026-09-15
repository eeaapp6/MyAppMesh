/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PointInterTwoCurveInfoDialog.h"
#include "ui_PointInterTwoCurveInfoDialog.h"
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

    PointInterTwoCurveInfoDialog::PointInterTwoCurveInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::PointInterTwoCurveInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    PointInterTwoCurveInfoDialog::~PointInterTwoCurveInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    PointInterTwoCurveInfoDialog* PointInterTwoCurveInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelTwoCurveIntersection*>(cmd);

        static PointInterTwoCurveInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new PointInterTwoCurveInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_intertwocurves_curve1picked->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->lb_intertwocurves_curve2picked->setText(tr("Picked (0)"));
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_TwoCurveInterCurve1 = command->sourceCurve1();
            uniqueObj->m_TwoCurveInterCurve2 = command->sourceCurve2();
            uniqueObj->m_Ui->comb_type->setCurrentIndex(command->isNearestPoint() ? 1 : 0);
            uniqueObj->m_Ui->lb_intertwocurves_curve1picked->setText(uniqueObj->m_TwoCurveInterCurve1.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            uniqueObj->m_Ui->lb_intertwocurves_curve2picked->setText(uniqueObj->m_TwoCurveInterCurve2.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        }
        /*@}*/

        return uniqueObj;
    }

    bool PointInterTwoCurveInfoDialog::checkDialogData()
    {
        // 数据检查
        if (m_TwoCurveInterCurve1.isNull() || m_TwoCurveInterCurve2.isNull()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto vCurve1 = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_TwoCurveInterCurve1.VirtualTopoId);
        auto vCurve2 = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_TwoCurveInterCurve2.VirtualTopoId);
        if (vCurve1 == nullptr || vCurve2 == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }
        return true;
    }

    void PointInterTwoCurveInfoDialog::finalize()
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

    void PointInterTwoCurveInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        //m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void PointInterTwoCurveInfoDialog::on_pb_intertwocurves_curve1pick_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJEdge;
        pickInfo._enableDatumPick = true;
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
                m_TwoCurveInterCurve1 = virtualShapes.at(0);
            }
            else {
                m_TwoCurveInterCurve1.reset();
            }
            m_Ui->lb_intertwocurves_curve1picked->setText(m_TwoCurveInterCurve1.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void PointInterTwoCurveInfoDialog::on_pb_intertwocurves_curve2pick_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJEdge;
        pickInfo._enableDatumPick = true;
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
                m_TwoCurveInterCurve2 = virtualShapes.at(0);
            }
            else {
                m_TwoCurveInterCurve2.reset();
            }
            m_Ui->lb_intertwocurves_curve2picked->setText(m_TwoCurveInterCurve2.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void PointInterTwoCurveInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("SourceCurve1", QVariant::fromValue(m_TwoCurveInterCurve1));
        m_Operator->setArgs("SourceCurve2", QVariant::fromValue(m_TwoCurveInterCurve2));
        m_Operator->setArgs("IsNearestPoint", m_Ui->comb_type->currentIndex() == 1);

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void PointInterTwoCurveInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelTwoCurveIntersection>(Interface::FITKGeoEnum::FITKGeometryComType::FGTTwoCurveIntersectPoint);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourceCurve(m_TwoCurveInterCurve1, m_TwoCurveInterCurve2);
        m_PreviewCmd->setNearestPoint(m_Ui->comb_type->currentIndex() == 1);
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void PointInterTwoCurveInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}

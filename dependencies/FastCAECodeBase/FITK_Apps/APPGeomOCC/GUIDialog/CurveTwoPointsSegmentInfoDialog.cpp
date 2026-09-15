/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CurveTwoPointsSegmentInfoDialog.h"
#include "ui_CurveTwoPointsSegmentInfoDialog.h"

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

    CurveTwoPointsSegmentInfoDialog::CurveTwoPointsSegmentInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::CurveTwoPointsSegmentInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    CurveTwoPointsSegmentInfoDialog::~CurveTwoPointsSegmentInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    CurveTwoPointsSegmentInfoDialog * CurveTwoPointsSegmentInfoDialog::New(QWidget * parent,
        EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoCommand * cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelSegment*>(cmd);

        static CurveTwoPointsSegmentInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new CurveTwoPointsSegmentInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->le_twopoint_p1x->setValue(0);
            uniqueObj->m_Ui->le_twopoint_p1y->setValue(0);
            uniqueObj->m_Ui->le_twopoint_p1z->setValue(0);

            uniqueObj->m_Ui->le_twopoint_p2x->setValue(0);
            uniqueObj->m_Ui->le_twopoint_p2y->setValue(0);
            uniqueObj->m_Ui->le_twopoint_p2z->setValue(0);
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            auto xyz = command->startPoint();
            uniqueObj->m_Ui->le_twopoint_p1x->setValue(xyz[0]);
            uniqueObj->m_Ui->le_twopoint_p1y->setValue(xyz[1]);
            uniqueObj->m_Ui->le_twopoint_p1z->setValue(xyz[2]);

            xyz = command->endPoint();
            uniqueObj->m_Ui->le_twopoint_p2x->setValue(xyz[0]);
            uniqueObj->m_Ui->le_twopoint_p2y->setValue(xyz[1]);
            uniqueObj->m_Ui->le_twopoint_p2z->setValue(xyz[2]);
        }
        /*@}*/

        return uniqueObj;
    }

    bool CurveTwoPointsSegmentInfoDialog::checkDialogData()
    {
        // 数据检查
        if (m_Ui->le_twopoint_p1x->text().isEmpty() || m_Ui->le_twopoint_p1y->text().isEmpty() || m_Ui->le_twopoint_p1z->text().isEmpty()
            || m_Ui->le_twopoint_p2x->text().isEmpty() || m_Ui->le_twopoint_p2y->text().isEmpty() || m_Ui->le_twopoint_p2z->text().isEmpty()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        return true;
    }

    void CurveTwoPointsSegmentInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void CurveTwoPointsSegmentInfoDialog::finalize()
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

    void CurveTwoPointsSegmentInfoDialog::on_pb_twopoint_pickpoint1_clicked()
    {
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
            if (pickedList.size() != 1) return;
            auto pickData = pickedList.at(0);
            if (pickData == nullptr) return;

            auto virtualTopo = pickData->getVirtualTopo();
            if (virtualTopo == nullptr) return;

            auto pointTool = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator()->createPointTool();
            double xyz[3];
            if (!pointTool->getXYZByID(virtualTopo->getDataObjectID(), xyz))return;


            m_Ui->le_twopoint_p1x->setValue(xyz[0]);
            m_Ui->le_twopoint_p1y->setValue(xyz[1]);
            m_Ui->le_twopoint_p1z->setValue(xyz[2]);


            delete pointTool;
        });
    }

    void CurveTwoPointsSegmentInfoDialog::on_pb_twopoint_pickpoint2_clicked()
    {
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
            if (pickedList.size() != 1) return;
            auto pickData = pickedList.at(0);
            if (pickData == nullptr) return;

            auto virtualTopo = pickData->getVirtualTopo();
            if (virtualTopo == nullptr) return;

            auto pointTool = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator()->createPointTool();
            double xyz[3];
            if (!pointTool->getXYZByID(virtualTopo->getDataObjectID(), xyz))return;

            m_Ui->le_twopoint_p2x->setValue(xyz[0]);
            m_Ui->le_twopoint_p2y->setValue(xyz[1]);
            m_Ui->le_twopoint_p2z->setValue(xyz[2]);

            delete pointTool;
        });
    }

    void CurveTwoPointsSegmentInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("P1x", m_Ui->le_twopoint_p1x->value());
        m_Operator->setArgs("P1y", m_Ui->le_twopoint_p1y->value());
        m_Operator->setArgs("P1z", m_Ui->le_twopoint_p1z->value());

        m_Operator->setArgs("P2x", m_Ui->le_twopoint_p2x->value());
        m_Operator->setArgs("P2y", m_Ui->le_twopoint_p2y->value());
        m_Operator->setArgs("P2z", m_Ui->le_twopoint_p2z->value());

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);

        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void CurveTwoPointsSegmentInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelSegment>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSegment);
        }
        // 更新预览对象数据
        m_PreviewCmd->setStartPoint(m_Ui->le_twopoint_p1x->value(), m_Ui->le_twopoint_p1y->value(), m_Ui->le_twopoint_p1z->value());
        m_PreviewCmd->setEndPoint(m_Ui->le_twopoint_p2x->value(), m_Ui->le_twopoint_p2y->value(), m_Ui->le_twopoint_p2z->value());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void CurveTwoPointsSegmentInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        // 关闭窗口
        reject();
    }
}

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CurveThreePointsEllipseInfoDialog.h"
#include "ui_CurveThreePointsEllipseInfoDialog.h"

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

    CurveThreePointsEllipseInfoDialog::CurveThreePointsEllipseInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::CurveThreePointsEllipseInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    CurveThreePointsEllipseInfoDialog::~CurveThreePointsEllipseInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    CurveThreePointsEllipseInfoDialog * CurveThreePointsEllipseInfoDialog::New(QWidget * parent,
        EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoCommand * cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelThreePointsEllipse*>(cmd);

        static CurveThreePointsEllipseInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new CurveThreePointsEllipseInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->le_3pellipse_x1->setValue(0);
            uniqueObj->m_Ui->le_3pellipse_y1->setValue(0);
            uniqueObj->m_Ui->le_3pellipse_z1->setValue(0);

            uniqueObj->m_Ui->le_3pellipse_x2->setValue(0);
            uniqueObj->m_Ui->le_3pellipse_y2->setValue(0);
            uniqueObj->m_Ui->le_3pellipse_z2->setValue(0);

            uniqueObj->m_Ui->le_3pellipse_x3->setValue(0);
            uniqueObj->m_Ui->le_3pellipse_y3->setValue(0);
            uniqueObj->m_Ui->le_3pellipse_z3->setValue(0);
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            auto point = command->origin();
            uniqueObj->m_Ui->le_3pellipse_x1->setValue(point[0]);
            uniqueObj->m_Ui->le_3pellipse_y1->setValue(point[1]);
            uniqueObj->m_Ui->le_3pellipse_z1->setValue(point[2]);

            point = command->majorPoint();
            uniqueObj->m_Ui->le_3pellipse_x2->setValue(point[0]);
            uniqueObj->m_Ui->le_3pellipse_y2->setValue(point[1]);
            uniqueObj->m_Ui->le_3pellipse_z2->setValue(point[2]);

            point = command->minorPoint();
            uniqueObj->m_Ui->le_3pellipse_x3->setValue(point[0]);
            uniqueObj->m_Ui->le_3pellipse_y3->setValue(point[1]);
            uniqueObj->m_Ui->le_3pellipse_z3->setValue(point[2]);
        }
        /*@}*/

        return uniqueObj;
    }

    bool CurveThreePointsEllipseInfoDialog::checkDialogData()
    {
        // 数据检查
        if (m_Ui->le_3pellipse_x1->text().isEmpty() || m_Ui->le_3pellipse_y1->text().isEmpty() || m_Ui->le_3pellipse_z1->text().isEmpty()
            || m_Ui->le_3pellipse_x2->text().isEmpty() || m_Ui->le_3pellipse_y2->text().isEmpty() || m_Ui->le_3pellipse_z2->text().isEmpty()
            || m_Ui->le_3pellipse_x3->text().isEmpty() || m_Ui->le_3pellipse_y3->text().isEmpty() || m_Ui->le_3pellipse_z3->text().isEmpty()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        return true;
    }

    void CurveThreePointsEllipseInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void CurveThreePointsEllipseInfoDialog::finalize()
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

    void CurveThreePointsEllipseInfoDialog::on_pb_3pellipse_pickpoint1_clicked()
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

            m_Ui->le_3pellipse_x1->setValue(xyz[0]);
            m_Ui->le_3pellipse_y1->setValue(xyz[1]);
            m_Ui->le_3pellipse_z1->setValue(xyz[2]);

            delete pointTool;
        });
    }

    void CurveThreePointsEllipseInfoDialog::on_pb_3pellipse_pickpoint2_clicked()
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

            m_Ui->le_3pellipse_x2->setValue(xyz[0]);
            m_Ui->le_3pellipse_y2->setValue(xyz[1]);
            m_Ui->le_3pellipse_z2->setValue(xyz[2]);

            delete pointTool;
        });
    }

    void CurveThreePointsEllipseInfoDialog::on_pb_3pellipse_pickpoint3_clicked()
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

            m_Ui->le_3pellipse_x3->setValue(xyz[0]);
            m_Ui->le_3pellipse_y3->setValue(xyz[1]);
            m_Ui->le_3pellipse_z3->setValue(xyz[2]);

            delete pointTool;
        });
    }


    void CurveThreePointsEllipseInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("P1x", m_Ui->le_3pellipse_x1->value());
        m_Operator->setArgs("P1y", m_Ui->le_3pellipse_y1->value());
        m_Operator->setArgs("P1z", m_Ui->le_3pellipse_z1->value());

        m_Operator->setArgs("P2x", m_Ui->le_3pellipse_x2->value());
        m_Operator->setArgs("P2y", m_Ui->le_3pellipse_y2->value());
        m_Operator->setArgs("P2z", m_Ui->le_3pellipse_z2->value());

        m_Operator->setArgs("P3x", m_Ui->le_3pellipse_x3->value());
        m_Operator->setArgs("P3y", m_Ui->le_3pellipse_y3->value());
        m_Operator->setArgs("P3z", m_Ui->le_3pellipse_z3->value());

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();

        // 关闭对话框
        accept();
    }

    void CurveThreePointsEllipseInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelThreePointsEllipse>(Interface::FITKGeoEnum::FITKGeometryComType::FGTThreePointsEllipse);
        }
        // 更新预览对象数据
        m_PreviewCmd->setOrigin(m_Ui->le_3pellipse_x1->value(), m_Ui->le_3pellipse_y1->value(), m_Ui->le_3pellipse_z1->value());
        m_PreviewCmd->setMajorPoint(m_Ui->le_3pellipse_x2->value(), m_Ui->le_3pellipse_y2->value(), m_Ui->le_3pellipse_z2->value());
        m_PreviewCmd->setMinorPoint(m_Ui->le_3pellipse_x3->value(), m_Ui->le_3pellipse_y3->value(), m_Ui->le_3pellipse_z3->value());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void CurveThreePointsEllipseInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        // 关闭窗口
        reject();
    }
}

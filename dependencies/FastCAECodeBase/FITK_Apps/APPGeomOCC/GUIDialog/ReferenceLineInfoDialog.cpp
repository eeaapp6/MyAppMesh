/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ReferenceLineInfoDialog.h"
#include "ui_ReferenceLineInfoDialog.h"

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

    ReferenceLineInfoDialog::ReferenceLineInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::ReferenceLineInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    ReferenceLineInfoDialog::~ReferenceLineInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    ReferenceLineInfoDialog * ReferenceLineInfoDialog::New(QWidget * parent,
        EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoDatumLine * cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoDatumLine*>(cmd);

        static ReferenceLineInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new ReferenceLineInfoDialog(parent);
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

            uniqueObj->m_Ui->le_pointanddir_dx->setValue(0);
            uniqueObj->m_Ui->le_pointanddir_dy->setValue(0);
            uniqueObj->m_Ui->le_pointanddir_dz->setValue(0);

            uniqueObj->m_Ui->le_pointanddir_px->setValue(0);
            uniqueObj->m_Ui->le_pointanddir_py->setValue(0);
            uniqueObj->m_Ui->le_pointanddir_pz->setValue(0);

            uniqueObj->m_Ui->chec_infinite->setChecked(false);
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            double xyz[3];
            command->getPosition(xyz);
            uniqueObj->m_Ui->le_twopoint_p1x->setValue(xyz[0]);
            uniqueObj->m_Ui->le_twopoint_p1y->setValue(xyz[1]);
            uniqueObj->m_Ui->le_twopoint_p1z->setValue(xyz[2]);

            command->getPosition2(xyz);
            uniqueObj->m_Ui->le_twopoint_p2x->setValue(xyz[0]);
            uniqueObj->m_Ui->le_twopoint_p2y->setValue(xyz[1]);
            uniqueObj->m_Ui->le_twopoint_p2z->setValue(xyz[2]);

            uniqueObj->m_Ui->chec_infinite->setChecked(command->getIsInfinite());
        }
        /*@}*/

        return uniqueObj;
    }

    bool ReferenceLineInfoDialog::checkDialogData()
    {
        // 数据检查
        switch (m_Ui->comb_type->currentIndex())
        {
            //两点创建基准线
        case 0:
            if (m_Ui->le_twopoint_p1x->text().isEmpty() || m_Ui->le_twopoint_p1y->text().isEmpty() || m_Ui->le_twopoint_p1z->text().isEmpty()
                || m_Ui->le_twopoint_p2x->text().isEmpty() || m_Ui->le_twopoint_p2y->text().isEmpty() || m_Ui->le_twopoint_p2z->text().isEmpty()) {
                QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
                return false;
            }
            break;
            // 点和方向
        case 1:
            if (m_Ui->le_pointanddir_dx->text().isEmpty() || m_Ui->le_pointanddir_dy->text().isEmpty() || m_Ui->le_pointanddir_dz->text().isEmpty()
                || m_Ui->le_pointanddir_px->text().isEmpty() || m_Ui->le_pointanddir_py->text().isEmpty() || m_Ui->le_pointanddir_pz->text().isEmpty()) {
                QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
                return false;
            }
            break;
        default:
            return false;
            break;
        }

        return true;
    }

    void ReferenceLineInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void ReferenceLineInfoDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 断开拾取信号
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->disconnect(this);

        // 清除箭头显示
        if (m_GraphToolOper != nullptr) {
            m_GraphToolOper->clearPickTool();
        }

        // 重置窗口数据
        delete m_PreviewCmd;
        m_PreviewCmd = nullptr;
        m_dataId = -1;
    }

    void ReferenceLineInfoDialog::on_pb_twopoint_pickpoint1_clicked()
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

    void ReferenceLineInfoDialog::on_pb_twopoint_pickpoint2_clicked()
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

    void ReferenceLineInfoDialog::on_pb_pointanddir_pickpoint_clicked()
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


            m_Ui->le_pointanddir_px->setValue(xyz[0]);
            m_Ui->le_pointanddir_py->setValue(xyz[1]);
            m_Ui->le_pointanddir_pz->setValue(xyz[2]);


            delete pointTool;
        });
    }

    void ReferenceLineInfoDialog::on_pb_pointanddir_pickdir_clicked()
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
            if (pickedList.size() != 1) return;

            m_GraphToolOper->setPickToolObjectVisible(EventOper::PickToolType::PTT_Direction, true);
            connect(pickDataProvider, SIGNAL(sig_directionPicked()), this, SLOT(onDirectionReverseSlot()), Qt::UniqueConnection);
            // 重新获取箭头方向
            onDirectionReverseSlot();
        });
    }

    void ReferenceLineInfoDialog::onDirectionReverseSlot()
    {
        QList<double> direction;

        auto graphToolOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphToolOperator>("GraphTool");
        if (graphToolOper == nullptr) return;
        if (graphToolOper->getPickedDirection(direction) && direction.size() == 3) {
            m_Ui->le_pointanddir_dx->setValue(direction.at(0));
            m_Ui->le_pointanddir_dy->setValue(direction.at(1));
            m_Ui->le_pointanddir_dz->setValue(direction.at(2));
        }
    }

    void ReferenceLineInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        switch (m_Ui->comb_type->currentIndex())
        {
            // 两点创建基准线
        case 0:
        {
            double xyz1[3] = { m_Ui->le_twopoint_p1x->value(), m_Ui->le_twopoint_p1y->value(), m_Ui->le_twopoint_p1z->value() };
            double xyz2[3] = { m_Ui->le_twopoint_p2x->value(), m_Ui->le_twopoint_p2y->value(), m_Ui->le_twopoint_p2z->value() };

            m_Operator->setArgs("P1x", xyz1[0]);
            m_Operator->setArgs("P1y", xyz1[1]);
            m_Operator->setArgs("P1z", xyz1[2]);
            m_Operator->setArgs("P2x", xyz2[0]);
            m_Operator->setArgs("P2y", xyz2[1]);
            m_Operator->setArgs("P2z", xyz2[2]);

            m_Operator->setArgs("Dx", xyz2[0] - xyz1[0]);
            m_Operator->setArgs("Dy", xyz2[1] - xyz1[1]);
            m_Operator->setArgs("Dz", xyz2[2] - xyz1[2]);
            break;
        }
        // 点和方向创建基准线
        case 1:
        {
            double xyz1[3] = { m_Ui->le_pointanddir_px->value(), m_Ui->le_pointanddir_py->value(), m_Ui->le_pointanddir_pz->value() };
            double xyz2[3] = { m_Ui->le_pointanddir_dx->value(), m_Ui->le_pointanddir_dy->value(), m_Ui->le_pointanddir_dz->value() };

            m_Operator->setArgs("P1x", xyz1[0]);
            m_Operator->setArgs("P1y", xyz1[1]);
            m_Operator->setArgs("P1z", xyz1[2]);
            m_Operator->setArgs("Dx", xyz2[0]);
            m_Operator->setArgs("Dy", xyz2[1]);
            m_Operator->setArgs("Dz", xyz2[2]);

            m_Operator->setArgs("P1x", xyz2[0] + xyz1[0]);
            m_Operator->setArgs("P1y", xyz2[1] + xyz1[1]);
            m_Operator->setArgs("P1z", xyz2[2] + xyz1[2]);
            break;
        }
        default:
            break;
        }

        m_Operator->setArgs("IsInfinite", m_Ui->chec_infinite->isChecked());

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void ReferenceLineInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createDatumT<Interface::FITKAbsGeoDatumLine>(Interface::FITKGeoEnum::FDTLine);
        }
        // 更新预览对象数据
        switch (m_Ui->comb_type->currentIndex())
        {
            // 两点创建基准线
        case 0:
        {
            double xyz1[3] = { m_Ui->le_twopoint_p1x->value(), m_Ui->le_twopoint_p1y->value(), m_Ui->le_twopoint_p1z->value() };
            m_PreviewCmd->setPosition(xyz1);
            double xyz2[3] = { m_Ui->le_twopoint_p2x->value(), m_Ui->le_twopoint_p2y->value(), m_Ui->le_twopoint_p2z->value() };
            m_PreviewCmd->setPosition2(xyz2);
            double xyz3[3] = { m_Ui->le_twopoint_p2x->value() - m_Ui->le_twopoint_p1x->value(),
                m_Ui->le_twopoint_p2y->value() - m_Ui->le_twopoint_p1y->value(),
                m_Ui->le_twopoint_p2z->value() - m_Ui->le_twopoint_p1z->value() };
            //m_PreviewCmd->setDirection(xyz3);
            break;
        }
        // 点和方向创建基准线
        case 1:
        {
            double xyz1[3] = { m_Ui->le_pointanddir_px->value(), m_Ui->le_pointanddir_py->value(), m_Ui->le_pointanddir_pz->value() };
            m_PreviewCmd->setPosition(xyz1);
            double xyz2[3] = { m_Ui->le_pointanddir_dx->value(), m_Ui->le_pointanddir_dy->value(), m_Ui->le_pointanddir_dz->value() };
            //m_PreviewCmd->setDirection(xyz2);
            double xyz3[3] = { xyz1[0] + xyz2[0], xyz1[1] + xyz2[1], xyz1[2] + xyz2[2] };
            m_PreviewCmd->setPosition2(xyz3);
            break;
        }
        default:
            return;
            break;
        }
        m_PreviewCmd->setIsInfinite(m_Ui->chec_infinite->isChecked());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void ReferenceLineInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        // 关闭窗口
        reject();
    }
}



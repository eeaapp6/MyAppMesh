/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ReferencePlaneInfoDialog.h"
#include "ui_ReferencePlaneInfoDialog.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
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

    ReferencePlaneInfoDialog::ReferencePlaneInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::ReferencePlaneInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    ReferencePlaneInfoDialog::~ReferencePlaneInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    ReferencePlaneInfoDialog * ReferencePlaneInfoDialog::New(QWidget * parent,
        EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoDatumPlane * cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoDatumPlane*>(cmd);

        static ReferencePlaneInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new ReferencePlaneInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_source->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->le_offset->setValue(0);

            uniqueObj->m_Ui->le_3p_x1->setValue(0);
            uniqueObj->m_Ui->le_3p_y1->setValue(0);
            uniqueObj->m_Ui->le_3p_z1->setValue(0);
            uniqueObj->m_Ui->le_3p_x2->setValue(0);
            uniqueObj->m_Ui->le_3p_y2->setValue(0);
            uniqueObj->m_Ui->le_3p_z2->setValue(0);
            uniqueObj->m_Ui->le_3p_x3->setValue(0);
            uniqueObj->m_Ui->le_3p_y3->setValue(0);
            uniqueObj->m_Ui->le_3p_z3->setValue(0);

            uniqueObj->m_Ui->le_equation_a->setValue(0);
            uniqueObj->m_Ui->le_equation_b->setValue(0);
            uniqueObj->m_Ui->le_equation_c->setValue(0);
            uniqueObj->m_Ui->le_equation_d->setValue(0);

            uniqueObj->m_Ui->le_pointanddir_px->setValue(0);
            uniqueObj->m_Ui->le_pointanddir_py->setValue(0);
            uniqueObj->m_Ui->le_pointanddir_pz->setValue(0);
            uniqueObj->m_Ui->le_pointanddir_dx->setValue(0);
            uniqueObj->m_Ui->le_pointanddir_dy->setValue(0);
            uniqueObj->m_Ui->le_pointanddir_dz->setValue(0);
        }
        else {

            switch (command->getDatumType())
            {
            case Interface::FITKGeoEnum::FITKDatumType::FDTOffsetPlane:
            {
                auto cmd = dynamic_cast<Interface::FITKAbsGeoReferenceOffsetPlane*>(command);
                if (cmd == nullptr) break;
                uniqueObj->m_dataId = cmd->getDataObjectID();
                uniqueObj->m_SourceFace = cmd->sourceSurface();
                uniqueObj->m_Ui->lb_source->setText(uniqueObj->m_SourceFace.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
                uniqueObj->m_Ui->le_offset->setValue(cmd->offset());
                uniqueObj->m_GraphToolOper->setPickDirectionAnchorTopoId(uniqueObj->m_SourceFace.CmdId, uniqueObj->m_SourceFace.VirtualTopoId);
                uniqueObj->m_GraphToolOper->setPickToolObjectVisible(EventOper::PickToolType::PTT_Direction, true);
                break;
            }
            case Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane:
            {
                auto cmd = dynamic_cast<Interface::FITKAbsGeoReferenceThreePointsPlane*>(command);
                if (cmd == nullptr) break;
                uniqueObj->m_dataId = cmd->getDataObjectID();
                auto p = cmd->point(0);
                uniqueObj->m_Ui->le_3p_x1->setValue(p.at(0));
                uniqueObj->m_Ui->le_3p_y1->setValue(p.at(1));
                uniqueObj->m_Ui->le_3p_z1->setValue(p.at(2));
                p = cmd->point(1);
                uniqueObj->m_Ui->le_3p_x2->setValue(p.at(0));
                uniqueObj->m_Ui->le_3p_y2->setValue(p.at(1));
                uniqueObj->m_Ui->le_3p_z2->setValue(p.at(2));
                p = cmd->point(2);
                uniqueObj->m_Ui->le_3p_x3->setValue(p.at(0));
                uniqueObj->m_Ui->le_3p_y3->setValue(p.at(1));
                uniqueObj->m_Ui->le_3p_z3->setValue(p.at(2));
                break;
            }
            case Interface::FITKGeoEnum::FITKDatumType::FDTEquationPlane:
            {
                auto cmd = dynamic_cast<Interface::FITKAbsGeoReferenceEquationPlane*>(command);
                if (cmd == nullptr) break;
                uniqueObj->m_dataId = cmd->getDataObjectID();
                uniqueObj->m_Ui->le_equation_a->setValue(cmd->a());
                uniqueObj->m_Ui->le_equation_b->setValue(cmd->b());
                uniqueObj->m_Ui->le_equation_c->setValue(cmd->c());
                uniqueObj->m_Ui->le_equation_d->setValue(cmd->d());
                break;
            }
            case Interface::FITKGeoEnum::FITKDatumType::FDTPointAndDirectionPlane:
            {
                auto cmd = dynamic_cast<Interface::FITKAbsGeoReferencePointAndDirectionPlane*>(command);
                if (cmd == nullptr) break;
                uniqueObj->m_dataId = cmd->getDataObjectID();
                auto p = cmd->point();
                uniqueObj->m_Ui->le_pointanddir_px->setValue(p.at(0));
                uniqueObj->m_Ui->le_pointanddir_py->setValue(p.at(1));
                uniqueObj->m_Ui->le_pointanddir_pz->setValue(p.at(2));
                p = cmd->direction();
                uniqueObj->m_Ui->le_pointanddir_dx->setValue(p.at(0));
                uniqueObj->m_Ui->le_pointanddir_dy->setValue(p.at(1));
                uniqueObj->m_Ui->le_pointanddir_dz->setValue(p.at(2));
                break;
            }
            default:
                break;
            }

        }
        /*@}*/

        return uniqueObj;
    }

    bool ReferencePlaneInfoDialog::checkDialogData() const
    {
        // 数据检查
        switch (m_Ui->comb_type->currentIndex())
        {
            // 偏移
        case 0:
        {
            if (m_SourceFace.isNull() || m_Ui->le_offset->text().isEmpty()) {
                QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
                return false;
            }
            // 检查虚拓扑数据是否存在
            auto vSurface = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceFace.VirtualTopoId);
            if (vSurface == nullptr)
            {
                QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
                return false;
            }
            return true;
            break;
        }
        // 三点
        case 1:
        {
            if (
                m_Ui->le_3p_x1->text().isEmpty() || m_Ui->le_3p_y1->text().isEmpty() || m_Ui->le_3p_z1->text().isEmpty() ||
                m_Ui->le_3p_x2->text().isEmpty() || m_Ui->le_3p_y2->text().isEmpty() || m_Ui->le_3p_z2->text().isEmpty() ||
                m_Ui->le_3p_x3->text().isEmpty() || m_Ui->le_3p_y3->text().isEmpty() || m_Ui->le_3p_z3->text().isEmpty()
                ) {
                QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
                return false;
            }
            return true;
            break;
        }
        // 方程
        case 2:
        {
            if (
                m_Ui->le_equation_a->text().isEmpty() || m_Ui->le_equation_b->text().isEmpty() || m_Ui->le_equation_c->text().isEmpty() ||
                m_Ui->le_equation_d->text().isEmpty()
                ) {
                QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
                return false;
            }
            return true;
            break;
        }
        // 点和法向
        case 3:
        {
            if (
                m_Ui->le_pointanddir_px->text().isEmpty() || m_Ui->le_pointanddir_py->text().isEmpty() || m_Ui->le_pointanddir_pz->text().isEmpty() ||
                m_Ui->le_pointanddir_dx->text().isEmpty() || m_Ui->le_pointanddir_dy->text().isEmpty() || m_Ui->le_pointanddir_dz->text().isEmpty()
                ) {
                QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
                return false;
            }
            return true;
            break;
        }
        default:
            break;
        }

        return true;
    }

    void ReferencePlaneInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void ReferencePlaneInfoDialog::finalize()
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

    void ReferencePlaneInfoDialog::on_pb_selectsource_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJFace;
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
                m_SourceFace = virtualShapes.at(0);

                // 设置方向箭头
                auto graphToolOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphToolOperator>("GraphTool");
                if (graphToolOper == nullptr) return;
                graphToolOper->setPickDirectionAnchorTopoId(m_SourceFace.CmdId, m_SourceFace.VirtualTopoId);
                m_GraphToolOper->setPickToolObjectVisible(EventOper::PickToolType::PTT_Direction, true);

            }
            else {
                m_SourceFace.reset();
            }
            m_Ui->lb_source->setText(m_SourceFace.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void ReferencePlaneInfoDialog::on_pb_3p_pickpoint1_clicked()
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

            m_Ui->le_3p_x1->setValue(xyz[0]);
            m_Ui->le_3p_y1->setValue(xyz[1]);
            m_Ui->le_3p_z1->setValue(xyz[2]);

            delete pointTool;
        });
    }

    void ReferencePlaneInfoDialog::on_pb_3p_pickpoint2_clicked()
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

            m_Ui->le_3p_x2->setValue(xyz[0]);
            m_Ui->le_3p_y2->setValue(xyz[1]);
            m_Ui->le_3p_z2->setValue(xyz[2]);

            delete pointTool;
        });
    }

    void ReferencePlaneInfoDialog::on_pb_3p_pickpoint3_clicked()
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

            m_Ui->le_3p_x3->setValue(xyz[0]);
            m_Ui->le_3p_y3->setValue(xyz[1]);
            m_Ui->le_3p_z3->setValue(xyz[2]);

            delete pointTool;
        });
    }

    void ReferencePlaneInfoDialog::on_pb_pointanddir_pickpoint_clicked()
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

    void ReferencePlaneInfoDialog::on_pb_pointanddir_pickdir_clicked()
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

    void ReferencePlaneInfoDialog::onDirectionReverseSlot()
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


    void ReferencePlaneInfoDialog::on_pb_ok_clicked()
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
            // 偏移
        case 0:
        {
            if (m_GraphToolOper == nullptr) return;
            QList<double> direction;
            if (!m_GraphToolOper->getPickedDirection(direction) || direction.size() != 3) return;
            m_Operator->setArgs("Type", Interface::FITKGeoEnum::FITKDatumType::FDTOffsetPlane);
            m_Operator->setArgs("SourceFace", QVariant::fromValue(m_SourceFace));
            m_Operator->setArgs("Offset", m_Ui->le_offset->value());
            m_Operator->setArgs("Dx", direction.at(0));
            m_Operator->setArgs("Dy", direction.at(1));
            m_Operator->setArgs("Dz", direction.at(2));
            break;
        }
        // 三点
        case 1:
        {
            m_Operator->setArgs("Type", Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane);
            m_Operator->setArgs("D1x", m_Ui->le_3p_x1->value());
            m_Operator->setArgs("D1y", m_Ui->le_3p_y1->value());
            m_Operator->setArgs("D1z", m_Ui->le_3p_z1->value());
            m_Operator->setArgs("D2x", m_Ui->le_3p_x2->value());
            m_Operator->setArgs("D2y", m_Ui->le_3p_y2->value());
            m_Operator->setArgs("D2z", m_Ui->le_3p_z2->value());
            m_Operator->setArgs("D3x", m_Ui->le_3p_x3->value());
            m_Operator->setArgs("D3y", m_Ui->le_3p_y3->value());
            m_Operator->setArgs("D3z", m_Ui->le_3p_z3->value());
            break;
        }
        // 方程
        case 2:
        {
            m_Operator->setArgs("Type", Interface::FITKGeoEnum::FITKDatumType::FDTEquationPlane);
            m_Operator->setArgs("A", m_Ui->le_equation_a->value());
            m_Operator->setArgs("B", m_Ui->le_equation_b->value());
            m_Operator->setArgs("C", m_Ui->le_equation_c->value());
            m_Operator->setArgs("D", m_Ui->le_equation_d->value());
            break;
        }
        // 点和法线
        case 3:
        {
            m_Operator->setArgs("Type", Interface::FITKGeoEnum::FITKDatumType::FDTPointAndDirectionPlane);
            m_Operator->setArgs("Px", m_Ui->le_pointanddir_px->value());
            m_Operator->setArgs("Py", m_Ui->le_pointanddir_py->value());
            m_Operator->setArgs("Pz", m_Ui->le_pointanddir_pz->value());
            m_Operator->setArgs("Dx", m_Ui->le_pointanddir_dx->value());
            m_Operator->setArgs("Dy", m_Ui->le_pointanddir_dy->value());
            m_Operator->setArgs("Dz", m_Ui->le_pointanddir_dz->value());
            break;
        }
        default:
            break;
        }

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void ReferencePlaneInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();

        // 更新预览对象数据
        switch (m_Ui->comb_type->currentIndex())
        {
            // 偏移
        case 0:
        {
            if (m_PreviewCmd == nullptr || m_PreviewCmd->getDatumType() != Interface::FITKGeoEnum::FITKDatumType::FDTOffsetPlane) {
                delete m_PreviewCmd;
                m_PreviewCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceOffsetPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTOffsetPlane);
            }
            auto preview = dynamic_cast<Interface::FITKAbsGeoReferenceOffsetPlane*>(m_PreviewCmd);
            preview->setSourceSurface(m_SourceFace);
            preview->setOffset(m_Ui->le_offset->value());
            auto graphToolOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphToolOperator>("GraphTool");
            if (graphToolOper == nullptr) return;
            QList<double> direction;
            if (graphToolOper->getPickedDirection(direction) && direction.size() == 3) {
                preview->setDirection(direction.at(0), direction.at(1), direction.at(2));
            }
            else {
                return;
            }
            break;
        }
        // 三点
        case 1:
        {
            if (m_PreviewCmd == nullptr || m_PreviewCmd->getDatumType() != Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane) {
                delete m_PreviewCmd;
                m_PreviewCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceThreePointsPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane);
            }
            auto preview = dynamic_cast<Interface::FITKAbsGeoReferenceThreePointsPlane*>(m_PreviewCmd);
            preview->setPoint(0, m_Ui->le_3p_x1->value(), m_Ui->le_3p_y1->value(), m_Ui->le_3p_z1->value());
            preview->setPoint(1, m_Ui->le_3p_x2->value(), m_Ui->le_3p_y2->value(), m_Ui->le_3p_z2->value());
            preview->setPoint(2, m_Ui->le_3p_x3->value(), m_Ui->le_3p_y3->value(), m_Ui->le_3p_z3->value());
            break;
        }
        // 方程
        case 2:
        {
            if (m_PreviewCmd == nullptr || m_PreviewCmd->getDatumType() != Interface::FITKGeoEnum::FITKDatumType::FDTEquationPlane) {
                delete m_PreviewCmd;
                m_PreviewCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceEquationPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTEquationPlane);
            }
            auto preview = dynamic_cast<Interface::FITKAbsGeoReferenceEquationPlane*>(m_PreviewCmd);
            preview->setA(m_Ui->le_equation_a->value());
            preview->setB(m_Ui->le_equation_b->value());
            preview->setC(m_Ui->le_equation_c->value());
            preview->setD(m_Ui->le_equation_d->value());
            break;
        }
        // 点和法线
        case 3:
        {
            if (m_PreviewCmd == nullptr || m_PreviewCmd->getDatumType() != Interface::FITKGeoEnum::FITKDatumType::FDTPointAndDirectionPlane) {
                delete m_PreviewCmd;
                m_PreviewCmd = fac->createDatumT<Interface::FITKAbsGeoReferencePointAndDirectionPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTPointAndDirectionPlane);
            }
            auto preview = dynamic_cast<Interface::FITKAbsGeoReferencePointAndDirectionPlane*>(m_PreviewCmd);

            preview->setPoint(m_Ui->le_pointanddir_px->value(), m_Ui->le_pointanddir_py->value(), m_Ui->le_pointanddir_pz->value());
            preview->setDirection(m_Ui->le_pointanddir_dx->value(), m_Ui->le_pointanddir_dy->value(), m_Ui->le_pointanddir_dz->value());
            break;
        }
        default:
            break;
        }

        if (m_PreviewCmd == nullptr || !m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void ReferencePlaneInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        // 关闭窗口
        reject();
    }
}
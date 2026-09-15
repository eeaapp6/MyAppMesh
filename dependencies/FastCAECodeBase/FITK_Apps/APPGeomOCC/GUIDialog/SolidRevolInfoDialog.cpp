/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SolidRevolInfoDialog.h"
#include "ui_SolidRevolInfoDialog.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"

#include <QMessageBox>

namespace GUI {
    SolidRevolInfoDialog::SolidRevolInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::SolidRevolInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    SolidRevolInfoDialog::~SolidRevolInfoDialog()
    {
        finalize();
        delete m_Ui;
    }
    SolidRevolInfoDialog* SolidRevolInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelRevolSolid*>(cmd);
        static SolidRevolInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new SolidRevolInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_source->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->le_p1x->setValue(0);
            uniqueObj->m_Ui->le_p1y->setValue(0);
            uniqueObj->m_Ui->le_p1z->setValue(0);
            uniqueObj->m_Ui->le_p2x->setValue(0);
            uniqueObj->m_Ui->le_p2y->setValue(0);
            uniqueObj->m_Ui->le_p2z->setValue(0);
            uniqueObj->m_Ui->le_angle->setValue(360);
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_SourceShape = command->sourceSurface();
            uniqueObj->m_Ui->lb_source->setText(uniqueObj->m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));

            auto xyz = command->rotateAxisPoint1();
            uniqueObj->m_Ui->le_p1x->setValue(xyz[0]);
            uniqueObj->m_Ui->le_p1y->setValue(xyz[1]);
            uniqueObj->m_Ui->le_p1z->setValue(xyz[2]);

            xyz = command->rotateAxisPoint2();
            uniqueObj->m_Ui->le_p2x->setValue(xyz[0]);
            uniqueObj->m_Ui->le_p2y->setValue(xyz[1]);
            uniqueObj->m_Ui->le_p2z->setValue(xyz[2]);

            uniqueObj->m_Ui->le_angle->setText(QString("%1").arg(command->angle()));
        }
        /*@}*/
        return uniqueObj;
    }
    bool SolidRevolInfoDialog::checkDialogData() const
    {
        // 数据检查
        if (m_SourceShape.isNull() || m_Ui->le_p1x->text().isEmpty() || m_Ui->le_p1y->text().isEmpty() || m_Ui->le_p1z->text().isEmpty() || m_Ui->le_p2x->text().isEmpty() || m_Ui->le_p2y->text().isEmpty() || m_Ui->le_p2z->text().isEmpty() || m_Ui->le_angle->text().isEmpty()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto vSurface = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceShape.VirtualTopoId);
        if (vSurface == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }
        return true;
    }


    void SolidRevolInfoDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->disconnect(this);

        // 重置窗口数据
       // 重置窗口数据
        delete m_PreviewCmd;
        m_PreviewCmd = nullptr;
        m_dataId = -1;

        m_SourceShape.reset();
    }

    void SolidRevolInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void SolidRevolInfoDialog::on_pb_selectsource_clicked()
    {
        // 设置拾取参数
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJFace;
        pickInfo._enableSketchPick = true;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        // 关联信号
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
                m_SourceShape = virtualShapes.at(0);
            }
            else {
                m_SourceShape.reset();
            }
            // 设置界面
            m_Ui->lb_source->setText(m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void SolidRevolInfoDialog::on_pb_pickpoint1_clicked()
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


            m_Ui->le_p1x->setValue(xyz[0]);
            m_Ui->le_p1y->setValue(xyz[1]);
            m_Ui->le_p1z->setValue(xyz[2]);


            delete pointTool;
        });
    }

    void SolidRevolInfoDialog::on_pb_pickpoint2_clicked()
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


            m_Ui->le_p2x->setValue(xyz[0]);
            m_Ui->le_p2y->setValue(xyz[1]);
            m_Ui->le_p2z->setValue(xyz[2]);


            delete pointTool;
        });
    }

    void SolidRevolInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("SourceShape", QVariant::fromValue(m_SourceShape));
        m_Operator->setArgs("P1x", m_Ui->le_p1x->value());
        m_Operator->setArgs("P1y", m_Ui->le_p1y->value());
        m_Operator->setArgs("P1z", m_Ui->le_p1z->value());
        m_Operator->setArgs("P2x", m_Ui->le_p2x->value());
        m_Operator->setArgs("P2y", m_Ui->le_p2y->value());
        m_Operator->setArgs("P2z", m_Ui->le_p2z->value());
        m_Operator->setArgs("Angle", m_Ui->le_angle->value());

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void SolidRevolInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelRevolSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTRevolSolid);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourceSurface(m_SourceShape);
        m_PreviewCmd->setRotateAxisPoint1(m_Ui->le_p1x->value(), m_Ui->le_p1y->value(), m_Ui->le_p1z->value());
        m_PreviewCmd->setRotateAxisPoint2(m_Ui->le_p2x->value(), m_Ui->le_p2y->value(), m_Ui->le_p2z->value());
        m_PreviewCmd->setAngle(m_Ui->le_angle->value());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void SolidRevolInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}


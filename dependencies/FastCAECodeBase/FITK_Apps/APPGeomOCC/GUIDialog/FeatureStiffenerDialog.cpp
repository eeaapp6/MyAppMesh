/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FeatureStiffenerDialog.h"
#include "ui_FeatureStiffenerDialog.h"

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

    FeatureStiffenerDialog::FeatureStiffenerDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::FeatureStiffenerDialog)
    {
        m_Ui->setupUi(this);
        // 设置初始状态
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    FeatureStiffenerDialog::~FeatureStiffenerDialog()
    {
        finalize();
        delete m_Ui;
    }

    FeatureStiffenerDialog * FeatureStiffenerDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoCommand * cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoStiffener*>(cmd);

        static FeatureStiffenerDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new FeatureStiffenerDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_source->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->lb_profile->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->le_thickness1->setValue(0);
            uniqueObj->m_Ui->le_thickness2->setValue(0);
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_SourceShape = command->sourceShape();
            uniqueObj->m_ProfileShape = command->profileShape();
            uniqueObj->m_Ui->lb_source->setText(uniqueObj->m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            uniqueObj->m_Ui->lb_profile->setText(uniqueObj->m_ProfileShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));

            uniqueObj->m_Ui->le_thickness1->setValue(command->thickness1());
            uniqueObj->m_Ui->le_thickness2->setValue(command->thickness2());
        }
        /*@}*/

        return uniqueObj;
    }

    bool FeatureStiffenerDialog::checkDialogData()
    {
        // 数据检查
        if (m_SourceShape.isNull() || m_ProfileShape.isNull() || m_Ui->lb_source->text().isEmpty() || m_Ui->lb_profile->text().isEmpty()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto vSource = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceShape.VirtualTopoId);
        auto vProfile = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_ProfileShape.VirtualTopoId);
        if (vSource == nullptr || vProfile == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }
        return true;
    }

    void FeatureStiffenerDialog::finalize()
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

        m_SourceShape.reset();
        m_ProfileShape.reset();
    }

    void FeatureStiffenerDialog::on_pb_picksource_clicked()
    {
        // 设置拾取参数
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJSolid;
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

    void FeatureStiffenerDialog::on_pb_pickprofile_clicked()
    {
        // 设置拾取参数
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJEdge;
        pickInfo._enableModelPick = false;
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
                m_ProfileShape = virtualShapes.at(0);
            }
            else {
                m_ProfileShape.reset();
            }
            // 设置界面
            m_Ui->lb_profile->setText(m_ProfileShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });

    }

    void FeatureStiffenerDialog::on_pb_reverse_clicked()
    {
        auto val = m_Ui->le_thickness1->value();
        m_Ui->le_thickness1->setValue(m_Ui->le_thickness2->value());
        m_Ui->le_thickness2->setValue(val);
    }

    void FeatureStiffenerDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("SourceShape", QVariant::fromValue(m_SourceShape));
        m_Operator->setArgs("ProfileShape", QVariant::fromValue(m_ProfileShape));
        m_Operator->setArgs("Thickness1", m_Ui->le_thickness1->value());
        m_Operator->setArgs("Thickness2", m_Ui->le_thickness2->value());
        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void FeatureStiffenerDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoStiffener>(Interface::FITKGeoEnum::FITKGeometryComType::FGTStiffener);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourceShape(m_SourceShape);
        m_PreviewCmd->setProfileShape(m_ProfileShape);
        m_PreviewCmd->setThickness1(m_Ui->le_thickness1->value());
        m_PreviewCmd->setThickness2(m_Ui->le_thickness2->value());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void FeatureStiffenerDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        // 关闭窗口
        reject();
    }
}

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SolidMultiSectionInfoDialog.h"
#include "ui_SolidMultiSectionInfoDialog.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"

#include <QMessageBox>

namespace GUI {
    SolidMultiSectionInfoDialog::SolidMultiSectionInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::SolidMultiSectionInfoDialog)
    {
        m_Ui->setupUi(this);
        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    SolidMultiSectionInfoDialog::~SolidMultiSectionInfoDialog()
    {
        finalize();
        delete m_Ui;
    }
    SolidMultiSectionInfoDialog* SolidMultiSectionInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelMultiSectionSolid*>(cmd);

        static SolidMultiSectionInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new SolidMultiSectionInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_source->setText(tr("Picked (0)"));
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_Sections = command->sections();
            uniqueObj->m_Ui->lb_source->setText(QString(tr("Picked (%1)")).arg(uniqueObj->m_Sections.size()));
        }
        /*@}*/
        return uniqueObj;
    }
    bool SolidMultiSectionInfoDialog::checkDialogData() const
    {
        // 数据检查
        if (m_Sections.size() < 1) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        for (auto shape : m_Sections) {
            if (shape.isNull()) {
                QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
                return false;
            }
            // 检查虚拓扑数据是否存在
            auto vCurve = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(shape.VirtualTopoId);
            if (vCurve == nullptr)
            {
                QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
                return false;
            }
        }
        return true;
    }

    void SolidMultiSectionInfoDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->disconnect(this);

        // 重置窗口数据
        delete m_PreviewCmd;
        m_PreviewCmd = nullptr;
        m_dataId = -1;

        m_Sections.clear();
    }

    void SolidMultiSectionInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void SolidMultiSectionInfoDialog::on_pb_selectsource_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._pickObjType = GUIPickInfo::POBJFace;
        pickInfo._enableSketchPick = true;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            //GUIPickInfo::ClearPickInfo();
            //disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            m_Sections.clear();
            for (auto pickData : pickedList) {
                if (pickData == nullptr) continue;

                // 获取cmd
                auto shapeAgent = pickData->getGeoShapeAgent();
                if (shapeAgent == nullptr) continue;
                auto cmd = shapeAgent->getGeoCommand();
                if (cmd == nullptr) continue;

                // 遍历虚拓扑
                m_Sections.append(pickData->getVirtualShapes());
            }

            m_Ui->lb_source->setText(QString(tr("Picked (%1)")).arg(m_Sections.size()));
        });
    }

    void SolidMultiSectionInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("Sections", QVariant::fromValue(m_Sections));

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void SolidMultiSectionInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelMultiSectionSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTMultiSectionSolid);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSections(m_Sections);
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void SolidMultiSectionInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}
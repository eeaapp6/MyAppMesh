/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PrimitivesSphereInfoDialog.h"
#include "ui_PrimitivesSphereInfoDialog.h"

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
    PrimitivesSphereInfoDialog::PrimitivesSphereInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::PrimitivesSphereInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    PrimitivesSphereInfoDialog::~PrimitivesSphereInfoDialog()
    {
        finalize();
        delete m_Ui;
    }
    PrimitivesSphereInfoDialog * PrimitivesSphereInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelSphere*>(cmd);
        static PrimitivesSphereInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new PrimitivesSphereInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr) {
#ifdef _DEBUG
            uniqueObj->m_Ui->le_radius_px->setValue(0);
            uniqueObj->m_Ui->le_radius_py->setValue(0);
            uniqueObj->m_Ui->le_radius_pz->setValue(0);

            uniqueObj->m_Ui->le_radius_radius->setValue(10);
#else
            uniqueObj->m_Ui->le_radius_px->setValue(0);
            uniqueObj->m_Ui->le_radius_py->setValue(0);
            uniqueObj->m_Ui->le_radius_pz->setValue(0);

            uniqueObj->m_Ui->le_radius_radius->setValue(0);
#endif // _DEBUG

        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            double temp[3];
            command->getLocation(temp);

            uniqueObj->m_Ui->le_radius_px->setValue(temp[0]);
            uniqueObj->m_Ui->le_radius_py->setValue(temp[1]);
            uniqueObj->m_Ui->le_radius_pz->setValue(temp[2]);

            uniqueObj->m_Ui->le_radius_radius->setValue(command->getRadius());
        }
        /*@}*/
        return uniqueObj;
    }

    bool PrimitivesSphereInfoDialog::checkDialogData() const
    {
        // 数据检查
        if (m_Ui->le_radius_px->text().isEmpty() || m_Ui->le_radius_py->text().isEmpty() || m_Ui->le_radius_pz->text().isEmpty()
            || m_Ui->le_radius_radius->text().isEmpty()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        return true;
    }

    void PrimitivesSphereInfoDialog::finalize()
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
    }

    void PrimitivesSphereInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }


    void PrimitivesSphereInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("LocX", m_Ui->le_radius_px->value());
        m_Operator->setArgs("LocY", m_Ui->le_radius_py->value());
        m_Operator->setArgs("LocZ", m_Ui->le_radius_pz->value());
        m_Operator->setArgs("Radius", m_Ui->le_radius_radius->value());

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void GUI::PrimitivesSphereInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelSphere>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSphere);
        }
        // 更新预览对象数据
        double loc[3] = { m_Ui->le_radius_px->value(),m_Ui->le_radius_py->value(),m_Ui->le_radius_pz->value() };
        m_PreviewCmd->setLocation(loc);
        m_PreviewCmd->setRadius(m_Ui->le_radius_radius->value());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void GUI::PrimitivesSphereInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }

}


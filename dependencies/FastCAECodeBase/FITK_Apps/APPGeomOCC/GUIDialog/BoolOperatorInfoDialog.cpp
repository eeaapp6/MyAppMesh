/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "BoolOperatorInfoDialog.h"
#include "ui_BoolOperatorInfoDialog.h"

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
    BoolOperatorInfoDialog::BoolOperatorInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::BoolOperatorInfoDialog)
    {
        m_Ui->setupUi(this);

        m_Ui->label->setVisible(false);
        m_Ui->comb_type->setVisible(false);
        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
    }

    BoolOperatorInfoDialog::~BoolOperatorInfoDialog()
    {
        finalize();

        delete m_Ui;
    }

    void BoolOperatorInfoDialog::setBoolType(Interface::FITKAbsGeoOperBool::GeoBoolOperType type)
    {
        switch (type)
        {
        case Interface::FITKAbsGeoOperBool::GBTAdd:
            setWindowTitle("Unite");
            m_Ui->comb_type->setCurrentIndex(0);
            break;
        case Interface::FITKAbsGeoOperBool::GBTCommon:
            setWindowTitle("Intersect");
            m_Ui->comb_type->setCurrentIndex(1);
            break;
        case Interface::FITKAbsGeoOperBool::GBTCut:
            setWindowTitle("Subtract");
            m_Ui->comb_type->setCurrentIndex(2);
            break;
        default:
            break;
        }
    }

    BoolOperatorInfoDialog * BoolOperatorInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoOperBool*>(cmd);
        static BoolOperatorInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new BoolOperatorInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->comb_type->setCurrentIndex(0);
            uniqueObj->m_Ui->lb_target->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->lb_tool->setText(tr("Picked (0)"));
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_Target = command->target();
            uniqueObj->m_Tool = command->tool();
            uniqueObj->m_Ui->lb_target->setText(uniqueObj->m_Target.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            uniqueObj->m_Ui->lb_tool->setText(uniqueObj->m_Tool.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            switch (command->getBoolOperType())
            {
            case Interface::FITKAbsGeoOperBool::GBTCommon:
                uniqueObj->m_Ui->comb_type->setCurrentIndex(1);
                break;
            case Interface::FITKAbsGeoOperBool::GBTCut:
                uniqueObj->m_Ui->comb_type->setCurrentIndex(2);
                break;
            default:
                uniqueObj->m_Ui->comb_type->setCurrentIndex(0);
                break;
            }
        }
        /*@}*/
        return uniqueObj;
    }

    bool BoolOperatorInfoDialog::checkDialogData() const
    {
        // 数据检查
        if (m_Target.isNull() || m_Tool.isNull()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        if (m_Target.VirtualTopoId == m_Tool.VirtualTopoId && m_Target.CmdId == m_Tool.CmdId) {
            QMessageBox::information(nullptr, tr("Information"), tr("The target shape cannot be the same as the tool shape!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto vTarget = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_Target.VirtualTopoId);
        auto vTool = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_Tool.VirtualTopoId);
        if (vTarget == nullptr || vTool == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }

        return true;
    }

    void BoolOperatorInfoDialog::finalize()
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

        m_Target.reset();
        m_Tool.reset();
    }

    void BoolOperatorInfoDialog::on_comb_type_currentIndexChanged(int index)
    {

    }

    void BoolOperatorInfoDialog::on_pb_selecttarget_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJSolid;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            // GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            if (pickedList.size() == 1) {
                auto pickData = pickedList.at(0);
                if (pickData == nullptr) return;

                // 获取虚拓扑形状
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_Target = virtualShapes.at(0);
            }
            else {
                m_Target.reset();
            }
            m_Ui->lb_target->setText(m_Target.isNull() ? tr("Picked (0)") : tr("Picked (1)"));

            //退出拾取
            GUIPickInfoStru pickInfo;
            pickInfo._pickMethod = GUIPickInfo::PickMethod::PMNone;
            pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJNone;
            GUIPickInfo::SetPickInfo(pickInfo);
        });
    }

    void BoolOperatorInfoDialog::on_pb_selecttool_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJSolid;
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
                m_Tool = virtualShapes.at(0);
            }
            else {
                m_Tool.reset();
            }
            m_Ui->lb_tool->setText(m_Tool.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            //退出拾取
            GUIPickInfoStru pickInfo;
            pickInfo._pickMethod = GUIPickInfo::PickMethod::PMNone;
            pickInfo._pickObjType = GUIPickInfo::PickObjType::POBJNone;
            GUIPickInfo::SetPickInfo(pickInfo);
        });
    }

    void BoolOperatorInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("TargetShape", QVariant::fromValue(m_Target));
        m_Operator->setArgs("ToolShape", QVariant::fromValue(m_Tool));

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void GUI::BoolOperatorInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoOperBool>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBool);
        }
        // 更新预览对象数据
        m_PreviewCmd->setTarget(m_Target);
        m_PreviewCmd->setTool(m_Tool);
        switch (m_Ui->comb_type->currentIndex())
        {
            // 并集
        case 0:
            m_PreviewCmd->setBoolOperType(Interface::FITKAbsGeoOperBool::GBTAdd);
            break;
            // 交集
        case 1:
            m_PreviewCmd->setBoolOperType(Interface::FITKAbsGeoOperBool::GBTCommon);
            break;
            // 差集
        case 2:
            m_PreviewCmd->setBoolOperType(Interface::FITKAbsGeoOperBool::GBTCut);
            break;
        default:
            printLog(3, tr("System Error!"));
            return;
            break;
        }
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void GUI::BoolOperatorInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}

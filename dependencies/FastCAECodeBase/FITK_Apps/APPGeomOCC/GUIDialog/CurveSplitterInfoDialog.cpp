/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CurveSplitterInfoDialog.h"
#include "ui_CurveSplitterInfoDialog.h"
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

    CurveSplitterInfoDialog::CurveSplitterInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::CurveSplitterInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        on_comb_type_currentIndexChanged(0);
    }

    CurveSplitterInfoDialog::~CurveSplitterInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    CurveSplitterInfoDialog* CurveSplitterInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoCurveSplitter*>(cmd);

        static CurveSplitterInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new CurveSplitterInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_sourcepicked->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->lb_toolpicked->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->comb_type->setCurrentIndex(0);
            uniqueObj->on_comb_type_currentIndexChanged(0);
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_SourceShape = command->sourceShape();
            uniqueObj->m_ToolShape = command->toolShape();

            auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
            auto virtualTopo = dynamic_cast<Interface::FITKAbsVirtualTopo*>(FITKDATAREPO->getDataByID(uniqueObj->m_ToolShape.VirtualTopoId));

            // 空指针(通过交互选择点会出现)
            if (virtualTopo == nullptr) {
                uniqueObj->m_Ui->comb_type->setCurrentIndex(0);
                uniqueObj->on_comb_type_currentIndexChanged(0);
                auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
                uniqueObj->m_InteractiveCmd = fac->createCommandT<Interface::FITKAbsGeoModelPoint>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPoint);

                auto xyz = command->point();
                uniqueObj->m_InteractiveCmd->setCoord(xyz[0], xyz[1], xyz[2]);
                uniqueObj->m_InteractiveCmd->update();
                geoCmdList->appendDataObj(uniqueObj->m_InteractiveCmd);
                uniqueObj->m_ToolShape.CmdId = uniqueObj->m_InteractiveCmd->getDataObjectID();
                uniqueObj->m_ToolShape.VirtualTopoId = uniqueObj->m_InteractiveCmd->getVirtualTopoManager()->getRootObj()->getDataObjectID();
            }
            // 虚拓扑
            else {
                uniqueObj->m_Ui->comb_type->setCurrentIndex(-1);
                switch (virtualTopo->getShapeType())
                {
                case Interface::FITKGeoEnum::VTopoShapeType::VSPoint:
                    uniqueObj->m_Ui->comb_type->setCurrentIndex(1);
                    break;
                case Interface::FITKGeoEnum::VTopoShapeType::VSEdge:
                    uniqueObj->m_Ui->comb_type->setCurrentIndex(2);
                    break;
                case Interface::FITKGeoEnum::VTopoShapeType::VSFace:
                    uniqueObj->m_Ui->comb_type->setCurrentIndex(3);
                    break;
                default:
                    break;
                }
            }
            // 下拉框切换会清除数据，需要重新设置
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_SourceShape = command->sourceShape();
            uniqueObj->m_ToolShape = command->toolShape();
            uniqueObj->m_Ui->lb_sourcepicked->setText(uniqueObj->m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            uniqueObj->m_Ui->lb_toolpicked->setText(uniqueObj->m_ToolShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        }
        /*@}*/

        return uniqueObj;
    }

    bool CurveSplitterInfoDialog::checkDialogData()
    {
        // 数据检查
        if (m_SourceShape.isNull()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto sourceShape = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceShape.VirtualTopoId);
        if (sourceShape == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }


        // 交互选择点分割线
        if (m_Ui->comb_type->currentIndex() == 0) {

            // 数据检查
            if (m_InteractionPoint.size() != 3) {
                QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
                return false;
            }
        }
        else {
            // 数据检查
            if (m_ToolShape.isNull()) {
                QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
                return false;
            }
            // 检查虚拓扑数据是否存在
            auto toolShape = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_ToolShape.VirtualTopoId);
            if (toolShape == nullptr)
            {
                QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
                return false;
            }
        }

        return true;
    }

    void CurveSplitterInfoDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 断开拾取信号
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->disconnect(this);
        m_GraphToolOper->setPickToolObjectVisible(EventOper::PickToolType::PTT_Proj_Pos, false);
        // 重置窗口数据
        delete m_PreviewCmd;
        m_PreviewCmd = nullptr;
        m_dataId = -1;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        geoCmdList->removeDataObj(m_InteractiveCmd);
        //delete m_InteractiveCmd;
        m_InteractiveCmd = nullptr;

        m_SourceShape.reset();
        m_ToolShape.reset();
    }

    void CurveSplitterInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        // 清除信号
        GUIPickInfo::ClearPickInfo();
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
        m_GraphToolOper->setPickToolObjectVisible(EventOper::PickToolType::PTT_Proj_Pos, false);
        m_Ui->pb_sourcepick->setDisabled(index == 0);
        m_Ui->pb_toolpick->setDisabled(index == 0);
        // 清除已选对象
        m_SourceShape.reset();
        m_Ui->lb_sourcepicked->setText(m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        m_ToolShape.reset();
        m_Ui->lb_toolpicked->setText(m_ToolShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        // 交互拾取,需要单独设置拾取模式
        if (index == 0) {
            // 设置拾取信息
            GUIPickInfoStru pickInfo{};
            pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
            pickInfo._pickObjType = GUIPickInfo::POBJEdgeProjPos;
            GUIPickInfo::SetPickInfo(pickInfo);
            auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
            m_GraphToolOper->setPickToolObjectVisible(EventOper::PickToolType::PTT_Proj_Pos, true);
            connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_projPosPicked, [this, pickDataProvider]() {

                // 处理拾取数据
                auto pickedList = pickDataProvider->getPickedList();

                // 第一个对象是边
                if (pickedList.size() == 1) {

                    // 拾取的边设置为m_SourceShape
                    auto edgePickData = pickedList.at(0);
                    if (edgePickData == nullptr) return;
                    auto virtualShapes = edgePickData->getVirtualShapes();
                    if (virtualShapes.size() != 1) return;
                    m_SourceShape = virtualShapes.at(0);

                    // 分割点生成临时cmd
                    if (!m_GraphToolOper->getPickedProjPosition(m_InteractionPoint) || m_InteractionPoint.size() != 3) {
                        m_InteractionPoint.clear();
                        return;
                    }
                    else {
                        m_ToolShape.reset();
                    }
                }
                else {
                    m_InteractionPoint.clear();
                    m_SourceShape.reset();
                    m_ToolShape.reset();
                }
                m_Ui->lb_sourcepicked->setText(m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
                m_Ui->lb_toolpicked->setText(m_ToolShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));

            });
        }
    }

    void CurveSplitterInfoDialog::on_pb_sourcepick_clicked()
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

            if (pickedList.size() == 1) {
                auto pickData = pickedList.at(0);
                if (pickData == nullptr) return;
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_SourceShape = virtualShapes.at(0);
            }
            else {
                m_SourceShape.reset();
            }
            m_Ui->lb_sourcepicked->setText(m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void CurveSplitterInfoDialog::on_pb_toolpick_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        switch (m_Ui->comb_type->currentIndex())
        {
            // 点分割
        case 1:
            pickInfo._pickObjType = GUIPickInfo::POBJVert;
            break;
            // 线分割
        case 2:
            pickInfo._pickObjType = GUIPickInfo::POBJEdge;
            break;
            // 面分割
        case 3:
            pickInfo._pickObjType = GUIPickInfo::POBJFace;
            break;
        default:
            return;
            break;
        }
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
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_ToolShape = virtualShapes.at(0);
            }
            else {
                m_ToolShape.reset();
            }
            m_Ui->lb_toolpicked->setText(m_ToolShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void CurveSplitterInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("SourceShape", QVariant::fromValue(m_SourceShape));

        if (m_Ui->comb_type->currentIndex() == 0) {
            m_Operator->setArgs("IsInteraction", true);
            m_Operator->setArgs("Point", QVariant::fromValue(m_InteractionPoint));
        }
        else {
            m_Operator->setArgs("IsInteraction", false);
            m_Operator->setArgs("ToolShape", QVariant::fromValue(m_ToolShape));
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

    void CurveSplitterInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoCurveSplitter>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCurveSplitter);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourceShape(m_SourceShape);
        m_PreviewCmd->setToolShape(m_ToolShape);
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void CurveSplitterInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}

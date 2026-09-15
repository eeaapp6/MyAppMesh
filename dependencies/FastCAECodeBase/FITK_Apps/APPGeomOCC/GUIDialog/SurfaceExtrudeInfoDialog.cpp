/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SurfaceExtrudeInfoDialog.h"
#include "ui_SurfaceExtrudeInfoDialog.h"

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
    SurfaceExtrudeInfoDialog::SurfaceExtrudeInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::SurfaceExtrudeInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    SurfaceExtrudeInfoDialog::~SurfaceExtrudeInfoDialog()
    {
        finalize();
        delete m_Ui;
    }
    SurfaceExtrudeInfoDialog* SurfaceExtrudeInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelExtrudeSurface*>(cmd);
        static SurfaceExtrudeInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new SurfaceExtrudeInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_source->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->le_vx->setValue(0);
            uniqueObj->m_Ui->le_vy->setValue(0);
            uniqueObj->m_Ui->le_vz->setValue(0);
            uniqueObj->m_Ui->le_length->setValue(0);
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_SourceShape = command->sourceCurve();
            uniqueObj->m_Ui->lb_source->setText(uniqueObj->m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            auto xyz = command->direction();
            uniqueObj->m_Ui->le_vx->setValue(xyz[0]);
            uniqueObj->m_Ui->le_vy->setValue(xyz[1]);
            uniqueObj->m_Ui->le_vz->setValue(xyz[2]);
            uniqueObj->m_Ui->le_length->setValue(command->length());
        }
        /*@}*/
        return uniqueObj;
    }
    bool SurfaceExtrudeInfoDialog::checkDialogData() const
    {
        // 数据检查
        if (m_SourceShape.isNull() || m_Ui->le_vx->text().isEmpty() || m_Ui->le_vy->text().isEmpty() || m_Ui->le_vz->text().isEmpty() || m_Ui->le_length->text().isEmpty()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto vCurve = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceShape.VirtualTopoId);
        if (vCurve == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }
        return true;
    }

    void SurfaceExtrudeInfoDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 断开拾取信号
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->disconnect(this);

        // 清除箭头显示
        if (m_GraphToolOper != nullptr) {
            m_GraphToolOper->clearPickTool();
        }

        // 重置窗口数据
        delete m_PreviewCmd;
        m_PreviewCmd = nullptr;
        m_dataId = -1;
        m_SourceShape.reset();
    }

    void SurfaceExtrudeInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void SurfaceExtrudeInfoDialog::on_pb_selectsource_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJEdge;
        pickInfo._enableSketchPick = true;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();


        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();

            if (pickedList.size() == 1) {

                // 处理拾取到的面
                auto facePickData = pickedList.at(0);
                if (facePickData == nullptr) return;
                // 获取虚拓扑形状
                auto virtualShapes = facePickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_SourceShape = virtualShapes.at(0);

                if (m_GraphToolOper == nullptr) return;
                m_GraphToolOper->setPickDirectionAnchorTopoId(m_SourceShape.CmdId, m_SourceShape.VirtualTopoId);

                // 如果拾取的是草图则显示箭头并绑定信号
                if (pickedList.at(0)->getGraphInfo().Type == GraphData::GraphDataType::SketchGraph)
                {
                    m_GraphToolOper->setPickToolObjectVisible(EventOper::PickToolType::PTT_Direction, true);
                    connect(pickDataProvider, SIGNAL(sig_directionPicked()), this, SLOT(onDirectionReverseSlot()), Qt::UniqueConnection);
                    // 获取方向并显示数值
                    onDirectionReverseSlot();
                }
                else {
                    m_GraphToolOper->setPickToolObjectVisible(EventOper::PickToolType::PTT_Direction, false);
                    disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
                }
            }
            else {
                m_SourceShape.reset();
                //m_GraphToolOper->clearPickTool();
            }
            m_Ui->lb_source->setText(m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void SurfaceExtrudeInfoDialog::onDirectionReverseSlot()
    {
        // 获取箭头方向
        QList<double> direction;

        if (m_GraphToolOper == nullptr) return;
        if (m_GraphToolOper->getPickedDirection(direction) && direction.size() == 3) {
            m_Ui->le_vx->setValue(direction.at(0));
            m_Ui->le_vy->setValue(direction.at(1));
            m_Ui->le_vz->setValue(direction.at(2));
        }

    }

    void SurfaceExtrudeInfoDialog::on_pb_pickdirection_clicked()
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

    void SurfaceExtrudeInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("SourceShape", QVariant::fromValue(m_SourceShape));
        m_Operator->setArgs("Dx", m_Ui->le_vx->value());
        m_Operator->setArgs("Dy", m_Ui->le_vy->value());
        m_Operator->setArgs("Dz", m_Ui->le_vz->value());
        m_Operator->setArgs("Length", m_Ui->le_length->value());

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void SurfaceExtrudeInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelExtrudeSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTExtrudeSurface);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourceCurve(m_SourceShape);
        m_PreviewCmd->setDirection(m_Ui->le_vx->value(), m_Ui->le_vy->value(), m_Ui->le_vz->value());
        m_PreviewCmd->setLength(m_Ui->le_length->value());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void SurfaceExtrudeInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}


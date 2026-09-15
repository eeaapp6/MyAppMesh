/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CurveSurfaceInterSurfaceInfoDialog.h"
#include "ui_CurveSurfaceInterSurfaceInfoDialog.h"

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

    CurveSurfaceInterSurfaceInfoDialog::CurveSurfaceInterSurfaceInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::CurveSurfaceInterSurfaceInfoDialog)
    {
        m_Ui->setupUi(this);
        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    CurveSurfaceInterSurfaceInfoDialog::~CurveSurfaceInterSurfaceInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    CurveSurfaceInterSurfaceInfoDialog * CurveSurfaceInterSurfaceInfoDialog::New(QWidget * parent,
        EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoCommand * cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelSurfaceIntersectionSurface*>(cmd);

        static CurveSurfaceInterSurfaceInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new CurveSurfaceInterSurfaceInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_surfaceintersurface_surface1picked->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->lb_surfaceintersurface_surface2picked->setText(tr("Picked (0)"));
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_SurfaceInterSurfaceSourceSurface1 = command->sourceSurface1();
            uniqueObj->m_SurfaceInterSurfaceSourceSurface2 = command->sourceSurface2();
            uniqueObj->m_Ui->lb_surfaceintersurface_surface1picked->setText(uniqueObj->m_SurfaceInterSurfaceSourceSurface1.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            uniqueObj->m_Ui->lb_surfaceintersurface_surface2picked->setText(uniqueObj->m_SurfaceInterSurfaceSourceSurface2.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        }
        /*@}*/
        return uniqueObj;
    }

    void CurveSurfaceInterSurfaceInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    bool CurveSurfaceInterSurfaceInfoDialog::checkDialogData()
    {
        // 数据检查
        if (m_SurfaceInterSurfaceSourceSurface1.isNull() || m_SurfaceInterSurfaceSourceSurface2.isNull()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto vSurface1 = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SurfaceInterSurfaceSourceSurface1.VirtualTopoId);
        auto vSurface2 = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SurfaceInterSurfaceSourceSurface2.VirtualTopoId);
        if (vSurface1 == nullptr || vSurface2 == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }
        return true;
    }

    void CurveSurfaceInterSurfaceInfoDialog::finalize()
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

    void CurveSurfaceInterSurfaceInfoDialog::on_pb_surfaceintersurface_surfacepick1_clicked()
    {
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
                m_SurfaceInterSurfaceSourceSurface1 = virtualShapes.at(0);
            }
            else {
                m_SurfaceInterSurfaceSourceSurface1.reset();
            }
            // 设置界面
            m_Ui->lb_surfaceintersurface_surface1picked->setText(m_SurfaceInterSurfaceSourceSurface1.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void CurveSurfaceInterSurfaceInfoDialog::on_pb_surfaceintersurface_surfacepick2_clicked()
    {
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
                m_SurfaceInterSurfaceSourceSurface2 = virtualShapes.at(0);
            }
            else {
                m_SurfaceInterSurfaceSourceSurface2.reset();
            }
            // 设置界面
            m_Ui->lb_surfaceintersurface_surface2picked->setText(m_SurfaceInterSurfaceSourceSurface2.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void CurveSurfaceInterSurfaceInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("InputEdgeSurface1", QVariant::fromValue(m_SurfaceInterSurfaceSourceSurface1));
        m_Operator->setArgs("InputEdgeSurface2", QVariant::fromValue(m_SurfaceInterSurfaceSourceSurface2));

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void CurveSurfaceInterSurfaceInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelSurfaceIntersectionSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSurfaceIntersectSurface);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourceSurface1(m_SurfaceInterSurfaceSourceSurface1);
        m_PreviewCmd->setSourceSurface2(m_SurfaceInterSurfaceSourceSurface2);
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void CurveSurfaceInterSurfaceInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        // 关闭窗口
        reject();
    }
}

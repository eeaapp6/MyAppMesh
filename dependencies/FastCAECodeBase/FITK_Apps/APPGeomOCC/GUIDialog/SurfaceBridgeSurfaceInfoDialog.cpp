/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SurfaceBridgeSurfaceInfoDialog.h"
#include "ui_SurfaceBridgeSurfaceInfoDialog.h"
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

    SurfaceBridgeSurfaceInfoDialog::SurfaceBridgeSurfaceInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::SurfaceBridgeSurfaceInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    SurfaceBridgeSurfaceInfoDialog::~SurfaceBridgeSurfaceInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    SurfaceBridgeSurfaceInfoDialog* SurfaceBridgeSurfaceInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelBridgeSurface*>(cmd);

        static SurfaceBridgeSurfaceInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new SurfaceBridgeSurfaceInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_bridge_surface1picked->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->comb_bridge_edge1->clear();
            uniqueObj->m_Ui->lb_bridge_surface2picked->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->comb_bridge_edge2->clear();
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_SourceSurface1 = command->sourceSurface1();
            uniqueObj->m_SourceSurface2 = command->sourceSurface2();


            /* 设置第一个面*/
            /*@{*/
            uniqueObj->m_Ui->lb_bridge_surface1picked->setText(uniqueObj->m_SourceSurface1.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            // 查找线段的虚拓扑数据
            auto face = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(uniqueObj->m_SourceSurface1.VirtualTopoId);
            // 查找线的顶点数据
            Interface::FITKVirtualTopoMapper vMapper;
            vMapper.mapTopo(face->getDataObjectID(), Interface::FITKGeoEnum::VTopoShapeType::VSEdge);

            int currentIndex = -1;
            for (int i = 0; i < vMapper.length(); ++i) {
                auto id = vMapper.virtualTopo(i)->getDataObjectID();
                if (id == command->edgeVirtualTopoId1()) { currentIndex = i; }
                uniqueObj->m_Ui->comb_bridge_edge1->addItem(QString("%1").arg(i), id);
            }
            uniqueObj->m_Ui->comb_bridge_edge1->setCurrentIndex(currentIndex);
            /*@}*/


            /* 设置第二个面*/
            /*@{*/
            uniqueObj->m_Ui->lb_bridge_surface2picked->setText(uniqueObj->m_SourceSurface2.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            // 查找线段的虚拓扑数据
            face = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(uniqueObj->m_SourceSurface2.VirtualTopoId);
            // 查找线的顶点数据
            vMapper.mapTopo(face->getDataObjectID(), Interface::FITKGeoEnum::VTopoShapeType::VSEdge);

            currentIndex = -1;
            for (int i = 0; i < vMapper.length(); ++i) {
                auto id = vMapper.virtualTopo(i)->getDataObjectID();
                if (id == command->edgeVirtualTopoId2()) { currentIndex = i; }
                uniqueObj->m_Ui->comb_bridge_edge2->addItem(QString("%1").arg(i), id);
            }
            uniqueObj->m_Ui->comb_bridge_edge2->setCurrentIndex(currentIndex);
            /*@}*/
        }
        /*@}*/

        return uniqueObj;
    }

    bool SurfaceBridgeSurfaceInfoDialog::checkDialogData()
    {
        // 数据检查
        if (m_SourceSurface1.isNull() || m_SourceSurface2.isNull() || m_Ui->comb_bridge_edge2->currentIndex() < 0 || m_Ui->comb_bridge_edge2->currentIndex() < 0) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto vSurface1 = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceSurface1.VirtualTopoId);
        auto vSurface2 = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceSurface2.VirtualTopoId);
        if (vSurface1 == nullptr || vSurface2 == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }
        return true;
    }

    void SurfaceBridgeSurfaceInfoDialog::finalize()
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

        // 取消显示边索引标签
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->clearPreview(m_SourceSurface1.VirtualTopoId);
            graphPreprocessOper->clearPreview(m_SourceSurface2.VirtualTopoId);
        }
    }

    void SurfaceBridgeSurfaceInfoDialog::on_pb_bridge_surface1pick_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJFace;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 清除面索引标签（如果有的话）
            auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (!m_SourceSurface1.isNull() && graphPreprocessOper != nullptr) {
                graphPreprocessOper->clearPreview(m_SourceSurface1.VirtualTopoId);
            }
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            m_Ui->comb_bridge_edge1->clear();
            if (pickedList.size() == 1) {
                auto pickData = pickedList.at(0);
                if (pickData == nullptr) return;

                // 获取虚拓扑形状
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_SourceSurface1 = virtualShapes.at(0);
                // 显示标签
                if (graphPreprocessOper != nullptr) {
                    graphPreprocessOper->preview(m_SourceSurface1.VirtualTopoId, EventOper::PreviewType::PT_IndexLabels, Qt::red);
                }
                // 查找线段的虚拓扑数据
                auto solid = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceSurface1.VirtualTopoId);
                // 查找线的顶点数据
                Interface::FITKVirtualTopoMapper vMapper;
                vMapper.mapTopo(solid->getDataObjectID(), Interface::FITKGeoEnum::VTopoShapeType::VSEdge);

                for (int i = 0; i < vMapper.length(); ++i) {
                    m_Ui->comb_bridge_edge1->addItem(QString("%1").arg(i), vMapper.virtualTopo(i)->getDataObjectID());
                }
            }
            else {
                m_SourceSurface1.reset();
            }
            m_Ui->lb_bridge_surface1picked->setText(m_SourceSurface1.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }



    void SurfaceBridgeSurfaceInfoDialog::on_pb_bridge_surface2pick_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJFace;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 清除面索引标签（如果有的话）
            auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (!m_SourceSurface2.isNull() && graphPreprocessOper != nullptr) {
                graphPreprocessOper->clearPreview(m_SourceSurface2.VirtualTopoId);
            }
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            m_Ui->comb_bridge_edge2->clear();
            if (pickedList.size() == 1) {
                auto pickData = pickedList.at(0);
                if (pickData == nullptr) return;

                // 获取虚拓扑形状
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_SourceSurface2 = virtualShapes.at(0);
                // 显示标签
                if (graphPreprocessOper != nullptr) {
                    graphPreprocessOper->preview(m_SourceSurface2.VirtualTopoId, EventOper::PreviewType::PT_IndexLabels, Qt::blue);
                }
                // 查找线段的虚拓扑数据
                auto solid = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceSurface2.VirtualTopoId);
                // 查找线的顶点数据
                Interface::FITKVirtualTopoMapper vMapper;
                vMapper.mapTopo(solid->getDataObjectID(), Interface::FITKGeoEnum::VTopoShapeType::VSEdge);

                for (int i = 0; i < vMapper.length(); ++i) {
                    m_Ui->comb_bridge_edge2->addItem(QString("%1").arg(i), vMapper.virtualTopo(i)->getDataObjectID());
                }
            }
            else {
                m_SourceSurface2.reset();
            }
            m_Ui->lb_bridge_surface2picked->setText(m_SourceSurface2.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }


    void SurfaceBridgeSurfaceInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void SurfaceBridgeSurfaceInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("SourceSurface1", QVariant::fromValue(m_SourceSurface1));
        m_Operator->setArgs("SourceSurface2", QVariant::fromValue(m_SourceSurface2));
        m_Operator->setArgs("EdgeId1", QVariant::fromValue(m_Ui->comb_bridge_edge1->currentData().toInt()));
        m_Operator->setArgs("EdgeId2", QVariant::fromValue(m_Ui->comb_bridge_edge2->currentData().toInt()));

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void SurfaceBridgeSurfaceInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelBridgeSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBridgeSurface);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourceSurface1(m_SourceSurface1);
        m_PreviewCmd->setEdgeVirtualTopoId1(m_Ui->comb_bridge_edge1->currentData().toInt());
        m_PreviewCmd->setSourceSurface2(m_SourceSurface2);
        m_PreviewCmd->setEdgeVirtualTopoId2(m_Ui->comb_bridge_edge2->currentData().toInt());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void SurfaceBridgeSurfaceInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}

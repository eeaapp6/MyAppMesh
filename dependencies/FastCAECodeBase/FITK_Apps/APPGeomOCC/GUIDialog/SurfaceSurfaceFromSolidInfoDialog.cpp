/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SurfaceSurfaceFromSolidInfoDialog.h"
#include "ui_SurfaceSurfaceFromSolidInfoDialog.h"
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

    SurfaceSurfaceFromSolidInfoDialog::SurfaceSurfaceFromSolidInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::SurfaceSurfaceFromSolidInfoDialog)
    {
        m_Ui->setupUi(this);

        // 设置初始状态
        m_Ui->comb_type->setCurrentIndex(0);
        m_Ui->stackedWidget->setCurrentIndex(0);
    }

    SurfaceSurfaceFromSolidInfoDialog::~SurfaceSurfaceFromSolidInfoDialog()
    {
        finalize();
        delete m_Ui;
    }

    SurfaceSurfaceFromSolidInfoDialog* SurfaceSurfaceFromSolidInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoModelSolidSurface*>(cmd);

        static SurfaceSurfaceFromSolidInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new SurfaceSurfaceFromSolidInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->lb_fromsolid_picked->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->comb_fromsolid_index->clear();
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_SourceSolid = command->sourceSolid();
            uniqueObj->m_Ui->lb_fromsolid_picked->setText(uniqueObj->m_SourceSolid.isNull() ? tr("Picked (0)") : tr("Picked (1)"));

            // 显示边索引标签
            if (uniqueObj->m_GraphOper != nullptr) {
                uniqueObj->m_GraphOper->preview(uniqueObj->m_SourceSolid.VirtualTopoId, EventOper::PreviewType::PT_IndexLabels);
            }
            // 查找线段的虚拓扑数据
            auto solid = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(uniqueObj->m_SourceSolid.VirtualTopoId);
            // 查找线的顶点数据
            Interface::FITKVirtualTopoMapper vMapper;
            vMapper.mapTopo(solid->getDataObjectID(), Interface::FITKGeoEnum::VTopoShapeType::VSFace);
            uniqueObj->m_Ui->comb_fromsolid_index->clear();
            int currentIndex = 0;
            for (int i = 0; i < vMapper.length(); ++i) {
                uniqueObj->m_Ui->comb_fromsolid_index->addItem(QString("%1").arg(i), vMapper.virtualTopo(i)->getDataObjectID());
                if (vMapper.virtualTopo(i)->getDataObjectID() == command->faceVirtualTopoId()) currentIndex = i;
            }
            uniqueObj->m_Ui->comb_fromsolid_index->setCurrentIndex(currentIndex);
        }
        /*@}*/

        return uniqueObj;
    }

    bool SurfaceSurfaceFromSolidInfoDialog::checkDialogData()
    {
        // 数据检查
        if (m_SourceSolid.isNull() || m_Ui->comb_fromsolid_index->currentText().isEmpty()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在
        auto vSolid = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceSolid.VirtualTopoId);
        if (vSolid == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }
        return true;
    }

    void SurfaceSurfaceFromSolidInfoDialog::finalize()
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
            graphPreprocessOper->clearPreview(m_SourceSolid.VirtualTopoId);
        }

        m_SourceSolid.reset();
    }

    void SurfaceSurfaceFromSolidInfoDialog::on_pb_fromsolid_pick_clicked()
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
            // 清除面索引标签（如果有的话）
            if (!m_SourceSolid.isNull() && m_GraphOper != nullptr) {
                m_GraphOper->clearPreview(m_SourceSolid.VirtualTopoId);
            }
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            m_Ui->comb_fromsolid_index->clear();
            if (pickedList.size() == 1) {
                auto pickData = pickedList.at(0);
                if (pickData == nullptr) return;

                // 获取虚拓扑形状
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_SourceSolid = virtualShapes.at(0);
                // 显示边索引标签
                if (m_GraphOper != nullptr) {
                    m_GraphOper->preview(m_SourceSolid.VirtualTopoId, EventOper::PreviewType::PT_IndexLabels);
                }
                // 查找线段的虚拓扑数据
                auto solid = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceSolid.VirtualTopoId);
                // 查找线的顶点数据
                Interface::FITKVirtualTopoMapper vMapper;
                vMapper.mapTopo(solid->getDataObjectID(), Interface::FITKGeoEnum::VTopoShapeType::VSFace);

                for (int i = 0; i < vMapper.length(); ++i) {
                    m_Ui->comb_fromsolid_index->addItem(QString("%1").arg(i), vMapper.virtualTopo(i)->getDataObjectID());
                }
            }
            else {
                m_SourceSolid.reset();
            }

            m_Ui->lb_fromsolid_picked->setText(m_SourceSolid.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void SurfaceSurfaceFromSolidInfoDialog::on_comb_type_currentIndexChanged(int index)
    {
        m_Ui->stackedWidget->setCurrentIndex(index);
    }

    void SurfaceSurfaceFromSolidInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        m_Operator->setArgs("SourceSolid", QVariant::fromValue(m_SourceSolid));
        m_Operator->setArgs("FaceId", QVariant::fromValue(m_Ui->comb_fromsolid_index->currentData().toInt()));

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void SurfaceSurfaceFromSolidInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr) {
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoModelSolidSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSurfaceFromSolid);
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourceSolid(m_SourceSolid);
        m_PreviewCmd->setFaceVirtualTopoId(m_Ui->comb_fromsolid_index->currentData().toInt());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void SurfaceSurfaceFromSolidInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}

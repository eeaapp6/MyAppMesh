/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PointInteractionInfoDialog.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "ui_PointInteractionInfoDialog.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedData.h"
#include "PickDataProvider/GraphPickedDataProvider.h"

#include <QMessageBox>

namespace GUI {

PointInteractionInfoDialog::PointInteractionInfoDialog(QWidget *parent)
    : GUIDialogBase(parent), m_Ui(new Ui::PointInteractionInfoDialog) {
  m_Ui->setupUi(this);

  // 设置初始状态
  m_Ui->comb_type->setCurrentIndex(0);
  m_Ui->stackedWidget->setCurrentIndex(0);
}

PointInteractionInfoDialog::~PointInteractionInfoDialog() {
  finalize();
  delete m_Ui;
}

PointInteractionInfoDialog *
PointInteractionInfoDialog::New(QWidget *parent,
                                EventOper::ParaWidgetInterfaceOper *oper,
                                Interface::FITKAbsGeoCommand *cmd) {
  auto command =
      dynamic_cast<Interface::FITKAbsGeoModelInteractionPoint *>(cmd);

  static PointInteractionInfoDialog *uniqueObj;
  if (uniqueObj == nullptr) {
    uniqueObj = new PointInteractionInfoDialog(parent);
  }
  if (parent != uniqueObj->parent()) {
    uniqueObj->setParent(parent);
  }
  if (oper != uniqueObj->m_Operator) {
    uniqueObj->m_Operator = oper;
  }
  /* 设置界面数据 */
  /*@{*/
  if (command == nullptr) {
    uniqueObj->m_PointXYZ = {};
    uniqueObj->m_Ui->lb_curve_picked->setText(tr("Picked (0)"));
    uniqueObj->m_Ui->lb_surface_picked->setText(tr("Picked (0)"));
  } else {
    uniqueObj->m_dataId = command->getDataObjectID();
    uniqueObj->m_SourceShape = command->source();
    uniqueObj->m_PointXYZ = {command->x(), command->y(), command->z()};
    if (command->type() == 1) {
      uniqueObj->m_Ui->lb_surface_picked->setText(
          uniqueObj->m_SourceShape.isNull() ? tr("Picked (0)")
                                            : tr("Picked (1)"));
    } else {
      uniqueObj->m_Ui->lb_curve_picked->setText(
          uniqueObj->m_SourceShape.isNull() ? tr("Picked (0)")
                                            : tr("Picked (1)"));
    }
  }
  /*@}*/

  return uniqueObj;
}

bool PointInteractionInfoDialog::checkDialogData() {
  // 数据检查
  if (m_SourceShape.isNull() || m_PointXYZ.size() != 3) {
    QMessageBox::information(nullptr, tr("Information"),
                             tr("The data is incomplete!"));
    return false;
  }
  // 检查虚拓扑数据是否存在
  auto vSource = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(
      m_SourceShape.VirtualTopoId);
  if (vSource == nullptr) {
    QMessageBox::information(nullptr, tr("Information"),
                             tr("The picked data does not exist!"));
    return false;
  }
  return true;
}

void PointInteractionInfoDialog::finalize() {
  // 清除拾取状态
  if (m_GraphPickOper) {
    m_GraphPickOper->setCustomPickMode();
  }

  // 隐藏工具。
  m_GraphToolOper->setPickToolObjectVisible(
      EventOper::PickToolType::PTT_Proj_Pos, false);

  // 重置窗口数据
  delete m_PreviewCmd;
  m_PreviewCmd = nullptr;
  m_dataId = -1;

  m_SourceShape.reset();
}

void PointInteractionInfoDialog::on_comb_type_currentIndexChanged(int index) {
  m_Ui->stackedWidget->setCurrentIndex(index);
  m_SourceShape.reset();
}

void PointInteractionInfoDialog::on_pb_curve_picksource_clicked() {
  // 设置拾取信息
  GUIPickInfoStru pickInfo{};
  pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
  pickInfo._pickObjType = GUIPickInfo::POBJEdge;
  pickInfo._enableDatumPick = true;
  m_GraphToolOper->setPickToolObjectVisible(
      EventOper::PickToolType::PTT_Proj_Pos, false);
  GUIPickInfo::SetPickInfo(pickInfo);
  auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();

  connect(
      pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked,
      [this, pickDataProvider]() {

      // 开启拾取边坐标方式。
      GUIPickInfoStru pickInfo{};
      pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
      pickInfo._pickObjType = GUIPickInfo::POBJEdgeProjPos;
      pickInfo._enableDatumPick = false;
      GUIPickInfo::SetPickInfo(pickInfo);

        disconnect(pickDataProvider,
                   &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr,
                   nullptr);
        // 处理拾取数据
        auto pickedList = pickDataProvider->getPickedList();

        if (pickedList.size() == 1) {
          auto pickData = pickedList.at(0);
          if (pickData == nullptr)
            return;
          // 获取虚拓扑形状
          auto virtualShapes = pickData->getVirtualShapes();
          if (virtualShapes.size() != 1)
            return;
          m_SourceShape = virtualShapes.at(0);
        } else {
          m_SourceShape.reset();
        }
        m_Ui->lb_curve_picked->setText(
            m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));

        // 拾取完形状拾取点
        if (!m_SourceShape.isNull()) {

          auto pickDataProvider =
              GraphData::GraphPickedDataProvider::getInstance();
          m_GraphToolOper->setPickToolObjectVisible(
              EventOper::PickToolType::PTT_Proj_Pos, true);
          m_GraphToolOper->setPickProjPosTargetId(m_SourceShape.CmdId,
                                                  m_SourceShape.VirtualTopoId);
          connect(pickDataProvider,
                  &GraphData::GraphPickedDataProvider::sig_projPosPicked,
                  [this, pickDataProvider]() {
                    // 处理拾取数据
                    // auto pickedList = pickDataProvider->getPickedList();

                    if (!m_GraphToolOper->getPickedProjPosition(m_PointXYZ) ||
                        m_PointXYZ.size() != 3) {
                        m_PointXYZ.clear();
                    }

                    //// 第一个对象是边
                    //if (pickedList.size() == 1) {

                    //  }
                    //} else {
                    //  m_PointXYZ.clear();
                    //}
                  });
        }
      });
}

void PointInteractionInfoDialog::on_pb_surface_picksource_clicked() {
  // 设置拾取信息
  GUIPickInfoStru pickInfo{};
  pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
  pickInfo._pickObjType = GUIPickInfo::POBJFace;
  pickInfo._enableDatumPick = true;
  m_GraphToolOper->setPickToolObjectVisible(
      EventOper::PickToolType::PTT_Proj_Pos, false);
  GUIPickInfo::SetPickInfo(pickInfo);
  auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();

  connect(
      pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked,
      [this, pickDataProvider]() {

      // 开启拾取面坐标方式。
      GUIPickInfoStru pickInfo{};
      pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
      pickInfo._pickObjType = GUIPickInfo::POBJFaceProjPos;
      pickInfo._enableDatumPick = false;
      GUIPickInfo::SetPickInfo(pickInfo);

        disconnect(pickDataProvider,
                   &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr,
                   nullptr);
        // 处理拾取数据
        auto pickedList = pickDataProvider->getPickedList();

        if (pickedList.size() == 1) {
          auto pickData = pickedList.at(0);
          if (pickData == nullptr)
            return;
          // 获取虚拓扑形状
          auto virtualShapes = pickData->getVirtualShapes();
          if (virtualShapes.size() != 1)
            return;
          m_SourceShape = virtualShapes.at(0);
        } else {
          m_SourceShape.reset();
        }
        m_Ui->lb_surface_picked->setText(
            m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));

        // 拾取完形状拾取点
        if (!m_SourceShape.isNull()) {
          auto pickDataProvider =
              GraphData::GraphPickedDataProvider::getInstance();
          m_GraphToolOper->setPickToolObjectVisible(
              EventOper::PickToolType::PTT_Proj_Pos, true);
          m_GraphToolOper->setPickProjPosTargetId(m_SourceShape.CmdId,
                                                  m_SourceShape.VirtualTopoId);
          connect(pickDataProvider,
                  &GraphData::GraphPickedDataProvider::sig_projPosPicked,
                  [this, pickDataProvider]() {
                    // 处理拾取数据
                    // auto pickedList = pickDataProvider->getPickedList();

                    if (!m_GraphToolOper->getPickedProjPosition(m_PointXYZ) ||
                        m_PointXYZ.size() != 3) {
                        m_PointXYZ.clear();
                    }
                    //// 第一个对象是边
                    //if (pickedList.size() == 1) {

                    //  }
                    //} else {
                    //  m_PointXYZ.clear();
                    //}
                  });
        }
      });
}

void PointInteractionInfoDialog::on_pb_ok_clicked() {
  // 检查输入
  if (!checkDialogData() || m_Operator == nullptr)
    return;

  // 清除缓存。
  m_Operator->clearArgs();

  // 传递历史数据ID。
  m_Operator->setArgs("Id", m_dataId);

  // 传递输入参数。
  m_Operator->setArgs("SourceShape", QVariant::fromValue(m_SourceShape));
  m_Operator->setArgs("Px", m_PointXYZ.at(0));
  m_Operator->setArgs("Py", m_PointXYZ.at(1));
  m_Operator->setArgs("Pz", m_PointXYZ.at(2));
  m_Operator->setArgs("Type", m_Ui->comb_type->currentIndex());

  // 执行功能。
  if (!m_Operator->execProfession())
    return;

  // 保存数据ID。
  m_Operator->argValue("Id", m_dataId);
  // 释放资源
  finalize();
  // 关闭对话框
  accept();
}

void PointInteractionInfoDialog::on_pb_preview_clicked() {
  // 检查数据
  if (!checkDialogData())
    return;

  // 判断预览对象是否为空
  if (m_PreviewCmd == nullptr) {
    auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
    m_PreviewCmd =
        fac->createCommandT<Interface::FITKAbsGeoModelInteractionPoint>(
            Interface::FITKGeoEnum::FITKGeometryComType::FGTInteractionPoint);
  }
  // 更新预览对象数据
  m_PreviewCmd->setCoord(m_PointXYZ.at(0), m_PointXYZ.at(1), m_PointXYZ.at(2));
  m_PreviewCmd->setSource(m_SourceShape);
  if (!m_PreviewCmd->update()) {
    return;
  }

  // 预览对象
  auto graphPreprocessOper =
      Core::FITKOperatorRepo::getInstance()
          ->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
  if (graphPreprocessOper != nullptr) {
    graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
  }
}

void PointInteractionInfoDialog::on_pb_cancel_clicked() {
  // 结束对象操作。
  finalize();

  reject();
}
} // namespace GUI

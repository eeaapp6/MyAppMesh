/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TransformationInfoDialog.h"
#include "ui_TransformationInfoDialog.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDelete.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"

#include <QMessageBox>

namespace GUI {
    TransformationInfoDialog::TransformationInfoDialog(QWidget *parent) :
        GUIDialogBase(parent),
        m_Ui(new Ui::TransformationInfoDialog)
    {
        m_Ui->setupUi(this);

        // transform
        m_Ui->comb_transform_method->setCurrentIndex(0);
        m_Ui->sw_transform->setCurrentIndex(0);

    }

    TransformationInfoDialog::~TransformationInfoDialog()
    {
        finalize();

        delete m_Ui;
    }

    void TransformationInfoDialog::setTransType(Interface::FITKGeoEnum::FITKGeometryComType type)
    {

        switch (type)
        {
        case Interface::FITKGeoEnum::FITKGeometryComType::FGTTransformByTwoPoints:
            m_Ui->stackedWidget->setCurrentIndex(0);
            m_Ui->sw_transform->setCurrentIndex(0);
            setWindowTitle(tr("Transform"));
            m_Ui->chec_duplication->setVisible(true);
            break;
        case Interface::FITKGeoEnum::FITKGeometryComType::FGTTransformByDirAndDis:
            m_Ui->stackedWidget->setCurrentIndex(0);
            m_Ui->sw_transform->setCurrentIndex(1);
            setWindowTitle(tr("Transform"));
            m_Ui->chec_duplication->setVisible(true);
            break;
        case Interface::FITKGeoEnum::FITKGeometryComType::FGTRotate:
            m_Ui->stackedWidget->setCurrentIndex(1);
            setWindowTitle(tr("Rotate"));
            m_Ui->chec_duplication->setVisible(true);
            break;
        case Interface::FITKGeoEnum::FITKGeometryComType::FGTScale:
            m_Ui->stackedWidget->setCurrentIndex(2);
            setWindowTitle(tr("Scale"));
            m_Ui->chec_duplication->setVisible(true);
            break;
        case Interface::FITKGeoEnum::FITKGeometryComType::FGTMirror:
            m_Ui->stackedWidget->setCurrentIndex(3);
            setWindowTitle(tr("Mirror"));
            m_Ui->chec_duplication->setVisible(true);
            break;
        case Interface::FITKGeoEnum::FITKGeometryComType::FGTRectangularPattern:
            m_Ui->stackedWidget->setCurrentIndex(4);
            setWindowTitle(tr("Rectangular Pattern"));
            m_Ui->chec_duplication->setVisible(false);
            m_Ui->chec_duplication->setChecked(true);
            break;
        case Interface::FITKGeoEnum::FITKGeometryComType::FGTCircularPattern:
            m_Ui->stackedWidget->setCurrentIndex(5);
            setWindowTitle(tr("Circular Pattern"));
            m_Ui->chec_duplication->setVisible(false);
            m_Ui->chec_duplication->setChecked(true);
            break;
        default:
            return;
            break;
        }
        m_TransType = type;
    }

    TransformationInfoDialog * TransformationInfoDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper * oper, Interface::FITKAbsGeoCommand* cmd)
    {
        auto command = dynamic_cast<Interface::FITKAbsGeoTransformation*>(cmd);
        static TransformationInfoDialog* uniqueObj;
        if (uniqueObj == nullptr) {
            uniqueObj = new TransformationInfoDialog(parent);
        }
        if (parent != uniqueObj->parent()) { uniqueObj->setParent(parent); }
        if (oper != uniqueObj->m_Operator) { uniqueObj->m_Operator = oper; }
        /* 设置界面数据 */
        /*@{*/
        if (command == nullptr)
        {
            uniqueObj->m_Ui->comb_transform_method->setCurrentIndex(0);
            uniqueObj->m_Ui->lb_source->setText(tr("Picked (0)"));
            uniqueObj->m_Ui->chec_duplication->setChecked(false);
            // 平移变换
            uniqueObj->m_Ui->comb_transform_method->setCurrentIndex(0);
            uniqueObj->m_Ui->le_transform_twopoint_p1x->setValue(0);
            uniqueObj->m_Ui->le_transform_twopoint_p1y->setValue(0);
            uniqueObj->m_Ui->le_transform_twopoint_p1z->setValue(0);
            uniqueObj->m_Ui->le_transform_twopoint_p2x->setValue(0);
            uniqueObj->m_Ui->le_transform_twopoint_p2y->setValue(0);
            uniqueObj->m_Ui->le_transform_twopoint_p2z->setValue(0);
            uniqueObj->m_Ui->le_transform_diranddistance_dirx->setValue(0);
            uniqueObj->m_Ui->le_transform_diranddistance_diry->setValue(0);
            uniqueObj->m_Ui->le_transform_diranddistance_dirz->setValue(0);
            uniqueObj->m_Ui->le_transform_diranddistance_distance->setValue(0);
            // 旋转变换
            uniqueObj->m_Ui->le_rotateaxis_p1x->setValue(0);
            uniqueObj->m_Ui->le_rotateaxis_p1y->setValue(0);
            uniqueObj->m_Ui->le_rotateaxis_p1z->setValue(0);
            uniqueObj->m_Ui->le_rotateaxis_p2x->setValue(0);
            uniqueObj->m_Ui->le_rotateaxis_p2y->setValue(0);
            uniqueObj->m_Ui->le_rotateaxis_p2z->setValue(0);
            uniqueObj->m_Ui->le_rotate_angle->setValue(0);
            // 缩放变换
            uniqueObj->m_Ui->le_scale_basex->setValue(0);
            uniqueObj->m_Ui->le_scale_basey->setValue(0);
            uniqueObj->m_Ui->le_scale_basez->setValue(0);
            uniqueObj->m_Ui->le_scale_factorx->setValue(1);
            uniqueObj->m_Ui->le_scale_factory->setValue(1);
            uniqueObj->m_Ui->le_scale_factorz->setValue(1);
            // 镜像
            uniqueObj->m_Ui->lb_mirror_reference->setText(tr("Picked (0)"));
            // 线性阵列
            uniqueObj->m_Ui->comb_rectfirst_parameters->setCurrentIndex(0);
            uniqueObj->on_comb_rectfirst_parameters_currentIndexChanged(0);
            uniqueObj->m_Ui->sb_rectfirst_instances->setValue(2);
            uniqueObj->m_Ui->le_rectfirst_spacing->setValue(20);
            uniqueObj->m_Ui->le_rectfirst_length->setValue(20);
            uniqueObj->m_Ui->le_rectfirst_refx->setValue(1);
            uniqueObj->m_Ui->le_rectfirst_refy->setValue(0);
            uniqueObj->m_Ui->le_rectfirst_refz->setValue(0);

            uniqueObj->m_Ui->comb_rectsecond_parameters->setCurrentIndex(0);
            uniqueObj->on_comb_rectsecond_parameters_currentIndexChanged(0);
            uniqueObj->m_Ui->sb_rectsecond_instances->setValue(1);
            uniqueObj->m_Ui->le_rectsecond_spacing->setValue(20);
            uniqueObj->m_Ui->le_rectsecond_length->setValue(20);
            uniqueObj->m_Ui->le_rectsecond_refx->setValue(0);
            uniqueObj->m_Ui->le_rectsecond_refy->setValue(1);
            uniqueObj->m_Ui->le_rectsecond_refz->setValue(0);

            // 环形阵列
            uniqueObj->m_Ui->comb_ciraxial_parameters->setCurrentIndex(0);
            uniqueObj->on_comb_ciraxial_parameters_currentIndexChanged(0);
            uniqueObj->m_Ui->sb_ciraxial_instances->setValue(2);
            uniqueObj->m_Ui->le_ciraxial_spacing->setValue(45);
            uniqueObj->m_Ui->le_ciraxial_length->setValue(45);
            uniqueObj->m_Ui->lb_ciraxial_axis->setText(tr("Picked (0)"));

            uniqueObj->m_Ui->comb_circrown_parameters->setCurrentIndex(0);
            uniqueObj->m_Ui->sb_circrown_instances->setValue(1);
            uniqueObj->m_Ui->le_circrown_spacing->setValue(20);
            uniqueObj->m_Ui->le_circrown_length->setValue(20);
        }
        else {
            uniqueObj->m_dataId = command->getDataObjectID();
            uniqueObj->m_SourceShape = command->sourceShape();
            uniqueObj->m_Ui->lb_source->setText(uniqueObj->m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
            uniqueObj->m_Ui->chec_duplication->setChecked(command->isCopy());
            switch (command->getGeometryCommandType())
            {
            case Interface::FITKGeoEnum::FGTTransformByTwoPoints:
            {
                auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelTransformByTwoPoints*>(command);
                if (tempCmd == nullptr) return nullptr;
                uniqueObj->m_Ui->comb_transform_method->setCurrentIndex(0);
                auto xyz = tempCmd->startPoint();
                uniqueObj->m_Ui->le_transform_twopoint_p1x->setValue(xyz[0]);
                uniqueObj->m_Ui->le_transform_twopoint_p1y->setValue(xyz[1]);
                uniqueObj->m_Ui->le_transform_twopoint_p1z->setValue(xyz[2]);
                xyz = tempCmd->endPoint();
                uniqueObj->m_Ui->le_transform_twopoint_p2x->setValue(xyz[0]);
                uniqueObj->m_Ui->le_transform_twopoint_p2y->setValue(xyz[1]);
                uniqueObj->m_Ui->le_transform_twopoint_p2z->setValue(xyz[2]);

                break;
            }
            case Interface::FITKGeoEnum::FGTTransformByDirAndDis:
            {
                auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelTransformByDirAndDis*>(command);
                if (tempCmd == nullptr) return nullptr;

                uniqueObj->m_Ui->comb_transform_method->setCurrentIndex(0);
                auto xyz = tempCmd->direction();
                uniqueObj->m_Ui->le_transform_diranddistance_dirx->setValue(xyz[0]);
                uniqueObj->m_Ui->le_transform_diranddistance_diry->setValue(xyz[1]);
                uniqueObj->m_Ui->le_transform_diranddistance_dirz->setValue(xyz[2]);
                uniqueObj->m_Ui->le_transform_diranddistance_distance->setValue(tempCmd->distance());

                break;
            }
            case Interface::FITKGeoEnum::FGTRotate:
            {
                auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelRotate*>(command);
                if (tempCmd == nullptr) return nullptr;
                auto xyz = tempCmd->axisStartPoint();
                uniqueObj->m_Ui->le_rotateaxis_p1x->setValue(xyz[0]);
                uniqueObj->m_Ui->le_rotateaxis_p1y->setValue(xyz[1]);
                uniqueObj->m_Ui->le_rotateaxis_p1z->setValue(xyz[2]);
                xyz = tempCmd->axisEndPoint();
                uniqueObj->m_Ui->le_rotateaxis_p2x->setValue(xyz[0]);
                uniqueObj->m_Ui->le_rotateaxis_p2y->setValue(xyz[1]);
                uniqueObj->m_Ui->le_rotateaxis_p2z->setValue(xyz[2]);
                uniqueObj->m_Ui->le_rotate_angle->setValue(0);
                break;
            }
            case Interface::FITKGeoEnum::FGTScale:
            {
                auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelScale*>(command);
                if (tempCmd == nullptr) return nullptr;
                auto xyz = tempCmd->basePoint();
                uniqueObj->m_Ui->le_scale_basex->setValue(xyz[0]);
                uniqueObj->m_Ui->le_scale_basey->setValue(xyz[1]);
                uniqueObj->m_Ui->le_scale_basez->setValue(xyz[2]);
                xyz = tempCmd->factors();
                uniqueObj->m_Ui->le_scale_factorx->setValue(xyz[0]);
                uniqueObj->m_Ui->le_scale_factory->setValue(xyz[1]);
                uniqueObj->m_Ui->le_scale_factorz->setValue(xyz[2]);
                break;
            }
            case Interface::FITKGeoEnum::FGTMirror:
            {
                auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelMirror*>(command);
                if (tempCmd == nullptr) return nullptr;
                uniqueObj->m_Ui->comb_mirror_type->setCurrentIndex(-1);
                uniqueObj->m_MirrorReference = tempCmd->reference();
                uniqueObj->m_Ui->lb_mirror_reference->setText(uniqueObj->m_MirrorReference.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
                break;
            }
            case Interface::FITKGeoEnum::FGTRectangularPattern:
            {
                auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelRectangularPattern*>(command);
                if (tempCmd == nullptr) return nullptr;

                uniqueObj->m_Ui->comb_rectfirst_parameters->setCurrentIndex(static_cast<int>(tempCmd->firstDefinition()));
                uniqueObj->m_Ui->sb_rectfirst_instances->setValue(tempCmd->firstCount());
                uniqueObj->m_Ui->le_rectfirst_spacing->setValue(tempCmd->firstSpacing());
                uniqueObj->m_Ui->le_rectfirst_length->setValue(tempCmd->firstTotal());

                auto xyz = tempCmd->firstDirection();
                uniqueObj->m_Ui->le_rectfirst_refx->setValue(xyz[0]);
                uniqueObj->m_Ui->le_rectfirst_refy->setValue(xyz[1]);
                uniqueObj->m_Ui->le_rectfirst_refz->setValue(xyz[2]);


                uniqueObj->m_Ui->comb_rectsecond_parameters->setCurrentIndex(static_cast<int>(tempCmd->secondDefinition()));
                uniqueObj->m_Ui->sb_rectsecond_instances->setValue(tempCmd->secondCount());
                uniqueObj->m_Ui->le_rectsecond_spacing->setValue(tempCmd->secondSpacing());
                uniqueObj->m_Ui->le_rectsecond_length->setValue(tempCmd->secondTotal());

                xyz = tempCmd->secondDirection();
                uniqueObj->m_Ui->le_rectsecond_refx->setValue(xyz[0]);
                uniqueObj->m_Ui->le_rectsecond_refy->setValue(xyz[1]);
                uniqueObj->m_Ui->le_rectsecond_refz->setValue(xyz[2]);
                break;
            }
            case Interface::FITKGeoEnum::FGTCircularPattern:
            {
                auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelCircularPattern*>(command);
                if (tempCmd == nullptr) return nullptr;
                uniqueObj->m_Ui->comb_ciraxial_parameters->setCurrentIndex(static_cast<int>(tempCmd->firstDefinition()));
                uniqueObj->m_Ui->sb_ciraxial_instances->setValue(tempCmd->axisCount());
                uniqueObj->m_Ui->le_ciraxial_spacing->setValue(tempCmd->axisSpacing());
                uniqueObj->m_Ui->le_ciraxial_length->setValue(tempCmd->axisTotal());
                uniqueObj->m_PatternAxis = tempCmd->axis();
                uniqueObj->m_Reverse = tempCmd->isReverse();
                uniqueObj->m_Ui->lb_ciraxial_axis->setText(uniqueObj->m_PatternAxis.isNull() ? tr("Picked (0)") : tr("Picked (1)"));

                uniqueObj->m_Ui->comb_circrown_parameters->setCurrentIndex(static_cast<int>(tempCmd->secondDefinition()));
                uniqueObj->m_Ui->sb_circrown_instances->setValue(tempCmd->crownCount());
                uniqueObj->m_Ui->le_circrown_spacing->setValue(tempCmd->crownSpacing());
                uniqueObj->m_Ui->le_circrown_length->setValue(tempCmd->crownTotal());
                break;
            }
            default:
                break;
            }

        }
        /*@}*/
        return uniqueObj;
    }

    bool TransformationInfoDialog::checkDialogData() const
    {
        // 数据检查
        bool hasEmpty{ true };
        switch (m_TransType)
        {
        case Interface::FITKGeoEnum::FGTTransformByTwoPoints:
            hasEmpty = m_Ui->le_transform_twopoint_p1x->text().isEmpty() ||
                m_Ui->le_transform_twopoint_p1y->text().isEmpty() ||
                m_Ui->le_transform_twopoint_p1z->text().isEmpty() ||
                m_Ui->le_transform_twopoint_p2x->text().isEmpty() ||
                m_Ui->le_transform_twopoint_p2y->text().isEmpty() ||
                m_Ui->le_transform_twopoint_p2z->text().isEmpty();
            break;
        case Interface::FITKGeoEnum::FGTTransformByDirAndDis:
            hasEmpty = m_Ui->le_transform_diranddistance_dirx->text().isEmpty() ||
                m_Ui->le_transform_diranddistance_diry->text().isEmpty() ||
                m_Ui->le_transform_diranddistance_dirz->text().isEmpty() ||
                m_Ui->le_transform_diranddistance_distance->text().isEmpty();
            break;
        case Interface::FITKGeoEnum::FGTRotate:
            hasEmpty = m_Ui->le_rotateaxis_p1x->text().isEmpty() ||
                m_Ui->le_rotateaxis_p1y->text().isEmpty() ||
                m_Ui->le_rotateaxis_p1z->text().isEmpty() ||
                m_Ui->le_rotateaxis_p2x->text().isEmpty() ||
                m_Ui->le_rotateaxis_p2y->text().isEmpty() ||
                m_Ui->le_rotateaxis_p2z->text().isEmpty() ||
                m_Ui->le_rotate_angle->text().isEmpty();
            break;
        case Interface::FITKGeoEnum::FGTScale:
            hasEmpty = m_Ui->le_scale_basex->text().isEmpty() ||
                m_Ui->le_scale_basey->text().isEmpty() ||
                m_Ui->le_scale_basez->text().isEmpty() ||
                m_Ui->le_scale_factorx->text().isEmpty() ||
                m_Ui->le_scale_factory->text().isEmpty() ||
                m_Ui->le_scale_factorz->text().isEmpty();
            break;
        case Interface::FITKGeoEnum::FGTMirror:
            hasEmpty = m_MirrorReference.isNull();
            break;
        case Interface::FITKGeoEnum::FGTRectangularPattern:
            switch (m_Ui->comb_rectfirst_parameters->currentIndex())
            {
            case 0:
                hasEmpty = m_Ui->le_rectfirst_spacing->text().isEmpty();
                break;
            case 1:
                hasEmpty = m_Ui->le_rectfirst_length->text().isEmpty();
                break;
            case 2:
                hasEmpty = m_Ui->le_rectfirst_spacing->text().isEmpty() || m_Ui->le_rectfirst_length->text().isEmpty();
                break;
            default:
                break;
            }
            hasEmpty = hasEmpty || m_Ui->le_rectfirst_refx->text().isEmpty() || m_Ui->le_rectfirst_refy->text().isEmpty() || m_Ui->le_rectfirst_refz->text().isEmpty();
            if (!hasEmpty && m_Ui->sb_rectsecond_instances->value() > 1) {
                switch (m_Ui->comb_rectsecond_parameters->currentIndex())
                {
                case 0:
                    hasEmpty = m_Ui->le_rectsecond_spacing->text().isEmpty();
                    break;
                case 1:
                    hasEmpty = m_Ui->le_rectsecond_length->text().isEmpty();
                    break;
                case 2:
                    hasEmpty = m_Ui->le_rectsecond_spacing->text().isEmpty() || m_Ui->le_rectsecond_length->text().isEmpty();
                    break;
                default:
                    break;
                }
                hasEmpty = hasEmpty || m_Ui->le_rectsecond_refx->text().isEmpty() || m_Ui->le_rectsecond_refy->text().isEmpty() || m_Ui->le_rectsecond_refz->text().isEmpty();
            }
            break;
        case Interface::FITKGeoEnum::FGTCircularPattern:
            switch (m_Ui->comb_ciraxial_parameters->currentIndex())
            {
            case 0:
                hasEmpty = m_Ui->le_ciraxial_spacing->text().isEmpty();
                break;
            case 1:
                hasEmpty = m_Ui->le_ciraxial_length->text().isEmpty();
                break;
            case 2:
                hasEmpty = m_Ui->le_ciraxial_spacing->text().isEmpty() || m_Ui->le_ciraxial_length->text().isEmpty();
                break;
            default:
                break;
            }
            hasEmpty = hasEmpty || m_PatternAxis.isNull();
            if (!hasEmpty && m_Ui->sb_circrown_instances->value() > 1) {
                switch (m_Ui->comb_circrown_parameters->currentIndex())
                {
                case 0:
                    hasEmpty = m_Ui->le_circrown_spacing->text().isEmpty();
                    break;
                case 1:
                    hasEmpty = m_Ui->le_circrown_length->text().isEmpty();
                    break;
                case 2:
                    hasEmpty = m_Ui->le_circrown_spacing->text().isEmpty() || m_Ui->le_circrown_length->text().isEmpty();
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }

        if (hasEmpty || m_SourceShape.isNull()) {
            QMessageBox::information(nullptr, tr("Information"), tr("The data is incomplete!"));
            return false;
        }
        // 检查虚拓扑数据是否存在(通用)
        auto shape = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_SourceShape.VirtualTopoId);
        if (shape == nullptr)
        {
            QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
            return false;
        }
        // 检查虚拓扑数据是否存在（镜像元素）
        if (m_TransType == Interface::FITKGeoEnum::FGTMirror) {
            auto shape = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_MirrorReference.VirtualTopoId);
            if (shape == nullptr)
            {
                QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
                return false;
            }
        }
        // 检查虚拓扑数据是否存在（环形阵列旋转中心轴）
        if (m_TransType == Interface::FITKGeoEnum::FGTCircularPattern) {
            auto shape = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(m_PatternAxis.VirtualTopoId);
            if (shape == nullptr)
            {
                QMessageBox::information(nullptr, tr("Information"), tr("The picked data does not exist!"));
                return false;
            }
        }
        return true;
    }

    void TransformationInfoDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 断开拾取信号
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
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
        m_MirrorReference.reset();
        m_PatternAxis.reset();
    }

    void TransformationInfoDialog::on_comb_transform_method_currentIndexChanged(int index)
    {
        m_Ui->sw_transform->setCurrentIndex(index);
        m_TransType = index == 0 ? Interface::FITKGeoEnum::FGTTransformByTwoPoints : Interface::FITKGeoEnum::FGTTransformByDirAndDis;
    }

    void TransformationInfoDialog::on_pb_selectsource_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJShape;
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
                /*auto shapeAgent = pickData->getGeoShapeAgent();
                if (shapeAgent == nullptr) return;
                auto cmd = shapeAgent->getGeoCommand();
                if (cmd == nullptr) return;
                m_SourceShape.CmdId = cmd->getDataObjectID();

                auto virtualTopo = pickData->getVirtualTopo();
                if (virtualTopo == nullptr) return;
                m_SourceShape.VirtualTopoId = virtualTopo->getDataObjectID(); */

                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_SourceShape = virtualShapes.at(0);
            }
            else {
                m_SourceShape.reset();
            }
            m_Ui->lb_source->setText(m_SourceShape.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void TransformationInfoDialog::on_pb_transform_twopoint_pickpoint1_clicked()
    {
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJVert;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            if (pickedList.size() != 1) return;
            auto pickData = pickedList.at(0);
            if (pickData == nullptr) return;

            auto virtualTopo = pickData->getVirtualTopo();
            if (virtualTopo == nullptr) return;

            auto pointTool = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator()->createPointTool();
            double xyz[3];
            if (!pointTool->getXYZByID(virtualTopo->getDataObjectID(), xyz))return;


            m_Ui->le_transform_twopoint_p1x->setValue(xyz[0]);
            m_Ui->le_transform_twopoint_p1y->setValue(xyz[1]);
            m_Ui->le_transform_twopoint_p1z->setValue(xyz[2]);


            delete pointTool;
        });
    }

    void TransformationInfoDialog::on_pb_transform_twopoint_pickpoint2_clicked()
    {
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJVert;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            if (pickedList.size() != 1) return;
            auto pickData = pickedList.at(0);
            if (pickData == nullptr) return;

            auto virtualTopo = pickData->getVirtualTopo();
            if (virtualTopo == nullptr) return;

            auto pointTool = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator()->createPointTool();
            double xyz[3];
            if (!pointTool->getXYZByID(virtualTopo->getDataObjectID(), xyz))return;


            m_Ui->le_transform_twopoint_p2x->setValue(xyz[0]);
            m_Ui->le_transform_twopoint_p2y->setValue(xyz[1]);
            m_Ui->le_transform_twopoint_p2z->setValue(xyz[2]);


            delete pointTool;
        });
    }

    void TransformationInfoDialog::on_pb_rotateaxis_pickpoint1_clicked()
    {
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJVert;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            if (pickedList.size() != 1) return;
            auto pickData = pickedList.at(0);
            if (pickData == nullptr) return;

            auto virtualTopo = pickData->getVirtualTopo();
            if (virtualTopo == nullptr) return;

            auto pointTool = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator()->createPointTool();
            double xyz[3];
            if (!pointTool->getXYZByID(virtualTopo->getDataObjectID(), xyz))return;


            m_Ui->le_rotateaxis_p1x->setValue(xyz[0]);
            m_Ui->le_rotateaxis_p1y->setValue(xyz[1]);
            m_Ui->le_rotateaxis_p1z->setValue(xyz[2]);


            delete pointTool;
        });
    }

    void TransformationInfoDialog::on_pb_rotateaxis_pickpoint2_clicked()
    {
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJVert;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            if (pickedList.size() != 1) return;
            auto pickData = pickedList.at(0);
            if (pickData == nullptr) return;

            auto virtualTopo = pickData->getVirtualTopo();
            if (virtualTopo == nullptr) return;

            auto pointTool = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator()->createPointTool();
            double xyz[3];
            if (!pointTool->getXYZByID(virtualTopo->getDataObjectID(), xyz))return;


            m_Ui->le_rotateaxis_p2x->setValue(xyz[0]);
            m_Ui->le_rotateaxis_p2y->setValue(xyz[1]);
            m_Ui->le_rotateaxis_p2z->setValue(xyz[2]);


            delete pointTool;
        });
    }

    void TransformationInfoDialog::on_pb_scale_pickbase_clicked()
    {
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJVert;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据
            auto pickedList = pickDataProvider->getPickedList();
            if (pickedList.size() != 1) return;
            auto pickData = pickedList.at(0);
            if (pickData == nullptr) return;

            auto virtualTopo = pickData->getVirtualTopo();
            if (virtualTopo == nullptr) return;

            auto pointTool = Interface::FITKInterfaceGeometryFactory::getInstance()->getGeomToolsCreator()->createPointTool();
            double xyz[3];
            if (!pointTool->getXYZByID(virtualTopo->getDataObjectID(), xyz))return;


            m_Ui->le_scale_basex->setValue(xyz[0]);
            m_Ui->le_scale_basey->setValue(xyz[1]);
            m_Ui->le_scale_basez->setValue(xyz[2]);


            delete pointTool;
        });
    }

    void TransformationInfoDialog::on_comb_mirror_type_currentIndexChanged(int index)
    {
        m_MirrorReference.reset();
        m_Ui->lb_mirror_reference->setText(tr("Picked (0)"));
    }
    void TransformationInfoDialog::on_lb_mirror_pickref_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        switch (m_Ui->comb_mirror_type->currentIndex())
        {
        case 0:
            pickInfo._pickObjType = GUIPickInfo::POBJVert;
            break;
        case 1:
            pickInfo._pickObjType = GUIPickInfo::POBJEdge;
            break;
        case 2:
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

                // 获取虚拓扑形状
                auto virtualShapes = pickData->getVirtualShapes();
                if (virtualShapes.size() != 1) return;
                m_MirrorReference = virtualShapes.at(0);
            }
            else {
                m_MirrorReference.reset();
            }
            m_Ui->lb_mirror_reference->setText(m_MirrorReference.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }


    void TransformationInfoDialog::on_comb_rectfirst_parameters_currentIndexChanged(int index)
    {
        m_Ui->sb_rectfirst_instances->setDisabled(index == 2);
        m_Ui->le_rectfirst_spacing->setDisabled(index == 1);
        m_Ui->le_rectfirst_length->setDisabled(index == 0);
    }

    void TransformationInfoDialog::onRectFirstDirectionReverseSlot()
    {
        // 获取箭头方向
        QList<double> direction;

        auto graphToolOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphToolOperator>("GraphTool");
        if (graphToolOper == nullptr) return;
        if (graphToolOper->getPickedDirection(direction) && direction.size() == 3) {
            m_Ui->le_rectfirst_refx->setValue(direction.at(0));
            m_Ui->le_rectfirst_refy->setValue(direction.at(1));
            m_Ui->le_rectfirst_refz->setValue(direction.at(2));
        }
    }

    void TransformationInfoDialog::on_pb_rectfirst_pickref_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJEdge;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据

            auto pickedList = pickDataProvider->getPickedList();
            if (pickedList.size() != 1) return;

            //m_GraphToolOper->setPickToolObjectVisible(EventOper::PickToolType::PTT_Direction, true);
            connect(pickDataProvider, SIGNAL(sig_directionPicked()), this, SLOT(onRectFirstDirectionReverseSlot()), Qt::UniqueConnection);
            // 重新获取箭头方向
            onRectFirstDirectionReverseSlot();
        });
    }

    void TransformationInfoDialog::on_comb_rectsecond_parameters_currentIndexChanged(int index)
    {
        m_Ui->sb_rectsecond_instances->setDisabled(index == 2);
        m_Ui->le_rectsecond_spacing->setDisabled(index == 1);
        m_Ui->le_rectsecond_length->setDisabled(index == 0);
    }

    void TransformationInfoDialog::onRectSecondDirectionReverseSlot()
    {
        // 获取箭头方向
        QList<double> direction;

        auto graphToolOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphToolOperator>("GraphTool");
        if (graphToolOper == nullptr) return;
        if (graphToolOper->getPickedDirection(direction) && direction.size() == 3) {
            m_Ui->le_rectsecond_refx->setValue(direction.at(0));
            m_Ui->le_rectsecond_refy->setValue(direction.at(1));
            m_Ui->le_rectsecond_refz->setValue(direction.at(2));
        }
    }

    void TransformationInfoDialog::on_pb_rectsecond_pickref_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJEdge;
        GUIPickInfo::SetPickInfo(pickInfo);
        auto pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
        connect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, [this, pickDataProvider]() {
            // 清除信号
            GUIPickInfo::ClearPickInfo();
            disconnect(pickDataProvider, &GraphData::GraphPickedDataProvider::sig_dataPicked, nullptr, nullptr);
            // 处理拾取数据

            auto pickedList = pickDataProvider->getPickedList();
            if (pickedList.size() != 1) return;

            //m_GraphToolOper->setPickToolObjectVisible(EventOper::PickToolType::PTT_Direction, true);
            connect(pickDataProvider, SIGNAL(sig_directionPicked()), this, SLOT(onRectSecondDirectionReverseSlot()), Qt::UniqueConnection);
            // 重新获取箭头方向
            onRectSecondDirectionReverseSlot();
        });
    }

    void TransformationInfoDialog::on_comb_ciraxial_parameters_currentIndexChanged(int index)
    {
        m_Ui->sb_ciraxial_instances->setDisabled(index == 2);
        m_Ui->le_ciraxial_spacing->setDisabled(index == 1);
        m_Ui->le_ciraxial_length->setDisabled(index == 0);
    }

    void TransformationInfoDialog::on_pb_ciraxial_pickaxis_clicked()
    {
        // 设置拾取信息
        GUIPickInfoStru pickInfo{};
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUIPickInfo::POBJEdge;
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
                m_PatternAxis = virtualShapes.at(0);
            }
            else {
                m_PatternAxis.reset();
            }
            m_Ui->lb_ciraxial_axis->setText(m_PatternAxis.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        });
    }

    void TransformationInfoDialog::on_pb_ciraxial_reverseaxis_clicked()
    {
        m_Reverse = !m_Reverse;
    }

    void TransformationInfoDialog::on_comb_circrown_parameters_currentIndexChanged(int index)
    {
        m_Ui->sb_circrown_instances->setDisabled(index == 2);
        m_Ui->le_circrown_spacing->setDisabled(index == 1);
        m_Ui->le_circrown_length->setDisabled(index == 0);
    }

    void TransformationInfoDialog::on_pb_ok_clicked()
    {
        // 检查输入
        if (!checkDialogData() || m_Operator == nullptr) return;

        // 清除缓存。
        m_Operator->clearArgs();

        // 传递历史数据ID。
        m_Operator->setArgs("Id", m_dataId);

        // 传递输入参数。
        switch (m_TransType)
        {
        case Interface::FITKGeoEnum::FGTTransformByTwoPoints:
        {
            m_Operator->setArgs("Type", Interface::FITKGeoEnum::FGTTransformByTwoPoints);
            m_Operator->setArgs("P1x", m_Ui->le_transform_twopoint_p1x->value());
            m_Operator->setArgs("P1y", m_Ui->le_transform_twopoint_p1y->value());
            m_Operator->setArgs("P1z", m_Ui->le_transform_twopoint_p1z->value());

            m_Operator->setArgs("P2x", m_Ui->le_transform_twopoint_p2x->value());
            m_Operator->setArgs("P2y", m_Ui->le_transform_twopoint_p2y->value());
            m_Operator->setArgs("P2z", m_Ui->le_transform_twopoint_p2z->value());
            break;
        }
        case Interface::FITKGeoEnum::FGTTransformByDirAndDis:
        {
            m_Operator->setArgs("Type", Interface::FITKGeoEnum::FGTTransformByDirAndDis);
            m_Operator->setArgs("Dx", m_Ui->le_transform_diranddistance_dirx->value());
            m_Operator->setArgs("Dy", m_Ui->le_transform_diranddistance_diry->value());
            m_Operator->setArgs("Dz", m_Ui->le_transform_diranddistance_dirz->value());

            m_Operator->setArgs("Distance", m_Ui->le_transform_diranddistance_distance->value());
            break;
        }
        case Interface::FITKGeoEnum::FGTRotate:
        {
            m_Operator->setArgs("P1x", m_Ui->le_rotateaxis_p1x->value());
            m_Operator->setArgs("P1y", m_Ui->le_rotateaxis_p1y->value());
            m_Operator->setArgs("P1z", m_Ui->le_rotateaxis_p1z->value());

            m_Operator->setArgs("P2x", m_Ui->le_rotateaxis_p2x->value());
            m_Operator->setArgs("P2y", m_Ui->le_rotateaxis_p2y->value());
            m_Operator->setArgs("P2z", m_Ui->le_rotateaxis_p2z->value());

            m_Operator->setArgs("Angle", m_Ui->le_rotate_angle->value());
            break;
        }
        case Interface::FITKGeoEnum::FGTScale:
        {
            m_Operator->setArgs("Px", m_Ui->le_scale_basex->value());
            m_Operator->setArgs("Py", m_Ui->le_scale_basey->value());
            m_Operator->setArgs("Pz", m_Ui->le_scale_basez->value());

            m_Operator->setArgs("Fx", m_Ui->le_scale_factorx->value());
            m_Operator->setArgs("Fy", m_Ui->le_scale_factory->value());
            m_Operator->setArgs("Fz", m_Ui->le_scale_factorz->value());
            break;
        }
        case Interface::FITKGeoEnum::FGTMirror:
        {
            m_Operator->setArgs("Reference", QVariant::fromValue(m_MirrorReference));
            break;
        }
        case Interface::FITKGeoEnum::FGTRectangularPattern:
        {
            m_Operator->setArgs("FirstDefinition", m_Ui->comb_rectfirst_parameters->currentIndex());
            m_Operator->setArgs("FirstInstances", m_Ui->sb_rectfirst_instances->value());
            m_Operator->setArgs("FirstSpacing", m_Ui->le_rectfirst_spacing->value());
            m_Operator->setArgs("FirstLength", m_Ui->le_rectfirst_length->value());
            m_Operator->setArgs("FirstDx", m_Ui->le_rectfirst_refx->value());
            m_Operator->setArgs("FirstDy", m_Ui->le_rectfirst_refy->value());
            m_Operator->setArgs("FirstDz", m_Ui->le_rectfirst_refz->value());

            m_Operator->setArgs("SecondDefinition", m_Ui->comb_rectsecond_parameters->currentIndex());
            m_Operator->setArgs("SecondInstances", m_Ui->sb_rectsecond_instances->value());
            m_Operator->setArgs("SecondSpacing", m_Ui->le_rectsecond_spacing->value());
            m_Operator->setArgs("SecondLength", m_Ui->le_rectsecond_length->value());
            m_Operator->setArgs("SecondDx", m_Ui->le_rectsecond_refx->value());
            m_Operator->setArgs("SecondDy", m_Ui->le_rectsecond_refy->value());
            m_Operator->setArgs("SecondDz", m_Ui->le_rectsecond_refz->value());
            break;
        }
        case Interface::FITKGeoEnum::FGTCircularPattern:
        {
            m_Operator->setArgs("AxisDefinition", m_Ui->comb_ciraxial_parameters->currentIndex());
            m_Operator->setArgs("AxisInstances", m_Ui->sb_ciraxial_instances->value());
            m_Operator->setArgs("AxisSpacing", m_Ui->le_ciraxial_spacing->value());
            m_Operator->setArgs("AxisLength", m_Ui->le_ciraxial_length->value());
            m_Operator->setArgs("PatternAxis", QVariant::fromValue(m_PatternAxis));
            m_Operator->setArgs("IsReverseAxis", m_Reverse);

            m_Operator->setArgs("CrownDefinition", m_Ui->comb_circrown_parameters->currentIndex());
            m_Operator->setArgs("CrownInstances", m_Ui->sb_circrown_instances->value());
            m_Operator->setArgs("CrownSpacing", m_Ui->le_circrown_spacing->value());
            m_Operator->setArgs("CrownLength", m_Ui->le_circrown_length->value());
            break;
        }
        default:
            break;
        }
        m_Operator->setArgs("SourceShape", QVariant::fromValue(m_SourceShape));
        m_Operator->setArgs("IsCopy", m_Ui->chec_duplication->isChecked());

        // 执行功能。
        if (!m_Operator->execProfession()) return;

        // 保存数据ID。
        m_Operator->argValue("Id", m_dataId);
        // 释放资源
        finalize();
        // 关闭对话框
        accept();
    }

    void TransformationInfoDialog::on_pb_preview_clicked()
    {
        // 检查数据
        if (!checkDialogData()) return;

        // 判断预览对象是否为空
        if (m_PreviewCmd == nullptr || m_PreviewCmd->getGeometryCommandType() != m_TransType) {
            delete m_PreviewCmd;
            auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
            m_PreviewCmd = fac->createCommandT<Interface::FITKAbsGeoTransformation>(m_TransType);
        }

        // 分别设置参数
        switch (m_TransType)
        {
        case Interface::FITKGeoEnum::FGTTransformByTwoPoints:
        {
            auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelTransformByTwoPoints*>(m_PreviewCmd);
            if (tempCmd == nullptr) return;
            tempCmd->setStartPoint(m_Ui->le_transform_twopoint_p1x->value(), m_Ui->le_transform_twopoint_p1y->value(), m_Ui->le_transform_twopoint_p1z->value());
            tempCmd->setEndPoint(m_Ui->le_transform_twopoint_p2x->value(), m_Ui->le_transform_twopoint_p2y->value(), m_Ui->le_transform_twopoint_p2z->value());

            break;
        }
        case Interface::FITKGeoEnum::FGTTransformByDirAndDis:
        {
            auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelTransformByDirAndDis*>(m_PreviewCmd);
            if (tempCmd == nullptr) return;
            tempCmd->setDirection(m_Ui->le_transform_diranddistance_dirx->value(), m_Ui->le_transform_diranddistance_diry->value(), m_Ui->le_transform_diranddistance_dirz->value());
            tempCmd->setDistance(m_Ui->le_transform_diranddistance_distance->value());

            break;
        }
        case Interface::FITKGeoEnum::FGTRotate:
        {
            auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelRotate*>(m_PreviewCmd);
            if (tempCmd == nullptr) return;
            tempCmd->setAxisStartPoint(m_Ui->le_rotateaxis_p1x->value(), m_Ui->le_rotateaxis_p1y->value(), m_Ui->le_rotateaxis_p1z->value());
            tempCmd->setAxisEndPoint(m_Ui->le_rotateaxis_p2x->value(), m_Ui->le_rotateaxis_p2y->value(), m_Ui->le_rotateaxis_p2z->value());
            tempCmd->setDegree(m_Ui->le_rotate_angle->value());
            break;
        }
        case Interface::FITKGeoEnum::FGTScale:
        {
            auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelScale*>(m_PreviewCmd);
            if (tempCmd == nullptr) return;
            tempCmd->setBasePoint(m_Ui->le_scale_basex->value(), m_Ui->le_scale_basey->value(), m_Ui->le_scale_basez->value());
            tempCmd->setFactors(m_Ui->le_scale_factorx->value(), m_Ui->le_scale_factory->value(), m_Ui->le_scale_factorz->value());
            break;
        }
        case Interface::FITKGeoEnum::FGTMirror:
        {
            auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelMirror*>(m_PreviewCmd);
            if (tempCmd == nullptr) return;
            tempCmd->setReference(m_MirrorReference);
            break;
        }
        case Interface::FITKGeoEnum::FGTRectangularPattern:
        {
            auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelRectangularPattern*>(m_PreviewCmd);
            if (tempCmd == nullptr) return;
            tempCmd->setFirstDefinition(static_cast<Interface::FITKAbsGeoModelRectangularPattern::Definition>(m_Ui->comb_rectfirst_parameters->currentIndex()));
            tempCmd->setFirstCount(m_Ui->sb_rectfirst_instances->value());
            tempCmd->setFirstSpacing(m_Ui->le_rectfirst_spacing->value());
            tempCmd->setFirstTotal(m_Ui->le_rectfirst_length->value());
            tempCmd->setFirstDirection(m_Ui->le_rectfirst_refx->value(), m_Ui->le_rectfirst_refy->value(), m_Ui->le_rectfirst_refz->value());

            tempCmd->setSecondDefinition(static_cast<Interface::FITKAbsGeoModelRectangularPattern::Definition>(m_Ui->comb_rectsecond_parameters->currentIndex()));
            tempCmd->setSecondCount(m_Ui->sb_rectsecond_instances->value());
            tempCmd->setSecondSpacing(m_Ui->le_rectsecond_spacing->value());
            tempCmd->setSecondTotal(m_Ui->le_rectsecond_length->value());
            tempCmd->setSecondDirection(m_Ui->le_rectsecond_refx->value(), m_Ui->le_rectsecond_refy->value(), m_Ui->le_rectsecond_refz->value());

            break;
        }
        case Interface::FITKGeoEnum::FGTCircularPattern:
        {
            auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelCircularPattern*>(m_PreviewCmd);
            if (tempCmd == nullptr) return;
            tempCmd->setFirstDefinition(static_cast<Interface::FITKAbsGeoModelCircularPattern::Definition>(m_Ui->comb_ciraxial_parameters->currentIndex()));
            tempCmd->setAxisCount(m_Ui->sb_ciraxial_instances->value());
            tempCmd->setAxisSpacing(m_Ui->le_ciraxial_spacing->value());
            tempCmd->setAxisTotal(m_Ui->le_ciraxial_length->value());
            tempCmd->setAxis(m_PatternAxis);
            tempCmd->reverse(m_Reverse);

            tempCmd->setSecondDefinition(static_cast<Interface::FITKAbsGeoModelCircularPattern::Definition>(m_Ui->comb_circrown_parameters->currentIndex()));
            tempCmd->setCrownCount(m_Ui->sb_circrown_instances->value());
            tempCmd->setCrownSpacing(m_Ui->le_circrown_spacing->value());
            tempCmd->setCrownTotal(m_Ui->le_circrown_length->value());
            break;
        }
        default:
            return;
            break;
        }
        // 更新预览对象数据
        m_PreviewCmd->setSourceShape(m_SourceShape);
        m_PreviewCmd->setCopy(m_Ui->chec_duplication->isChecked());
        if (!m_PreviewCmd->update()) {
            return;
        }

        // 预览对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->preview(m_PreviewCmd->getDataObjectID());
        }
    }

    void TransformationInfoDialog::on_pb_cancel_clicked()
    {
        // 结束对象操作。
        finalize();

        reject();
    }
}

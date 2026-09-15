/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIRadiossResultRequestDialog.h"
#include "ui_GUIRadiossResultRequestDialog.h"

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossResultRequest.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractTimeHistory.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"

#include "GUITimeHistoryACCELDialog.h"
#include "GUITimeHistoryINTERDialog.h"
#include "GUITimeHistoryRBODYDialog.h"
#include "GUITimeHistoryRWALLDialog.h"
#include "GUITimeHistorySECTIODialog.h"
#include "GUITimeHistorySPHCELDialog.h"

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>

namespace GUI
{
    GUIRadiossResultRequestDialog::GUIRadiossResultRequestDialog(Core::FITKActionOperator* oper, QWidget *parent) :
        GUIDialogBase(nullptr, parent),
        _oper(oper),
        _ui(new Ui::GUIRadiossResultRequestDialog)
    {
        _ui->setupUi(this);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData)
            _resultRequest = caseData->getResultRequest();
        //初始化
        this->init();
        //设置窗口自适应隐藏组件
        this->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);
    }

    GUIRadiossResultRequestDialog::~GUIRadiossResultRequestDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void GUIRadiossResultRequestDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUIRadiossResultRequestDialog::on_pushButton_OK_clicked()
    {
        //设置参数

        this->setAnimFileParam();
        this->setNodalOutputAnim();
        this->setElementalOutputAnim();
        this->setBrickOutputAnim();
        this->setShellOutputAnim();

        _resultRequest->setHisFrequency(getLineEditValue("HisFrequency_Edit"));

        this->setTimeHistory();

        this->writePythonScript();
        this->accept();
    }

    void GUIRadiossResultRequestDialog::pushButton_ACCEL_clicked()
    {
        GUITimeHistoryACCELDialog* dialog = new GUITimeHistoryACCELDialog(_oper, this);
        dialog->show();
    }
    void GUIRadiossResultRequestDialog::pushButton_INTER_clicked()
    {
        GUITimeHistoryINTERDialog* dialog = new GUITimeHistoryINTERDialog(_oper, this);
        dialog->show();
    }
    void GUIRadiossResultRequestDialog::pushButton_RWALL_clicked()
    {
        GUITimeHistoryRWALLDialog* dialog = new GUITimeHistoryRWALLDialog(_oper, this);
        dialog->show();

    }
    void GUIRadiossResultRequestDialog::pushButton_RBODY_clicked()
    {
        GUITimeHistoryRBODYDialog* dialog = new GUITimeHistoryRBODYDialog(_oper, this);
        dialog->show();
    }
    void GUIRadiossResultRequestDialog::pushButton_SECTIO_clicked()
    {
        GUITimeHistorySECTIODialog* dialog = new GUITimeHistorySECTIODialog(_oper, this);
        dialog->show();
    }
    void GUIRadiossResultRequestDialog::pushButton_SPHCEL_clicked()
    {
        GUITimeHistorySPHCELDialog* dialog = new GUITimeHistorySPHCELDialog(_oper, this);
        dialog->show();
    }

    void GUIRadiossResultRequestDialog::init()
    {
        if (!_resultRequest) return;

        this->initAnimationParamsTreeNode();
        this->initNodalOutputAnimationsTreeNode();
        this->initElementalOutputAnimationsTreeNode();
        this->initBrickOutputAnimationsTreeNode();
        this->initShellOutputAnimationsTreeNode();

        ChildTreeNodeData childItemData;
        QTreeWidgetItem* basicParamsRoot = this->addTopLevelItem(QObject::tr("Plot File Parameters"));
        // 时间
        childItemData.setText(QObject::tr(" Time Frequency To Write Data On History Plot File T-file"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "HisFrequency_Edit", QString::number(_resultRequest->getHisFrequency()));
        this->addChildItem(basicParamsRoot, childItemData);

        this->initTimeHistoryTreeNode();

        _ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        _ui->treeWidget->expandAll();
    }
  
    void GUIRadiossResultRequestDialog::initAnimationParamsTreeNode()
    {
        if (!_resultRequest) return;

        ChildTreeNodeData childItemData;
        QTreeWidgetItem* basicParamsRoot = this->addTopLevelItem(QObject::tr(" Animation File Parameters"));
        // 时间
        childItemData.setText(QObject::tr("Start time"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "StartTime_Edit", QString::number(_resultRequest->getStartTime()), "s");
        this->addChildItem(basicParamsRoot, childItemData);
        // 频率
        childItemData.setText(QObject::tr("Frequency"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "Frequency_Edit", QString::number(_resultRequest->getFrequency()), "");
        this->addChildItem(basicParamsRoot, childItemData);
    }

    void GUIRadiossResultRequestDialog::initNodalOutputAnimationsTreeNode()
    {
        if (!_resultRequest) return;
        QList<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType> nodalAnims = _resultRequest->getNodalOutputAnim();

        ChildTreeNodeData childItemData;
        QTreeWidgetItem* nodalAnimRoot = this->addTopLevelItem(QObject::tr("Nodal Output Animations"));
        // 位移
        childItemData.setText(QObject::tr("Displacement"));
        childItemData.setEnableCheckBox(true, "Nodal_Displacement_CheckBox",
            nodalAnims.contains(Radioss::FITKRadiossResultRequest::Nodal_Displacement));
        this->addChildItem(nodalAnimRoot, childItemData);
        // 速度
        childItemData.setText(QObject::tr("Velocity"));
        childItemData.setEnableCheckBox(true, "Nodal_Velocity_CheckBox",
            nodalAnims.contains(Radioss::FITKRadiossResultRequest::Nodal_Velocity));
        this->addChildItem(nodalAnimRoot, childItemData);
        // 加速度
        childItemData.setText(QObject::tr("Acceleration"));
        childItemData.setEnableCheckBox(true, "Nodal_Acceleration_CheckBox",
            nodalAnims.contains(Radioss::FITKRadiossResultRequest::Nodal_Acceleration));
        this->addChildItem(nodalAnimRoot, childItemData);
        // 加速度
        childItemData.setText(QObject::tr("Contact forces"));
        childItemData.setEnableCheckBox(true, "Nodal_ContactForces_CheckBox",
            nodalAnims.contains(Radioss::FITKRadiossResultRequest::Nodal_ContactForces));
        this->addChildItem(nodalAnimRoot, childItemData);
        // 刚体的力和力矩
        childItemData.setText(QObject::tr("Rigid body forces and moments"));
        childItemData.setEnableCheckBox(true, "Nodal_RigidBodyForcesMoments_CheckBox",
            nodalAnims.contains(Radioss::FITKRadiossResultRequest::Nodal_RigidBodyForcesMoments));
        this->addChildItem(nodalAnimRoot, childItemData);
        // 内部力
        childItemData.setText(QObject::tr("Internal forces"));
        childItemData.setEnableCheckBox(true, "Nodal_InternalForces_CheckBox",
            nodalAnims.contains(Radioss::FITKRadiossResultRequest::Nodal_InternalForces));
        this->addChildItem(nodalAnimRoot, childItemData);
        // 接触压力
        childItemData.setText(QObject::tr("Contact pressure"));
        childItemData.setEnableCheckBox(true, "Nodal_ContactPressure_CheckBox",
            nodalAnims.contains(Radioss::FITKRadiossResultRequest::Nodal_ContactPressure));
        this->addChildItem(nodalAnimRoot, childItemData);
    }

    void GUIRadiossResultRequestDialog::initElementalOutputAnimationsTreeNode()
    {
        if (!_resultRequest) return;
        QList<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType> elementalAnims = _resultRequest->getElementalOutputAnim();

        ChildTreeNodeData childItemData;
        QTreeWidgetItem* elementalAnimRoot = this->addTopLevelItem(QObject::tr("Elemental Output Animations"));
        // 特殊能量密度
        childItemData.setText(QObject::tr("Specific energy density"));
        childItemData.setEnableCheckBox(true, "Elemental_SpecificEnergyDensity_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_SpecificEnergyDensity));
        this->addChildItem(elementalAnimRoot, childItemData);
        // 塑性应变
        childItemData.setText(QObject::tr("Plastic strain"));
        childItemData.setEnableCheckBox(true, "Elemental_PlasticStrain_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_PlasticStrain));
        this->addChildItem(elementalAnimRoot, childItemData);
        // 沙漏能量 (发动机舱)
        childItemData.setText(QObject::tr("Hourglass energy"));
        childItemData.setEnableCheckBox(true, "Elemental_HourglassEnergy_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_HourglassEnergy));
        this->addChildItem(elementalAnimRoot, childItemData);
        // 等效应力
        childItemData.setText(QObject::tr("Von mises stress"));
        childItemData.setEnableCheckBox(true, "Elemental_VonMisesStress_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_VonMisesStress));
        this->addChildItem(elementalAnimRoot, childItemData);
        // 压力
        childItemData.setText(QObject::tr("Pressure"));
        childItemData.setEnableCheckBox(true, "Elemental_Pressure_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_Pressure));
        this->addChildItem(elementalAnimRoot, childItemData);
        // 密度
        childItemData.setText(QObject::tr("Density"));
        childItemData.setEnableCheckBox(true, "Elemental_Density_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_Density));
        this->addChildItem(elementalAnimRoot, childItemData);
        // 塑性应变 梁
        childItemData.setText(QObject::tr("Beam Plastic strain"));
        childItemData.setEnableCheckBox(true, "Elemental_BeamPlasticStrain_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_BeamPlasticStrain));
        this->addChildItem(elementalAnimRoot, childItemData);
        // 等效应力 梁
        childItemData.setText(QObject::tr("Beam VonMises stress"));
        childItemData.setEnableCheckBox(true, "Elemental_BeamVonMisesStress_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_BeamVonMisesStress));
        this->addChildItem(elementalAnimRoot, childItemData);
        // 梁的力与力矩
        childItemData.setText(QObject::tr("Beam FORC"));
        childItemData.setEnableCheckBox(true, "Elemental_BeamFORC_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_BeamFORC));
        this->addChildItem(elementalAnimRoot, childItemData);
        // Truss的力与力矩
        childItemData.setText(QObject::tr("Truss FORC"));
        childItemData.setEnableCheckBox(true, "Elemental_TrussFORC_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_TrussFORC));
        this->addChildItem(elementalAnimRoot, childItemData);
        // Spring的力与力矩
        childItemData.setText(QObject::tr("Spring FORC"));
        childItemData.setEnableCheckBox(true, "Elemental_SpringFORC_CheckBox",
            elementalAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_SpringFORC));
        this->addChildItem(elementalAnimRoot, childItemData);
    }

    void GUIRadiossResultRequestDialog::initBrickOutputAnimationsTreeNode()
    {
        if (!_resultRequest) return;
        QList<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType> brickAnims = _resultRequest->getBrickOutputAnim();

        ChildTreeNodeData childItemData;
        QTreeWidgetItem* brickAnimRoot = this->addTopLevelItem(QObject::tr("Brick Output Animations"));
        // 体应力
        childItemData.setText(QObject::tr("Brick Element stress"));
        childItemData.setEnableCheckBox(true, "Brick_BrickElementStress_CheckBox",
            brickAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_BrickElementStress));
        this->addChildItem(brickAnimRoot, childItemData);
        // 体应变
        childItemData.setText(QObject::tr("Brick Element strain"));
        childItemData.setEnableCheckBox(true, "Brick_BrickElementStrain_CheckBox",
            brickAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_BrickElementStrain));
        this->addChildItem(brickAnimRoot, childItemData);
        // 体张量
        childItemData.setText(QObject::tr("Brick Element tensor"));
        childItemData.setEnableCheckBox(true, "Brick_BrickElementTensor_CheckBox",
            brickAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_BrickElementTensor));
        this->addChildItem(brickAnimRoot, childItemData);
        // 体塑性应变
        childItemData.setText(QObject::tr("Brick Element EPSP"));
        childItemData.setEnableCheckBox(true, "Brick_BrickElementEPSP_CheckBox",
            brickAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_BrickElementEPSP));
        this->addChildItem(brickAnimRoot, childItemData);
        // 实体的损伤
        childItemData.setText(QObject::tr("Brick Damage Of All"));
        childItemData.setEnableCheckBox(true, "Brick_BrickDamageOfAll_CheckBox",
            brickAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_BrickDamageOfAll));
        this->addChildItem(brickAnimRoot, childItemData);
    }

    void GUIRadiossResultRequestDialog::initShellOutputAnimationsTreeNode()
    {
        if (!_resultRequest) return;
        QList<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType> shellAnims = _resultRequest->getShellOutputAnim();

        ChildTreeNodeData childItemData;
        QTreeWidgetItem* shellAnimRoot = this->addTopLevelItem(QObject::tr("Shell Output Animations"));
        // 壳应力
        childItemData.setText(QObject::tr("Shell Element stress"));
        childItemData.setEnableCheckBox(true, "Shell_ShellElementStress_CheckBox",
            shellAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_ShellElementStress));
        this->addChildItem(shellAnimRoot, childItemData);
        // 壳应变
        childItemData.setText(QObject::tr("Shell Element strain"));
        childItemData.setEnableCheckBox(true, "Shell_ShellElementStrain_CheckBox",
            shellAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_ShellElementStrain));
        this->addChildItem(shellAnimRoot, childItemData);
        // 壳应变率张量
        childItemData.setText(QObject::tr("Shell Element EPSDOT"));
        childItemData.setEnableCheckBox(true, "Shell_ShellElementEPSDOT_CheckBox",
            shellAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_ShellElementEPSDOT));
        this->addChildItem(shellAnimRoot, childItemData);
        // 壳的损伤
        childItemData.setText(QObject::tr("Shell Damage Of All"));
        childItemData.setEnableCheckBox(true, "Shell_ShellDamageOfAll_CheckBox",
            shellAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_ShellDamageOfAll));
        this->addChildItem(shellAnimRoot, childItemData);
        // 壳的塑性应变
        childItemData.setText(QObject::tr("Shell Plastic strain"));
        childItemData.setEnableCheckBox(true, "Shell_ShellPlasticStrain_CheckBox",
            shellAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_ShellPlasticStrain));
        this->addChildItem(shellAnimRoot, childItemData);
        // 壳的FLD损伤
        childItemData.setText(QObject::tr("Shell FLD Failure"));
        childItemData.setEnableCheckBox(true, "Shell_ShellFLDFailure_CheckBox",
            shellAnims.contains(Radioss::FITKRadiossResultRequest::Elemental_ShellFLDFailure));
        this->addChildItem(shellAnimRoot, childItemData);
    }

    void GUIRadiossResultRequestDialog::initTimeHistoryTreeNode()
    {
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKRadiossSolution* solution = caseObj->getCurrentSolution();
        if (!solution)return;
        Radioss::FITKTimeHistoryManager* mgr = solution->getTimeHietoryManager();
        if (!mgr)return;
        QList<Radioss::FITKAbstractTimeHistory*> accelList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_ACCEL);
        QList<Radioss::FITKAbstractTimeHistory*> interList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_INTER);
        QList<Radioss::FITKAbstractTimeHistory*> rbodyList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_RBODY);
        QList<Radioss::FITKAbstractTimeHistory*> rwallList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_RWALL);
        QList<Radioss::FITKAbstractTimeHistory*> sectioList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_SECTIO);
        QList<Radioss::FITKAbstractTimeHistory*> sphcelList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_SPHCEL);
        bool isEnable = false;
        ChildTreeNodeData childItemData;
        QTreeWidgetItem* timeHistoryRoot = this->addTopLevelItem(QObject::tr("Time History"));
        //ACCEL
        if (!accelList.isEmpty() && accelList.first()->isEnable() == true) 
            isEnable = true;
        else
            isEnable = false;
        childItemData.setText(QObject::tr("ACCEL"));
        childItemData.setEnableCheckBox(true,"checkBox_ACCEL", isEnable);
        childItemData.setEnablePushButton(true, "pushButton_ACCEL", "Edit Time History ACCEL");
        this->addChildItem(timeHistoryRoot, childItemData);
        //INTER
        if (!interList.isEmpty() && interList.first()->isEnable() == true)             
            isEnable = true;
        else
            isEnable = false;        
        childItemData.setText(QObject::tr("INTER"));
        childItemData.setEnableCheckBox(true, "checkBox_INTER", isEnable);
        childItemData.setEnablePushButton(true, "pushButton_INTER", "Edit Time History INTER");
        this->addChildItem(timeHistoryRoot, childItemData);
        //RWALL
        if (!rwallList.isEmpty() && rwallList.first()->isEnable() == true) 
            isEnable = true;
        else
            isEnable = false;
        childItemData.setText(QObject::tr("RWALL"));
        childItemData.setEnableCheckBox(true, "checkBox_RWALL", isEnable);
        childItemData.setEnablePushButton(true, "pushButton_RWALL", "Edit Time History RWALL");
        this->addChildItem(timeHistoryRoot, childItemData);
        //RBODY
        if (!rbodyList.isEmpty() && rbodyList.first()->isEnable() == true) 
            isEnable = true;
        else
            isEnable = false;
        childItemData.setText(QObject::tr("RBODY"));
        childItemData.setEnableCheckBox(true, "checkBox_RBODY", isEnable);
        childItemData.setEnablePushButton(true, "pushButton_RBODY", "Edit Time History RBODY");
        this->addChildItem(timeHistoryRoot, childItemData);
        //SECTIO
        if (!sectioList.isEmpty() && sectioList.first()->isEnable() == true) 
            isEnable = true;
        else
            isEnable = false;
        childItemData.setText(QObject::tr("SECTIO"));
        childItemData.setEnableCheckBox(true, "checkBox_SECTIO", isEnable);
        childItemData.setEnablePushButton(true, "pushButton_SECTIO", "Edit Time History SECTIO");
        this->addChildItem(timeHistoryRoot, childItemData);
        //SPHCEL
        if (!sphcelList.isEmpty() && sphcelList.first()->isEnable() == true) 
            isEnable = true;
        else
            isEnable = false;
        childItemData.setText(QObject::tr("SPHCEL"));
        childItemData.setEnableCheckBox(true, "checkBox_SPHCEL", isEnable);
        childItemData.setEnablePushButton(true, "pushButton_SPHCEL", "Edit Time History SPHCEL");
        this->addChildItem(timeHistoryRoot, childItemData);
    }

    void GUIRadiossResultRequestDialog::setAnimFileParam()
    {
        if (!_resultRequest) return;

        // 设置时间，频率
        _resultRequest->setStartTime(getLineEditValue("StartTime_Edit"));
        _resultRequest->setFrequency(getLineEditValue("Frequency_Edit"));
    }

    void GUIRadiossResultRequestDialog::setNodalOutputAnim()
    {
        if (!_resultRequest) return;
        // 设置节点动画输出
        QList<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType> nodalAnims;
        if (getCheckBoxState("Nodal_Displacement_CheckBox"))
            nodalAnims.append(Radioss::FITKRadiossResultRequest::Nodal_Displacement);
        if (getCheckBoxState("Nodal_Velocity_CheckBox"))
            nodalAnims.append(Radioss::FITKRadiossResultRequest::Nodal_Velocity);
        if (getCheckBoxState("Nodal_Acceleration_CheckBox"))
            nodalAnims.append(Radioss::FITKRadiossResultRequest::Nodal_Acceleration);
        if (getCheckBoxState("Nodal_ContactForces_CheckBox"))
            nodalAnims.append(Radioss::FITKRadiossResultRequest::Nodal_ContactForces);
        if (getCheckBoxState("Nodal_RigidBodyForcesMoments_CheckBox"))
            nodalAnims.append(Radioss::FITKRadiossResultRequest::Nodal_RigidBodyForcesMoments);
        if (getCheckBoxState("Nodal_InternalForces_CheckBox"))
            nodalAnims.append(Radioss::FITKRadiossResultRequest::Nodal_InternalForces);
        if (getCheckBoxState("Nodal_ContactPressure_CheckBox"))
            nodalAnims.append(Radioss::FITKRadiossResultRequest::Nodal_ContactPressure);
        _resultRequest->setNodalOutputAnim(nodalAnims);
    }

    void GUIRadiossResultRequestDialog::setElementalOutputAnim()
    {
        if (!_resultRequest) return;
        // 设置单元动画输出
        QList<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType> elementalAnims;
        if (getCheckBoxState("Elemental_SpecificEnergyDensity_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_SpecificEnergyDensity);
        if (getCheckBoxState("Elemental_PlasticStrain_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_PlasticStrain);
        if (getCheckBoxState("Elemental_HourglassEnergy_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_HourglassEnergy);
        if (getCheckBoxState("Elemental_VonMisesStress_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_VonMisesStress);
        if (getCheckBoxState("Elemental_Pressure_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_Pressure);
        if (getCheckBoxState("Elemental_Density_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_Density);
        if (getCheckBoxState("Elemental_BeamPlasticStrain_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_BeamPlasticStrain);
        if (getCheckBoxState("Elemental_BeamVonMisesStress_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_BeamVonMisesStress);
        if (getCheckBoxState("Elemental_BeamFORC_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_BeamFORC);
        if (getCheckBoxState("Elemental_TrussFORC_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_TrussFORC);
        if (getCheckBoxState("Elemental_SpringFORC_CheckBox"))
            elementalAnims.append(Radioss::FITKRadiossResultRequest::Elemental_SpringFORC);
        _resultRequest->setElementalOutputAnim(elementalAnims);
    }

    void GUIRadiossResultRequestDialog::setBrickOutputAnim()
    {
        if (!_resultRequest) return;
        // 设置体单元动画输出
        QList<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType> brickAnims;
        if (getCheckBoxState("Brick_BrickElementStress_CheckBox"))
            brickAnims.append(Radioss::FITKRadiossResultRequest::Elemental_BrickElementStress);
        if (getCheckBoxState("Brick_BrickElementStrain_CheckBox"))
            brickAnims.append(Radioss::FITKRadiossResultRequest::Elemental_BrickElementStrain);
        if (getCheckBoxState("Brick_BrickElementTensor_CheckBox"))
            brickAnims.append(Radioss::FITKRadiossResultRequest::Elemental_BrickElementTensor);
        if (getCheckBoxState("Brick_BrickElementEPSP_CheckBox"))
            brickAnims.append(Radioss::FITKRadiossResultRequest::Elemental_BrickElementEPSP);
        if (getCheckBoxState("Brick_BrickDamageOfAll_CheckBox"))
            brickAnims.append(Radioss::FITKRadiossResultRequest::Elemental_BrickDamageOfAll);
        _resultRequest->setBrickOutputAnim(brickAnims);
    }

    void GUIRadiossResultRequestDialog::setShellOutputAnim()
    {
        if (!_resultRequest) return;
        // 设置节点动画输出
        QList<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType> shellAnims;
        if (getCheckBoxState("Shell_ShellElementStress_CheckBox"))
            shellAnims.append(Radioss::FITKRadiossResultRequest::Elemental_ShellElementStress);
        if (getCheckBoxState("Shell_ShellElementStrain_CheckBox"))
            shellAnims.append(Radioss::FITKRadiossResultRequest::Elemental_ShellElementStrain);
        if (getCheckBoxState("Shell_ShellElementEPSDOT_CheckBox"))
            shellAnims.append(Radioss::FITKRadiossResultRequest::Elemental_ShellElementEPSDOT);
        if (getCheckBoxState("Shell_ShellDamageOfAll_CheckBox"))
            shellAnims.append(Radioss::FITKRadiossResultRequest::Elemental_ShellDamageOfAll);
        if (getCheckBoxState("Shell_ShellPlasticStrain_CheckBox"))
            shellAnims.append(Radioss::FITKRadiossResultRequest::Elemental_ShellPlasticStrain);
        if (getCheckBoxState("Shell_ShellFLDFailure_CheckBox"))
            shellAnims.append(Radioss::FITKRadiossResultRequest::Elemental_ShellFLDFailure);
        _resultRequest->setShellOutputAnim(shellAnims);
    }

    void GUIRadiossResultRequestDialog::setTimeHistory()
    {
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKRadiossSolution* solution = caseObj->getCurrentSolution();
        if (!solution)return;
        Radioss::FITKTimeHistoryManager* mgr = solution->getTimeHietoryManager();
        if (!mgr)return;
        QList<Radioss::FITKAbstractTimeHistory*> accelList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_ACCEL);
        QList<Radioss::FITKAbstractTimeHistory*> interList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_INTER);
        QList<Radioss::FITKAbstractTimeHistory*> rbodyList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_RBODY);
        QList<Radioss::FITKAbstractTimeHistory*> rwallList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_RWALL);
        QList<Radioss::FITKAbstractTimeHistory*> sectioList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_SECTIO);
        QList<Radioss::FITKAbstractTimeHistory*> sphcelList = mgr->getTimeHistoryByType(Radioss::FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_SPHCEL);
        if (!accelList.isEmpty())
            accelList.first()->enable(getCheckBoxState("checkBox_ACCEL"));
        if (!interList.isEmpty())
            interList.first()->enable(getCheckBoxState("checkBox_INTER"));
        if (!rbodyList.isEmpty())
            rbodyList.first()->enable(getCheckBoxState("checkBox_RBODY"));
        if (!rwallList.isEmpty())
            rwallList.first()->enable(getCheckBoxState("checkBox_RWALL"));
        if (!sectioList.isEmpty())
            sectioList.first()->enable(getCheckBoxState("checkBox_SECTIO"));
        if (!sphcelList.isEmpty())
            sphcelList.first()->enable(getCheckBoxState("checkBox_SPHCEL"));
    }

    QTreeWidgetItem * GUIRadiossResultRequestDialog::addTopLevelItem(const QString & text)
    {
        if (text.isEmpty()) return nullptr;
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, text);
        _ui->treeWidget->addTopLevelItem(item);
        return item;
    }
    QTreeWidgetItem * GUIRadiossResultRequestDialog::addChildItem(QTreeWidgetItem * p, const ChildTreeNodeData & childItemData)
    {
        if (!p || childItemData._text.isEmpty()) return nullptr;
        //创建子节点数据
        QTreeWidgetItem* item = new QTreeWidgetItem(p);
        item->setText(0, childItemData._text);
        //创建窗口
        QWidget* widget = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout();
        widget->setLayout(layout);
        //创建交互逻辑
        if (childItemData._isEnableCheckBox && !childItemData._isEnableLineEdit && !childItemData._isEnablePushButton)
        {
            QCheckBox* checkBox = new QCheckBox(this);
            checkBox->setObjectName(childItemData._checkBox_objectName);
            checkBox->setCheckState((childItemData._checkBox_Data ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
            QLabel* qlabel = new QLabel(this);
            qlabel->setText((childItemData._checkBox_Data ? "True" : "False"));
            layout->addWidget(checkBox);
            layout->addWidget(qlabel);
            layout->setStretch(1, 1);
            connect(checkBox, &QCheckBox::stateChanged, [=](int state) {
                if (state == Qt::CheckState::Checked)
                    qlabel->setText("True");
                else
                    qlabel->setText("False");
            });
        }
        else if (!childItemData._isEnableCheckBox && childItemData._isEnableLineEdit)
        {
            QLineEdit* lineEdit = new QLineEdit(this);
            lineEdit->setObjectName(childItemData._lineEdit_objectName);
            lineEdit->setText(childItemData._lineEdit_Data);
            QLabel* qlabel = new QLabel(this);
            qlabel->setText(childItemData._unit);
            layout->addWidget(lineEdit);
            layout->addWidget(qlabel);
            layout->setStretch(1, 1);
        }
        else if (childItemData._isEnableCheckBox && childItemData._isEnableLineEdit)
        {
            QCheckBox* checkBox = new QCheckBox(this);
            checkBox->setObjectName(childItemData._checkBox_objectName);
            checkBox->setCheckState((childItemData._checkBox_Data ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
            QLineEdit* lineEdit = new QLineEdit(this);
            lineEdit->setObjectName(childItemData._lineEdit_objectName);
            lineEdit->setText(childItemData._lineEdit_Data);
            QLabel* qlabel = new QLabel(this);
            qlabel->setText(childItemData._unit);
            lineEdit->setEnabled((childItemData._checkBox_Data ? true : false));
            layout->addWidget(checkBox);
            layout->addWidget(lineEdit);
            layout->addWidget(qlabel);
            layout->setStretch(2, 1);

            connect(checkBox, &QCheckBox::stateChanged, [=](int state) {
                if (state == Qt::CheckState::Checked)
                    lineEdit->setEnabled(true);
                else
                    lineEdit->setEnabled(false);
            });
        }
        else if (childItemData._isEnableComboBox && childItemData._comboBox_ItemDatas.size() == childItemData._comboBox_items.size())
        {
            QComboBox* comboBox = new QComboBox(this);
            comboBox->setObjectName(childItemData._comboBox_objectName);
            for (int i = 0; i < childItemData._comboBox_ItemDatas.size(); ++i)
            {
                comboBox->addItem(childItemData._comboBox_items[i], childItemData._comboBox_ItemDatas[i]);
            }
            layout->addWidget(comboBox);
            int index = comboBox->findData(childItemData._comboBox_Data);
            if(index < 0)
                comboBox->setCurrentIndex(0);
            else
                comboBox->setCurrentIndex(index);
        }
        else if (childItemData._isEnableCheckBox && !childItemData._isEnableLineEdit && childItemData._isEnablePushButton)
        {
            QCheckBox* checkBox = new QCheckBox(this);
            checkBox->setObjectName(childItemData._checkBox_objectName);
            checkBox->setCheckState(childItemData._checkBox_Data ? Qt::Checked : Qt::Unchecked);

            QPushButton* pushButton = new QPushButton(this);
            pushButton->setObjectName(childItemData._pushButton_objectName);
            pushButton->setText(childItemData._pushButton_Text);
            pushButton->setEnabled(childItemData._checkBox_Data);

            layout->addWidget(checkBox);
            layout->addWidget(pushButton);
            layout->setStretch(1, 1);

            connect(checkBox, &QCheckBox::stateChanged, [=](int state) {
                pushButton->setEnabled(state == Qt::Checked);
            });
            // 手动连接按钮点击信号
            if (pushButton->objectName() == "pushButton_ACCEL")
                connect(pushButton, &QPushButton::clicked, this, &GUIRadiossResultRequestDialog::pushButton_ACCEL_clicked);
            else if (pushButton->objectName() == "pushButton_INTER")
                connect(pushButton, &QPushButton::clicked, this, &GUIRadiossResultRequestDialog::pushButton_INTER_clicked);
            else if (pushButton->objectName() == "pushButton_RWALL")
                connect(pushButton, &QPushButton::clicked, this, &GUIRadiossResultRequestDialog::pushButton_RWALL_clicked);
            else if (pushButton->objectName() == "pushButton_RBODY")
                connect(pushButton, &QPushButton::clicked, this, &GUIRadiossResultRequestDialog::pushButton_RBODY_clicked);
            else if (pushButton->objectName() == "pushButton_SECTIO")
                connect(pushButton, &QPushButton::clicked, this, &GUIRadiossResultRequestDialog::pushButton_SECTIO_clicked);
            else if (pushButton->objectName() == "pushButton_SPHCEL")
                connect(pushButton, &QPushButton::clicked, this, &GUIRadiossResultRequestDialog::pushButton_SPHCEL_clicked);
        
        }
        _ui->treeWidget->setItemWidget(item, 1, widget);
        return item;
    }

    bool GUIRadiossResultRequestDialog::getCheckBoxState(const QString& objectName)
    {
        QCheckBox* checkBox = findChild<QCheckBox*>(objectName);
        return checkBox ? checkBox->isChecked() : false;
    }

    double GUIRadiossResultRequestDialog::getLineEditValue(const QString& objectName)
    {
        QLineEdit* lineEdit = findChild<QLineEdit*>(objectName);
        return lineEdit ? lineEdit->text().toDouble() : 0.0;
    }

    void GUIRadiossResultRequestDialog::writePythonScript()
    {
        if (!_resultRequest) return;

        QStringList script;
        script.append("resultRequest=ResultRequest()");

        script.append(QString("resultRequest.setStartTime(%1)").arg(_resultRequest->getStartTime()));
        script.append(QString("resultRequest.setFrequency(%1)").arg(_resultRequest->getFrequency()));
        script.append(QString("resultRequest.setHisFrequency(%1)").arg(_resultRequest->getHisFrequency()));

        QList<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType> nodalAnims = _resultRequest->getNodalOutputAnim();
        if (!nodalAnims.isEmpty())
        {
            QStringList nodalList;
            for (auto type : nodalAnims)
            {
                nodalList.append(QString::number((int)type));
            }
            script.append(QString("resultRequest.setNodalOutputAnim([%1])").arg(nodalList.join(", ")));
        }

        QList<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType> elementalAnims = _resultRequest->getElementalOutputAnim();
        if (!elementalAnims.isEmpty())
        {
            QStringList elementalList;
            for (auto type : elementalAnims)
            {
                elementalList.append(QString::number((int)type));
            }
            script.append(QString("resultRequest.setElementalOutputAnim([%1])").arg(elementalList.join(", ")));
        }

        QList<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType> brickAnims = _resultRequest->getBrickOutputAnim();
        if (!brickAnims.isEmpty())
        {
            QStringList brickList;
            for (auto type : brickAnims)
            {
                brickList.append(QString::number((int)type));
            }
            script.append(QString("resultRequest.setBrickOutputAnim([%1])").arg(brickList.join(", ")));
        }

        QList<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType> shellAnims = _resultRequest->getShellOutputAnim();
        if (!shellAnims.isEmpty())
        {
            QStringList shellList;
            for (auto type : shellAnims)
            {
                shellList.append(QString::number((int)type));
            }
            script.append(QString("resultRequest.setShellOutputAnim([%1])").arg(shellList.join(", ")));
        }

        this->saveScript(script);
    }
}


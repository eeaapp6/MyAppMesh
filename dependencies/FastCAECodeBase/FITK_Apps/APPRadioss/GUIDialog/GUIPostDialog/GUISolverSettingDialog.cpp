/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUISolverSettingDialog.h"
#include "ui_GUISolverSettingDialog.h"

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>

namespace GUI
{
    GUISolverSettingDialog::GUISolverSettingDialog(Core::FITKActionOperator* oper, QWidget *parent) :
        GUIDialogBase(nullptr, parent),
        _oper(oper),
        _ui(new Ui::GUISolverSettingDialog)
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
            _solverSettings = caseData->getSolverSettings();
        //初始化
        this->init();
        //设置窗口自适应隐藏组件
        this->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);
    }

    GUISolverSettingDialog::~GUISolverSettingDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void GUISolverSettingDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUISolverSettingDialog::on_pushButton_OK_clicked()
    {
        this->setRunExeTimeParam();
        this->setNodalTimeStepControlParam();
        this->setElementalTimeStepControlParam();
        this->setGlobalTimeParam();
        this->setAdaptiveDynamicRelaxation();
        this->setAMSElementalTimeStepControlParam();
        this->setComputationControlParam();
        this->setConvergenceCriteriaParam();
        this->setSPHGlobalParam();

        this->writePythonScript();
        this->accept();
    }
    void GUISolverSettingDialog::init()
    {
        if (!_solverSettings) return;
        this->initRunExeTimeParamTreeNode();
        this->initNodalTimeStepControlParamTreeNode();
        this->initElementalTimeStepControlParamTreeNode();
        this->initGlobalTimeParamTreeNode();
        this->initAdaptiveDynamicRelaxationTreeNode();
        this->initAMSElementalTimeStepControlParamTreeNode();
        this->initComputationControlParamTreeNode();
        this->initConvergenceCriteriaParamTreeNode();
        this->initSPHGlobalParamTreeNode();

        _ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        _ui->treeWidget->expandAll();
    }
    void GUISolverSettingDialog::initRunExeTimeParamTreeNode()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverRunExeTimeParam* runExeTimeParam = _solverSettings->getRunExeTimeParam();
        if (!runExeTimeParam) return;
        ChildTreeNodeData childItemData;
        QTreeWidgetItem* runExeTimeParamRoot = this->addTopLevelItem(QObject::tr("Run Execution Time Parameters"));
        childItemData.setText(QObject::tr("Final time"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "Final_time", QString::number(runExeTimeParam->getFinalTime()), "s");
        this->addChildItem(runExeTimeParamRoot, childItemData);
    }
    void GUISolverSettingDialog::initGlobalTimeParamTreeNode()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverGlobalTimeParam* globalTimeParam = _solverSettings->getGlobalTimeParam();
        if (!globalTimeParam) return;

        ChildTreeNodeData childItemData;
        QTreeWidgetItem* globalTimeParamRoot = this->addTopLevelItem(QObject::tr("Global Time Step Parameters"));
        childItemData.setText(QObject::tr("Scale Factor On Critical Time Stop"));
        childItemData.setEnableLineEdit(true, "Scale_factor_Data", QString::number(globalTimeParam->getScaleFactor()));
        this->addChildItem(globalTimeParamRoot, childItemData);

        childItemData.setText(QObject::tr("Tolerance For GTS Convergence"));
        childItemData.setEnableCheckBox(true, "EnableAdditionalCards_CheckBox", globalTimeParam->isEnableAdditionalCards());
        childItemData.setEnableLineEdit(true, "EnableAdditionalCards_Edit", QString::number(globalTimeParam->getToleranceGTS()));
        this->addChildItem(globalTimeParamRoot, childItemData);

        childItemData.setText(QObject::tr("Number Of Cycles For Global Time Step Estimation"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "NumCycle_Edit", QString::number(globalTimeParam->getNumCycles()));
        this->addChildItem(globalTimeParamRoot, childItemData);

    }
    void GUISolverSettingDialog::initNodalTimeStepControlParamTreeNode()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverNodalTimeStepControlParam * nodalParam =  _solverSettings->getNodalTimeStepControlParam();
        if (!nodalParam) return;

        ChildTreeNodeData childItemData;
        QTreeWidgetItem* nodalTimeStepRoot = this->addTopLevelItem(QObject::tr("Nodal Time Step Control Parameters"));

        childItemData.setText(QObject::tr("Time step control type"));
        childItemData.setEnableComboBox(true, "Nodal_ControlType_ComboBox",
            QStringList() << QObject::tr("Solver Default" )<< QObject::tr("CST") << QObject::tr("SET") << QObject::tr("STOP"),
            QList<int>()<< Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStep_SolverDefault << Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStep_CST << Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStep_SET << Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStep_STOP ,
            nodalParam->getTimeStepControlType());
        this->addChildItem(nodalTimeStepRoot, childItemData);

        childItemData.setText(QObject::tr("Minimum time step"));
        childItemData.setEnableLineEdit(true, "Nodal_MinTimeStep_Edit", QString::number(nodalParam->getMinimumTimeStep()), "s");
        this->addChildItem(nodalTimeStepRoot, childItemData);
        childItemData.setText(QObject::tr("Scale factor"));
        childItemData.setEnableLineEdit(true, "Nodal_ScaleFactor_Edit", QString::number(nodalParam->getScaleFactor()), "");
        this->addChildItem(nodalTimeStepRoot, childItemData);
        childItemData.setText(QObject::tr("Initial added mass ratio"));
        childItemData.setEnableLineEdit(true, "Nodal_InitAddMassRatio_Edit", QString::number(nodalParam->getInitialAddMassRatio()), "");
        this->addChildItem(nodalTimeStepRoot, childItemData);
    }
    void GUISolverSettingDialog::initElementalTimeStepControlParamTreeNode()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverElementalTimeStepControlParam* elementalParam = _solverSettings->getElementalTimeStepControlParam();
        if (!elementalParam) return;
        ChildTreeNodeData childItemData;
        QTreeWidgetItem* elementalTimeStepRoot = this->addTopLevelItem(QObject::tr("Elemental Time Step Control Parameters"));

        childItemData.setText(QObject::tr("Time step control type"));
        childItemData.setEnableComboBox(true, "Elemental_ControlType_ComboBox",
            QStringList() << QObject::tr("Solver Default") << QObject::tr("CST") << QObject::tr("DEL") << QObject::tr("STOP"),
            QList<int>() << Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStep_SolverDefault << Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStep_CST << Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStep_DEL << Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStep_STOP ,
            elementalParam->getTimeStepControlType());
        this->addChildItem(elementalTimeStepRoot, childItemData);

        childItemData.setText(QObject::tr("Minimum time step"));
        childItemData.setEnableLineEdit(true, "Elemental_MinTimeStep_Edit", QString::number(elementalParam->getMinimumTimeStep()), "s");
        this->addChildItem(elementalTimeStepRoot, childItemData);
        childItemData.setText(QObject::tr("Scale factor"));
        childItemData.setEnableLineEdit(true, "Elemental_ScaleFactor_Edit", QString::number(elementalParam->getScaleFactor()), "");
        this->addChildItem(elementalTimeStepRoot, childItemData);
        childItemData.setText(QObject::tr("Number of additional cards"));
        childItemData.setEnableLineEdit(false);
        childItemData.setEnableCheckBox(true, "Elemental_AddCards_CheckBox", elementalParam->getNumberOfAddCards());
        this->addChildItem(elementalTimeStepRoot, childItemData);
        childItemData.setText(QObject::tr("Minimum aspect ratio"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "Elemental_MinAspectRatio_Edit", QString::number(elementalParam->getMinAspectRatio()), "");
        this->addChildItem(elementalTimeStepRoot, childItemData);
        childItemData.setText(QObject::tr("Minimum volume ratio"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "Elemental_MinVolumeRatio_Edit", QString::number(elementalParam->getMinVolumeRatio()), "");
        this->addChildItem(elementalTimeStepRoot, childItemData);
    }
    void GUISolverSettingDialog::initAdaptiveDynamicRelaxationTreeNode()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverAdaptiveDynamicRelaxation* adrParam = _solverSettings->getAdaptiveDynamicRelaxation();
        if (!adrParam) return;
        ChildTreeNodeData childItemData;
        QTreeWidgetItem* adrRoot = this->addTopLevelItem(QObject::tr("Adaptive Dynamic Relaxation"));
        childItemData.setText(QObject::tr("Enable dynamic relaxation"));
        childItemData.setEnableCheckBox(true, "ADR_Enable_CheckBox", adrParam->getEnableDynamicRelaxation());
        this->addChildItem(adrRoot, childItemData);
        childItemData.setText(QObject::tr("Start Time"));
        childItemData.setEnableCheckBox(true, "ADR_EnableStartTime_CheckBox", adrParam->isEnableStartTime());
        childItemData.setEnableLineEdit(true, "ADR_StartTime_Edit", QString::number(adrParam->getStartTime()), "s");
        this->addChildItem(adrRoot, childItemData);
        childItemData.setText(QObject::tr("Stop Time"));
        childItemData.setEnableCheckBox(true, "ADR_EnableStopTime_CheckBox", adrParam->isEnableStopTime());
        childItemData.setEnableLineEdit(true, "ADR_StopTime_Edit", QString::number(adrParam->getStopTime()), "s");
        this->addChildItem(adrRoot, childItemData);
    }
    void GUISolverSettingDialog::initAMSElementalTimeStepControlParamTreeNode()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* amsParam = _solverSettings->getAMSElementalTimeStepControlParam();
        if (!amsParam) return;
        ChildTreeNodeData childItemData;
        QTreeWidgetItem* amsRoot = this->addTopLevelItem(QObject::tr("AMS Elemental Time Step Control Parameters"));
        // Critical time step scale factor
        childItemData.setText(QObject::tr("Critical time step scale factor"));
        childItemData.setEnableCheckBox(true, "AMS_CriticalScale_CheckBox", amsParam->isEnableCriticalTimeStepScaleFactor());
        childItemData.setEnableLineEdit(true, "AMS_CriticalScale_Edit", QString::number(amsParam->getCriticalTimeStepScaleFactor()), "");
        this->addChildItem(amsRoot, childItemData);
        // Minimum time step
        childItemData.setText(QObject::tr("Minimum time step"));
        childItemData.setEnableCheckBox(true, "AMS_MinTimeStep_CheckBox", amsParam->isEnableMinTimeStep());
        childItemData.setEnableLineEdit(true, "AMS_MinTimeStep_Edit", QString::number(amsParam->getMinTimeStep()), "s");
        this->addChildItem(amsRoot, childItemData);
        // AMS convergence tolerance
        childItemData.setText(QObject::tr("AMS convergence tolerance"));
        childItemData.setEnableCheckBox(true, "AMS_ConvergenceTol_CheckBox", amsParam->isEnableAMSConvergenceTolerance());
        childItemData.setEnableLineEdit(true, "AMS_ConvergenceTol_Edit", QString::number(amsParam->getAMSConvergenceTolerance()), "");
        this->addChildItem(amsRoot, childItemData);
        // Max iterations conjugate gradient
        childItemData.setText(QObject::tr("Maximum iterations conjugate gradient"));
        childItemData.setEnableCheckBox(true, "AMS_MaxIterations_CheckBox", amsParam->isEnableMaxIterationsConjugateGradient());
        childItemData.setEnableLineEdit(true, "AMS_MaxIterations_Edit", QString::number(amsParam->getMaxIterationsConjugateGradient()), "");
        this->addChildItem(amsRoot, childItemData);
        // Number of additional outputs
        childItemData.setText(QObject::tr("Number of additional outputs"));
        childItemData.setEnableCheckBox(true, "AMS_AdditionalOutputs_CheckBox", amsParam->isEnableNumberOfAdditionalOutputs());
        childItemData.setEnableLineEdit(true, "AMS_AdditionalOutputs_Edit", QString::number(amsParam->getNumberOfAdditionalOutputs()), "");
        this->addChildItem(amsRoot, childItemData);
    }
    void GUISolverSettingDialog::initComputationControlParamTreeNode()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverComputationControlParam* controlParam = _solverSettings->getComputationControlParam();
        if(!controlParam)return;
        ChildTreeNodeData childItemData;
        QTreeWidgetItem* computationControlParamRoot = this->addTopLevelItem(QObject::tr("Computation control parameters"));
        childItemData.setText(QObject::tr("Stop computation when negative volume"));
        childItemData.setEnableLineEdit(false);
        childItemData.setEnableCheckBox(true, "Stop_c_negative_Volume", controlParam->getStopComputationWhenNegativeVolume());
        this->addChildItem(computationControlParamRoot, childItemData);
        childItemData.setText(QObject::tr("Parallel arithmetic"));
        childItemData.setEnableLineEdit(false);
        childItemData.setEnableCheckBox(true, "Parallel_arithmetic", controlParam->getParallelArithmetic());
        this->addChildItem(computationControlParamRoot, childItemData);
    }
    void GUISolverSettingDialog::initConvergenceCriteriaParamTreeNode()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverConvergenceCriteriaParam* convergenceParam = _solverSettings->getConvergenceCriteriaParam();
        if (!convergenceParam) return;
        ChildTreeNodeData childItemData;
        QTreeWidgetItem* convergenceRoot = this->addTopLevelItem(QObject::tr("Convergence Criteria Parameters"));
        // Stop computation
        childItemData.setText(QObject::tr("Enable State"));
        childItemData.setEnableCheckBox(true, "Convergence_EnableState_CheckBox", convergenceParam->getEnableState());
        this->addChildItem(convergenceRoot, childItemData);
        // Energy error ratio
        childItemData.setText(QObject::tr("Energy error ratio"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "Convergence_EnergyErrorRatio_Edit", QString::number(convergenceParam->getEnergyErrorRatio()), "");
        this->addChildItem(convergenceRoot, childItemData);
        // Total mass ratio
        childItemData.setText(QObject::tr("Total mass ratio"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "Convergence_TotalMassRatio_Edit", QString::number(convergenceParam->getTotalMassRatio()), "");
        this->addChildItem(convergenceRoot, childItemData);
        // Nodal mass ratio
        childItemData.setText(QObject::tr("Nodal mass ratio"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "Convergence_NodalMassRatio_Edit", QString::number(convergenceParam->getNodalMassRatio()), "");
        this->addChildItem(convergenceRoot, childItemData);
        // Write time history file
        childItemData.setText(QObject::tr("Write time history file"));
        childItemData.setEnableLineEdit(false);
        childItemData.setEnableCheckBox(true, "Convergence_WriteTimeHistory_CheckBox", convergenceParam->getWriteTimeHistoryFile());
        this->addChildItem(convergenceRoot, childItemData);
        // Write animation file
        childItemData.setText(QObject::tr("Write animation file"));
        childItemData.setEnableLineEdit(false);
        childItemData.setEnableCheckBox(true, "Convergence_WriteAnimation_CheckBox", convergenceParam->getWriteAnimationFile());
        this->addChildItem(convergenceRoot, childItemData);
        // Energy error
        childItemData.setText(QObject::tr("Energy error"));
        childItemData.setEnableLineEdit(false);
        childItemData.setEnableCheckBox(true, "Convergence_EnergyError_CheckBox", convergenceParam->getEnergyError());
        this->addChildItem(convergenceRoot, childItemData);
    }

    void GUISolverSettingDialog::initSPHGlobalParamTreeNode()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverSPHGlobal* globalSPHParam = _solverSettings->getGlobalSPHParam();
        if (!globalSPHParam) return;
        ChildTreeNodeData childItemData;
        QTreeWidgetItem* globalSPHRoot = this->addTopLevelItem(QObject::tr("Global SPH Parameters"));
        // Enable State
        childItemData.setText(QObject::tr("Enable State"));
        childItemData.setEnableCheckBox(true, "GlobalSPH_EnableState_CheckBox", globalSPHParam->isEnable());
        this->addChildItem(globalSPHRoot, childItemData);
        // search
        childItemData.setText(QObject::tr("Security coefficient on search for neighbors"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "GlobalSPH_search_LineEdit", QString::number(globalSPHParam->getAsort()));
        this->addChildItem(globalSPHRoot, childItemData);
        // Maxsph
        childItemData.setText(QObject::tr("Maximum number of ghost particles"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "GlobalSPH_Maxsph_LineEdit", QString::number(globalSPHParam->getMaxsph()));
        this->addChildItem(globalSPHRoot, childItemData);
        // Lneigh
        childItemData.setText(QObject::tr("Maximum number of neighbors for the SPH approximation"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "GlobalSPH_Lneigh_LineEdit", QString::number(globalSPHParam->getLneigh()));
        this->addChildItem(globalSPHRoot, childItemData);
        // Nneigh
        childItemData.setText(QObject::tr("Maximum number of neighbors"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "GlobalSPH_Nneigh_LineEdit", QString::number(globalSPHParam->getNneigh()));
        this->addChildItem(globalSPHRoot, childItemData);
        // Isol2sph
        childItemData.setText(QObject::tr("Particle activation process method for Sol2SPH"));
        childItemData.setEnableCheckBox(false);
        childItemData.setEnableLineEdit(true, "GlobalSPH_Isol2sph_LineEdit", QString::number(globalSPHParam->getIsol2sph()));
        this->addChildItem(globalSPHRoot, childItemData);
    }



    QTreeWidgetItem * GUISolverSettingDialog::addTopLevelItem(const QString & text)
    {
        if (text.isEmpty()) return nullptr;
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, text);
        _ui->treeWidget->addTopLevelItem(item);
        return item;
    }
    QTreeWidgetItem * GUISolverSettingDialog::addChildItem(QTreeWidgetItem * p, const ChildTreeNodeData & childItemData)
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
        if (childItemData._isEnableCheckBox && !childItemData._isEnableLineEdit)
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
        _ui->treeWidget->setItemWidget(item, 1, widget);
        return item;
    }

    bool GUISolverSettingDialog::getCheckBoxState(const QString& objectName)
    {
        QCheckBox* checkBox = findChild<QCheckBox*>(objectName);
        return checkBox ? checkBox->isChecked() : false;
    }

    double GUISolverSettingDialog::getLineEditValue(const QString& objectName)
    {
        QLineEdit* lineEdit = findChild<QLineEdit*>(objectName);
        return lineEdit ? lineEdit->text().toDouble() : 0.0;
    }

    int GUISolverSettingDialog::getTimeStepControlType(const QString& objectName)
    {
        QComboBox* comboBox = findChild<QComboBox*>(objectName);
        if (!comboBox) return 0;

        int data = comboBox->currentData().toInt();
        return data;
    }

    void GUISolverSettingDialog::setRunExeTimeParam()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverRunExeTimeParam* runExeTimeParam = _solverSettings->getRunExeTimeParam();
        if (!runExeTimeParam) return;
        runExeTimeParam->setFinalTime(getLineEditValue("Final_time"));
    }
    void GUISolverSettingDialog::setGlobalTimeParam()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverGlobalTimeParam* globalTimeParam = _solverSettings->getGlobalTimeParam();
        if (!globalTimeParam) return;
        globalTimeParam->setEnableAdditionalCards(getCheckBoxState("EnableAdditionalCards_CheckBox"));
        globalTimeParam->setToleranceGTS(getLineEditValue("EnableAdditionalCards_Edit"));
        globalTimeParam->setScaleFactor(getLineEditValue("Scale_factor_Data"));
        globalTimeParam->setNumCycles(getLineEditValue("NumCycle_Edit"));

    }
    void GUISolverSettingDialog::setNodalTimeStepControlParam()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverNodalTimeStepControlParam * nodalParam = _solverSettings->getNodalTimeStepControlParam();
        if (!nodalParam) return;
        nodalParam->setTimeStepControlType(Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType(getTimeStepControlType("Nodal_ControlType_ComboBox")));
        nodalParam->setMinimumTimeStep(getLineEditValue("Nodal_MinTimeStep_Edit"));
        nodalParam->setScaleFactor(getLineEditValue("Nodal_ScaleFactor_Edit"));
        nodalParam->setInitialAddMassRatio(getLineEditValue("Nodal_InitAddMassRatio_Edit"));
    }
    void GUISolverSettingDialog::setElementalTimeStepControlParam()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverElementalTimeStepControlParam* elementalParam = _solverSettings->getElementalTimeStepControlParam();
        if (!elementalParam) return;
        elementalParam->setTimeStepControlType(Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType(getTimeStepControlType("Elemental_ControlType_ComboBox")));
        elementalParam->setMinimumTimeStep(getLineEditValue("Elemental_MinTimeStep_Edit"));
        elementalParam->setScaleFactor(getLineEditValue("Elemental_ScaleFactor_Edit"));
        elementalParam->setNumberOfAddCards(getCheckBoxState("Elemental_AddCards_CheckBox"));
        elementalParam->setMinAspectRatio(getLineEditValue("Elemental_MinAspectRatio_Edit"));
        elementalParam->setMinVolumeRatio(getLineEditValue("Elemental_MinVolumeRatio_Edit"));
    }
    void GUISolverSettingDialog::setAdaptiveDynamicRelaxation()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverAdaptiveDynamicRelaxation* adrParam = _solverSettings->getAdaptiveDynamicRelaxation();
        if (!adrParam) return;
        adrParam->setEnableDynamicRelaxation(getCheckBoxState("ADR_Enable_CheckBox"));
        adrParam->setEnableStartTime(getCheckBoxState("ADR_EnableStartTime_CheckBox"));
        adrParam->setStartTime(getLineEditValue("ADR_StartTime_Edit"));
        adrParam->setEnableStopTime(getCheckBoxState("ADR_EnableStopTime_CheckBox"));
        adrParam->setStopTime(getLineEditValue("ADR_StopTime_Edit"));
    }
    void GUISolverSettingDialog::setAMSElementalTimeStepControlParam()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* amsParam = _solverSettings->getAMSElementalTimeStepControlParam();
        if (!amsParam) return;
        amsParam->setEnableCriticalTimeStepScaleFactor(getCheckBoxState("AMS_CriticalScale_CheckBox"));
        amsParam->setCriticalTimeStepScaleFactor(getLineEditValue("AMS_CriticalScale_Edit"));
        amsParam->setEnableMinTimeStep(getCheckBoxState("AMS_MinTimeStep_CheckBox"));
        amsParam->setMinTimeStep(getLineEditValue("AMS_MinTimeStep_Edit"));
        amsParam->setEnableAMSConvergenceTolerance(getCheckBoxState("AMS_ConvergenceTol_CheckBox"));
        amsParam->setAMSConvergenceTolerance(getLineEditValue("AMS_ConvergenceTol_Edit"));
        amsParam->setEnableMaxIterationsConjugateGradient(getCheckBoxState("AMS_MaxIterations_CheckBox"));
        amsParam->setMaxIterationsConjugateGradient(getLineEditValue("AMS_MaxIterations_Edit"));
        amsParam->setEnableNumberOfAdditionalOutputs(getCheckBoxState("AMS_AdditionalOutputs_CheckBox"));
        amsParam->setNumberOfAdditionalOutputs(getLineEditValue("AMS_AdditionalOutputs_Edit"));
    }
    void GUISolverSettingDialog::setComputationControlParam()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverComputationControlParam* controlParam = _solverSettings->getComputationControlParam();
        if (!controlParam)return;
        controlParam->setStopComputationWhenNegativeVolume(getCheckBoxState("Stop_c_negative_Volume"));
        controlParam->setParallelArithmetic(getCheckBoxState("Parallel_arithmetic"));
    }
    void GUISolverSettingDialog::setConvergenceCriteriaParam()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverConvergenceCriteriaParam* convergenceParam = _solverSettings->getConvergenceCriteriaParam();
        if (!convergenceParam) return;
        convergenceParam->setEnableState(getCheckBoxState("Convergence_EnableState_CheckBox"));
        convergenceParam->setEnergyErrorRatio(getLineEditValue("Convergence_EnergyErrorRatio_Edit"));
        convergenceParam->setTotalMassRatio(getLineEditValue("Convergence_TotalMassRatio_Edit"));
        convergenceParam->setNodalMassRatio(getLineEditValue("Convergence_NodalMassRatio_Edit"));
        convergenceParam->setWriteTimeHistoryFile(getCheckBoxState("Convergence_WriteTimeHistory_CheckBox"));
        convergenceParam->setWriteAnimationFile(getCheckBoxState("Convergence_WriteAnimation_CheckBox"));
        convergenceParam->setEnergyError(getCheckBoxState("Convergence_EnergyError_CheckBox"));
    }

    void GUISolverSettingDialog::setSPHGlobalParam()
    {
        if (!_solverSettings) return;
        Radioss::FITKRadiossSolverSPHGlobal* globalSPHParam = _solverSettings->getGlobalSPHParam();
        if (!globalSPHParam) return;
        globalSPHParam->enable(getCheckBoxState("GlobalSPH_EnableState_CheckBox"));
        globalSPHParam->setAsort(getLineEditValue("GlobalSPH_search_LineEdit"));
        globalSPHParam->setMaxsph(getLineEditValue("GlobalSPH_Maxsph_LineEdit"));
        globalSPHParam->setLneigh(getLineEditValue("GlobalSPH_Lneigh_LineEdit"));
        globalSPHParam->setNneigh(getLineEditValue("GlobalSPH_Nneigh_LineEdit"));
        globalSPHParam->setIsol2sph(getLineEditValue("GlobalSPH_Isol2sph_LineEdit"));
    }

    void GUISolverSettingDialog::writePythonScript()
    {
        if (!_solverSettings) return;

        QStringList script;
        script.append("solverSetting=SolverSetting()");

        auto* runExeTimeParam = _solverSettings->getRunExeTimeParam();
        if (runExeTimeParam)
        {
            script.append(QString("solverSetting.setRunExeTime(%1)").arg(runExeTimeParam->getFinalTime()));
        }

        auto* globalTimeParam = _solverSettings->getGlobalTimeParam();
        if (globalTimeParam)
        {
            script.append(QString("solverSetting.setScaleFactor(%1)").arg(globalTimeParam->getScaleFactor()));
            script.append(QString("solverSetting.setEnableAdditionalCards(%1)").arg(globalTimeParam->isEnableAdditionalCards() ? "True" : "False"));
            script.append(QString("solverSetting.setToleranceGTS(%1)").arg(globalTimeParam->getToleranceGTS()));
            script.append(QString("solverSetting.setNumCycles(%1)").arg(globalTimeParam->getNumCycles()));
        }

        auto* nodalParam = _solverSettings->getNodalTimeStepControlParam();
        if (nodalParam)
        {
            script.append(QString("solverSetting.setNodalTimeStepControlType(%1)").arg((int)nodalParam->getTimeStepControlType()));
            script.append(QString("solverSetting.setNodalMinimumTimeStep(%1)").arg(nodalParam->getMinimumTimeStep()));
            script.append(QString("solverSetting.setNodalScaleFactor(%1)").arg(nodalParam->getScaleFactor()));
            script.append(QString("solverSetting.setInitialAddMassRatio(%1)").arg(nodalParam->getInitialAddMassRatio()));
        }

        auto* elementalParam = _solverSettings->getElementalTimeStepControlParam();
        if (elementalParam)
        {
            script.append(QString("solverSetting.setElementalTimeStepControlType(%1)").arg((int)elementalParam->getTimeStepControlType()));
            script.append(QString("solverSetting.setElementalMinimumTimeStep(%1)").arg(elementalParam->getMinimumTimeStep()));
            script.append(QString("solverSetting.setElementalScaleFactor(%1)").arg(elementalParam->getScaleFactor()));
            script.append(QString("solverSetting.setNumberOfAddCards(%1)").arg(elementalParam->getNumberOfAddCards() ? "True" : "False"));
            script.append(QString("solverSetting.setMinAspectRatio(%1)").arg(elementalParam->getMinAspectRatio()));
            script.append(QString("solverSetting.setMinVolumeRatio(%1)").arg(elementalParam->getMinVolumeRatio()));
        }

        auto* adrParam = _solverSettings->getAdaptiveDynamicRelaxation();
        if (adrParam)
        {
            script.append(QString("solverSetting.setEnableDynamicRelaxation(%1)").arg(adrParam->getEnableDynamicRelaxation() ? "True" : "False"));
            script.append(QString("solverSetting.setEnableStartTime(%1)").arg(adrParam->isEnableStartTime() ? "True" : "False"));
            script.append(QString("solverSetting.setStartTime(%1)").arg(adrParam->getStartTime()));
            script.append(QString("solverSetting.setEnableStopTime(%1)").arg(adrParam->isEnableStopTime() ? "True" : "False"));
            script.append(QString("solverSetting.setStopTime(%1)").arg(adrParam->getStopTime()));
        }

        auto* amsParam = _solverSettings->getAMSElementalTimeStepControlParam();
        if (amsParam)
        {
            script.append(QString("solverSetting.setEnableCriticalTimeStepScaleFactor(%1)").arg(amsParam->isEnableCriticalTimeStepScaleFactor() ? "True" : "False"));
            script.append(QString("solverSetting.setCriticalTimeStepScaleFactor(%1)").arg(amsParam->getCriticalTimeStepScaleFactor()));
            script.append(QString("solverSetting.setEnableMinTimeStep(%1)").arg(amsParam->isEnableMinTimeStep() ? "True" : "False"));
            script.append(QString("solverSetting.setAMSMinTimeStep(%1)").arg(amsParam->getMinTimeStep()));
            script.append(QString("solverSetting.setEnableAMSConvergenceTolerance(%1)").arg(amsParam->isEnableAMSConvergenceTolerance() ? "True" : "False"));
            script.append(QString("solverSetting.setAMSConvergenceTolerance(%1)").arg(amsParam->getAMSConvergenceTolerance()));
            script.append(QString("solverSetting.setEnableMaxIterationsConjugateGradient(%1)").arg(amsParam->isEnableMaxIterationsConjugateGradient() ? "True" : "False"));
            script.append(QString("solverSetting.setMaxIterationsConjugateGradient(%1)").arg(amsParam->getMaxIterationsConjugateGradient()));
            script.append(QString("solverSetting.setEnableNumberOfAdditionalOutputs(%1)").arg(amsParam->isEnableNumberOfAdditionalOutputs() ? "True" : "False"));
            script.append(QString("solverSetting.setNumberOfAdditionalOutputs(%1)").arg(amsParam->getNumberOfAdditionalOutputs()));
        }

        auto* controlParam = _solverSettings->getComputationControlParam();
        if (controlParam)
        {
            script.append(QString("solverSetting.setStopComputationWhenNegativeVolume(%1)").arg(controlParam->getStopComputationWhenNegativeVolume() ? "True" : "False"));
            script.append(QString("solverSetting.setParallelArithmetic(%1)").arg(controlParam->getParallelArithmetic() ? "True" : "False"));
        }

        auto* convergenceParam = _solverSettings->getConvergenceCriteriaParam();
        if (convergenceParam)
        {
            script.append(QString("solverSetting.setConvergenceEnableState(%1)").arg(convergenceParam->getEnableState() ? "True" : "False"));
            script.append(QString("solverSetting.setEnergyErrorRatio(%1)").arg(convergenceParam->getEnergyErrorRatio()));
            script.append(QString("solverSetting.setTotalMassRatio(%1)").arg(convergenceParam->getTotalMassRatio()));
            script.append(QString("solverSetting.setNodalMassRatio(%1)").arg(convergenceParam->getNodalMassRatio()));
            script.append(QString("solverSetting.setWriteTimeHistoryFile(%1)").arg(convergenceParam->getWriteTimeHistoryFile() ? "True" : "False"));
            script.append(QString("solverSetting.setWriteAnimationFile(%1)").arg(convergenceParam->getWriteAnimationFile() ? "True" : "False"));
            script.append(QString("solverSetting.setEnergyError(%1)").arg(convergenceParam->getEnergyError() ? "True" : "False"));
        }

        this->saveScript(script);
    }

}


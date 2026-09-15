/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIContactMultiUsageImpactDialog.h"
#include "ui_GUIContactMultiUsageImpactDialog.h"

#include "NodeGroupWidget.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionMultiUsageImpact.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QMessageBox>

namespace
{
    QString getCurveNameByID(Radioss::FITKRadiossCase* dataCase, int curveID)
    {
        if (dataCase == nullptr || curveID <= 0)
        {
            return QString();
        }

        Radioss::FITKNonLinerCurveManager* curveManager = dataCase->getNonLinerCurveManager();
        if (curveManager == nullptr)
        {
            return QString();
        }

        Radioss::FITKNonLinerCurve* curve = curveManager->getDataByID(curveID);
        if (curve == nullptr)
        {
            return QString();
        }

        return curve->getDataObjectName();
    }
}

namespace GUI
{
    GUIContactMultiUsageImpactDialog::GUIContactMultiUsageImpactDialog(Core::FITKActionOperator* oper, QWidget* parent) : Core::FITKDialog(parent)
        , _ui(new Ui::GUIContactMultiUsageImpactDialog), _oper(oper), _isCreate(true)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸
        //创建接触Tie
        _obj = new Radioss::FITKInteractionMultiUsageImpact();
        //初始化组件
        this->initWidget();
        //初始化界面数据
        this->init();
    }

    GUIContactMultiUsageImpactDialog::GUIContactMultiUsageImpactDialog(Radioss::FITKInteractionMultiUsageImpact* multiUsageImpact, Core::FITKActionOperator* oper, QWidget* parent) : Core::FITKDialog(parent)
        , _ui(new Ui::GUIContactMultiUsageImpactDialog), _oper(oper),
        _obj(multiUsageImpact)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸

        //初始化组件
        this->initWidget();
        //初始化界面数据
        this->init();
    }

    GUIContactMultiUsageImpactDialog::~GUIContactMultiUsageImpactDialog()
    {
        if (_ui)
            delete _ui;
        _ui = nullptr;
    }

    void GUIContactMultiUsageImpactDialog::init()
    {
        if (_obj == nullptr||!_oper) return;
        Radioss::FITKInteractionMultiUsageImpactDefaultValue* propValue = _obj->getValues();
        Radioss::MultiUsageImpactOptionalData optionalData = _obj->getOptionalData();
        Radioss::FITKInteractionManager* mgr = this->getInteractionManager();
        if (!mgr || !propValue) return;
        //获取数据
        int mainSurFaceID = _obj->getMasterSurfaceID();
        int nodeGroupID = _obj->getNodeGroupID();
        //设置名称
        if (_isCreate) {
            QString name = "MultiUsage_Impact-1";
            _oper->argValue<QString>("ContactName", name);
            _ui->lineEdit_name->setText(mgr->checkName(name));
        }
        else
            //设置UI数据
            _ui->lineEdit_name->setText(_obj->getDataObjectName());
        //开启信号拦截
        this->blockSignals(true);
        //设置界面数据
        _ui->widget_MainSurface->findNodeGroup(mainSurFaceID);
        _ui->widget_SecNode->findNodeGroup(nodeGroupID);
        int index = _ui->comboBox_InterfaceStiffness->findText(QString::number(propValue->_Istf));
        _ui->comboBox_InterfaceStiffness->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_HeatContact->findText(_obj->_Ithe ? "1" : "0");
        _ui->comboBox_HeatContact->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_GapOption->findText(QString::number(propValue->_Igap));
        _ui->comboBox_GapOption->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_AirbagVent->findText(QString::number(propValue->_Ibag));
        _ui->comboBox_AirbagVent->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_NodeDeletion->findText(QString::number(propValue->_Idel));
        _ui->comboBox_NodeDeletion->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_GapCurvature->findText(QString::number(_obj->_Icurv));
        _ui->comboBox_GapCurvature->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_LocalCurvature->findText(QString::number(_obj->_Iadm));
        _ui->comboBox_LocalCurvature->setCurrentIndex(index < 0 ? 0 : index);
        _ui->lineEdit_GapScale->setText(QString::number(_obj->_Fscale_gap));
        _ui->lineEdit_MaxGap->setText(QString::number(_obj->_GAPmax));
        _ui->lineEdit_MaxInitPen->setText(QString::number(_obj->_Fpenmax));
        index = _ui->comboBox_TieContact->findText(QString::number(_obj->_Itied));
        _ui->comboBox_TieContact->setCurrentIndex(index < 0 ? 0 : index);
        _ui->lineEdit_MinStiffness->setText(QString::number(_obj->_STmin));
        _ui->lineEdit_MaxStiffness->setText(QString::number(_obj->_STmax));
        _ui->lineEdit_PercentageMeshSize->setText(QString::number(_obj->_mesh_size));
        _ui->lineEdit_MinInterTimeStep->setText(QString::number(_obj->_dtmin));
        index = _ui->comboBox_DeacSecNodeESize->findText(QString::number(propValue->_Irem_gap));
        _ui->comboBox_DeacSecNodeESize->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_DeacSecNodeContact->findText(QString::number(propValue->_Irem_i2));
        _ui->comboBox_DeacSecNodeContact->setCurrentIndex(index < 0 ? 0 : index);
        _ui->lineEdit_Node1->setText(QString::number(optionalData.node_ID1));
        _ui->lineEdit_Node2->setText(QString::number(optionalData.node_ID2));
        _ui->lineEdit_C1->setText(QString::number(optionalData.C1));
        _ui->lineEdit_C2->setText(QString::number(optionalData.C2));
        _ui->lineEdit_C3->setText(QString::number(optionalData.C3));
        _ui->lineEdit_C4->setText(QString::number(optionalData.C4));
        _ui->lineEdit_C5->setText(QString::number(optionalData.C5));
        _ui->lineEdit_C6->setText(QString::number(optionalData.C6));
        _ui->lineEdit_NRadm->setText(QString::number(optionalData.NRadm));
        _ui->lineEdit_Padm->setText(QString::number(optionalData.Padm));
        _ui->lineEdit_Angladm->setText(QString::number(optionalData.Angladm));
        _ui->lineEdit_Kthe->setText(QString::number(optionalData.Kthe));
        _ui->lineEdit_Fct_IDK->setText(QString::number(optionalData.fct_IDK));
        _ui->lineEdit_Tint->setText(QString::number(optionalData.Tint));
        index = _ui->comboBox_The_form->findText(QString::number(optionalData.Ithe_form));
        _ui->comboBox_The_form->setCurrentIndex(index < 0 ? 0 : index);
        _ui->lineEdit_AscaleK->setText(QString::number(optionalData.AscaleK));
        _ui->lineEdit_Frad->setText(QString::number(optionalData.Frad));
        _ui->lineEdit_Drad->setText(QString::number(optionalData.Drad));
        _ui->lineEdit_Fheats->setText(QString::number(optionalData.Fheats));
        _ui->lineEdit_Fheatm->setText(QString::number(optionalData.Fheatm));
        _ui->lineEdit_Stfac->setText(QString::number(_obj->_Stfac));
        _ui->lineEdit_Fric->setText(QString::number(_obj->_Fric));
        _ui->lineEdit_Gapmin->setText(QString::number(_obj->_Gapmin));
        _ui->lineEdit_Tstart->setText(QString::number(_obj->_Tstart));
        _ui->lineEdit_Tstop->setText(QString::number(_obj->_Tstop));
        _ui->checkBox_IbcX->setChecked(_obj->_Ibc / 100 > 0 ? true : false);
        _ui->checkBox_IbcY->setChecked(_obj->_Ibc % 100 / 10 > 0 ? true : false);
        _ui->checkBox_IbcZ->setChecked(_obj->_Ibc % 10 > 0 ? true : false);
        index = _ui->comboBox_Inacti->findText(QString::number(propValue->_Inacti));
        _ui->comboBox_Inacti->setCurrentIndex(index < 0 ? 0 : index);
        _ui->lineEdit_VISs->setText(QString::number(_obj->_VisS));
        _ui->lineEdit_VISf->setText(QString::number(_obj->_VisF));
        _ui->lineEdit_Bumult->setText(QString::number(_obj->_Bumult));
        index = _ui->comboBox_Ifric->findText(QString::number(_obj->_Ifric));
        _ui->comboBox_Ifric->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_Ifiltr->findText(QString::number(_obj->_Ifiltr));
        _ui->comboBox_Ifiltr->setCurrentIndex(index < 0 ? 0 : index);
        _ui->lineEdit_Xfreq->setText(QString::number(_obj->_Xfreq));
        index = _ui->comboBox_Iform->findText(QString::number(propValue->_Iform));
        _ui->comboBox_Iform->setCurrentIndex(index < 0 ? 0 : index);
        _ui->lineEdit_sens_ID->setText(QString::number(_obj->_sens_ID));
        _ui->lineEdit_fct_IDF->setText(QString::number(_obj->_fct_IDf));
        _ui->lineEdit_AscaleF->setText(QString::number(_obj->_AscaleF));
        _ui->lineEdit_fric_ID->setText(QString::number(_obj->_fric_ID));
        //关闭信号拦截
        this->blockSignals(false);
    }

    void GUIContactMultiUsageImpactDialog::initWidget()
    {
        //开启信号拦截
        this->blockSignals(true);
        //初始化组件
        _ui->widget_MainSurface->setWidgetGroupType(WidgetGroupType::Surface_Element);
        _ui->widget_SecNode->setWidgetGroupType(WidgetGroupType::Set_Node);
        _ui->widget_MainSurface->setFirstNodeGroup(tr("None"), 0);
        _ui->widget_SecNode->setFirstNodeGroup(tr("None"), 0);
        //初始化关键字
        _ui->lineEdit_KeyWord->setText(Radioss::FITKInteractionMultiUsageImpact::GetFITKInteractionMultiUsageImpactRadiossKeyWord());
        //初始化下拉框
        _ui->comboBox_InterfaceStiffness->addItems({ "0", "1", "2", "3", "4", "5", "1000" });
        _ui->comboBox_HeatContact->addItems({ "0", "1" });
        _ui->comboBox_GapOption->addItems({ "0", "1", "2", "3", "1000" });
        _ui->comboBox_AirbagVent->addItems({ "0", "1", "2" });
        _ui->comboBox_NodeDeletion->addItems({ "0", "1", "2", "1000", "-1", "-2" });
        _ui->comboBox_GapCurvature->addItems({ "0", "1", "2", "3" });
        _ui->comboBox_LocalCurvature->addItems({ "0", "1", "2" });
        _ui->comboBox_TieContact->addItems({ "0", "1", "2" });
        _ui->comboBox_DeacSecNodeESize->addItems({ "0", "1", "2" });
        _ui->comboBox_DeacSecNodeContact->addItems({ "0", "1", "2", "3" });
        _ui->comboBox_The_form->addItems({ "0", "1" });
        _ui->comboBox_Inacti->addItems({ "0", "1", "2", "3", "4", "5", "6", "1000" });
        _ui->comboBox_Ifric->addItems({ "0", "1", "2", "3" });
        _ui->comboBox_Ifiltr->addItems({ "0", "1", "2", "3" });
        _ui->comboBox_Iform->addItems({ "0", "1", "2" });
        //关闭信号拦截
        this->blockSignals(false);
    }

    Radioss::FITKInteractionManager * GUIContactMultiUsageImpactDialog::getInteractionManager()
    {
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return nullptr;

        Radioss::FITKRadiossSolution* solutionManager = caseObj->getCurrentSolution();
        if (solutionManager == nullptr) return nullptr;
        return solutionManager->getInteractionManager();
    }

    bool GUIContactMultiUsageImpactDialog::createObjData()
    {
        if (!_oper || !_obj) return false;
        //获取管理器
        Radioss::FITKInteractionManager* mgr = this->getInteractionManager();
        if (!mgr) return false;

        //获取板名称-查重处理
        QString name = _ui->lineEdit_name->text();
        if (mgr->getDataByName(name)) {
            QMessageBox::warning(this, "", tr("\"%1\" already exists and cannot be overwritten.").arg(name), QMessageBox::Ok);
            return false;
        }
        //设置名称
        _obj->setDataObjectName(name);
        //设置界面数据
        this->getDataFormWidget();
        //设置数据
        mgr->appendDataObj(_obj);
        _oper->setArgs("objID", _obj->getDataObjectID());
        return true;
    }

    bool GUIContactMultiUsageImpactDialog::editObjData()
    {
        if (!_oper || !_obj) return false;
        //获取管理器
        Radioss::FITKInteractionManager* mgr = this->getInteractionManager();
        if (!mgr) return false;

        //获取板名称-查重处理
        QString name = _ui->lineEdit_name->text();
        //名称重复判断
        if (name != _obj->getDataObjectName())
            if (mgr->getDataByName(name)) {
                QMessageBox::warning(this, "", tr("\"%1\" already exists and cannot be overwritten.").arg(name), QMessageBox::Ok);
                return false;
            }
        //设置名称
        _obj->setDataObjectName(name);
        //设置界面数据
        this->getDataFormWidget();
        _oper->setArgs("objID", _obj->getDataObjectID());
        return true;
    }

    void GUIContactMultiUsageImpactDialog::getDataFormWidget()
    {
        if (!_obj) return;
        Radioss::FITKInteractionMultiUsageImpactDefaultValue* propValue = _obj->getValues();
        Radioss::MultiUsageImpactOptionalData& optionalData = _obj->getOptionalData();
        if (!propValue) return;
        //从界面获取数据
        int mainSurFaceID = _ui->widget_MainSurface->getNodeGroupId();
        int nodeGroupID = _ui->widget_SecNode->getNodeGroupId();
        int Istf = _ui->comboBox_InterfaceStiffness->currentText().toInt();
        bool Ithe = _ui->comboBox_HeatContact->currentText().toInt() > 0 ? true : false;
        int Igap = _ui->comboBox_GapOption->currentText().toInt();
        int Ibag = _ui->comboBox_AirbagVent->currentText().toInt();
        int Idel = _ui->comboBox_NodeDeletion->currentText().toInt();
        int Icurv = _ui->comboBox_GapCurvature->currentText().toInt();
        int Iadm = _ui->comboBox_LocalCurvature->currentText().toInt();
        double Fscale_gap = _ui->lineEdit_GapScale->text().toDouble();
        double GAPmax = _ui->lineEdit_MaxGap->text().toDouble();
        double Fpenmax = _ui->lineEdit_MaxInitPen->text().toDouble();
        int Itied = _ui->comboBox_TieContact->currentText().toInt();
        double STmin = _ui->lineEdit_MinStiffness->text().toDouble();
        double STmax = _ui->lineEdit_MaxStiffness->text().toDouble();
        double mesh_size = _ui->lineEdit_PercentageMeshSize->text().toDouble();
        double dtmin = _ui->lineEdit_MinInterTimeStep->text().toDouble();
        int Irem_gap = _ui->comboBox_DeacSecNodeESize->currentText().toInt();
        int Irem_i2 = _ui->comboBox_DeacSecNodeContact->currentText().toInt();
        int node_ID1 = _ui->lineEdit_Node1->text().toInt();
        int node_ID2 = _ui->lineEdit_Node2->text().toInt();
        double C1 = _ui->lineEdit_C1->text().toDouble();
        double C2 = _ui->lineEdit_C2->text().toDouble();
        double C3 = _ui->lineEdit_C3->text().toDouble();
        double C4 = _ui->lineEdit_C4->text().toDouble();
        double C5 = _ui->lineEdit_C5->text().toDouble();
        double C6 = _ui->lineEdit_C6->text().toDouble();
        int NRadm = _ui->lineEdit_NRadm->text().toInt();
        double Padm = _ui->lineEdit_Padm->text().toDouble();
        double Angladm = _ui->lineEdit_Angladm->text().toDouble();
        double Kthe = _ui->lineEdit_Kthe->text().toDouble();
        int fct_IDK = _ui->lineEdit_Fct_IDK->text().toInt();
        double Tint = _ui->lineEdit_Tint->text().toDouble();
        int Ithe_form = _ui->comboBox_The_form->currentText().toInt();
        double AscaleK = _ui->lineEdit_AscaleK->text().toDouble();
        double Frad = _ui->lineEdit_Frad->text().toDouble();
        double Drad = _ui->lineEdit_Drad->text().toDouble();
        double Fheats = _ui->lineEdit_Fheats->text().toDouble();
        double Fheatm = _ui->lineEdit_Fheatm->text().toDouble();
        double Stfac = _ui->lineEdit_Stfac->text().toDouble();
        double Fric = _ui->lineEdit_Fric->text().toDouble();
        double Gapmin = _ui->lineEdit_Gapmin->text().toDouble();
        double Tstart = _ui->lineEdit_Tstart->text().toDouble();
        double Tstop = _ui->lineEdit_Tstop->text().toDouble();
        int Ibc = (_ui->checkBox_IbcX->isChecked() ? 100 : 0) + (_ui->checkBox_IbcY->isChecked() ? 10 : 0) + (_ui->checkBox_IbcZ->isChecked() ? 1 : 0);
        int Inacti = _ui->comboBox_Inacti->currentText().toInt();
        double VisS = _ui->lineEdit_VISs->text().toDouble();
        double VisF = _ui->lineEdit_VISf->text().toDouble();
        double Bumult = _ui->lineEdit_Bumult->text().toDouble();
        int Ifric = _ui->comboBox_Ifric->currentText().toInt();
        int Ifiltr = _ui->comboBox_Ifiltr->currentText().toInt();
        double Xfreq = _ui->lineEdit_Xfreq->text().toDouble();
        int Iform = _ui->comboBox_Iform->currentText().toInt();
        int sens_ID = _ui->lineEdit_sens_ID->text().toInt();
        int fct_IDf = _ui->lineEdit_fct_IDF->text().toInt();
        double AscaleF = _ui->lineEdit_AscaleF->text().toDouble();
        int fric_ID = _ui->lineEdit_fric_ID->text().toInt();
        //数据对象设置数据
        _obj->setMasterSurfaceID(mainSurFaceID);
        _obj->setNodeGroupID(nodeGroupID);
        _obj->_Ithe = Ithe;
        _obj->_Icurv = Icurv;
        _obj->_Iadm = Iadm;
        _obj->_Fscale_gap = Fscale_gap;
        _obj->_GAPmax = GAPmax;
        _obj->_Fpenmax = Fpenmax;
        _obj->_Itied = Itied;
        _obj->_STmin = STmin;
        _obj->_STmax = STmax;
        _obj->_mesh_size = mesh_size;
        _obj->_dtmin = dtmin;
        _obj->_Stfac = Stfac;
        _obj->_Fric = Fric;
        _obj->_Gapmin = Gapmin;
        _obj->_Tstart = Tstart;
        _obj->_Tstop = Tstop;
        _obj->_Ibc = Ibc;
        _obj->_VisS = VisS;
        _obj->_VisF = VisF;
        _obj->_Bumult = Bumult;
        _obj->_Ifric = Ifric;
        _obj->_Ifiltr = Ifiltr;
        _obj->_Xfreq = Xfreq;
        _obj->_sens_ID = sens_ID;
        _obj->_fct_IDf = fct_IDf;
        _obj->_AscaleF = AscaleF;
        _obj->_fric_ID = fric_ID;
        propValue->_Istf = Istf;
        propValue->_Igap = Igap;
        propValue->_Ibag = Ibag;
        propValue->_Idel = Idel;
        propValue->_Irem_gap = Irem_gap;
        propValue->_Irem_i2 = Irem_i2;
        propValue->_Inacti = Inacti;
        propValue->_Iform = Iform;
        optionalData.node_ID1 = node_ID1;
        optionalData.node_ID2 = node_ID2;
        optionalData.C1 = C1;
        optionalData.C2 = C2;
        optionalData.C3 = C3;
        optionalData.C4 = C4;
        optionalData.C5 = C5;
        optionalData.C6 = C6;
        optionalData.NRadm = NRadm;
        optionalData.Padm = Padm;
        optionalData.Angladm = Angladm;
        optionalData.Kthe = Kthe;
        optionalData.fct_IDK = fct_IDK;
        optionalData.Tint = Tint;
        optionalData.Ithe_form = Ithe_form;
        optionalData.AscaleK = AscaleK;
        optionalData.Frad = Frad;
        optionalData.Drad = Drad;
        optionalData.Fheats = Fheats;
        optionalData.Fheatm = Fheatm;
    }

    void GUIContactMultiUsageImpactDialog::writePythonScript(const QString& oldName)
    {
        if (_obj == nullptr)
        {
            return;
        }

        Radioss::FITKRadiossCase* dataCase =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return;
        }

        Radioss::FITKRadiossSolution* solutionData = dataCase->getCurrentSolution();
        if (solutionData == nullptr)
        {
            return;
        }

        Radioss::FITKInteractionMultiUsageImpactDefaultValue* propValue = _obj->getValues();
        Radioss::MultiUsageImpactOptionalData& optionalData = _obj->getOptionalData();
        if (propValue == nullptr)
        {
            return;
        }

        QStringList script;
        script.append(QString("Solution.Solution.setCurrentSolution('%1')").arg(solutionData->getDataObjectName()));

        if (_isCreate)
        {
            script.append("p=InterMultiUsageImpact.InterMultiUsageImpact()");
            script.append(QString("p.createInteraction('%1')").arg(_obj->getDataObjectName()));
        }
        else
        {
            if (oldName.isEmpty())
            {
                return;
            }

            script.append(QString("p=InterMultiUsageImpact.InterMultiUsageImpact.GetInterMultiUsageImpact('%1')").arg(oldName));
            if (oldName != _obj->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(_obj->getDataObjectName()));
            }
        }

        script.append(QString("p.setMasterSurface('%1')").arg(_ui->widget_MainSurface->getCurrentName()));
        script.append(QString("p.setNodeGroup('%1')").arg(_ui->widget_SecNode->getCurrentName()));
        script.append(QString("p.setIthe(%1)").arg(_obj->_Ithe ? 1 : 0));
        script.append(QString("p.setIcurv(%1)").arg(_obj->_Icurv));
        script.append(QString("p.setIadm(%1)").arg(_obj->_Iadm));
        script.append(QString("p.setFscaleGap(%1)").arg(_obj->_Fscale_gap));
        script.append(QString("p.setGAPmax(%1)").arg(_obj->_GAPmax));
        script.append(QString("p.setFpenmax(%1)").arg(_obj->_Fpenmax));
        script.append(QString("p.setItied(%1)").arg(_obj->_Itied));
        script.append(QString("p.setSTmin(%1)").arg(_obj->_STmin));
        script.append(QString("p.setSTmax(%1)").arg(_obj->_STmax));
        script.append(QString("p.setMeshSize(%1)").arg(_obj->_mesh_size));
        script.append(QString("p.setDtmin(%1)").arg(_obj->_dtmin));
        script.append(QString("p.setStfac(%1)").arg(_obj->_Stfac));
        script.append(QString("p.setFric(%1)").arg(_obj->_Fric));
        script.append(QString("p.setGapmin(%1)").arg(_obj->_Gapmin));
        script.append(QString("p.setTstart(%1)").arg(_obj->_Tstart));
        script.append(QString("p.setTstop(%1)").arg(_obj->_Tstop));
        script.append(QString("p.setIbc(%1)").arg(_obj->_Ibc));
        script.append(QString("p.setVisS(%1)").arg(_obj->_VisS));
        script.append(QString("p.setVisF(%1)").arg(_obj->_VisF));
        script.append(QString("p.setBumult(%1)").arg(_obj->_Bumult));
        script.append(QString("p.setIfric(%1)").arg(_obj->_Ifric));
        script.append(QString("p.setIfiltr(%1)").arg(_obj->_Ifiltr));
        script.append(QString("p.setXfreq(%1)").arg(_obj->_Xfreq));
        script.append(QString("p.setSensID(%1)").arg(_obj->_sens_ID));
        script.append(QString("p.setAscaleF(%1)").arg(_obj->_AscaleF));
        script.append(QString("p.setFricID(%1)").arg(_obj->_fric_ID));

        script.append(QString("p.setIstf(%1)").arg(propValue->_Istf));
        script.append(QString("p.setIgap(%1)").arg(propValue->_Igap));
        script.append(QString("p.setIbag(%1)").arg(propValue->_Ibag));
        script.append(QString("p.setIdel(%1)").arg(propValue->_Idel));
        script.append(QString("p.setIremGap(%1)").arg(propValue->_Irem_gap));
        script.append(QString("p.setIremI2(%1)").arg(propValue->_Irem_i2));
        script.append(QString("p.setInacti(%1)").arg(propValue->_Inacti));
        script.append(QString("p.setIform(%1)").arg(propValue->_Iform));

        script.append(QString("p.setC1(%1)").arg(optionalData.C1));
        script.append(QString("p.setC2(%1)").arg(optionalData.C2));
        script.append(QString("p.setC3(%1)").arg(optionalData.C3));
        script.append(QString("p.setC4(%1)").arg(optionalData.C4));
        script.append(QString("p.setC5(%1)").arg(optionalData.C5));
        script.append(QString("p.setC6(%1)").arg(optionalData.C6));
        script.append(QString("p.setNRadm(%1)").arg(optionalData.NRadm));
        script.append(QString("p.setPadm(%1)").arg(optionalData.Padm));
        script.append(QString("p.setAngladm(%1)").arg(optionalData.Angladm));
        script.append(QString("p.setKthe(%1)").arg(optionalData.Kthe));
        script.append(QString("p.setTint(%1)").arg(optionalData.Tint));
        script.append(QString("p.setItheForm(%1)").arg(optionalData.Ithe_form));
        script.append(QString("p.setAscaleK(%1)").arg(optionalData.AscaleK));
        script.append(QString("p.setFrad(%1)").arg(optionalData.Frad));
        script.append(QString("p.setDrad(%1)").arg(optionalData.Drad));
        script.append(QString("p.setFheats(%1)").arg(optionalData.Fheats));
        script.append(QString("p.setFheatm(%1)").arg(optionalData.Fheatm));

        QString curveName = getCurveNameByID(dataCase, _obj->_fct_IDf);
        if (!curveName.isEmpty())
        {
            script.append(QString("p.setFctIDf('%1')").arg(curveName));
        }

        curveName = getCurveNameByID(dataCase, optionalData.fct_IDK);
        if (!curveName.isEmpty())
        {
            script.append(QString("p.setFctIDK('%1')").arg(curveName));
        }

        this->saveScript(script);
    }
    
    void GUIContactMultiUsageImpactDialog::on_pushButton_OK_clicked()
    {
        //点击确定按键动作
        Radioss::FITKInteractionManager* mgr = this->getInteractionManager();
        if (!mgr || !_obj || !_oper) return;
        QString oldName = _obj->getDataObjectName();
        if (_isCreate)
        {
            if (!this->createObjData())
                return;
        }
        else
        {
            if (!this->editObjData())
                return;
        }
        _oper->execProfession();
        this->writePythonScript(oldName);
        this->accept();
    }

    void GUIContactMultiUsageImpactDialog::on_pushButton_Cancel_clicked()
    {
        //点击取消按键动作
        this->reject();
    }

    void GUIContactMultiUsageImpactDialog::on_comboBox_GapCurvature_currentTextChanged(const QString & text)
    {
        int data = text.toInt();
        if (data == 1 || data == 2)
        {
            _ui->groupBox_Node->setEnabled(true);
        }
        else
        {
            _ui->groupBox_Node->setEnabled(false);
        }
    }

    void GUIContactMultiUsageImpactDialog::on_comboBox_LocalCurvature_currentTextChanged(const QString & text)
    {
        int data = text.toInt();
        if (data == 2)
        {
            _ui->groupBox_AdaM->setEnabled(true);
        }
        else
        {
            _ui->groupBox_AdaM->setEnabled(false);
        }
    }

    void GUIContactMultiUsageImpactDialog::on_comboBox_HeatContact_currentTextChanged(const QString & text)
    {
        int data = text.toInt();
        if (data == 1)
        {
            _ui->groupBox_HeatCont->setEnabled(true);
        }
        else
        {
            _ui->groupBox_HeatCont->setEnabled(false);
        }
    }

    void GUIContactMultiUsageImpactDialog::on_comboBox_Ifric_currentTextChanged(const QString & text)
    {
        int data = text.toInt();
        if (data > 0)
        {
            _ui->groupBox_FriLawCoeff->setEnabled(true);
        }
        else
        {
            _ui->groupBox_FriLawCoeff->setEnabled(false);
        }
    }

    void GUIContactMultiUsageImpactDialog::accept()
    {
        //清除高亮
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr)operGraph->clearAllHight();

        this->QDialog::accept();
    }

    void GUIContactMultiUsageImpactDialog::reject()
    {
        //清除高亮
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr)operGraph->clearAllHight();
        
        //关闭窗口
        if (_oper) _oper->setArgs("objID", -1);
        if (_isCreate)
        {
            delete _obj;
            _obj = nullptr;
        }
        this->QDialog::reject();
    }



}



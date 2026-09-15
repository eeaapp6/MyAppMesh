/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIAccelDialog.h"
#include "ui_GUIAccelDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKProbeAcceleration.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QAction>
#include <QComboBox>
#include <QMessageBox>

namespace GUI
{
    GUIAccelDialog::GUIAccelDialog(Radioss::FITKProbeAcceleration* accel, Core::FITKActionOperator* oper, QWidget* parent) : QDialog(parent)
        , _ui(new Ui::GUIAccelDialog), _oper(oper)
    {
        _ui->setupUi(this);
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

        _accel = accel;
        init();
    }


    GUIAccelDialog::~GUIAccelDialog()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
    }

    void GUIAccelDialog::init()
    {
        //集合
        initSet();
        //坐标系
        _ui->comboBoxCS->addItem("Global", 0);
        //初始名称
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKProbeManager* pMgr = caseObj->getCurrentSolution()->getProbeManager();
        _ui->lineEditName->setText(pMgr->checkName("Accelerometer-1"));
        //默认频率
        _ui->lineEditFcut->setCurrentValidValue(1.65);

        //初始化数据
        if (!_accel)return;
        _ui->lineEditName->setText(_accel->getDataObjectName());//名字
        _ui->lineEditFcut->setCurrentValidValue(_accel->getCutOffFreq());//频率
        int index = _ui->comboBoxSet->findData(_accel->getSetID());
        _ui->comboBoxSet->setCurrentIndex(index);//集合
        //坐标 to do
    }

    void GUIAccelDialog::initSet()
    {
        _ui->comboBoxSet->clear();
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKRadiossMeshModel* radiossMeshModel = caseObj->getMeshModel();
        if (!radiossMeshModel)return;
        Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
        if (!componentManager)return;

        _ui->comboBoxSet->addItem("None", -1);
        for (int i = 0; i < componentManager->getDataCount(); ++i) {
            Interface::FITKModelSet* set = dynamic_cast<Interface::FITKModelSet*>(componentManager->getDataByIndex(i));
            if (!set)continue;
            if (set->getModelSetType() != Interface::FITKModelEnum::FITKModelSetType::FMSNode)continue;
            if (set->getMemberCount() != 1)continue;
            _ui->comboBoxSet->addItem(set->getDataObjectName(), set->getDataObjectID());
        }
    }

    void GUIAccelDialog::on_pushButtonOK_clicked()
    {
        //检查
        QString name = _ui->lineEditName->text();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "", tr("The name is illegal"), QMessageBox::Ok);
            return;
        }
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKProbeManager* pMgr = caseObj->getCurrentSolution()->getProbeManager();
        if (pMgr->getDataByName(name)&& pMgr->getDataByName(name)!= _accel) {
            QMessageBox::warning(this, "", tr("The name is illegal"), QMessageBox::Ok);
            return;
        }

        int setID = _ui->comboBoxSet->currentData().toInt();
        if (setID < 1) {
            QMessageBox::warning(this, "", tr("Please select a single point set"), QMessageBox::Ok);
            return;
        }

        double Fcut;
        if (!_ui->lineEditFcut->getCurrentValidValue(Fcut)) {
            QMessageBox::warning(this, "", tr("The Cutoff Frequency is illegal"), QMessageBox::Ok);
            return;
        }

        QString oldName;
        if (_accel != nullptr)
        {
            oldName = _accel->getDataObjectName();
        }

        if (!_accel) {
            _accel = new Radioss::FITKProbeAcceleration();
            pMgr->appendDataObj(_accel);
        }
        _accel->setDataObjectName(name);
        _accel->setSetID(setID);
        _accel->setCutOffFreq(Fcut);

        _oper->setArgs("DataID", _accel->getDataObjectID());
        writePythonScript(oldName);
        this->accept();
    }

    void GUIAccelDialog::writePythonScript(const QString& oldName)
    {
        if (_accel == nullptr || _oper == nullptr)
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

        QStringList script;
        script.append(QString("Solution.Solution.setCurrentSolution('%1')").arg(solutionData->getDataObjectName()));

        if (oldName.isEmpty())
        {
            script.append("p=Probe.ProbeAccelerometer()");
            script.append(QString("p.createProbe('%1')").arg(_accel->getDataObjectName()));
        }
        else
        {
            script.append(QString("p=Probe.ProbeAccelerometer.GetProbeAccelerometer('%1')").arg(oldName));
            if (oldName != _accel->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(_accel->getDataObjectName()));
            }
        }

        script.append(QString("p.setSystemID(%1)").arg(_ui->comboBoxCS->currentData().toInt()));
        script.append(QString("p.setSingleNodeGroup('%1')").arg(_ui->comboBoxSet->currentText()));
        script.append(QString("p.setCutOffFreq(%1)").arg(_ui->lineEditFcut->text()));

        _oper->saveScript(script);
    }

    void GUIAccelDialog::on_pushButtonCancel_clicked()
    {
        this->reject();
    }

    void GUIAccelDialog::on_pushButtonPick_clicked()
    {
        //获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr)return;
        //清除高亮
        operGraph->clearAllHight();
        Core::FITKActionOperator* opt = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionSetNodesCreate");
        if (opt == nullptr)return;
        connect(opt, &Core::FITKActionOperator::operatorAbstractSig1, this, &GUIAccelDialog::onComboBoxAboutToShow);

        QAction* action = new QAction();
        opt->setArgs("IsSingle",true);
        action->setObjectName("actionSetNodesCreate");
        opt->setEmitter(action);
        opt->execGUI();
    }

    void GUIAccelDialog::onComboBoxAboutToShow(Core::FITKAbstractOperator * oper)
    {
        disconnect(oper, &Core::FITKActionOperator::operatorAbstractSig1, this, &GUIAccelDialog::onComboBoxAboutToShow);
        initSet();
    }


}
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUISectionDialog.h"
#include "ui_GUISectionDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKProbeSection.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QAction>
#include <QComboBox>
#include <QMessageBox>
#include <QHeaderView>
namespace GUI
{
    GUISectionDialog::GUISectionDialog(Radioss::FITKProbeSection* section, Core::FITKActionOperator* oper, QWidget* parent) : QDialog(parent)
        , _ui(new Ui::GUISectionDialog),_oper(oper)
    {
        _ui->setupUi(this);
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

        _section = section;
        init();
    }

    GUISectionDialog::~GUISectionDialog()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
    }

    void GUISectionDialog::init()
    {
        //初始化UI
        initUI();

        //初始化数据
        if (!_section)return;
        _ui->lineEditName->setText(_section->getDataObjectName());
        int index = _ui->comboBoxSet1->findData(_section->getSetID1());
        _ui->comboBoxSet1->setCurrentIndex(index);

        index = _ui->comboBoxSet2->findData(_section->getSetID2());
        _ui->comboBoxSet2->setCurrentIndex(index);

        index = _ui->comboBoxSet3->findData(_section->getSetID3());
        _ui->comboBoxSet2->setCurrentIndex(index);

        index = _ui->comboBoxSetGroup->findData(_section->getGrndID());
        _ui->comboBoxSetGroup->setCurrentIndex(index);

        index = _ui->comboBoxFrom->findData(_section->getIsave());
        _ui->comboBoxFrom->setCurrentIndex(index);

        index = _ui->comboBoxframeID->findData(_section->getFrameID());
        _ui->comboBoxframeID->setCurrentIndex(index);

        _ui->lineEditFile->setText(_section->getFileName());

        _ui->lineEditDt->setCurrentValidValue(_section->getDt());

        _ui->lineEditDtAlph->setCurrentValidValue(_section->getAlpha());

        int eleID = _section->getEleGroupID(_section->getElementGroupType());
        index = _ui->comboBoxEleSet->findData(eleID);
        _ui->comboBoxEleSet->setCurrentIndex(index);

        _ui->lineEditDtNinter->setCurrentValidValue(_section->getNinter());

        index = _ui->comboBoxIframe->findData(_section->getIframe());
        _ui->comboBoxIframe->setCurrentIndex(index);
    }

    void GUISectionDialog::initUI()
    {
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKProbeManager* pMgr = caseObj->getCurrentSolution()->getProbeManager();
        if (!pMgr)return;
        Radioss::FITKRadiossMeshModel* radiossMeshModel = caseObj->getMeshModel();
        if (!radiossMeshModel)return;
        Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
        if (!componentManager)return;
        //名字
        _ui->lineEditName->setText(pMgr->checkName("Section-1"));
        //集合
        _ui->comboBoxSetGroup->setObjectName("comboBoxSetGroup");
        initNodeSet(_ui->comboBoxSet1);
        initNodeSet(_ui->comboBoxSet2);
        initNodeSet(_ui->comboBoxSet3);
        initNodeSet(_ui->comboBoxSetGroup);

        connect(_ui->pushButtonPick1, &QPushButton::clicked, this, [&]() {
            GUISectionDialog::onPushButtonPickClicked(_ui->comboBoxSet1);
        });
        connect(_ui->pushButtonPick2, &QPushButton::clicked, this, [&]() {
            GUISectionDialog::onPushButtonPickClicked(_ui->comboBoxSet2);
        });
        connect(_ui->pushButtonPick3, &QPushButton::clicked, this, [&]() {
            GUISectionDialog::onPushButtonPickClicked(_ui->comboBoxSet3);
        });
        connect(_ui->pushButtonPickGroup, &QPushButton::clicked, this, [&]() {
            GUISectionDialog::onPushButtonPickClicked(_ui->comboBoxSetGroup,false);
        });
        //I_SAVE
        _ui->comboBoxFrom->addItem("Do not save any data", 0);
        _ui->comboBoxFrom->addItem("Save section displacements in the section cut file", 1);
        _ui->comboBoxFrom->addItem("Save section displacements, resultant force, and moment components in the section cut file", 2);
        _ui->comboBoxFrom->addItem("Read and apply the section data as loads to the model", 100);
        _ui->comboBoxFrom->addItem("Read and apply the section data as loads to the model", 101);
        this->adjustComboBoxViewWidth(_ui->comboBoxFrom);

        //文件名字暂时false
        _ui->lineEditFile->setEnabled(false);
        //多点集合
        _ui->comboBoxEleSet->addItem("None",-1);
        for (int i = 0; i < componentManager->getDataCount(); ++i) {
            Interface::FITKModelSet* set = dynamic_cast<Interface::FITKModelSet*>(componentManager->getDataByIndex(i));
            if (!set)continue;
            if (set->getModelSetType() != Interface::FITKModelEnum::FITKModelSetType::FMSElem)continue;
            _ui->comboBoxEleSet->addItem(set->getDataObjectName(), set->getDataObjectID());
        }
        //接口暂时false
        _ui->lineEditDtNinter->setText("0");
        _ui->lineEditDtNinter->setEnabled(false);
        //Iframe
        _ui->comboBoxIframe->addItem("Origin of the local section system", 0);
        _ui->comboBoxIframe->addItem("Geometrical center of the section", 1);
        _ui->comboBoxIframe->addItem("CoG of the section", 2);
        _ui->comboBoxIframe->addItem("Center is the global system point", 3);
        _ui->comboBoxIframe->addItem("Origin of the global local section system", 10);
        _ui->comboBoxIframe->addItem("Geometrical center of the section", 11);
        _ui->comboBoxIframe->addItem("CoG of the section", 12);
        _ui->comboBoxIframe->addItem("Center is the global system point (0,0,0)", 13);
        this->adjustComboBoxViewWidth(_ui->comboBoxIframe);

        _ui->comboBoxSet1->setCurrentIndex(0);
        _ui->comboBoxSet2->setCurrentIndex(0);
        _ui->comboBoxSet3->setCurrentIndex(0);
        _ui->comboBoxSetGroup->setCurrentIndex(0);
    }

    void GUISectionDialog::initNodeSet(QComboBox* box)
    {
        if (!box)return;
        int index = box->currentIndex();
        box->clear();
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        Radioss::FITKRadiossMeshModel* radiossMeshModel = caseObj->getMeshModel();
        if (!radiossMeshModel)return;
        Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
        if (!componentManager)return;

        box->addItem("None", -1);
        for (int i = 0; i < componentManager->getDataCount(); ++i) {
            Interface::FITKModelSet* set = dynamic_cast<Interface::FITKModelSet*>(componentManager->getDataByIndex(i));
            if (!set)continue;
            if (set->getModelSetType() != Interface::FITKModelEnum::FITKModelSetType::FMSNode)continue;
            if (box->objectName()!="comboBoxSetGroup" && set->getMemberCount() != 1)continue;
            box->addItem(set->getDataObjectName(), set->getDataObjectID());
        }
        if(box->count()> index)
            box->setCurrentIndex(index);
    }

    bool GUISectionDialog::ckeckData()
    {
        //检查
        QString name = _ui->lineEditName->text();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "", tr("The name is illegal"), QMessageBox::Ok);
            return false;
        }
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return false;
        Radioss::FITKProbeManager* pMgr = caseObj->getCurrentSolution()->getProbeManager();
        if (pMgr->getDataByName(name) && pMgr->getDataByName(name) != _section) {
            QMessageBox::warning(this, "", tr("The name is illegal"), QMessageBox::Ok);
            return false;
        }
        return true;
    }

    void GUISectionDialog::adjustComboBoxViewWidth(QComboBox * comboBox)
    {
        //comboBox->setMaximumWidth(150);
        // 计算所有item的最大文本宽度
        QFontMetrics fm(comboBox->font());
        int maxWidth = 0;
        for (int i = 0; i < comboBox->count(); ++i) {
            int itemWidth = fm.horizontalAdvance(comboBox->itemText(i));
            maxWidth = qMax(maxWidth, itemWidth);
        }

        // 设置下拉视图的最小宽度
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents); // 关键策略
        comboBox->view()->setMinimumWidth(maxWidth+20); // 确保完整显示
    }

    void GUISectionDialog::on_pushButtonOK_clicked()
    {
        if (!ckeckData())return;
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        Radioss::FITKProbeManager* pMgr = caseObj->getCurrentSolution()->getProbeManager();
        if (!pMgr)return;

        QString oldName;
        if (_section != nullptr)
        {
            oldName = _section->getDataObjectName();
        }

        if (!_section) {
            _section = new Radioss::FITKProbeSection();
            pMgr->appendDataObj(_section);
        }
        double dt = 0.0, alpha = 0.0;
        _ui->lineEditDt->getCurrentValidValue(dt);
        _ui->lineEditDtAlph->getCurrentValidValue(alpha);

        _section->setDataObjectName(_ui->lineEditName->text());
        _section->setSetID1(_ui->comboBoxSet1->currentData().toInt());
        _section->setSetID2(_ui->comboBoxSet2->currentData().toInt());
        _section->setSetID3(_ui->comboBoxSet3->currentData().toInt());
        _section->setGrndID(_ui->comboBoxSetGroup->currentData().toInt());
        _section->setIsave(_ui->comboBoxFrom->currentData().toInt());
        _section->setFrameID(_ui->comboBoxframeID->currentData().toInt());
        _section->setFileName(_ui->lineEditFile->text());
        _section->setDt(dt);
        _section->setAlpha(alpha);

        //单元集合,暂定类型为shell
        _section->setElementGroupType(Radioss::FITKRadiossElementGroup::ElementGroupType::GRSHEL);
        _section->setGrshelID(_ui->comboBoxEleSet->currentData().toInt());

        _section->setIframe(_ui->comboBoxIframe->currentData().toInt());

        _oper->setArgs("DataID", _section->getDataObjectID());
        writePythonScript(oldName);
        this->accept();
    }

    void GUISectionDialog::writePythonScript(const QString& oldName)
    {
        if (_section == nullptr || _oper == nullptr)
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
            script.append("p=Probe.ProbeSection()");
            script.append(QString("p.createProbe('%1')").arg(_section->getDataObjectName()));
        }
        else
        {
            script.append(QString("p=Probe.ProbeSection.GetProbeSection('%1')").arg(oldName));
            if (oldName != _section->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(_section->getDataObjectName()));
            }
        }

        script.append(QString("p.setSingleNodeGroup1('%1')").arg(_ui->comboBoxSet1->currentText()));
        script.append(QString("p.setSingleNodeGroup2('%1')").arg(_ui->comboBoxSet2->currentText()));
        script.append(QString("p.setSingleNodeGroup3('%1')").arg(_ui->comboBoxSet3->currentText()));
        script.append(QString("p.setNodeGroup('%1')").arg(_ui->comboBoxSetGroup->currentText()));
        script.append(QString("p.setIsave(%1)").arg(_ui->comboBoxFrom->currentData().toInt()));
        script.append(QString("p.setFileName('%1')").arg(_ui->lineEditFile->text()));
        script.append(QString("p.setDt(%1)").arg(_ui->lineEditDt->text()));
        script.append(QString("p.setAlpha(%1)").arg(_ui->lineEditDtAlph->text()));
        script.append(QString("p.setShellElementGroup('%1')").arg(_ui->comboBoxEleSet->currentText()));
        script.append(QString("p.setIframe(%1)").arg(_ui->comboBoxIframe->currentData().toInt()));

        _oper->saveScript(script);
    }

    void GUISectionDialog::on_pushButtonCancel_clicked()
    {
        this->reject();
    }

    void GUISectionDialog::onPushButtonPickClicked(QComboBox* comboBox, bool isSingle)
    {
        //获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr)return;
        //清除高亮
        operGraph->clearAllHight();
        Core::FITKActionOperator* opt = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionSetNodesCreate");
        if (opt == nullptr)return;
        connect(opt, &Core::FITKActionOperator::operatorAbstractSig1, this, &GUISectionDialog::onComboBoxAboutToShow);

        QAction* action = new QAction();
        action->setObjectName("actionSetNodesCreate");
        opt->setArgs("IsSingle", isSingle);
        opt->setArgs("sectionComboBox", QVariant::fromValue(comboBox));
        opt->setEmitter(action);
        opt->execGUI();
    }

    void GUISectionDialog::onComboBoxAboutToShow(Core::FITKAbstractOperator * oper)
    {
        disconnect(oper, &Core::FITKActionOperator::operatorAbstractSig1, this, &GUISectionDialog::onComboBoxAboutToShow);
        QComboBox* comboBox{};
        oper->argValue("sectionComboBox", comboBox);
        if (!comboBox)return;
        //刷新点下拉框
        initNodeSet(_ui->comboBoxSet1);
        initNodeSet(_ui->comboBoxSet2);
        initNodeSet(_ui->comboBoxSet3);
        initNodeSet(_ui->comboBoxSetGroup);

        comboBox->setCurrentIndex(comboBox->count()-1);
    }
}
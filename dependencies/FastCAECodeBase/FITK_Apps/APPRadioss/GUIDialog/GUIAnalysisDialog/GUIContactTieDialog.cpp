/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIContactTieDialog.h"
#include "ui_GUIContactTieDialog.h"

#include "NodeGroupWidget.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QMessageBox>

namespace GUI
{
    GUIContactTieDialog::GUIContactTieDialog(Core::FITKActionOperator* oper, QWidget* parent) : Core::FITKDialog(parent)
        , _ui(new Ui::GUIContactTieDialog), _oper(oper), _isCreate(true)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸
        //创建接触Tie
        _obj = new Radioss::FITKInteractionKinematicTied();
        //初始化组件
        this->initWidget();
        //初始化界面数据
        this->init();
    }

    GUIContactTieDialog::GUIContactTieDialog(Radioss::FITKInteractionKinematicTied* tie, Core::FITKActionOperator* oper, QWidget* parent) : Core::FITKDialog(parent)
        , _ui(new Ui::GUIContactTieDialog), _oper(oper),
        _obj(tie)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸

        //初始化组件
        this->initWidget();
        //初始化界面数据
        this->init();
    }

    GUIContactTieDialog::~GUIContactTieDialog()
    {
        if (_ui)
            delete _ui;
        _ui = nullptr;
    }

    void GUIContactTieDialog::init()
    {
        if (_obj == nullptr||!_oper) return;
        Radioss::FITKInteractionKinematicTiedDefault* propValue = _obj->getValue();
        Radioss::FITKInteractionManager* mgr = this->getInteractionManager();
        if (!mgr || !propValue) return;
        //获取数据
        int mainSurFaceID = _obj->getMasterSurfaceID();
        int nodeGroupID = _obj->getNodeGroupID();
        int level = _obj->getLevel();
        double searchDistance = _obj->getSearchDistance();
        int ignoreFlag = propValue->getIgnoreFlag();
        int spotWeldFormulationFlag = propValue->getSpotWeldFormulationFlag();
        int searchFormulationFlag = propValue->getSearchFormulationFlag();
        int nodeDeletionFlag = propValue->getNodeDeletionFlag();
        //int IstfFlag_Default = propValue->getIstfFlagDefault();
        //Radioss::TiedOptionalData& optionalData = _obj->getOptionalData();
        //设置名称
        if (_isCreate) {
            QString name = "ContactTie-1";
            _oper->argValue<QString>("ContactName", name);
            _ui->lineEdit_name->setText(mgr->checkName(name));
        }
        else
            //设置UI数据
            _ui->lineEdit_name->setText(_obj->getDataObjectName());
        //设置界面数据
        _ui->widget_MainSurface->findNodeGroup(mainSurFaceID);
        _ui->widget_SecNode->findNodeGroup(nodeGroupID);
        _ui->lineEdit_HL->setText(QString::number(level));
        _ui->lineEdit_SearchDistance->setText(QString::number(searchDistance));
        int index = _ui->comboBox_IgnoreSecond->findText(QString::number(ignoreFlag));
        _ui->comboBox_IgnoreSecond->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_NodeDeletion->findText(QString::number(nodeDeletionFlag));
        _ui->comboBox_NodeDeletion->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_SearchForm->findText(QString::number(searchFormulationFlag));
        _ui->comboBox_SearchForm->setCurrentIndex(index < 0 ? 0 : index);
        index = _ui->comboBox_SpotwldForm->findText(QString::number(spotWeldFormulationFlag));
        _ui->comboBox_SpotwldForm->setCurrentIndex(index < 0 ? 0 : index);
    }

    void GUIContactTieDialog::initWidget()
    {
        //初始化组件
        _ui->widget_MainSurface->setWidgetGroupType(WidgetGroupType::Surface_Element);
        _ui->widget_SecNode->setWidgetGroupType(WidgetGroupType::Set_Node);
        _ui->widget_MainSurface->setFirstNodeGroup(tr("None"), 0);
        _ui->widget_SecNode->setFirstNodeGroup(tr("None"), 0);
        //初始化关键字
        _ui->lineEdit_KeyWord->setText(Radioss::FITKInteractionKinematicTied::GetFITKInteractionKinematicTiedRadiossKeyWord());
        //初始化下拉框
        _ui->comboBox_IgnoreSecond->addItems(QStringList() << "1" << "2" << "3" << "1000");
        _ui->comboBox_SpotwldForm->addItems(QStringList() << "1" << "2" << "4" << "5" << "20" << "21" << "22" << "25" << "27" << "28" << "30");
        _ui->comboBox_SearchForm->addItems(QStringList() << "1" << "2");
        _ui->comboBox_NodeDeletion->addItems(QStringList() << "1" << "2" << "1000");
    }

    Radioss::FITKInteractionManager * GUIContactTieDialog::getInteractionManager()
    {
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return nullptr;

        Radioss::FITKRadiossSolution* solutionManager = caseObj->getCurrentSolution();
        if (solutionManager == nullptr) return nullptr;
        return solutionManager->getInteractionManager();
    }

    bool GUIContactTieDialog::createObjData()
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

    bool GUIContactTieDialog::editObjData()
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

    void GUIContactTieDialog::getDataFormWidget()
    {
        if (!_obj) return;
        Radioss::FITKInteractionKinematicTiedDefault* propValue = _obj->getValue();
        if (!propValue) return;
        //从界面获取数据
        int mainSurFaceID = _ui->widget_MainSurface->getNodeGroupId();
        int nodeGroupID = _ui->widget_SecNode->getNodeGroupId();
        int level = _ui->lineEdit_HL->text().toInt();
        double searchDistance = _ui->lineEdit_SearchDistance->text().toDouble();
        int ignoreFlag = _ui->comboBox_IgnoreSecond->currentText().toInt();
        int spotWeldFormulationFlag = _ui->comboBox_SpotwldForm->currentText().toInt();
        int searchFormulationFlag = _ui->comboBox_SearchForm->currentText().toInt();
        int nodeDeletionFlag = _ui->comboBox_NodeDeletion->currentText().toInt();
        //设置数据
        _obj->setMasterSurfaceID(mainSurFaceID);
        _obj->setNodeGroupID(nodeGroupID);
        _obj->setLevel(level);
        _obj->setSearchDistance(searchDistance);
        propValue->setIgnoreFlag(ignoreFlag);
        propValue->setSpotWeldFormulationFlag(spotWeldFormulationFlag);
        propValue->setSearchFormulationFlag(searchFormulationFlag);
        propValue->setNodeDeletionFlag(nodeDeletionFlag);
    }

    void GUIContactTieDialog::writePythonScript(const QString& oldName)
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

        Radioss::FITKInteractionKinematicTiedDefault* propValue = _obj->getValue();
        if (propValue == nullptr)
        {
            return;
        }

        QStringList script;
        script.append(QString("Solution.Solution.setCurrentSolution('%1')").arg(solutionData->getDataObjectName()));

        if (_isCreate)
        {
            script.append("p=InterTied.InterTied()");
            script.append(QString("p.createInteraction('%1')").arg(_obj->getDataObjectName()));
        }
        else
        {
            if (oldName.isEmpty())
            {
                return;
            }

            script.append(QString("p=InterTied.InterTied.GetInterTied('%1')").arg(oldName));
            if (oldName != _obj->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(_obj->getDataObjectName()));
            }
        }

        script.append(QString("p.setMasterSurface('%1')").arg(_ui->widget_MainSurface->getCurrentName()));
        script.append(QString("p.setNodeGroup('%1')").arg(_ui->widget_SecNode->getCurrentName()));
        script.append(QString("p.setLevel(%1)").arg(_obj->getLevel()));
        script.append(QString("p.setSearchDistance(%1)").arg(_obj->getSearchDistance()));
        script.append(QString("p.setIgnoreFlag(%1)").arg(propValue->getIgnoreFlag()));
        script.append(QString("p.setSpotWeldFormulationFlag(%1)").arg(propValue->getSpotWeldFormulationFlag()));
        script.append(QString("p.setSearchFormulationFlag(%1)").arg(propValue->getSearchFormulationFlag()));
        script.append(QString("p.setNodeDeletionFlag(%1)").arg(propValue->getNodeDeletionFlag()));

        this->saveScript(script);
    }

    void GUIContactTieDialog::on_pushButton_OK_clicked()
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

    void GUIContactTieDialog::on_pushButton_Cancel_clicked()
    {
        //点击取消按键动作
        this->reject();
    }

    void GUIContactTieDialog::accept()
    {
        //清除高亮
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr)operGraph->clearAllHight();

        this->QDialog::accept();
    }

    void GUIContactTieDialog::reject()
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



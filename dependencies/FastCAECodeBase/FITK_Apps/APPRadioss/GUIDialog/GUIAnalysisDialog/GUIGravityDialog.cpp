/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIGravityDialog.h"
#include "ui_GUIGravityDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include  "NodeGroupWidget.h"
#include <QMessageBox>

namespace GUI
{
    GUIGravityDialog::GUIGravityDialog(Radioss::FITKGravity* obj, Core::FITKActionOperator* oper, QWidget* parent)
        : Core::FITKDialog(parent), _obj(obj), _oper(oper),_ui(new Ui::GUIGravityDialog)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        //获取管理器
        _mgr = caseObj->getGravityManager();

        if (!_obj)
        {
            _isCreate = true;
            setWindowTitle(tr("Create Gravity"));
        }
        else
        {
            setWindowTitle(tr("Edit Gravity"));
        }
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

        connect(this, &QDialog::finished, this, &GUIGravityDialog::clearHight);

        //初始化
        this->init();
    }

    GUIGravityDialog::~GUIGravityDialog()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
    }

    void GUIGravityDialog::init()
    {
        _nodeGroup = new NodeGroupWidget(this);
        _nodeGroup->setFirstNodeGroup(tr("Global"), 0);
        _ui->horizontalLayout->insertWidget(1, _nodeGroup);
        _ui->lineEdit->setReadOnly(true);
        _ui->lineEdit->setText(Radioss::FITKGravity::GetFITKGravityRadiossKeyWord());
        _ui->comboBox_GravityDirection->addItem(tr("X"), 0);
        _ui->comboBox_GravityDirection->addItem(tr("Y"), 1);
        _ui->comboBox_GravityDirection->addItem(tr("Z"), 2);

        if (_isCreate)
        {
            _ui->lineEdit_name->setText(_mgr->checkName("Gravity-1"));
            _ui->lineEdit_FScaleY->setText(QString::number(1));
        }
        else
        {
            _ui->lineEdit_name->setText(_obj->getDataObjectName());
            Radioss::FITKGravity::GravityDirection gravityDirection = _obj->getGravityDirection();
            if (gravityDirection == Radioss::FITKGravity::GravityDirection::GRAVITY_X)
                _ui->comboBox_GravityDirection->setCurrentIndex(0);
            else if (gravityDirection == Radioss::FITKGravity::GravityDirection::GRAVITY_Y)
                _ui->comboBox_GravityDirection->setCurrentIndex(1);
            else if (gravityDirection == Radioss::FITKGravity::GravityDirection::GRAVITY_Z)
                _ui->comboBox_GravityDirection->setCurrentIndex(2);

            int nodeGroupID = _obj->getNodeGroupID();
            double FScaleY = _obj->getFScaleY();
            _ui->lineEdit_FScaleY->setText(QString::number(FScaleY));

            _nodeGroup->findNodeGroup(nodeGroupID);
        }
    }
   
    void GUIGravityDialog::getDataFormWidget()
    {
        int nodeGroupID = _nodeGroup->getNodeGroupId();
        double FScaleY = _ui->lineEdit_FScaleY->text().toDouble();

        _obj->setNodeGroupID(nodeGroupID);
        _obj->setFScaleY(FScaleY);

        if (_ui->comboBox_GravityDirection->currentData().toInt() == 0)
        {
            _obj->setGravityDirection(Radioss::FITKGravity::GravityDirection::GRAVITY_X);
        }
        else if (_ui->comboBox_GravityDirection->currentData().toInt() == 1)
        {
            _obj->setGravityDirection(Radioss::FITKGravity::GravityDirection::GRAVITY_Y);
        }
        else if (_ui->comboBox_GravityDirection->currentData().toInt() == 2)
        {
            _obj->setGravityDirection(Radioss::FITKGravity::GravityDirection::GRAVITY_Z);
        }
    }

    bool GUIGravityDialog::createObjData()
    {
        if (!_mgr) return false;
        if (!editObjData())return false;
        _mgr->appendDataObj(_obj);
        return true;
    }

    bool GUIGravityDialog::editObjData()
    {
        if (!_oper || !_obj || !_mgr) return false;
        if (!this->ckeckData()) return false;

        _oldNameForScript = _obj->getDataObjectName();
        //设置名称
        _obj->setDataObjectName(_ui->lineEdit_name->text());
        //设置界面数据
        this->getDataFormWidget();

        _oper->setArgs("objID", _obj->getDataObjectID());
        return true;
    }

    bool GUIGravityDialog::ckeckData()
    {
        //获取名称，查重处理
        QString name = _ui->lineEdit_name->text();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "", tr("Name cannot be empty."), QMessageBox::Ok);
            return false;
        }
        else if (_mgr->getDataByName(name) && name != _obj->getDataObjectName()) {
            QMessageBox::warning(this, "", tr("\"%1\" The name already exists").arg(name), QMessageBox::Ok);
            return false;
        }
        return true;
    }

    void GUIGravityDialog::writePythonScript()
    {
        if (!_obj)
            return;
        Radioss::FITKRadiossSolution* solutionData =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>()->getCurrentSolution();
        if (!solutionData)
            return;
        //保存执行的脚本
        QStringList str;
        str.append(QString("Solution.Solution.setCurrentSolution('%1')").arg(solutionData->getDataObjectName()));
       
        if (_isCreate) {
            str.append("p=Gravity.Gravity()");
            str.append(QString("p.createGravity('%1')").arg(_obj->getDataObjectName()));
        }
        else {
            QString oldName = _oldNameForScript.isEmpty() ? _obj->getDataObjectName() : _oldNameForScript;
            str.append(QString("p=Gravity.Gravity.GetGravity('%1')").arg(oldName));
            if (oldName != _obj->getDataObjectName())
            {
                str.append(QString("p.setName('%1')").arg(_obj->getDataObjectName()));
            }
        }

        str.append(QString("p.setGravityDirection('%1')").arg(_ui->comboBox_GravityDirection->currentText()));
        str.append(QString("p.setFScaleY(%1)").arg(_obj->getFScaleY()));
        str.append(QString("p.setNodeGroup('%1')").arg(_nodeGroup->getCurrentName()));

        this->saveScript(str);
    }

    void GUIGravityDialog::on_pushButtonOK_clicked()
    {
        //创建动作
        if (_isCreate)
        {
            _obj = new Radioss::FITKGravity();
            if (!this->createObjData())
                return;
            _oper->execProfession();
        }
        //修改动作
        else
        {
            if (!this->editObjData())
                return;
            _oper->execProfession();
        }
		writePythonScript();

        this->accept();
    }

    void GUIGravityDialog::on_pushButtonCancel_clicked()
    {
        //关闭窗口
        _oper->setArgs("objID", -1);
        if (_isCreate) {
            //删除对象
            if (_obj) delete _obj;
            _obj = nullptr;
        }
        this->close();
    }
    void GUIGravityDialog::clearHight()
    {
        // 获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr) return;
        // 清除高亮
        operGraph->clearAllHight();
    }
}
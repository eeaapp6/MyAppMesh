/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIBCSDialog.h"
#include "ui_GUIBCSDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include  "NodeGroupWidget.h"
#include <QMessageBox>

namespace GUI
{
    GUIBCSDialog::GUIBCSDialog(Core::FITKActionOperator* oper, QWidget* parent)
        : Core::FITKDialog(parent), _oper(oper), _isCreate(true), _ui(new Ui::GUIBCSDialog)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        setWindowTitle(tr("Create Boundary Condition "));
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //初始化界面组件
        _ui->lineEdit_keyword->setReadOnly(true);
        _nodeGroup = new NodeGroupWidget(this);
        _nodeGroup->setFirstNodeGroup(tr("None"), 0);
        _ui->horizontalLayout->insertWidget(1, _nodeGroup);
        adjustSize(); // 根据内容调整尺寸
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        //获取管理器
        _mgr = caseObj->getBCSManager();
        connect(this, &QDialog::finished, this, &GUIBCSDialog::clearHight);
        _obj = new Radioss::FITKRadiossBCS;
        //初始化
        this->init();
    }

    GUIBCSDialog::GUIBCSDialog(Radioss::FITKRadiossBCS* obj, Core::FITKActionOperator* oper, QWidget* parent)
        : Core::FITKDialog(parent), _obj(obj), _oper(oper), _isCreate(false), _ui(new Ui::GUIBCSDialog)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        setWindowTitle(tr("Edit Gravity"));
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //初始化界面组件
        _ui->lineEdit_keyword->setReadOnly(true);
        _nodeGroup = new NodeGroupWidget(this);
        _nodeGroup->setFirstNodeGroup(tr("None"), 0);
        _ui->horizontalLayout->insertWidget(1, _nodeGroup);
        adjustSize(); // 根据内容调整尺寸
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        //获取管理器
        _mgr = caseObj->getBCSManager();
        connect(this, &QDialog::finished, this, &GUIBCSDialog::clearHight);

        //初始化
        this->init();
    }

    GUIBCSDialog::~GUIBCSDialog()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
    }

    void GUIBCSDialog::init()
    {
        if (!_obj || !_nodeGroup) return;
        //获取参数
        QString name = _obj->getDataObjectName();
        if (name.isEmpty() && _mgr)
            name = _mgr->checkName("BCS-1");
        QString keyword = Radioss::FITKRadiossBCS::GetFITKRadiossBCSRadiossKeyWord();
        int skewID = _obj->getCoordinateSystemID();
        int nodeGroupID = _obj->getGroupID1();
        bool freedom1 = _obj->isFreedomFixed(1), freedom2 = _obj->isFreedomFixed(2), freedom3 = _obj->isFreedomFixed(3), 
            freedom4 = _obj->isFreedomFixed(4), freedom5 = _obj->isFreedomFixed(5), freedom6 = _obj->isFreedomFixed(6);
        //初始化界面参数
        _ui->lineEdit_name->setText(name);
        _ui->lineEdit_keyword->setText(keyword);
        _ui->checkBox_Tx->setChecked(freedom1);
        _ui->checkBox_Ty->setChecked(freedom2);
        _ui->checkBox_Tz->setChecked(freedom3);
        _ui->checkBox_Wx->setChecked(freedom4);
        _ui->checkBox_Wy->setChecked(freedom5);
        _ui->checkBox_Wz->setChecked(freedom6);
        _nodeGroup->findNodeGroup(nodeGroupID);
    }
   
    void GUIBCSDialog::getDataFormWidget()
    {
        if (!_obj || !_nodeGroup) return;
        //从界面获取数据
        int nodeGroupID = _nodeGroup->getNodeGroupId();
        int skewID = -1;
        bool freedom1 = _ui->checkBox_Tx->isChecked(), freedom2 = _ui->checkBox_Ty->isChecked(), freedom3 = _ui->checkBox_Tz->isChecked(),
            freedom4 = _ui->checkBox_Wx->isChecked(), freedom5 = _ui->checkBox_Wy->isChecked(), freedom6 = _ui->checkBox_Wz->isChecked();
        //设置数据
        _obj->setGroupID(nodeGroupID);
        _obj->setCoordinateSystemID(skewID);
        _obj->setFreedomFixed(1, freedom1);
        _obj->setFreedomFixed(2, freedom2);
        _obj->setFreedomFixed(3, freedom3);
        _obj->setFreedomFixed(4, freedom4);
        _obj->setFreedomFixed(5, freedom5);
        _obj->setFreedomFixed(6, freedom6);
    }

    bool GUIBCSDialog::ckeckData()
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

    void GUIBCSDialog::writePythonScript(const QString& oldName)
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

        QStringList script;
        script.append(QString("Solution.Solution.setCurrentSolution('%1')").arg(solutionData->getDataObjectName()));

        if (_isCreate)
        {
            script.append("p=BCS.BCS()");
            script.append(QString("p.createBCS('%1')").arg(_obj->getDataObjectName()));
        }
        else
        {
            if (oldName.isEmpty())
            {
                return;
            }
            script.append(QString("p=BCS.BCS.GetBCS('%1')").arg(oldName));
            if (oldName != _obj->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(_obj->getDataObjectName()));
            }
        }

        script.append(QString("p.setNodeGroup('%1')").arg(_nodeGroup->getCurrentName()));
        script.append(QString("p.setFreedomFixed(1, %1)").arg(_ui->checkBox_Tx->isChecked() ? "True" : "False"));
        script.append(QString("p.setFreedomFixed(2, %1)").arg(_ui->checkBox_Ty->isChecked() ? "True" : "False"));
        script.append(QString("p.setFreedomFixed(3, %1)").arg(_ui->checkBox_Tz->isChecked() ? "True" : "False"));
        script.append(QString("p.setFreedomFixed(4, %1)").arg(_ui->checkBox_Wx->isChecked() ? "True" : "False"));
        script.append(QString("p.setFreedomFixed(5, %1)").arg(_ui->checkBox_Wy->isChecked() ? "True" : "False"));
        script.append(QString("p.setFreedomFixed(6, %1)").arg(_ui->checkBox_Wz->isChecked() ? "True" : "False"));

        this->saveScript(script);
    }

    void GUIBCSDialog::on_pushButtonOK_clicked()
    {
        if (!_oper || !_obj || !_mgr) return;
        if (!this->ckeckData()) return;
        QString oldName = _obj->getDataObjectName();
        //设置名称
        _obj->setDataObjectName(_ui->lineEdit_name->text());
        //设置界面数据
        this->getDataFormWidget();
        if (_isCreate)
        {
            _mgr->appendDataObj(_obj);
        }
        _oper->setArgs("objID", _obj->getDataObjectID());
        _oper->execProfession();
        this->writePythonScript(oldName);
        this->accept();
    }

    void GUIBCSDialog::on_pushButtonCancel_clicked()
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

    void GUIBCSDialog::clearHight()
    {
        // 获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr) return;
        // 清除高亮
        operGraph->clearAllHight();
    }
}
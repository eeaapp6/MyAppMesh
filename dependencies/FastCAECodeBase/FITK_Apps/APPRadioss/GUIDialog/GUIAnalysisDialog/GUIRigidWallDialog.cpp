/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIRigidWallDialog.h"
#include "ui_GUIRigidWallDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "PickDataProvider/PickedDataProvider.h"
#include "PickDataProvider/PickedData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "NodeGroupWidget.h"
#include <QAction>

namespace GUI
{
    GUIRigidWallDialog::GUIRigidWallDialog(Radioss::FITKAbstractRWall* obj, Core::FITKActionOperator* oper, QWidget* parent)
        : Core::FITKDialog(parent), _obj(obj), _oper(oper),_ui(new Ui::GUIRigidWallDialog)
    {
        _ui->setupUi(this);
        setMinimumSize(0, 0); // 允许窗口任意缩小
        adjustSize(); // 根据内容调整尺寸
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        //获取管理器
        _mgr = caseObj->getRWallManager();

        if (!_obj)
            _isCreate = true;

        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        //获取拾取器，绑定拾取结束信号
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        //清除拾取对象所有的连接
        if (pickProvider != nullptr)
        {
            connect(pickProvider, &GraphData::PickedDataProvider::sigPickOver, this, &GUIRigidWallDialog::pickFinishedOper);
        }
        connect(this, &QDialog::finished, this, &GUIRigidWallDialog::clearHight);

        this->initUI();
    }

    GUIRigidWallDialog::~GUIRigidWallDialog()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
        //所有的类在执行完毕后，将清空对3维的操作
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        if (!pickProvider) return;
        pickProvider->clearPickedData();
        ////拾取对象 关闭
        ////拾取方式 空
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBNone, GUI::GUIPickInfo::PickMethod::PMNone);
    }
    void GUIRigidWallDialog::writePythonScript()
    {
        //虚函数重写
    }
    void GUIRigidWallDialog::initUI()
    {
        _nodeGroup1 = new NodeGroupWidget(this);
        _nodeGroup2 = new NodeGroupWidget(this);
        
        _nodeGroup1->setFirstNodeGroup(tr("None"), 0);
        _nodeGroup2->setFirstNodeGroup(tr("None"), 0);


        _ui->horizontalLayout->insertWidget(0, _nodeGroup1);
        _ui->horizontalLayout_3->insertWidget(0, _nodeGroup2);

        _ui->comboBox_Sliding->addItem(tr("Sliding"), 0);
        _ui->comboBox_Sliding->addItem(tr("Tied"), 1);
        _ui->comboBox_Sliding->addItem(tr("Sliding With Friction"), 2);

    }
    void GUIRigidWallDialog::pickFinishedOper()
    {
        //获取拾取数据
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        if (!pickProvider) return;

        QList<GraphData::PickedData*> pickList = pickProvider->getPickedList();
        QList<int> ids;
        for (GraphData::PickedData* pickData : pickList) {
            if (pickData == nullptr)continue;
            for (GraphData::PickDataInfo i : pickData->getPickOtherInfos())
            {
                //添加拾取数据
                ids.push_back(i._id);
            }
        }
        if (ids.isEmpty()) return;
        int id = ids.at(0);
        if (_pickFlag == PickFlag::Node1)
        {
            std::array<double, 3> xyz = this->getXYZ(id);
            _ui->lineEdit_X->setText(QString::number(xyz.at(0)));
            _ui->lineEdit_Y->setText(QString::number(xyz.at(1)));
            _ui->lineEdit_Z->setText(QString::number(xyz.at(2)));
        }
        else if (_pickFlag == PickFlag::Node2)
        {
            std::array<double, 3> xyz = this->getXYZ(id);
            _ui->lineEdit_X_2->setText(QString::number(xyz.at(0)));
            _ui->lineEdit_Y_2->setText(QString::number(xyz.at(1)));
            _ui->lineEdit_Z_2->setText(QString::number(xyz.at(2)));
        }
    }
    bool GUIRigidWallDialog::createObjData()
    {
        if (!_mgr) return false;
        if (!editObjData())return false;
        _mgr->appendDataObj(_obj);
        return true;
    }

    bool GUIRigidWallDialog::editObjData()
    {
        if (!_oper || !_mgr) return false;
        if (!this->ckeckData()) return false;
        //设置界面数据
        this->getDataFormWidget();
        //设置名称
        _oldNameForScript = _obj->getDataObjectName();
        _obj->setDataObjectName(_ui->lineEdit_name->text());
        _oper->setArgs("objID", _obj->getDataObjectID());
        return true;
    }

    bool GUIRigidWallDialog::ckeckData()
    {
        //虚函数重写
        return true;
    }

    void GUIRigidWallDialog::on_pushButtonOK_clicked()
    {
        //创建动作
        if (_isCreate)
        {
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

    void GUIRigidWallDialog::on_pushButtonCancel_clicked()
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
    void GUIRigidWallDialog::clearHight()
    {
        // 获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr) return;
        // 清除高亮
        operGraph->clearAllHight();
    }
    void GUIRigidWallDialog::on_comboBox_Sliding_currentIndexChanged(int index)
    {
        if (index == 0 || index == 1)
        {
            _ui->label_6->hide();
            _ui->lineEdit_frictionCoef->setText(QString::number(0));
            _ui->lineEdit_frictionCoef->hide();
        }
        else
        {
            _ui->label_6->show();
            _ui->lineEdit_frictionCoef->show();
        }
    }

    void GUIRigidWallDialog::on_pushButton_PickNode1_clicked()
    {
        _pickFlag = PickFlag::Node1;
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBMeshVert, GUI::GUIPickInfo::PickMethod::PMSingle);
    }
    void GUIRigidWallDialog::on_pushButton_PickNode2_clicked()
    {
        _pickFlag = PickFlag::Node2;
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBMeshVert, GUI::GUIPickInfo::PickMethod::PMSingle);
    }
    void GUIRigidWallDialog::enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType)
    {
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType(objType);
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod(methodType);
        //设置拾取方式
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
    }
    std::array<double, 3> GUIRigidWallDialog::getXYZ(int id)
    {
        std::array<double, 3> xyz = { -1,-1,-1 };
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return xyz;
        Radioss::FITKRadiossMeshModel* meshModel = caseObj->getMeshModel();
        if (!meshModel) return xyz;
        Radioss::FITKRadiossNodes* nodes = meshModel->getNodes();
        if (!nodes) return xyz;
        Core::FITKNode* ndoe = nodes->getNodeByID(id);
        if (!ndoe) return xyz;
        double coor[3] = {0.0,0.0,0.0 };
        ndoe->getCoor(coor);
        xyz.at(0) = coor[0];
        xyz.at(1) = coor[1];
        xyz.at(2) = coor[2];
        return xyz;
    }

}
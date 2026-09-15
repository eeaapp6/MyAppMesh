/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIRBodyDialog.h"
#include "ui_GUIRBodyDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKConnectionRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "GraphDataVTKAdaptor/GraphVTKCommons.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
// Pick
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/PickedDataProvider.h"
#include "PickDataProvider/PickedData.h"

#include <QMessageBox>

namespace GUI
{
    GUIRBodyDialog::GUIRBodyDialog(Core::FITKActionOperator * oper, QWidget * parent)
        : Core::FITKDialog(parent), _oper(oper), _ui(new Ui::GUIRBodyDialog)
    {
        _ui->setupUi(this);
        //创建数据
        _isCreate = true;
        _obj = new Radioss::FITKConnectionRBODY;
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        //获取管理器
        _mgr = caseObj->getConnectionManager();
        //设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        this->initWidget();
        this->init();
    }

    GUIRBodyDialog::GUIRBodyDialog(Radioss::FITKConnectionRBODY * obj, Core::FITKActionOperator * oper, QWidget * parent)
        : Core::FITKDialog(parent), _obj(obj), _oper(oper), _ui(new Ui::GUIRBodyDialog)
    {
        _ui->setupUi(this);
        _isCreate = false;
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
            getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return;
        //获取管理器
        _mgr = caseObj->getConnectionManager();
        //设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        this->initWidget();
        this->init();
    }

    GUIRBodyDialog::~GUIRBodyDialog()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
        // 获取操作器
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr) return;
        // 清除高亮
        operGraph->clearAllHight();
    }

    void GUIRBodyDialog::init()
    {
        if (_obj == nullptr || _mgr == nullptr) return;
        //获取参数
        int nodeID = _obj->getMainNodeID();
        int sensorID = _obj->getSensorID();
        int systemID = _obj->getSystemID();
        int inertiaType = _obj->getInertiaType();
        double mass = _obj->getMass();
        int nodeSetID = _obj->getSecondaryNodesGroupID();
        bool ikrem = _obj->getDeactivationRigidWall();
        int gravityCenter = _obj->getGravityCenterComputation();
        int surfID = _obj->getEnvelopeSurface();
        double inertiaJ[6]{ 0.0 };
        _obj->setInertiaValue(inertiaJ);
        bool ioptoff = _obj->getDomainDecomposition();
        //设置名称
        if (_isCreate)
            _ui->lineEdit_Name->setText(_mgr->checkName("RigidBody-1"));
        else
            _ui->lineEdit_Name->setText(_obj->getDataObjectName());
        //设置界面参数
        if (nodeID > 0) _ui->checkBox_Auto->setChecked(false);
        _ui->lineEdit_MainNodeID->setText(QString::number(nodeID));
        int index = _ui->comboBox_Ispher->findData(inertiaType);
        _ui->comboBox_Ispher->setCurrentIndex(index);
        _ui->lineEdit_Mass->setText(QString::number(mass));
        _ui->widget_SecNode->findNodeGroup(nodeSetID);
        index = _ui->comboBox_Ikrem->findData((ikrem ? 1 : 0));
        _ui->comboBox_Ikrem->setCurrentIndex(index);
        index = _ui->comboBox_ICoG->findData(gravityCenter);
        _ui->comboBox_ICoG->setCurrentIndex(index);
        _ui->widget_Surf->findNodeGroup(surfID);
        _ui->lineEdit_Jxx->setText(QString::number(inertiaJ[0]));
        _ui->lineEdit_Jyy->setText(QString::number(inertiaJ[1]));
        _ui->lineEdit_Jzz->setText(QString::number(inertiaJ[2]));
        _ui->lineEdit_Jxy->setText(QString::number(inertiaJ[3]));
        _ui->lineEdit_Jyz->setText(QString::number(inertiaJ[4]));
        _ui->lineEdit_Jxz->setText(QString::number(inertiaJ[5]));
        index = _ui->comboBox_Ioptoff->findData((ioptoff ? 1 : 0));
        _ui->comboBox_Ioptoff->setCurrentIndex(index);
    }

    void GUIRBodyDialog::on_pushButtonOK_clicked()
    {
        //点击确定按键动作
        if (!_mgr || !_obj || !_oper) return;
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

    void GUIRBodyDialog::on_pushButtonCancel_clicked()
    {
        //点击取消按键动作
        this->reject();
    }

    void GUIRBodyDialog::on_pushButton_MainNodeID_clicked()
    {
        //获取拾取器，绑定拾取结束信号
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        connect(pickProvider, &GraphData::PickedDataProvider::sigDataPicked, this, &GUIRBodyDialog::pickData_MainNode_Finished);
        //设置拾取方式
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBMeshVert;
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
        _ui->pushButton_MainNodeID->setStyleSheet("QPushButton { background-color: green; }");
    }

    void GUIRBodyDialog::pickData_MainNode_Finished()
    {
        //清除拾取对象所有的连接
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        QObject::disconnect(pickProvider, SIGNAL(sigDataPicked()), this, nullptr);
        if (!pickProvider)return;
        //获取拾取数据对象
        int nodeID = 0;
        QList<GraphData::PickedData*> pickList = pickProvider->getPickedList();
        for (GraphData::PickedData* pickData : pickList)
        {
            if (pickData == nullptr) continue;
            QList<GraphData::PickDataInfo> pickInfos = pickData->getPickOtherInfos();
            if (pickInfos.isEmpty()) continue;
            nodeID = pickInfos[0]._id;
        }
        _ui->lineEdit_MainNodeID->setText(QString::number(nodeID));
        //清理拾取操作器，并关闭拾取模式
        pickProvider->clearPickedData();
        GUI::GUIPickInfo::ClearPickInfo();
        _ui->pushButton_MainNodeID->setStyleSheet("");
    }

    void GUIRBodyDialog::on_checkBox_Auto_stateChanged(int state)
    {
        if (state == Qt::CheckState::Checked)
        {
            _ui->lineEdit_MainNodeID->setEnabled(false);
            _ui->pushButton_MainNodeID->setEnabled(false);
        }
        else
        {
            _ui->lineEdit_MainNodeID->setEnabled(true);
            _ui->pushButton_MainNodeID->setEnabled(true);
        }
    }

    void GUIRBodyDialog::initWidget()
    {
        //开启信号拦截
        this->blockSignals(true);
        //初始化组件
        this->initSensorPropComboBox();
        this->initSkewComboBox();
        _ui->widget_Surf->setWidgetGroupType(WidgetGroupType::Surface_Element);
        _ui->widget_SecNode->setWidgetGroupType(WidgetGroupType::Set_Node);
        _ui->widget_Surf->setFirstNodeGroup(tr("None"), 0);
        _ui->widget_SecNode->setFirstNodeGroup(tr("None"), 0);
        _ui->lineEdit_KeyWord->setText(Radioss::FITKConnectionRBODY::GetFITKConnectionRBODYRadiossKeyWord());
        //初始化主节点
        _ui->checkBox_Auto->setChecked(true);
        //初始化下拉框Ispher
        _ui->comboBox_Ispher->addItem(QObject::tr("Default"), 0);
        _ui->comboBox_Ispher->addItem(QObject::tr("Spherical"), 1);
        _ui->comboBox_Ispher->addItem(QObject::tr("Auto"), 2);
        _ui->comboBox_Ispher->addItem(QObject::tr("Input"), 3);
        //初始化下拉框Ikrem
        _ui->comboBox_Ikrem->addItem(QObject::tr("Remove secondary nodes"), 0);
        _ui->comboBox_Ikrem->addItem(QObject::tr("Not remove secondary nodes"), 1);
        //初始化下拉框ICoG
        _ui->comboBox_ICoG->addItem(QObject::tr("Default"), 0);
        _ui->comboBox_ICoG->addItem(QObject::tr("Main Secondary Node Coord"), 1);
        _ui->comboBox_ICoG->addItem(QObject::tr("Secondary Mass"), 2);
        _ui->comboBox_ICoG->addItem(QObject::tr("Main Node"), 3);
        _ui->comboBox_ICoG->addItem(QObject::tr("Main Node Ignored Secondary"), 4);
        //初始化下拉框Ioptoff
        _ui->comboBox_Ioptoff->addItem(QObject::tr("Not taken into account"), 0);
        _ui->comboBox_Ioptoff->addItem(QObject::tr("Taken into account"), 1);
        //关闭信号拦截
        this->blockSignals(false);
    }

    void GUIRBodyDialog::initSensorPropComboBox()
    {
        _ui->comboBox_sens_ID->addItem("None", -1);
    }

    void GUIRBodyDialog::initSkewComboBox()
    {
        _ui->comboBox_Skew_ID->addItem("None", -1);
    }

    bool GUIRBodyDialog::createObjData()
    {
        if (!_mgr || !_oper || !_obj) return false;

        //获取板名称-查重处理
        QString name = _ui->lineEdit_Name->text();
        if (_mgr->getDataByName(name)) {
            QMessageBox::warning(this, "", tr("\"%1\" already exists and cannot be overwritten.").arg(name), QMessageBox::Ok);
            return false;
        }
        //设置名称
        _obj->setDataObjectName(name);
        //设置界面数据
        this->getDataFormWidget();
        //设置数据
        _mgr->appendDataObj(_obj);
        _oper->setArgs("objID", _obj->getDataObjectID());
        return true;
    }

    bool GUIRBodyDialog::editObjData()
    {
        if (!_mgr || !_oper || !_obj) return false;

        //获取板名称-查重处理
        QString name = _ui->lineEdit_Name->text();
        //名称重复判断
        if (name != _obj->getDataObjectName())
            if (_mgr->getDataByName(name)) {
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

    void GUIRBodyDialog::getDataFormWidget()
    {
        if (!_obj) return;
        //从界面获取数据
        bool isAuto = _ui->checkBox_Auto->checkState() == Qt::CheckState::Checked;
        int nodeID = _ui->lineEdit_MainNodeID->text().toInt();
        int sensorID = _ui->comboBox_sens_ID->currentData().toInt();
        int systemID = _ui->comboBox_Skew_ID->currentData().toInt();
        int inertiaType = _ui->comboBox_Ispher->currentData().toInt();
        double mass = _ui->lineEdit_Mass->text().toDouble();
        int nodeSetID = _ui->widget_SecNode->getNodeGroupId();
        bool ikrem = _ui->comboBox_Ikrem->currentData().toInt() > 0 ? true : false;
        int gravityCenter = _ui->comboBox_ICoG->currentData().toInt();
        int surfID = _ui->widget_Surf->getNodeGroupId();
        double inertiaJ[6]{ 0.0 };
        inertiaJ[0] = _ui->lineEdit_Jxx->text().toDouble();
        inertiaJ[1] = _ui->lineEdit_Jyy->text().toDouble();
        inertiaJ[2] = _ui->lineEdit_Jzz->text().toDouble();
        inertiaJ[3] = _ui->lineEdit_Jxy->text().toDouble();
        inertiaJ[4] = _ui->lineEdit_Jyz->text().toDouble();
        inertiaJ[5] = _ui->lineEdit_Jxz->text().toDouble();
        bool ioptoff = _ui->comboBox_Ioptoff->currentData().toInt() > 0 ? true : false;
        //数据对象设置数据
        if (isAuto) nodeID = this->getCenterNodeIDBySetNodeID(nodeSetID);
        _obj->setMainNodeID(nodeID);
        _obj->setSensorID(sensorID);
        _obj->setSystemID(systemID);
        _obj->setInertiaType(Radioss::FITKConnectionRBODY::RbodyIspherType(inertiaType));
        _obj->setMass(mass);
        _obj->setSecondaryNodesGroupID(nodeSetID);
        _obj->setDeactivationRigidWall(ikrem);
        _obj->setGravityCenterComputation(Radioss::FITKConnectionRBODY::GravityCenterComputationType(gravityCenter));
        _obj->setEnvelopeSurface(surfID);
        _obj->setInertiaValue(inertiaJ);
        _obj->setDomainDecomposition(ioptoff);
    }

    void GUIRBodyDialog::writePythonScript(const QString& oldName)
    {
        if (_obj == nullptr)
        {
            return;
        }

        QStringList script;
        if (_isCreate)
        {
            script.append("p=RigidBody.RigidBody()");
            script.append(QString("p.createRigidBody('%1')").arg(_obj->getDataObjectName()));
        }
        else
        {
            if (oldName.isEmpty())
            {
                return;
            }

            script.append(QString("p=RigidBody.RigidBody.GetRigidBody('%1')").arg(oldName));
            if (oldName != _obj->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(_obj->getDataObjectName()));
            }
        }

        script.append(QString("p.setMainNodeID(%1)").arg(_obj->getMainNodeID()));
        script.append(QString("p.setInertiaType(%1)").arg(_obj->getInertiaType()));
        script.append(QString("p.setMass(%1)").arg(_obj->getMass()));

        if (_obj->getSecondaryNodesGroupID() > 0)
        {
            script.append(QString("p.setSecondaryNodesGroup('%1')").arg(_ui->widget_SecNode->getCurrentName()));
        }

        script.append(QString("p.setDeactivationRigidWall(%1)").arg(_obj->getDeactivationRigidWall() ? 1 : 0));
        script.append(QString("p.setGravityCenterComputation(%1)").arg(_obj->getGravityCenterComputation()));

        if (_obj->getEnvelopeSurface() > 0)
        {
            script.append(QString("p.setEnvelopeSurface('%1')").arg(_ui->widget_Surf->getCurrentName()));
        }

        double inertiaValue[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        _obj->getInertiaValue(inertiaValue);
        for (int index = 0; index < 6; ++index)
        {
            script.append(QString("p.setInertiaValue(%1, %2)").arg(index).arg(inertiaValue[index]));
        }

        script.append(QString("p.setDomainDecomposition(%1)").arg(_obj->getDomainDecomposition() ? 1 : 0));

        this->saveScript(script);
    }

    int GUIRBodyDialog::getCenterNodeIDBySetNodeID(int setNodeID)
    {
        //获取RadiossCase
        Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseObj == nullptr) return -1;
        Radioss::FITKRadiossMeshModel* meshModel = caseObj->getMeshModel();
        if (!meshModel) return -1;
        if (Exchange::GraphVTKCommons::_unusedNodesPartId <0)
        {
            Radioss::FITKRadiossPartManager* mgr = meshModel->getPartsManager();
            if (!mgr) return -1;
            Radioss::FITKRadiossPart* part = new Radioss::FITKRadiossPart();
            part->setDataObjectName(mgr->checkName(tr("Part-1")));
            mgr->appendDataObj(part);
            Exchange::GraphVTKCommons::_unusedNodesPartId = part->getDataObjectID();
            _oper->setArgs("partID", Exchange::GraphVTKCommons::_unusedNodesPartId);
        }
        Radioss::FITKRadiossNodes* nodes = meshModel->getNodes();
        Interface::FITKComponentManager* componentMgr = meshModel->getComponentManager();
        if (!componentMgr || !nodes) return -1;
        Interface::FITKModelSet* set = dynamic_cast<Interface::FITKModelSet*>(componentMgr->getDataByID(setNodeID));
        if (!set) return -1;
        //获取节点集合ID链表
        QList<int> nodeIDs = set->getAbsoluteMember();
        double coorMin[3] = { 0.0, 0.0, 0.0 };
        double coorMax[3] = { 0.0, 0.0, 0.0 };
        int nodeNum = 0;
        //获取类盒子范围
        for (int i = 0; i < nodeIDs.size(); ++i)
        {
            Core::FITKNode* node = nodes->getNodeByID(nodeIDs[i]);
            if (!node) continue;
            double coor[3] = { 0.0, 0.0, 0.0 };
            node->getCoor(coor);
            if (i == 0)
            {
                coorMin[0] = coor[0];
                coorMin[1] = coor[1];
                coorMin[2] = coor[2];
                coorMax[0] = coor[0];
                coorMax[1] = coor[1];
                coorMax[2] = coor[2];
            }
            if (coorMin[0] > coor[0])
                coorMin[0] = coor[0];
            if (coorMin[1] > coor[1])
                coorMin[1] = coor[1];
            if (coorMin[2] > coor[2])
                coorMin[2] = coor[2];
            if (coorMax[0] < coor[0])
                coorMax[0] = coor[0];
            if (coorMax[1] < coor[1])
                coorMax[1] = coor[1];
            if (coorMax[2] < coor[2])
                coorMax[2] = coor[2];
            ++nodeNum;
        }
        //判断节点集的点数量是否大于等于2
        if (nodeNum < 2) return -1;
        int nID = nodes->addNode((coorMin[0] + coorMax[0]) / 2, (coorMin[1] + coorMax[1]) / 2, (coorMin[2] + coorMax[2]) / 2);
        return nID;
    }

}
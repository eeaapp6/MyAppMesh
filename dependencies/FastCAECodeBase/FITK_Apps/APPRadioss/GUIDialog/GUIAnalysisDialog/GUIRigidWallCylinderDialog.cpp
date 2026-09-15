/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIRigidWallCylinderDialog.h"
#include "ui_GUIRigidWallDialog.h"

#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <QMessageBox>

namespace GUI
{
    GUIRigidWallCylinderDialog::GUIRigidWallCylinderDialog(Radioss::FITKAbstractRWall* obj, Core::FITKActionOperator* oper, QWidget* parent)
        : GUIRigidWallDialog(obj, oper, parent)
    {
        QString RigidWalllName;
        _oper->argValue("RigidWallName", RigidWalllName);
        if (!_obj) {
            _ui->lineEdit_name->setReadOnly(true);
            setWindowTitle(tr("Create Rigid Wall"));
        }
        else
        {
            setWindowTitle(tr("Edit Rigid Wall"));
        }
        _ui->groupBox_2->setTitle(tr("Tool Point"));
        this->init();
    }

    bool GUIRigidWallCylinderDialog::ckeckData()
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
        if (_ui->lineEdit_diameter->text().toDouble() <= 0)
        {
            QMessageBox::warning(this, "", tr("Diameter Error"), QMessageBox::Ok);
            return false;
        }
        if (_ui->lineEdit_X_2->text().toDouble() == 0 && _ui->lineEdit_Y_2->text().toDouble() == 0 && _ui->lineEdit_Z_2->text().toDouble() == 0)
        {
            QMessageBox::warning(this, "", tr("Invalid Tool Point"), QMessageBox::Ok);
            return false;
        }
        return true;
    }
    void GUIRigidWallCylinderDialog::init()
    {
        _ui->lineEdit->setReadOnly(true);
        _ui->lineEdit_model->setReadOnly(true);
        //设置key word
        _ui->lineEdit->setText(Radioss::FITKRWallCylinder::GetFITKRWallCylinderRadiossKeyWord());
        //设置类型
        _ui->lineEdit_model->setText("Cylinder");
        if (_isCreate)
        {
            QString rigidWalllName;
            _oper->argValue("RigidWallName", rigidWalllName);
            _ui->lineEdit_name->setReadOnly(true);
            _ui->lineEdit_name->setText(rigidWalllName);
        }
        else
        {
            Radioss::FITKRWallCylinder* plane = dynamic_cast<Radioss::FITKRWallCylinder*>(_obj);
            if (!plane) return;
            //设置名称
            _ui->lineEdit_name->setText(plane->getDataObjectName());
            Radioss::FITKAbstractRWall::RWallSliding slidingType = plane->getSlidingType();
            if (slidingType == Radioss::FITKAbstractRWall::RWallSliding::Sliding)
                _ui->comboBox_Sliding->setCurrentIndex(0);
            else if (slidingType == Radioss::FITKAbstractRWall::RWallSliding::Tied)
                _ui->comboBox_Sliding->setCurrentIndex(1);
            else if (slidingType == Radioss::FITKAbstractRWall::RWallSliding::SlidingWithFriction)
                _ui->comboBox_Sliding->setCurrentIndex(2);

            int nodeId = plane->getNodeId();
            int nodeGroupId1 = plane->getNodeGroupId1();
            int nodeGroupId2 = plane->getNodeGroupId2();
            double DSearch = plane->getDSearch();
            double frictionCoef = plane->getFrictionCoef();
            double diameter = plane->getDiameter();
            double filteringFactor = plane->getFilteringFactor();
            int filteringFlag = plane->getFilteringFlag();
            double coordinatesM[3] = {};
            plane->getBasicPoint(coordinatesM[0], coordinatesM[1], coordinatesM[2]);
            double coordinatesM1[3] = {};
            plane->getAxisVector(coordinatesM1[0], coordinatesM1[1], coordinatesM1[2]);

            _ui->lineEdit_nodeID->setText(QString::number(nodeId));
            _ui->lineEdit_SearchDistance->setText(QString::number(DSearch));
            _ui->lineEdit_frictionCoef->setText(QString::number(frictionCoef));
            _ui->lineEdit_diameter->setText(QString::number(diameter));
            _ui->lineEdit_filteringFactor->setText(QString::number(filteringFactor));
            _ui->lineEdit_filtingFlag->setText(QString::number(filteringFlag));
            _ui->lineEdit_X->setText(QString::number(coordinatesM[0]));
            _ui->lineEdit_Y->setText(QString::number(coordinatesM[1]));
            _ui->lineEdit_Z->setText(QString::number(coordinatesM[2]));
            _ui->lineEdit_X_2->setText(QString::number(coordinatesM1[0] + coordinatesM[0]));
            _ui->lineEdit_Y_2->setText(QString::number(coordinatesM1[1] + coordinatesM[1]));
            _ui->lineEdit_Z_2->setText(QString::number(coordinatesM1[2] + coordinatesM[2]));
            _nodeGroup1->findNodeGroup(nodeGroupId1);
            _nodeGroup2->findNodeGroup(nodeGroupId2);
        }
    }
    void GUIRigidWallCylinderDialog::getDataFormWidget()
    {
        if (_isCreate)
        {
            _obj = new Radioss::FITKRWallCylinder();

        }
        Radioss::FITKRWallCylinder* plane = dynamic_cast<Radioss::FITKRWallCylinder*>(_obj);
        if (!plane) return;

        int nodeId = _ui->lineEdit_nodeID->text().toInt();
        int nodeGroupId1 = _nodeGroup1->getNodeGroupId();
        int nodeGroupId2 = _nodeGroup2->getNodeGroupId();
        double DSearch = _ui->lineEdit_SearchDistance->text().toDouble();
        double frictionCoef = _ui->lineEdit_frictionCoef->text().toDouble();
        double diameter = _ui->lineEdit_diameter->text().toDouble();
        double filteringFactor = _ui->lineEdit_filteringFactor->text().toDouble();
        int filteringFlag = _ui->lineEdit_filtingFlag->text().toInt();
        double coordinatesM[3] = { _ui->lineEdit_X->text().toDouble(),
                                   _ui->lineEdit_Y->text().toDouble(),
                                   _ui->lineEdit_Z->text().toDouble(), };
        double coordinatesM1[3] = { _ui->lineEdit_X_2->text().toDouble() - _ui->lineEdit_X->text().toDouble(),
                                   _ui->lineEdit_Y_2->text().toDouble() - _ui->lineEdit_Y->text().toDouble(),
                                   _ui->lineEdit_Z_2->text().toDouble() - _ui->lineEdit_Z->text().toDouble(), };
        plane->setNodeId(nodeId);
        plane->setNodeGroupId1(nodeGroupId1);
        plane->setNodeGroupId2(nodeGroupId2);
        plane->setDSearch(DSearch);
        plane->setFrictionCoef(frictionCoef);
        plane->setDiameter(diameter);
        plane->setFilteringFactor(filteringFactor);
        plane->setFilteringFlag(filteringFlag);
        plane->setBasicPoint(coordinatesM[0], coordinatesM[1], coordinatesM[2]);
        plane->setAxisVector(coordinatesM1[0], coordinatesM1[1], coordinatesM1[2]);

        if (_ui->comboBox_Sliding->currentData().toInt() == 0)
        {
            plane->setSlidingType(Radioss::FITKAbstractRWall::RWallSliding::Sliding);
        }
        else if (_ui->comboBox_Sliding->currentData().toInt() == 1)
        {
            plane->setSlidingType(Radioss::FITKAbstractRWall::RWallSliding::Tied);
        }
        else if (_ui->comboBox_Sliding->currentData().toInt() == 2)
        {
            plane->setSlidingType(Radioss::FITKAbstractRWall::RWallSliding::SlidingWithFriction);
        }
    }

    void GUIRigidWallCylinderDialog::writePythonScript()
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

        Radioss::FITKRWallCylinder* cylinder = dynamic_cast<Radioss::FITKRWallCylinder*>(_obj);
        if (cylinder == nullptr)
        {
            return;
        }

        double basicPoint[3] = { 0.0, 0.0, 0.0 };
        cylinder->getBasicPoint(basicPoint[0], basicPoint[1], basicPoint[2]);
        double axisVector[3] = { 0.0, 0.0, 0.0 };
        cylinder->getAxisVector(axisVector[0], axisVector[1], axisVector[2]);

        QStringList script;
        script.append(QString("Solution.Solution.setCurrentSolution('%1')").arg(solutionData->getDataObjectName()));

        if (_isCreate)
        {
            script.append("p=RWall.RWallCylinder()");
            script.append(QString("p.createRWall('%1')").arg(cylinder->getDataObjectName()));
        }
        else
        {
            QString oldName = _oldNameForScript.isEmpty() ? cylinder->getDataObjectName() : _oldNameForScript;
            script.append(QString("p=RWall.RWallCylinder.GetRWallCylinder('%1')").arg(oldName));
            if (oldName != cylinder->getDataObjectName())
            {
                script.append(QString("p.setName('%1')").arg(cylinder->getDataObjectName()));
            }
        }

        //script.append(QString("p.setNodeId(%1)").arg(cylinder->getNodeId()));
        script.append(QString("p.setSlidingType(%1)").arg((int)cylinder->getSlidingType()));
        script.append(QString("p.setDSearch(%1)").arg(cylinder->getDSearch()));
        if(cylinder->getSlidingType() == Radioss::FITKAbstractRWall::RWallSliding::SlidingWithFriction)
        {
            script.append(QString("p.setFrictionCoef(%1)").arg(cylinder->getFrictionCoef()));
        }
        script.append(QString("p.setDiameter(%1)").arg(cylinder->getDiameter()));
        script.append(QString("p.setFilteringFactor(%1)").arg(cylinder->getFilteringFactor()));
        script.append(QString("p.setFilteringFlag(%1)").arg(cylinder->getFilteringFlag()));
        script.append(QString("p.setNodeGroup1('%1')").arg(_nodeGroup1->getCurrentName()));
        script.append(QString("p.setNodeGroup2('%1')").arg(_nodeGroup2->getCurrentName()));
        script.append(QString("p.setBasicPoint(%1, %2, %3)")
            .arg(basicPoint[0]).arg(basicPoint[1]).arg(basicPoint[2]));
        script.append(QString("p.setAxisVector(%1, %2, %3)")
            .arg(axisVector[0]).arg(axisVector[1]).arg(axisVector[2]));

        this->saveScript(script);
    }
}
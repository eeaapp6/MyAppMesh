/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PHengLEIBoundaryCreateDialog.h"
#include "ui_PHengLEIBoundaryCreateDialog.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIBoundary.h"

#include <QObject>

//类型注册
Q_DECLARE_METATYPE(Interface::FITKPHengLEIBoundary::PHengLEIBoundaryType)

namespace GUI
{
    PHengLEIBoundaryCreateDialog::PHengLEIBoundaryCreateDialog(EventOper::ParaWidgetInterfaceOperator * oper) :
        GUIDialogBase(FITKAPP->getGlobalData()->getMainWindow()), _oper(oper)
    {
        _ui = new Ui::PHengLEIBoundaryCreateDialog();
        _ui->setupUi(this);

        _physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
        _factoryData = FITKAPP->getComponents()->getComponentTByName<Interface::FITKPhysicsHandlerFactory>("FITKPHengLEIPhysicsHandlerFactory");
        init();

        this->setWindowTitle(tr("Create Boundary"));
    }

    PHengLEIBoundaryCreateDialog::~PHengLEIBoundaryCreateDialog()
    {
        if (_ui)delete _ui;
    }

    void PHengLEIBoundaryCreateDialog::init()
    {
        if (_physicsData == nullptr)return;
        auto boundManager = _physicsData->getBoundaryManager();
        if (boundManager == nullptr)return;
        auto globalData = FITKAPP->getGlobalData();
        if (globalData == nullptr)return;
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData< Interface::FITKUnstructuredFluidMeshVTK>();
        if (meshData == nullptr)return;
        Interface::FITKBoundaryMeshVTKManager* boundMeshManager = meshData->getBoundaryMeshManager();
        if (boundMeshManager == nullptr)return;

        //名称添加
        _ui->lineEdit_Name->setText(boundManager->checkName("Boundary-0"));

        //边界选择添加
        for (int i = 0; i < boundMeshManager->getDataCount(); i++) {
            Interface::FITKBoundaryMeshVTK* boundMesh = boundMeshManager->getDataByIndex(i);
            if (boundMesh == nullptr)continue;
            if (boundManager->getBoundary(boundMesh->getDataObjectID()))continue;
            _ui->comboBox_Boundary->addItem(boundMesh->getDataObjectName(), boundMesh->getDataObjectID());
        }
        if (boundManager->getDataCount() > 0) _ui->comboBox_Boundary->setCurrentIndex(0);

		//边界类型添加
		_ui->comboBox_Type->addItem(tr("Wall"), Interface::FITKPHengLEIBoundary::PHBWall);
		_ui->comboBox_Type->addItem(tr("Pressure Inlet"), Interface::FITKPHengLEIBoundary::PHBPressureInlet);
		_ui->comboBox_Type->addItem(tr("Pressure Outlet"), Interface::FITKPHengLEIBoundary::PHBPressureOutlet);
		_ui->comboBox_Type->addItem(tr("Farfield"), Interface::FITKPHengLEIBoundary::PHBFarField);
		_ui->comboBox_Type->addItem(tr("InFlow"), Interface::FITKPHengLEIBoundary::PHBInFlow);
		_ui->comboBox_Type->addItem(tr("Mass FlowInlet"), Interface::FITKPHengLEIBoundary::PHBMassFlowInlet);
		_ui->comboBox_Type->addItem(tr("Mass FlowOutlet"), Interface::FITKPHengLEIBoundary::PHBMassFlowOutlet);
		_ui->comboBox_Type->addItem(tr("SYMMETRY"), Interface::FITKPHengLEIBoundary::PHBSYMMETRY);
		_ui->comboBox_Type->addItem(tr("OutFlow"), Interface::FITKPHengLEIBoundary::PhBOutFlow);
    }

    void PHengLEIBoundaryCreateDialog::hideEvent(QHideEvent * event)
    {
        GUIDialogBase::hideEvent(event);
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->clearHighlight();
    }

    void PHengLEIBoundaryCreateDialog::showEvent(QShowEvent * event)
    {
        GUIDialogBase::showEvent(event);
        int id = -1;
        if (_ui->comboBox_Boundary->count() > 0) {
            id = _ui->comboBox_Boundary->currentData().toInt();
        }
        highlightMeshBoundary(id);
    }

    void PHengLEIBoundaryCreateDialog::on_comboBox_Boundary_activated(int index)
    {
        Q_UNUSED(index);
        highlightMeshBoundary(_ui->comboBox_Boundary->currentData().toInt());
    }

    void PHengLEIBoundaryCreateDialog::on_pushButton_OK_clicked()
    {
        if (_ui->comboBox_Boundary->count() == 0)return;

        if (_physicsData == nullptr)return;
        auto boundManager = _physicsData->getBoundaryManager();
        if (boundManager == nullptr)return;
        if (_factoryData == nullptr)return;

        QString name = _ui->lineEdit_Name->text();
        _factoryData->setBoundary(_ui->comboBox_Boundary->currentData().toInt(), 
            _ui->comboBox_Type->currentData().value<Interface::FITKPHengLEIBoundary::PHengLEIBoundaryType>());
        auto boundary = boundManager->getBoundary(_ui->comboBox_Boundary->currentData().toInt());
        if (boundary) {
            boundary->setDataObjectName(name);
        }

		//获取网格边界
		auto globalData = FITKAPP->getGlobalData();
		if (globalData == nullptr)return;
		Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData< Interface::FITKUnstructuredFluidMeshVTK>();
		if (meshData == nullptr)return;
		Interface::FITKBoundaryMeshVTKManager* boundMeshManager = meshData->getBoundaryMeshManager();
		if (boundMeshManager == nullptr)return;
		auto boundMesh = boundMeshManager->getDataByID(_ui->comboBox_Boundary->currentData().toInt());

		//QObject::connect(boundMesh, &Core::FITKAbstractDataObject::dataObjectDestoried, [=](Core::FITKAbstractDataObject* obj) {
		//	boundManager->removeDataByID(boundMesh->getDataObjectID());
		//});
        
        if (_oper) {
            _oper->execProfession();
        }

        this->accept();
    }

    void PHengLEIBoundaryCreateDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void PHengLEIBoundaryCreateDialog::highlightMeshBoundary(int meshBoundID)
    {
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->clearHighlight();
        graphOper->highlight(meshBoundID);
    }
}
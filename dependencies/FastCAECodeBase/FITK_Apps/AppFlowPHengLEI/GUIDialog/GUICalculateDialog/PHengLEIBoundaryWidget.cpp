/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PHengLEIBoundaryWidget.h"
#include "ui_PHengLEIBoundaryWidget.h"
#include "CompSelectComBoxWidget.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"
#include "FITK_Kernel/FITKEasyParam/FITKEasyParamWidgetFactory.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIDataManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIBoundaryType.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIBoundaryTypeManager.h"

#include <QTabBar>
#include <QToolBox>

namespace GUI
{
    /**
     * @brief    边界条件类型子数据回调函数
     * @param[i] type 类型
     * @param[i] widget 对应的界面
     * @return   Core::FITKParameter* 子数据对象
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-08-27
     */
    Core::FITKParameter* BoundaryTypeGetSubData(const QString& type, CompSelectComBoxWidget* widget)
    {
        if (widget == nullptr)return nullptr;
        auto phyFactory = FITKAPP->getComponents()->getComponentTByName<Interface::FITKPhysicsHandlerFactory>("FITKPHengLEIPhysicsHandlerFactory");
        if (phyFactory == nullptr)return nullptr;
        auto phyData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
        if (phyData == nullptr)return nullptr;
		int MeshID = widget->getData("MeshID").toInt();
		Interface::FITKPHengLEIBoundary::PHengLEIBoundaryType boundaryType = widget->getData("BoundaryType").value<Interface::FITKPHengLEIBoundary::PHengLEIBoundaryType>();

		phyFactory->setBoundaryType(MeshID,type,boundaryType);

		Interface::FITKPHengLEIBoundary* boundary = phyData->getBoundaryManager()->getBoundary(MeshID);

		return boundary->getBoundaryType()->getBoundaryTypeParams();
    }

    PHengLEIBoundaryWidget::PHengLEIBoundaryWidget(Interface::FITKPHengLEIBoundary* boundaryObj, EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUICalculateWidgetBase(oper, parent), _boundaryObj(boundaryObj)
    {
        _ui = new Ui::PHengLEIBoundaryWidget();
        _ui->setupUi(this);
        init();
    }

    PHengLEIBoundaryWidget::~PHengLEIBoundaryWidget()
    {
        if (_ui) delete _ui;
    }

    void PHengLEIBoundaryWidget::init()
    {
        if (_boundaryObj == nullptr)return;
        auto globalData = FITKAPP->getGlobalData();
        if (globalData == nullptr)return;
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData< Interface::FITKUnstructuredFluidMeshVTK>();
        if (meshData == nullptr)return;
        Interface::FITKBoundaryMeshVTKManager* boundMeshManager = meshData->getBoundaryMeshManager();
        if (boundMeshManager == nullptr)return;

        //名称设置
        QString name = _boundaryObj->getDataObjectName();
        _ui->lineEdit_Name->setText(name);
        _ui->lineEdit_Name->setEnabled(false);

        //网格边界名称设置
        auto meshBoundary = boundMeshManager->getDataByID(_boundaryObj->getMeshBoundaryID());
        _ui->lineEdit_Boundary->setEnabled(false);
        if (meshBoundary)_ui->lineEdit_Boundary->setText(meshBoundary->getDataObjectName());

        //边界类型名称设置
        _currentType = _boundaryObj->getBoundaryEnumType();
        QString typeName = "";
        switch (_currentType) {
        case Interface::FITKPHengLEIBoundary::PHBWall:typeName = tr("Wall"); break;
        case Interface::FITKPHengLEIBoundary::PHBPressureInlet:typeName = tr("Pressure Inlet"); break;
        case Interface::FITKPHengLEIBoundary::PHBFarField:typeName = tr("Farield"); break;
        case Interface::FITKPHengLEIBoundary::PHBPressureOutlet:typeName = tr("Pressure Outlet"); break;
        case Interface::FITKPHengLEIBoundary::PHBInFlow:typeName = tr("InFlow"); break;
        case Interface::FITKPHengLEIBoundary::PHBMassFlowInlet:typeName = tr("MassFlowInlet"); break;
        case Interface::FITKPHengLEIBoundary::PHBMassFlowOutlet:typeName = tr("MassFlowOutlet"); break;
        }
        _ui->lineEdit_Type->setText(typeName);
        _ui->lineEdit_Type->setEnabled(false);

        update();
    }

    void PHengLEIBoundaryWidget::update()
    {
        if (_boundaryObj == nullptr)return;

		updateBoundaryType();
    }

    Interface::FITKPHengLEIBoundary * PHengLEIBoundaryWidget::getCurrentObj()
    {
        return _boundaryObj;
    }

    void PHengLEIBoundaryWidget::showEvent(QShowEvent * event)
    {
        GUICalculateWidgetBase::showEvent(event);
        if (_boundaryObj == nullptr)return;

        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->clearHighlight();
        graphOper->highlight(_boundaryObj->getMeshBoundaryID());
    }

    void PHengLEIBoundaryWidget::hideEvent(QHideEvent * event)
    {
        GUICalculateWidgetBase::hideEvent(event);
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->clearHighlight();
    }

    void PHengLEIBoundaryWidget::updateBoundaryType()
    {
        if (_boundaryObj == nullptr)return;
        if (_dataManager_PHengLEI == nullptr)return;
        auto boundartTypeMan = _dataManager_PHengLEI->getBoundaryTypeManager();
        if (boundartTypeMan == nullptr)return;

		QList<QWidget*> widgetList{};
		//InFlowType数据添加
		Interface::FITKPHengLEIBoundaryType* boundaryType = _boundaryObj->getBoundaryType();
		if (boundaryType)
		{
			QStringList options = boundartTypeMan->filterBoundary(_boundaryObj->getBoundaryEnumType());
			CompSelectComBoxWidget* comp = new CompSelectComBoxWidget("Boundary Type", this);
			comp->setFunction(&BoundaryTypeGetSubData);
			comp->setOptions(options);
			comp->setCurrentText(boundaryType->getDataObjectName());
			comp->setSubWidgetData(boundaryType->getBoundaryTypeParams());
			comp->setData("MeshID", _boundaryObj->getMeshBoundaryID());
			comp->setData("BoundaryType", _boundaryObj->getBoundaryEnumType());
			widgetList.append(comp);
		}
		for (auto w : widgetList)
		{
			_ui->verticalLayout->addWidget(w);
		}





















        //QWidget* widget = new QWidget(this);
        //QVBoxLayout* layout = new QVBoxLayout();
        //QToolBox* toolBox = Core::FITKWidgetComLine::CreateToolBox(widget);
        //for (int i = 0; i < num; i++) {
        //    Interface::FITKOFAbsBoundaryType* data = _boundaryObj->getFlowVBType(i);
        //    if (data == nullptr)continue;
        //    QStringList options = boundartTypeMan->filterBoundariesType(_physicsData->getSolver()->getSolverType(),
        //        _currentType, _boundaryObj->getFlowVariableName(i));
        //    CompSelectComBoxWidget* w = new CompSelectComBoxWidget(_boundaryObj->getFlowVariableName(i), this);
        //    w->setData("boundaryID", _boundaryObj->getDataObjectID());
        //    w->setData("objID", data->getDataObjectID());
        //    w->setData("objName", _boundaryObj->getFlowVariableName(i));
        //    w->setData("index", i);
        //    w->setSubWidgetData(data->getBoundaryTypePara());
        //    w->setOptions(options);
        //    w->setCurrentText(data->getDataObjectName());
        //    w->setFunction(&getBoundaryFlowSubData);
        //    toolBox->addItem(w, _boundaryObj->getFlowVariableName(i));
        //}
        //layout->addWidget(toolBox);
        //widget->setLayout(layout);
        //_ui->tabWidget->addTab(widget, tr("Flow"));
    }

    //void PHengLEIBoundaryWidget::updateTurbulence()
    //{
    //    if (_boundaryObj == nullptr)return;
    //    if (_physicsManager == nullptr)return;
    //    auto boundartTypeMan = _physicsManager->getBoundaryTypeManager();
    //    if (boundartTypeMan == nullptr)return;
    //    int num = _boundaryObj->getTurbulenceCount();
    //    if (num == 0) {
    //        return;
    //    }

    //    QWidget* widget = new QWidget(this);
    //    QVBoxLayout* layout = new QVBoxLayout();
    //    QToolBox* toolBox = Core::FITKWidgetComLine::CreateToolBox(widget);
    //    for (int i = 0; i < num; i++) {
    //        Interface::FITKOFAbsBoundaryType* data = _boundaryObj->getTurbulenceVBType(i);
    //        if (data == nullptr)continue;
    //        QStringList options = boundartTypeMan->filterBoundariesType(_physicsData->getSolver()->getSolverType(),
    //            _currentType, _boundaryObj->getTurbulenceVariableName(i));
    //        CompSelectComBoxWidget* w = new CompSelectComBoxWidget(_boundaryObj->getTurbulenceVariableName(i), this);
    //        w->setData("boundaryID", _boundaryObj->getDataObjectID());
    //        w->setData("objID", data->getDataObjectID());
    //        w->setData("objName", _boundaryObj->getTurbulenceVariableName(i));
    //        w->setData("index", i);
    //        w->setSubWidgetData(data->getBoundaryTypePara());
    //        w->setOptions(options);
    //        w->setCurrentText(data->getDataObjectName());
    //        w->setFunction(&getBoundaryTurbulenceSubData);
    //        toolBox->addItem(w, _boundaryObj->getTurbulenceVariableName(i));
    //    }
    //    layout->addWidget(toolBox);
    //    widget->setLayout(layout);
    //    _ui->tabWidget->addTab(widget, tr("Turbulence"));
    //}

    //void PHengLEIBoundaryWidget::updatePhases()
    //{
    //    if (_boundaryObj == nullptr)return;
    //    if (_physicsManager == nullptr)return;
    //    auto boundartTypeMan = _physicsManager->getBoundaryTypeManager();
    //    if (boundartTypeMan == nullptr)return;
    //    int num = _boundaryObj->getPhasesCount();
    //    if (num == 0) {
    //        _ui->tabWidget->removeTab(2);
    //        return;
    //    }

    //    QWidget* widget = new QWidget(this);
    //    QVBoxLayout* layout = new QVBoxLayout();
    //    QToolBox* toolBox = Core::FITKWidgetComLine::CreateToolBox(widget);
    //    for (int i = 0; i < num; i++) {
    //        Interface::FITKOFAbsBoundaryType* data = _boundaryObj->getPhasesVBType(i);
    //        if (data == nullptr)continue;
    //        QStringList options = boundartTypeMan->filterBoundariesType(_physicsData->getSolver()->getSolverType(),
    //            _currentType, _boundaryObj->getPhasesVariableName(i));
    //        CompSelectComBoxWidget* w = new CompSelectComBoxWidget(_boundaryObj->getPhasesVariableName(i), this);
    //        w->setData("boundaryID", _boundaryObj->getDataObjectID());
    //        w->setData("objID", data->getDataObjectID());
    //        w->setData("objName", _boundaryObj->getPhasesVariableName(i));
    //        w->setData("index", i);
    //        w->setSubWidgetData(data->getBoundaryTypePara());
    //        w->setOptions(options);
    //        w->setCurrentText(data->getDataObjectName());
    //        w->setFunction(&getBoundaryPhasesSubData);
    //        toolBox->addItem(w, _boundaryObj->getPhasesVariableName(i));
    //    }
    //    layout->addWidget(toolBox);
    //    widget->setLayout(layout);
    //    _ui->tabWidget->addTab(widget, tr("Phases"));
    //}
}


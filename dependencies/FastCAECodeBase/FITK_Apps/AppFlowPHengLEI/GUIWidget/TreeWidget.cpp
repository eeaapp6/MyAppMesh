/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TreeWidget.h"
#include "CompTreeItem.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/PropertyWidget.h"
#include "GUIFrame/MainTreeWidget.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFGeometryData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractGeometryMeshSizeGenerator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGeometryMeshSize.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIAbsSolver.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManagerHelper.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIMultiGrid.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIBoundary.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIViscousModel.h"

#include <QMenu>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIModels.h"

Q_DECLARE_METATYPE(GUI::MainTreeEnum)

namespace GUI{

    // TreeWidget类构造函数
    TreeWidget::TreeWidget(QWidget* parent) :
        QTreeWidget(parent)
    {
        //右键 不可少否则右键无反应
        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onItemClicked(QTreeWidgetItem*, int)));
        connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onModelCustomContextMenu(QPoint)));

        //隐藏列标题
        setHeaderHidden(true);
        
        // 设置自定义图标样式
        setCustomTreeIconStyle();
    }
    // TreeWidget类析构函数
    TreeWidget::~TreeWidget()
    {

    }

    // 更新所有模型案例树方法
    void TreeWidget::updateTree()
    {
        this->clear();
        updateGeometryItems();
        updateMeshItems();
        updateSetupItems();
		updateCoupling();
        updatePostItems();

        //展开全部子集
        setItemsExpandable(true);		
        expandAll();

        QList<CompTreeItem*> compWidgets = this->findChildren<CompTreeItem*>();
        for (auto widget : compWidgets) {
            if (widget == nullptr)continue;
            connect(widget, SIGNAL(sigIconButtonClicked()), this, SLOT(soltIconButtonClicked()));
        }
    }

    void TreeWidget::onItemClicked(QTreeWidgetItem * item, int column)
    {
        EventOper::TreeEventOperator* treeOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::TreeEventOperator>("ModelTreeEvent");
        if (treeOper == nullptr) return;
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr)return;
		graphOper->clearHighlight();

        if (item == nullptr)return;
        int objID = item->data(1, 0).toInt();
        GUI::MainTreeEnum treeType = item->data(2, 0).value<GUI::MainTreeEnum>();
        Interface::FITKGeoEnum::FITKGeometryComType geometryType = item->data(3, 0).value<Interface::FITKGeoEnum::FITKGeometryComType>();

        QString name = "";

        switch (treeType) {
        case GUI::MainTreeEnum::MainTree_Geomety: break;
        case GUI::MainTreeEnum::MainTree_GeometyBoxItem:name = "actionGeoCubeEdit"; break;
        case GUI::MainTreeEnum::MainTree_GeometyCylinderItem:name = "actionGeoCylinderEdit"; break;
        case GUI::MainTreeEnum::MainTree_GeometySphereItem:name = "actionGeoSphereEdit"; break;
        case GUI::MainTreeEnum::MainTree_GeometyBoolOrImportItem:name = "actionGeoBoolOrImportEdit"; break;
        case GUI::MainTreeEnum::MainTree_Mesh: break;
        case GUI::MainTreeEnum::MainTree_MeshBoundary:break;
        case GUI::MainTreeEnum::MainTree_MeshBoundaryItem: {
            graphOper->highlight(objID);
            break;
        }
        case GUI::MainTreeEnum::MainTree_Setup:name = "actionSetupEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupGeneralControl:name = "actionGeneralControlEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupGeneral:name = "actionSetupGeneralEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupModels:name = "actionSetupModels"; break;
		case GUI::MainTreeEnum::MainTree_SetupInFlow:name = "actionInFlowEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupMeshPara:name = "actionMeshParaEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupMeshProcess:name = "actionMeshProcessEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupSolverModel:name = "actionSolverModelEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupPHengLEIDiscretisation:name = "actionPHengLEIDiscretisationEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupPHengLEIInitialCondition:name = "actionInitialConditionEdit"; break;
		case GUI::MainTreeEnum::MainTree_setupOutputSetting:name = "actionOutputSettingEdit"; break;
		case GUI::MainTreeEnum::MainTree_setCalculation:name = "actionCalculationEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupPHengLEIBoundaryConditionsItem:name = "actionPHengLEIBoundaryConditionsEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupTurbulence:name = "actionTurbulenceEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupEneEquPara:name = "actionEnergyModelEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupSpeEquPara:name = "actionSpeciesModelEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupMultiGrid:name = "actionMultiGridEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupTransportProperties:name = "actionTransportEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupDiscretization:name = "actionDiscretizationEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupSolution:name = "actionSolutionEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupPassiveScalars:name = "actionPassiveScalarsEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupOperatingConditions:name = "actionOperConditionEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupCellZones:name = "actionCellZonesEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupBoundaryConditionsItem:name = "actionBoundaryConditionsEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupInitialConditions:name = "actionInitialEdit"; break;
        case GUI::MainTreeEnum::MainTree_SetupMonitors:name = "actionMonitorsEdit"; break;
		case GUI::MainTreeEnum::MainTree_SetupFlow:name = "actionFlowEdit"; break;
        case GUI::MainTreeEnum::MainTree_Run:name = "actionRun"; break;
        case GUI::MainTreeEnum::MainTree_Post:name = "actionPost"; break;
		case GUI::MainTreeEnum::MainTree_SetupSolMethod:name = "actionSolMethodEdit"; break;
		case GUI::MainTreeEnum::MainTree_Coupling:name = "actionCoupling"; break;
		
        }

        if (!name.isEmpty()) {
            QObject sender;
            sender.setObjectName(name);
            auto acOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>(name);
            if (acOper == nullptr)return;
            acOper->setEmitter(&sender);
            acOper->setArgs("objID", objID);
            acOper->actionTriggered();
        }
        else {
            treeOper->moveProcessToStep(0);
        }
    }

    void TreeWidget::onDoubleClicked(QTreeWidgetItem * item, int column)
    {
        if (item == nullptr)return;
        int objID = item->data(1, 0).toInt();
        GUI::MainTreeEnum treeType = item->data(2, 0).value<GUI::MainTreeEnum>();
        Interface::FITKGeoEnum::FITKGeometryComType geometryType = item->data(3, 0).value<Interface::FITKGeoEnum::FITKGeometryComType>();
    }

    void TreeWidget::onModelCustomContextMenu(QPoint point)
    {
        QTreeWidgetItem* item = this->currentItem();
        if (item == nullptr)return;

        int objID = item->data(1, 0).toInt();
        GUI::MainTreeEnum treeType = item->data(2, 0).value<GUI::MainTreeEnum>();
        Interface::FITKGeoEnum::FITKGeometryComType geometryType = item->data(3, 0).value<Interface::FITKGeoEnum::FITKGeometryComType>();

        QMenu menu;
        switch (treeType) {
        case GUI::MainTreeEnum::MainTree_Geomety: break;
        case GUI::MainTreeEnum::MainTree_GeometyBoxItem: {
            //addMenuActions(menu, "actionRenameCube", "Cube rename"); 
            addMenuActions(menu, "actionGeoCubeDelete", tr("Delete")); 
            break; 
        }
        case GUI::MainTreeEnum::MainTree_GeometyCylinderItem: {
            //addMenuActions(menu, "actionRenameCylinder", "Cylinder rename");
            addMenuActions(menu, "actionGeoCylinderDelete", tr("Delete"));
            break;
        }
        case GUI::MainTreeEnum::MainTree_GeometySphereItem: {
            //addMenuActions(menu, "actionRenameSphere", "Sphere rename");
            addMenuActions(menu, "actionGeoSphereDelete", tr("Delete"));
            break;
        }
        case GUI::MainTreeEnum::MainTree_GeometyBoolOrImportItem: {
            //addMenuActions(menu, "actionRenameSphere", "Sphere rename");
            addMenuActions(menu, "actionGeoBoolOrImportDelete", tr("Delete"));
            break;
        }
        case GUI::MainTreeEnum::MainTree_Mesh: {
            addMenuActions(menu, "actionClearMesh", tr("Clear mesh"));
            break;
        }
        case GUI::MainTreeEnum::MainTree_MeshGeometry: break;
        case GUI::MainTreeEnum::MainTree_MeshBase: break;
        case GUI::MainTreeEnum::MainTree_MeshLocal: {
            break;
        }
        case GUI::MainTreeEnum::MainTree_MeshLocalItem: {
            addMenuActions(menu, "actionMeshLocalDelete", tr("Delete"));
            break;
        }
        case GUI::MainTreeEnum::MainTree_SetupBoundaryConditions: {
            addMenuActions(menu, "actionBoundaryConditionsCreate", tr("Create"));
            break;
        }
        case GUI::MainTreeEnum::MainTree_SetupBoundaryConditionsItem: {
            addMenuActions(menu, "actionBoundaryConditionsDelete", tr("Delete"));
            break;
        }
		case MainTreeEnum::MainTree_SetupPHengLEIBoundaryConditions: {
			addMenuActions(menu, "actionPHengLEIBoundaryConditionsCreate", tr("Create"));
			break;
		}
		case MainTreeEnum::MainTree_SetupPHengLEIBoundaryConditionsItem: {
			addMenuActions(menu, "actionPHengLEIBoundaryConditionsDelete", tr("Delete"));
			break;
		}
		case GUI::MainTreeEnum::MainTree_SetupEneEquPara: {
			addMenuActions(menu, "actionEnergyModelEdit", tr("Edit Energy Model"));
			addMenuActions(menu, "actionEnergyReset", tr("Reset to Default"));
			break;
		}
		case GUI::MainTreeEnum::MainTree_SetupSpeEquPara: {
			addMenuActions(menu, "actionSpeciesModelEdit", tr("Edit Species Model"));
			addMenuActions(menu, "actionSpeciesAdd", tr("Add Species"));
			addMenuActions(menu, "actionSpeciesReset", tr("Reset to Default"));
			break;
		}
		case GUI::MainTreeEnum::MainTree_SetupTurbulence: {
			addMenuActions(menu, "actionTurbulenceModelEdit", tr("Edit Turbulence Model"));
			addMenuActions(menu, "actionTurbulenceReset", tr("Reset to Default"));
			break;
		}
        }

        if (menu.actions().size() == 0) return;
        // 在鼠标点击位置执行上下文菜单
        menu.exec(QCursor::pos());
    }

    void TreeWidget::acitonClicked()
    {
        QTreeWidgetItem* item = this->currentItem();
        if (item == nullptr)return;
        int objID = item->data(1, 0).toInt();

        QObject* senderObject = sender();
        if (senderObject == nullptr)return;

        auto acOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>(senderObject->objectName());
        if (acOper == nullptr)return;
        acOper->setEmitter(senderObject);
        acOper->setArgs("objID", objID);
        acOper->actionTriggered();
    }

    void TreeWidget::soltIconButtonClicked()
    {
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr)return;

        CompTreeItem* senderWidget = dynamic_cast<CompTreeItem*>(this->sender());
        if (senderWidget == nullptr)return;
        QTreeWidgetItem* item = senderWidget->getTreeItem();
        if (item == nullptr)return;
        int objID = item->data(1, 0).toInt();
        GUI::MainTreeEnum type = item->data(2, 0).value<GUI::MainTreeEnum>();

        switch (type){
        case GUI::MainTreeEnum::MainTree_GeometyBoxItem:
        case GUI::MainTreeEnum::MainTree_GeometyCylinderItem:
        case GUI::MainTreeEnum::MainTree_GeometySphereItem:
        case GUI::MainTreeEnum::MainTree_GeometyBoolOrImportItem:
        {
            //几何显示隐藏控制
            Interface::FITKGeoCommandList* geometryData = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
            if (geometryData == nullptr) break;
            auto geoObj = geometryData->getDataByID(objID);
            if(geoObj == nullptr)break;
            if (geoObj->isEnable()){
                geoObj->enable(false);
                senderWidget->setButtonIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
            }
            else {
                geoObj->enable(true);
                senderWidget->setButtonIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
            }
            break;
        }
        case GUI::MainTreeEnum::MainTree_MeshBoundaryItem:
        {
            //边界显示隐藏控制
            auto globalData = FITKAPP->getGlobalData();
            if (globalData == nullptr)break;
            Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData< Interface::FITKUnstructuredFluidMeshVTK>();
            if (meshData == nullptr)break;
            Interface::FITKBoundaryMeshVTKManager* boundMeshManager = meshData->getBoundaryMeshManager();
            if (boundMeshManager == nullptr)break;
            Interface::FITKBoundaryMeshVTK* boundMesh = boundMeshManager->getDataByID(objID);
            if (boundMesh == nullptr)break;
            if (boundMesh->FITKAbstractNDataObject::isEnable()) {
                boundMesh->FITKAbstractNDataObject::enable(false);
                senderWidget->setButtonIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
            }
            else {
                boundMesh->FITKAbstractNDataObject::enable(true);
                senderWidget->setButtonIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
            }
            break;
        }
        }

        graphOper->updateGraph(objID);
        graphOper->reRender(true);
    }

    void TreeWidget::updateGeometryItems()
    {
        Interface::FITKOFGeometryData* geometryData = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKOFGeometryData>();
        if (geometryData == nullptr) return;

        QTreeWidgetItem* geometryItem = new QTreeWidgetItem();
        geometryItem->setText(0, tr("Geometry"));
        this->addTopLevelItem(geometryItem);

        for (int i = 0; i < geometryData->getDataCount(); i++) {
            auto geometryObj = dynamic_cast<Interface::FITKAbsGeoCommand*>(geometryData->getDataByIndex(i));
            if (geometryObj == nullptr)continue;

            QTreeWidgetItem* item = new QTreeWidgetItem();
            //item->setText(0, geometryObj->getDataObjectName());
            item->setData(1, 0, geometryObj->getDataObjectID());

            GUI::MainTreeEnum treeType = GUI::MainTreeEnum::MainTree_None;
            Interface::FITKGeoEnum::FITKGeometryComType geometryType = geometryObj->getGeometryCommandType();
            switch (geometryType) {
            case Interface::FITKGeoEnum::FGTNone:break;
            case Interface::FITKGeoEnum::FGTBox:  treeType = GUI::MainTreeEnum::MainTree_GeometyBoxItem; break;
            case Interface::FITKGeoEnum::FGTCylinder:treeType = GUI::MainTreeEnum::MainTree_GeometyCylinderItem; break;
            case Interface::FITKGeoEnum::FGTSphere:treeType = GUI::MainTreeEnum::MainTree_GeometySphereItem;  break;
            case Interface::FITKGeoEnum::FGTBool:treeType = GUI::MainTreeEnum::MainTree_GeometyBoolOrImportItem;  break;
            case Interface::FITKGeoEnum::FGTImport:treeType = GUI::MainTreeEnum::MainTree_GeometyBoolOrImportItem;  break;
            }
            item->setData(2, 0, QVariant::fromValue(treeType));
            geometryItem->addChild(item);

            CompTreeItem* widget = new CompTreeItem(item, this);
            if (geometryObj->isEnable()) {
                widget->setButtonIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
            }
            else {
                widget->setButtonIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
            }
            widget->setText(geometryObj->getDataObjectName());
            this->setItemWidget(item, 0, widget);
        }
    }

    void TreeWidget::updateMeshItems()
    {
        QTreeWidgetItem* meshItem = new QTreeWidgetItem();
        meshItem->setText(0, tr("Mesh"));
        meshItem->setData(1, 0, -1);
        meshItem->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_Mesh));
        this->addTopLevelItem(meshItem);

        QTreeWidgetItem* meshBoundItem = new QTreeWidgetItem();
        meshBoundItem->setText(0, tr("Boundary"));
        meshBoundItem->setData(1, 0, -1);
        meshBoundItem->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_MeshBoundary));
        meshItem->addChild(meshBoundItem);

        //update sub item
        updateMeshBoundaryItems(meshBoundItem);
    }

    void TreeWidget::updateSetupItems()
    {
        QTreeWidgetItem* setupItem = new QTreeWidgetItem();
        setupItem->setText(0, tr("Setup"));
        setupItem->setData(1, 0, -1);
        setupItem->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_Setup));
        this->addTopLevelItem(setupItem);

        Interface::FITKPHengLEIData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
        if (physicsData == nullptr)return;
        Interface::FITKPHengLEIAbsSolver* solverData = physicsData->getSolver();
		if (solverData != nullptr)
		{
			QString  solverType = solverData->getDataObjectName();
		}
        if (solverData == nullptr)return;

		QTreeWidgetItem* item = nullptr;

		//// 添加求解器节点-General子节点
		//item = new QTreeWidgetItem();
		//item->setText(0, tr("General"));
		//item->setData(1, 0, -1);
		//item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupGeneral));
		//setupItem->addChild(item);

		//meshprocess临时添加
		item = new QTreeWidgetItem();
		item->setText(0, tr("MeshProcess"));
		item->setData(1, 0, -1);
		item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupMeshProcess));
		setupItem->addChild(item);

        Interface::FITKPHengLEIModels* models = physicsData->getModels();
        // 添加求解器节点->Models子节点
        if (models)
        {
            item = new QTreeWidgetItem();
            item->setText(0, tr("Models"));
            item->setData(1, 0, -1);
            item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupModels));
            setupItem->addChild(item);

            // 保存Models节点引用
            QTreeWidgetItem* modelsItem = item;

            // 添加Turbulence子节点
            if (models->getViscousModel())
            {
				QTreeWidgetItem* turbulenceItem = new QTreeWidgetItem();
				turbulenceItem->setText(0, tr("Turbulence Model"));
				turbulenceItem->setData(1, 0, -1);
				turbulenceItem->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupTurbulence));
				modelsItem->addChild(turbulenceItem);
            }
            // 添加Energy子节点
            if (models->getEnergyModel())
            {
                QTreeWidgetItem* energyItem = new QTreeWidgetItem();
                energyItem->setText(0, tr("Energy Model"));
                energyItem->setData(1, 0, -1);
                energyItem->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupEneEquPara));
                modelsItem->addChild(energyItem);
            }

            // 添加Species子节点
            if (models->getSpeciesModel())
            {
				QTreeWidgetItem* speciesItem = new QTreeWidgetItem();
				speciesItem->setText(0, tr("Species Model"));
				speciesItem->setData(1, 0, -1);
				speciesItem->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupSpeEquPara));
				modelsItem->addChild(speciesItem);
            }
        }

		// 添加求解器节点-Solver Model子节点
		if (physicsData->getViscousModel())
		{
			item = new QTreeWidgetItem();
			item->setText(0, tr("Solver Model"));
			item->setData(1, 0, -1);
			item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupSolverModel));
			setupItem->addChild(item);
		}

		//if (physicsData->getGeneralControl())
		//{
		//	item = new QTreeWidgetItem();
		//	item->setText(0, tr("GeneralControl"));
		//	item->setData(1, 0, -1);
		//	item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupGeneralControl));
		//	setupItem->addChild(item);
		//}

        if (physicsData->getSolution())
        {
			item = new QTreeWidgetItem();
			item->setText(0, tr("Solution"));
			item->setData(1, 0, -1);
			item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupSolution));
			setupItem->addChild(item);
        }

		if (physicsData->getInFlow())
		{
			item = new QTreeWidgetItem();
			item->setText(0, tr("InFlow"));
			item->setData(1, 0, -1);
			item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupInFlow));
			setupItem->addChild(item);
		}

		if (physicsData->getMeshPara())
		{
			item = new QTreeWidgetItem();
			item->setText(0, tr("MeshPara"));
			item->setData(1, 0, -1);
			item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupMeshPara));
			setupItem->addChild(item);
		}



		if (physicsData->getDiscretisation()) 
		{
			item = new QTreeWidgetItem();
			item->setText(0, tr("Discretization"));
			item->setData(1, 0, -1);
			item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupPHengLEIDiscretisation));
			setupItem->addChild(item);
		}

        if (physicsData->getInitialCondition())
        {
			item = new QTreeWidgetItem();
			item->setText(0, tr("Initial Condition"));
			item->setData(1, 0, -1);
			item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupPHengLEIInitialCondition));
			setupItem->addChild(item);
        }

		if (physicsData->getMultiGrid())
		{
			item = new QTreeWidgetItem();
			item->setText(0, tr("MultiGrid"));
			item->setData(1, 0, -1);
			item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupMultiGrid));
			setupItem->addChild(item);
		}

		if (physicsData->getFlow())
		{
			item = new QTreeWidgetItem();
			item->setText(0, tr("Flow"));
			item->setData(1, 0, -1);
			item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupFlow));
			setupItem->addChild(item);
		}

		item = new QTreeWidgetItem();
		item->setText(0, tr("Boundary Conditions"));
		item->setData(1, 0, -1);
		item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupPHengLEIBoundaryConditions));
		setupItem->addChild(item);

		for (int i = 0; i < physicsData->getBoundaryManager()->getDataCount(); i++) {
			auto boundary = physicsData->getBoundaryManager()->getDataByIndex(i);
			if (boundary == nullptr)continue;
			QTreeWidgetItem* bitem = new QTreeWidgetItem();
			bitem->setText(0, boundary->getDataObjectName());
			bitem->setData(1, 0, boundary->getDataObjectID());
			bitem->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_SetupPHengLEIBoundaryConditionsItem));
			item->addChild(bitem);
		}

		if (physicsData->getOutputSetting())
		{
			item = new QTreeWidgetItem();
			item->setText(0, tr("Flow Visualization"));
			item->setData(1, 0, -1);
			item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_setupOutputSetting));
			setupItem->addChild(item);
		}



		// 添加求解器节点-Calculation子节点
		if (physicsData->getCalculation())
		{
			item = new QTreeWidgetItem();
			item->setText(0, tr("Calculation"));
			item->setData(1, 0, -1);
			item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_setCalculation));
			setupItem->addChild(item);
		}
    }

	void TreeWidget::updateCoupling()
	{
		QTreeWidgetItem* CouplingItem = new QTreeWidgetItem();
		CouplingItem->setText(0, tr("Coupling"));
		CouplingItem->setData(1, 0, -1);
		CouplingItem->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_Coupling));
		this->addTopLevelItem(CouplingItem);
	}
    void TreeWidget::updatePostItems()
    {
        QTreeWidgetItem* postItem = new QTreeWidgetItem();
        postItem->setText(0, tr("Post"));
        postItem->setData(1, 0, -1);
        postItem->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_Post));
        this->addTopLevelItem(postItem);
    }

    void TreeWidget::updateMeshLocalItems(QTreeWidgetItem* parentItem)
    {
        Interface::FITKMeshGenInterface* genInterface = Interface::FITKMeshGenInterface::getInstance();
        Interface::FITKAbstractGeometryMeshSizeGenerator* generator = genInterface->getGeometryMeshSizeGenerator();
        if (generator == nullptr)return;
        Interface::FITKGeometryMeshSizeManager* manger = genInterface->getGeometryMeshSizeManager();
        if (manger == nullptr)return;

        for (int i = 0; i < manger->getDataCount(); i++) {
            Interface::FITKGeometryMeshSize* geoMeshSize = manger->getDataByIndex(i);
            if (geoMeshSize == nullptr)continue;

            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, geoMeshSize->getDataObjectName());
            item->setData(1, 0, geoMeshSize->getDataObjectID());
            item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_MeshLocalItem));
            parentItem->addChild(item);
        }
    }

    void TreeWidget::updateMeshBoundaryItems(QTreeWidgetItem * parentItem)
    {
        auto globalData = FITKAPP->getGlobalData();
        if (globalData == nullptr)return;
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData< Interface::FITKUnstructuredFluidMeshVTK>();
        if (meshData == nullptr)return;
        Interface::FITKBoundaryMeshVTKManager* boundMeshManager = meshData->getBoundaryMeshManager();
        if (boundMeshManager == nullptr)return;

        for (int i = 0; i < boundMeshManager->getDataCount(); i++) {
            Interface::FITKBoundaryMeshVTK* boundMesh = boundMeshManager->getDataByIndex(i);
            if (boundMesh == nullptr)continue;

            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setData(1, 0, boundMesh->getDataObjectID());
            item->setData(2, 0, QVariant::fromValue(GUI::MainTreeEnum::MainTree_MeshBoundaryItem));
            parentItem->addChild(item);

            CompTreeItem* widget = new CompTreeItem(item, this);
            if (boundMesh->FITKAbstractNDataObject::isEnable()) {
                widget->setButtonIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
            }
            else {
                widget->setButtonIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
            }

            widget->setText(boundMesh->getDataObjectName());
            this->setItemWidget(item, 0, widget);
        }
    }

    void TreeWidget::addMenuActions(QMenu & menu, QString actions, QString objectName)
    {
        auto act = menu.addAction(objectName); // 添加动作
        act->setObjectName(actions);
        connect(act, SIGNAL(triggered()), this, SLOT(acitonClicked()));
    }

    void TreeWidget::setCustomTreeIconStyle()
    {
        // 设置自定义的折叠/展开图标样式
        setStyleSheet(
            "QTreeWidget::branch:has-children:!has-siblings:closed,"
            "QTreeWidget::branch:closed:has-children:has-siblings {"
            "    border-image: none;"
            "    image: url(:/icons/tree_fold.png);"
            "}"
            "QTreeWidget::branch:open:has-children:!has-siblings,"
            "QTreeWidget::branch:open:has-children:has-siblings {"
            "    border-image: none;"
            "    image: url(:/icons/tree_unfold.png);"
            "}"
        );
    }
} // namespace GUIOper
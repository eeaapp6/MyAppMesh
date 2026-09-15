/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsImportManager.h"

#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/PropertyWidget.h"
#include "GUIDialog/GUIGeometryDialog/GeometryWidgetBase.h"
#include "GUIDialog/GUIMeshDialog/MeshGeoWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImport.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshSizeInfoGenerator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGeometryMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeGeom.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Component/FITKCGNSIO/FITKCGNSIOInterface.h"

#include <QFileDialog>
#include <QApplication>

namespace ModelOper {
    OperatorsImportManager::OperatorsImportManager()
    {

    }

    OperatorsImportManager::~OperatorsImportManager()
    {

    }

    bool OperatorsImportManager::execGUI()
    {
        GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWindow == nullptr)return false;
        GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
        if (propertyWidget == nullptr)return false;


        QString workDir = "";
        if (FITKAPP->getAppSettings()) {
            workDir = FITKAPP->getAppSettings()->getWorkingDir();
        }
        if (workDir.isEmpty()) workDir = QApplication::applicationDirPath();
        QString fileName;
        QFileDialog fileDialog;
        if (_senderName == "actionImportGeometry") {
            fileName = fileDialog.getOpenFileName(_mainWindow, tr("Import Geometry"), workDir, tr("File(*.brep ; *.stp ; *.step ; *.igs ; *.stl)"));
            if (fileName.isEmpty())return false;
        }
        else  if (_senderName == "actionImportMesh") {
            fileName = fileDialog.getOpenFileName(_mainWindow, tr("Import Mesh"), workDir, tr("File(*.cgns)"));
            if (fileName.isEmpty())return false;
        }
        else if (_senderName == "actionImportOpenFoamMesh") {
            fileName = fileDialog.getExistingDirectory(_mainWindow, tr("Import OpenFoam Mesh"), workDir);
            if (fileName.isEmpty())return false;
        }        
        this->setArgs("FileName", fileName);
        this->setArgs("SenderName", _senderName);

        return true;
    }

    bool OperatorsImportManager::execProfession()
    {

        //获取线程池
        Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
        if (pool == nullptr)return false;
        QString fileName, senderName; 
        this->argValue<QString>("FileName",fileName);
        if (fileName.isEmpty()) return false;
        this->argValue<QString>("SenderName", senderName);
        if (senderName.isEmpty()) return false;

        if (senderName == "actionImportGeometry")
        {
  
            ImportReadThread* importThread = new ImportReadThread();
            importThread->_type = ImportType::ImportGeo;
            importThread->_fileName = fileName;
            connect(importThread, SIGNAL(sigImportFinish(bool, int)), this, SLOT(slotGeoImportFinish(bool, int)));
            pool->execTask(importThread);
        }
        else  if (senderName == "actionImportMesh") 
        {
            ImportReadThread* importThread = new ImportReadThread();
            importThread->_type = ImportType::ImportMesh;
            importThread->_fileName = fileName;
            connect(importThread, SIGNAL(sigImportFinish(bool, int)), this, SLOT(slotMeshImportFinish(bool, int)));
            pool->execTask(importThread);
        }
        else if (_senderName == "actionImportOpenFoamMesh") {
            ImportReadThread* importThread = new ImportReadThread();
            importThread->_type = ImportType::ImportOpenFoamMesh;
            importThread->_fileName = fileName;
            connect(importThread, SIGNAL(sigImportFinish(bool, int)), this, SLOT(slotFoamMeshInportFinish()));
            pool->execTask(importThread);
        }

        this->clearArgs();
        return true;
    }

    void OperatorsImportManager::slotGeoImportFinish(bool result, int objID)
    {
        if (result == false)return;
        if (objID < 0)return;

        Interface::FITKGeoCommandList* geometryData = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (geometryData == nullptr) return;

        //默认添加Default面组
        GUI::GeometryWidgetBase::createDefaultFaceGroup(geometryData->getDataByID(objID));

        //更新网格划分区域界面
        GUI::MainWindow* mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWin) {
            GUI::MeshGeoWidget* widget = dynamic_cast<GUI::MeshGeoWidget*>(mainWin->getPropertyWidget()->getCurrentWidget());
            if (widget)widget->updateWidget();
        }

        //获取模型树控制器
        auto treeOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::TreeEventOperator>("ModelTreeEvent");
        if (treeOper == nullptr) return;
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr)return;

        graphOper->updateGraph(objID);
        treeOper->updateTree();
        graphOper->reRender(true);
    }

    void OperatorsImportManager::slotMeshImportFinish(bool result, int objID)
    {
        if (result == false)return;
        if (objID < 0)return;

        // 获取模型树控制器
        auto treeOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::TreeEventOperator>("ModelTreeEvent");
        if (treeOper == nullptr) return;
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr)return;

        graphOper->updateGraph(objID);
        treeOper->updateTree();
        graphOper->reRender(true);
    }

    void OperatorsImportManager::slotFoamMeshInportFinish()
    {
        //刷新渲染窗口
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr)return;
        // 网格对象
        auto mesh = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        graphOper->updateGraph(mesh->getDataObjectID());

        // 获取模型树控制器
        auto treeOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::TreeEventOperator>("ModelTreeEvent");
        if (treeOper == nullptr) return;
        treeOper->updateTree();
    }

    void ImportReadThread::run()
    {
        switch (_type) {
        case ModelOper::ImportType::ImportGeo: {
            Interface::FITKGeoCommandList* geometryData = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
            if (geometryData == nullptr) return;

            Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
            if (geoFactory == nullptr)return;
            auto geoObj = geoFactory->createCommandT<Interface::FITKAbsGeoModelImport>(Interface::FITKGeoEnum::FITKGeometryComType::FGTImport);
            if (geoObj == nullptr)return;
            geoObj->setFileName(_fileName);


            if (geoObj->getDataObjectName().isEmpty()) {
                QFileInfo fileInfo(_fileName);
                // 获取文件名称（不包含路径与文件类型）
                QString name = fileInfo.baseName();
                geoObj->setDataObjectName(name);
            }
            geometryData->appendDataObj(geoObj);
            bool result = geoObj->update();

            emit sigImportFinish(result, geoObj->getDataObjectID());
            break;
        }
        case ModelOper::ImportType::ImportMesh: {
            Interface::FITKUnstructuredFluidMeshVTK* mesh = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
            if (!mesh)return;
            if (_fileName.endsWith(".cgns"))
            {
                //设置网格处理所需的数据参数
                IO::CGNSDataIO data;
                data.CreateMeshFun = [&]() {return new Interface::FITKUnstructuredMeshVTK; };
                auto ofMeshReader = FITKAPP->getComponents()->getComponentTByName<IO::FITKCGNSIOInterface>("IO::FITKCGNSIOInterface");
                if (ofMeshReader == nullptr) return;
                ofMeshReader->setFileName(_fileName);
                ofMeshReader->setCGNSDataIO(&data);
                ofMeshReader->exec(1);
                //处理读取完之后的数据
                this->readCGNSMeshFinishProcessing();
            }
            emit sigImportFinish(true, mesh->getDataObjectID());
            break;
        }
        case ModelOper::ImportType::ImportOpenFoamMesh:{
            // 获取单例
            auto meshGen = Interface::FITKMeshGenInterface::getInstance();
            // 读取网格
            auto meshProcessor = meshGen->getMeshProcessor();
            if (meshProcessor == nullptr) return;
            meshProcessor->setValue("WorkDir", _fileName);
            meshProcessor->start();
            bool result = true;
            emit sigImportFinish(true, -1);
            break;
        }
        }
    }

    void ImportReadThread::readCGNSMeshFinishProcessing()
    {
        Interface::FITKUnstructuredFluidMeshVTK* meshFluid = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!meshFluid)return;
        meshFluid->clear();
        //获取读取接口
        IO::FITKCGNSIOInterface* interfaceIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKCGNSIOInterface>("IO::FITKCGNSIOInterface");
        if (!interfaceIO) return;
        IO::CGNSDataIO data = interfaceIO->getCGNSDataIO();
        for (int i = 0; i < data._mesh.size() && i < data._componentManager.size(); ++i)
        {
            Interface::FITKFluidRegionsMesh* regionsMesh = new Interface::FITKFluidRegionsMesh;
            //获取区域网格和边界网格数据
            Interface::FITKUnstructuredMeshVTK* regionMeshData = regionsMesh->getFieldMesh();
            Interface::FITKBoundaryMeshVTKManager* boundaryMeshMgr = regionsMesh->getBoundaryMeshManager();
            if (!regionMeshData || !boundaryMeshMgr) continue;
            //处理数据
            Interface::FITKUnstructuredMeshVTK* mesh = dynamic_cast<Interface::FITKUnstructuredMeshVTK*>(data._mesh.at(i));
            if (mesh)
            {
                regionsMesh->setDataObjectName(mesh->getDataObjectName());
                int nodeCount = mesh->getNodeCount();
                int elementCount = mesh->getElementCount();
                for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
                {
                    Core::FITKNode* node = mesh->getNodeAt(nodeIndex);
                    if (!node) continue;
                    regionMeshData->addNode(node->getNodeID(), node->x(), node->y(), node->z());
                }
                for (int elemIndex = 0; elemIndex < elementCount; ++elemIndex)
                {
                    Interface::FITKAbstractElement* element = mesh->getElementAt(elemIndex);
                    if (!element)continue;
                    Interface::FITKAbstractElement* ele = Interface::FITKElementFactory::createElement(element->getEleType());
                    ele->setNodeID(element->getAllNodes());
                    regionMeshData->appendElement(ele);
                }
            }
			//保存网格文件路径
			regionMeshData->setUserData(Core::FITKUserData::FITKUserRole + 10, _fileName);
            //处理边界数据
            QList<Interface::FITKModelSet*> setList = data._componentManager.at(i);
            for (int j = 0; j < setList.size(); ++j)
            {
                Interface::FITKModelSet* setData = setList.at(j);
                if (!setData) continue;
                Interface::FITKBoundaryMeshVTK* boundary = new Interface::FITKBoundaryMeshVTK(regionMeshData);
                QList<int> boundaryElementIDs = setData->getAbsoluteMember();
                for (int eID : boundaryElementIDs)
                {
                    Interface::FITKAbstractElement* element = mesh->getElementByID(eID);
                    if (!element)continue;
                    Interface::FITKAbstractElement* ele = Interface::FITKElementFactory::createElement(element->getEleType());
                    ele->setNodeID(element->getAllNodes());
                    boundary->appendElement(ele);
                }
                boundary->setDataObjectName(setData->getDataObjectName());
                boundaryMeshMgr->appendDataObj(boundary);
                //清理边界数据
                delete setData;
            }
            meshFluid->appendDataObj(regionsMesh);
            //销毁数据
            delete mesh;
        }
        interfaceIO->clearCGNSDataIO();
    }

}


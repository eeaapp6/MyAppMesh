/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsExportManager.h"

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

#include <QFileDialog>
#include <QApplication>

namespace ModelOper {
    OperatorsExportManager::OperatorsExportManager()
    {

    }

    OperatorsExportManager::~OperatorsExportManager()
    {

    }

    bool OperatorsExportManager::execGUI()
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
        if (_senderName == "actionExportGeometry") {
            fileName = fileDialog.getSaveFileName(_mainWindow, tr("Export Geometry"), workDir, tr("File(*.brep ; *.stp ; *.step ; *.igs ; *.stl)"));
            if (fileName.isEmpty())return false;
        }
        else  if (_senderName == "actionExportMesh") {
            fileName = fileDialog.getSaveFileName(_mainWindow, tr("Export Mesh"), workDir, tr("File(*.cgns)"));
            if (fileName.isEmpty())return false;
        }
        else if (_senderName == "actionExportOpenFoamMesh") {

        }        
        this->setArgs("FileName", fileName);
        this->setArgs("SenderName", _senderName);

        return true;
    }

    bool OperatorsExportManager::execProfession()
    {

        //获取线程池
        Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
        if (pool == nullptr)return false;
        QString fileName, senderName; 
        this->argValue<QString>("FileName",fileName);
        if (fileName.isEmpty()) return false;
        this->argValue<QString>("SenderName", senderName);
        if (senderName.isEmpty()) return false;

        if (senderName == "actionExportGeometry")
        {
  
        }
        else  if (senderName == "actionExportMesh") 
        {
            ExportWriteThread* exportThread = new ExportWriteThread();
            exportThread->_type = ExportType::ExportMesh;
            exportThread->_fileName = fileName;
            pool->execTask(exportThread);
        }
        else if (_senderName == "actionExportOpenFoamMesh") {

        }

        this->clearArgs();
        return true;
    }

    void ExportWriteThread::run()
    {
        switch (_type) {
        case ModelOper::ExportType::ExportGeo: {
            break;
        }
        case ModelOper::ExportType::ExportMesh: {
            Interface::FITKUnstructuredFluidMeshVTK* mesh = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
            if (!mesh)return;
            if (_fileName.endsWith(".cgns"))
            {
                //设置网格处理所需的数据参数
                IO::CGNSDataIO data;
                data.CreateMeshFun = [&]() {return new Interface::FITKUnstructuredMeshVTK; };
                //预处理
                this->writeCGNSMeshPreProcessing(data);
                auto ofMeshWriter = FITKAPP->getComponents()->getComponentTByName<IO::FITKCGNSIOInterface>("IO::FITKCGNSIOInterface");
                if (ofMeshWriter == nullptr) return;
                ofMeshWriter->setFileName(_fileName);
                ofMeshWriter->setCGNSDataIO(&data);
                ofMeshWriter->exec(2);
                //处理写出完之后的数据
                this->writeCGNSMeshFinishProcessing();
            }
            break;
        }
        case ModelOper::ExportType::ExportOpenFoamMesh:{
            break;
        }
        }
    }

    void ExportWriteThread::writeCGNSMeshPreProcessing(IO::CGNSDataIO& data)
    {
        Interface::FITKUnstructuredFluidMeshVTK* meshFluid = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!meshFluid)return;
        //数据转换
        int count = meshFluid->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKFluidRegionsMesh* regionMesh = meshFluid->getDataByIndex(i);
            if (!regionMesh) continue;
            //获取区域网格和边界网格数据
            Interface::FITKUnstructuredMeshVTK* regionMeshData = regionMesh->getFieldMesh();
            Interface::FITKBoundaryMeshVTKManager* boundaryMeshMgr = regionMesh->getBoundaryMeshManager();
            if (!regionMeshData || !boundaryMeshMgr) continue;
            //创建数据
            Interface::FITKUnstructuredMeshVTK* mesh = dynamic_cast<Interface::FITKUnstructuredMeshVTK*>(data.CreateMeshFun());
            QList<Interface::FITKModelSet*> setList;
            mesh->setDataObjectName(regionMesh->getDataObjectName());
            //处理数据-节点数据
            QHash<int, int> nodeMapping;
            for (int iNode = 0; iNode < regionMeshData->getNodeCount(); ++iNode)
            {
                Core::FITKNode* node = regionMeshData->getNodeAt(iNode);
                if (!node) continue;
                int id = node->getNodeID();
                int nodeID = mesh->addNode(node->x(), node->y(), node->z());
                nodeMapping.insert(id, nodeID);
            }
            //处理数据-单元数据-边界
            for (int iBC = 0; iBC < boundaryMeshMgr->getDataCount(); ++iBC)
            {
                Interface::FITKBoundaryMeshVTK* boundaryMesh = boundaryMeshMgr->getDataByIndex(iBC);
                if (!boundaryMesh) continue;
                int eleMaxID = mesh->getElementMaxID();
                Interface::FITKModelSet* set = new Interface::FITKModelSet(Interface::FITKModelEnum::FITKModelSetType::FMSElem);
                QList<int> eleIDs;
                for (int iEle = 0; iEle < boundaryMesh->getElementCount(); ++iEle)
                {
                    Interface::FITKAbstractElement* element = boundaryMesh->getElementAt(iEle);
                    if (!element) continue;
                    QList<int> ids = element->getAllNodes();
                    QList<int> nodeIDs;
                    for (int id : ids)
                        nodeIDs.append(nodeMapping[id]);
                    Interface::FITKAbstractElement* ele = Interface::FITKElementFactory::createElement(element->getEleType());
                    ele->setNodeID(nodeIDs);
                    ele->setEleID(++eleMaxID);
                    mesh->appendElement(ele);
                    eleIDs.append(ele->getEleID());
                }
                set->setDataObjectName(boundaryMesh->getDataObjectName());
                set->setAbsoluteMember(eleIDs);
                setList.append(set);
            }
            data._mesh.append(mesh);
            data._componentManager.append(setList);
        }
    }

    void ExportWriteThread::writeCGNSMeshFinishProcessing()
    {
        //获取读取接口
        IO::FITKCGNSIOInterface* interfaceIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKCGNSIOInterface>("IO::FITKCGNSIOInterface");
        if (!interfaceIO) return;
        IO::CGNSDataIO data = interfaceIO->getCGNSDataIO();
        //清空数据
        for (int i = 0; i < data._mesh.size(); ++i)
        {
            for (int j = 0; j < data._componentManager[i].size(); ++j)
            {
                if (data._componentManager[i][j])
                    delete data._componentManager[i][j];
            }
            if (data._mesh.at(i))
                delete data._mesh[i];
        }
        interfaceIO->clearCGNSDataIO();
    }

}


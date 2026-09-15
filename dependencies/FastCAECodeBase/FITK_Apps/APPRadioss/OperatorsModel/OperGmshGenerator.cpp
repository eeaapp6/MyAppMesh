/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGmshGenerator.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKCore/FITKScriptTextRepo.h"
#include "OperatorsInterface/TreeEventOperator.h"
//#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
//#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExport.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExportTopos.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Component/FITKGmshExeDriver/FITKMeshGenGmshExecInterface.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Component/FITKGmshExeDriver/GUIGmshSettings.h"

#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QHash>

namespace ModelOper
{
    bool OperGmshGenerator::execGUI()
    {
        //获取动作名称
        QString actionName = _emitter->objectName();
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;
        if (actionName == "actionGmshSettings")
        {
            this->meshGenOper();
        }
        return false;
    }

    bool OperGmshGenerator::execProfession()
    {
        this->meshGenFinished();
        return true;
    }

    void OperGmshGenerator::meshGenOper()
    {
        //获取工作目录
        QString meshPath = FITKAPP->getTempDir(false, "Gmsh");
        QString meshFile = QString("%1/%2").arg(meshPath).arg("mesh.msh");
        //获取网格划分接口
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKAbstractMesherDriver* mesher = mf->getMesherDriver("GmshExec");
        if (!mesher) return;

        //关联信号
        disconnect(mesher, SIGNAL(mesherFinished()), nullptr, nullptr);
        connect(mesher, SIGNAL(mesherFinished()), this, SLOT(meshGenFinished()));
        //设置参数
        mesher->setValue("MeshFile", meshFile);
        mesher->setValue("Method", 1);
        //开始划分网格
        mesher->startMesher();
        this->setArgs("MeshFile", meshFile);
        
    }

    void OperGmshGenerator::writePythonScript()
    {
        Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf || !cmdList) return;
        Interface::FITKAbstractMesherDriver* mesher = mf->getMesherDriver("GmshExec");
        Interface::FITKGlobalMeshSizeInfo* sizeInfo = mf->getGlobalMeshSizeInfo("GmshExec");
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* algorithmInfo = mf->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
        if (!mesher) return;
        //获取网格划分参数数据
        double maxSize = sizeInfo->getMaxSize();
        double minSize = sizeInfo->getMinSize();
        int dim = algorithmInfo->getMeshGenerateDimension();
        //获取网格划分数据
        QVariant v = {};
        v = mesher->getValue("virtualTopos");
        QList<Interface::VirtualShape> topos = v.value<QList<Interface::VirtualShape>>();
        if (topos.size() == 0) return;
        //生成Python脚本
        QStringList pythonCmd;
        pythonCmd.append("meshGen=MeshGenerate()");
        pythonCmd.append(QString("meshGen.setMeshSize(%1, %2)").arg(minSize).arg(maxSize));
        for (const Interface::VirtualShape& vShape : topos)
        {
            Interface::FITKAbsGeoCommand* geo_cmd = cmdList->getDataByID(vShape.CmdId);
            if (!geo_cmd) continue;
            QString gName = geo_cmd->getDataObjectName();
            switch (vShape.Type)
            {
            case Interface::FITKGeoEnum::VTopoShapeType::VSFace:
                pythonCmd.append(QString("meshGen.addFace('%1', [%2])").arg(gName).arg(vShape.VirtualTopoIndex));
                break;
            case Interface::FITKGeoEnum::VTopoShapeType::VSSolid:
                pythonCmd.append(QString("meshGen.addVolume('%1', [%2])").arg(gName).arg(vShape.VirtualTopoIndex));
                break;
            default:
                break;
            }
        }
        pythonCmd.append(QString("meshGen.generate(%1)").arg(dim));
        Core::FITKScriptTextRepo::getInstance()->appendScript(pythonCmd);
    }

    void OperGmshGenerator::meshGenFinished()
    {
        this->writePythonScript();
        QString meshFile = {};
        QVariantList dimList{ 0,1 };
        //获取网格文件，并判断是否生成成功
        if (!this->argValue<QString>("MeshFile", meshFile) || !QFileInfo(meshFile).isFile())
        {
            QMessageBox::warning(FITKAPP->getGlobalData()->getMainWindow(), tr("warning"), tr("Err! Mesher ProgramExec generate meshes failed."), QMessageBox::StandardButton::Ok);
            return;
        }
        //获取网格划分接口
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKAbstractMeshProcessor* processorMesh = mf->getMeshProcessor("GmshExec");
        if (!processorMesh) return;
        processorMesh->setValue("File", meshFile);
        //创建网格数据
        Interface::FITKUnstructuredMesh* mesh = new Interface::FITKUnstructuredMesh;
        Interface::FITKComponentManager* comp = new Interface::FITKComponentManager;
        //网格数据填充
        processorMesh->insertDataObject("Mesh", mesh);
        processorMesh->insertDataObject("ComponentManager", comp);
        processorMesh->setValue("FilterDim", dimList);
        //调用网格处理接口
        processorMesh->start(QStringList() << "MSH");
        AppFrame::FITKMessageNormal(QString("Read file succeed:%1 %2").arg(meshFile).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")));

        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!radiossCase) return;
        Radioss::FITKRadiossMeshModel* radiossMeshModel = radiossCase->getMeshModel();
        if (!radiossMeshModel) return;
        Radioss::FITKRadiossNodes* radiossNodes = radiossMeshModel->getNodes();
        if (!radiossNodes) return;
        Radioss::FITKRadiossPartManager* radiossPartManager = radiossMeshModel->getPartsManager();
        if (!radiossPartManager) return;
        Radioss::FITKRadiossPart* radiossPart = new Radioss::FITKRadiossPart;
        //将mesh中的网格数据填充至RadiossMeshModel
        const int nodeNum = mesh->getNodeCount();
        const int elementNum = mesh->getElementCount();
        QHash<int, int> nodeIdHash;
        //遍历node列表
        for (int i = 0; i < nodeNum; i++)
        {
            Core::FITKNode* node = mesh->getNodeAt(i);
            int nodeId = node->getNodeID();
            int newNodeID = radiossNodes->addNode(node->x(), node->y(), node->z());
            //将nodeID做映射
            nodeIdHash.insert(nodeId, newNodeID);
        }
        //遍历element列表
        for (int i = 0; i < elementNum; i++)
        {
            Interface::FITKAbstractElement* ele = mesh->getElementAt(i);
            if (ele == nullptr) continue;
            Interface::FITKModelEnum::FITKEleType type = ele->getEleType();
            //新建单元
            Interface::FITKAbstractElement* newElement = Interface::FITKElementFactory::createElement(type);
            if (newElement == nullptr) continue;
            QList<int> allnNodes = ele->getAllNodes();
            QList<int> newAllNodes{};
            for (int nodeId : allnNodes)
            {
                if (nodeIdHash.contains(nodeId))
                {
                    newAllNodes.append(nodeIdHash.value(nodeId));
                }
            }
            if (newElement->getNodeCount() != newAllNodes.size())
            {
                continue;
            }
            //添加映射后的节点ID
            newElement->setNodeID(newAllNodes);
            newElement->setEleID(ele->getEleID());
            radiossPart->addElement(newElement);
        }
        QString partName = radiossPartManager->checkName("Part-1");
        radiossPart->setDataObjectName(partName);
        radiossPartManager->appendDataObj(radiossPart);

        //释放暂存数据
        delete mesh;
        delete comp;

        // 通过树形菜单事件处理器刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }

        //更新渲染
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateGraph(radiossPart->getDataObjectID());
        }

        AppFrame::FITKMessageNormal(QString("Graph succeed:%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")));

        this->setArgs("MeshPartName", partName);
    }
}

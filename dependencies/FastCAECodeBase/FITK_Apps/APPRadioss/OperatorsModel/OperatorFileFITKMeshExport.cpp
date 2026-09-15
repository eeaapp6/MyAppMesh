/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorFileFITKMeshExport.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKInterfaceModel/FITKStructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementVertex.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementLine.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementTri.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementQuad.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementTet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementHex.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementWedge.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementPolygon.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceNode.h"
#include "FITK_Component/FITKMeshIO/FITKMeshIOInterface.h"
#include "FITK_Component/FITKMeshIO/FITKMeshIOEnum.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodeGroup.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossElementGroup.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"
#include <QFileDialog>

namespace ModelOper
{
    OperatorFileFITKMeshExport::OperatorFileFITKMeshExport()
    {
        m_mapSetID = QHash<int, int>();
    }

    OperatorFileFITKMeshExport::~OperatorFileFITKMeshExport()
    {
        m_mapSetID.clear();
        QHash<int, int>().swap(m_mapSetID);
    }

    bool OperatorFileFITKMeshExport::execGUI()
    {
        // 2.执行界面操作

        // 判断是否正常触发
        if (_emitter == nullptr || _emitter->objectName() != "actionExportFITKMesh") return ERROR(tr("Failed to operate!"));
        
        // 获取全局设置
        AppFrame::FITKAppSettings* setting = FITKAPP->getAppSettings();
        if (setting == nullptr) return ERROR(tr("Failed to get the global setting!"));

        // 获取工作路径
        QString workdir = setting->getWorkingDir();
        if (workdir.isEmpty())
        {
            AppFrame::FITKMessageWarning(tr("Failed to get the working directory! Please check if the working directory has been set successfully!"));
            workdir = QApplication::applicationDirPath();
        }
        AppFrame::FITKMessageNormal(tr("Current working directory: %1").arg(workdir));

        // 显示文件对话框
        QFileDialog dialog(FITKAPP->getGlobalData()->getMainWindow(), tr("Export FITKMesh File"), workdir, tr("FITKMesh File(*.fitkmesh)"));
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.show();

        // 阻塞，否则键盘事件处理会出问题
        bool accept = false;
        QEventLoop loop;
        connect(&dialog, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
        connect(&dialog, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
        loop.exec();
        if (!accept) return false;

        // 获取文件列表
        QStringList list = dialog.selectedFiles();
        if (list.size() != 1) return ERROR(tr("Please specify the file saving path."));

        // 获取文件路径
        QString filePath = list.at(0);
        if (filePath.isEmpty()) return ERROR(tr("Failed to get the file path!"));
        if (!filePath.endsWith(".fitkmesh")) filePath += ".fitkmesh";
        AppFrame::FITKMessageNormal(tr("The file will be saved to the following path: %1.").arg(filePath));

        // 设置参数
        this->setArgs("FilePath", filePath);

        return true;
    }

    bool OperatorFileFITKMeshExport::execProfession()
    {
        // 3.执行业务逻辑
        
        // 获取文件保存路径
        QString fileName = "";
        if (!this->argValue<QString>("FilePath", fileName)) return ERROR(tr("Failed to get the file path!"));

        // 导出开始信息提示
        AppFrame::FITKMessageNormal(tr("Start the export process!"));

        // 初始化
        QList<Interface::FITKStructuredMesh*> listStructuredMesh = {};
        QList<Interface::FITKUnstructuredMesh*> listUnstructuredMesh = {};
        QList<Interface::FITKAbstractModelComponent*> listComponent = {};

        // 获取Radioss网格模型数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return ERROR(tr("Failed to get the current case date!"));
        Radioss::FITKRadiossMeshModel* dataMeshModel = dataCase->getMeshModel();
        if (dataMeshModel == nullptr) return ERROR(tr("Failed to get the mesh model data!"));
        
        // 创建网格数据对象
        Interface::FITKUnstructuredMesh* mesh = new Interface::FITKUnstructuredMesh();
        if (mesh == nullptr) return ERROR(tr("Failed to create the mesh data!"));
        int idMesh = mesh->getDataObjectID();
        
        // 复制节点数据
        Radioss::FITKRadiossNodes* dataNodes = dataMeshModel->getNodes();
        if (dataNodes == nullptr) return ERROR(tr("Failed to get the nodes data!"));
        int countNode = dataNodes->getNodeCount();
        for (int i = 0; i < countNode; ++i) copyNode(mesh, dataNodes->getNodeAt(i));

        // 复制单元数据
        Radioss::FITKRadiossPartManager* managerPart = dataMeshModel->getPartsManager();
        if (managerPart == nullptr) return ERROR(tr("Failed to get the part manager!"));
        int countPart = managerPart->getDataCount();
        for (int i = 0; i < countPart; ++i)
        {
            // 获取部件数据
            Radioss::FITKRadiossPart* part = managerPart->getDataByIndex(i);
            if (part == nullptr) return ERROR(tr("Failed to get the part data!"));
            for (int idElement : part->getPartElementIDs()) copyElement(mesh, part->getElementByID(idElement));
        }

        // 复制网格数据
        mesh->setDataObjectName(dataMeshModel->getDataObjectName());
        mesh->enable(dataMeshModel->isEnable());

        // 追加网格数据
        listUnstructuredMesh.append(mesh);
        mesh = nullptr;

        // 复制组件数据
        Interface::FITKComponentManager* managerComponent = dataMeshModel->getComponentManager();
        if (managerComponent == nullptr) return ERROR(tr("Failed to get the component manager!"));
        if (!copyComponent(managerComponent, listComponent, idMesh)) return ERROR(tr("Failed to copy the component data!"));
        
        // 开启IO线程
        IO::FITKMeshIOInterface* interface = FITKAPP->getComponents()->getComponentTByName<IO::FITKMeshIOInterface>("FITKMeshIO");
        if (interface == nullptr) return ERROR(tr("Failed to get the component interface!"));
        interface->setRunInThread(true);
        interface->setFileName(fileName);
        interface->setVersion(1.0);
        interface->setStructuredMeshList(listStructuredMesh);
        interface->setUnstructuredMeshList(listUnstructuredMesh);
        interface->setComponentList(listComponent);
        connect(interface, &IO::FITKMeshIOInterface::ioThreadFinishedSig, this, &OperatorFileFITKMeshExport::ioThreadFinishedSlot);
        return interface->exec(2);
    }

    void OperatorFileFITKMeshExport::preArgs()
    {
        // 1.预处理参数
    }

    void OperatorFileFITKMeshExport::ioThreadFinishedSlot()
    {
        // 4.线程结束信号槽
        
        // 导出结束信息提示
        AppFrame::FITKMessageNormal(tr("End the export process!"));

        // 清除参数
        this->clearArgs();

        // 获取组件接口
        IO::FITKMeshIOInterface* interface = FITKAPP->getComponents()->getComponentTByName<IO::FITKMeshIOInterface>("FITKMeshIO");
        if (interface == nullptr)
        {
            AppFrame::FITKMessageError(tr("Failed to get the component interface!"));
            return;
        }
        interface->clearStructuredMesh();
        interface->clearUnstructuredMesh();
        interface->clearComponentList();

        m_mapSetID.clear();
        QHash<int, int>().swap(m_mapSetID);

        // 判断结果
        if (interface->isSuccess()) AppFrame::FITKMessageNormal(tr("%1 file exported successfully!").arg(interface->getFileName()));
        else AppFrame::FITKMessageError(tr("Failed to export %1 file!").arg(interface->getFileName()));
    }

    bool OperatorFileFITKMeshExport::ERROR(const QString& message)
    {
        AppFrame::FITKMessageError(message);
        return false;
    }

    bool OperatorFileFITKMeshExport::copyNode(Interface::FITKUnstructuredMesh* mesh, Core::FITKNode* node)
    {
        // 检查参数
        if (mesh == nullptr) return ERROR(tr("Failed to get the mesh data!"));
        if (node == nullptr) return ERROR(tr("Failed to get the node data!"));

        // 复制节点数据
        double coord[3] = { 0.0, 0.0, 0.0 };
        node->getCoor(coord);
        mesh->addNode(node->getNodeID(), coord[0], coord[1], coord[2], node->getNativeFlag(), node->getTag());

        return true;
    }

    bool OperatorFileFITKMeshExport::copyElement(Interface::FITKUnstructuredMesh* mesh, Interface::FITKAbstractElement* element)
    {
        // 检查参数
        if (mesh == nullptr) return ERROR(tr("Failed to get the mesh data!"));
        if (element == nullptr) return ERROR(tr("Failed to get the element data!"));

        // 根据单元类型创建新的单元对象
        Interface::FITKAbstractElement* elementNew = nullptr;
        Interface::FITKModelEnum::FITKEleType type = element->getEleType();
        if (type == Interface::FITKModelEnum::FITKEleType::Vertex1) elementNew = new Interface::FITKElementVertex1();
        else if (type == Interface::FITKModelEnum::FITKEleType::Line2) elementNew = new Interface::FITKElementLine2();
        else if (type == Interface::FITKModelEnum::FITKEleType::Line3) elementNew = new Interface::FITKElementLine3();
        else if (type == Interface::FITKModelEnum::FITKEleType::Tri3) elementNew = new Interface::FITKElementTri3();
        else if (type == Interface::FITKModelEnum::FITKEleType::Tri6) elementNew = new Interface::FITKElementTri6();
        else if (type == Interface::FITKModelEnum::FITKEleType::Quad4) elementNew = new Interface::FITKElementQuad4();
        else if (type == Interface::FITKModelEnum::FITKEleType::Quad8) elementNew = new Interface::FITKElementQuad8();
        else if (type == Interface::FITKModelEnum::FITKEleType::Tet4) elementNew = new Interface::FITKElementTet4();
        else if (type == Interface::FITKModelEnum::FITKEleType::Tet10) elementNew = new Interface::FITKElementTet10();
        else if (type == Interface::FITKModelEnum::FITKEleType::Wedge6) elementNew = new Interface::FITKElementWedge6();
        else if (type == Interface::FITKModelEnum::FITKEleType::Wedge15) elementNew = new Interface::FITKElementWedge15();
        else if (type == Interface::FITKModelEnum::FITKEleType::Hex8) elementNew = new Interface::FITKElementHex8();
        else if (type == Interface::FITKModelEnum::FITKEleType::Hex20) elementNew = new Interface::FITKElementHex20();
        else if (type == Interface::FITKModelEnum::FITKEleType::Polygon) elementNew = new Interface::FITKElementPolygon();
        else return ERROR(tr("Unsupported element type!"));
        if (elementNew == nullptr) return ERROR(tr("Failed to create the element!"));
        
        // 复制单元数据
        elementNew->setEleID(element->getEleID());
        elementNew->setNodeID(element->getAllNodes());
        elementNew->setBlockTag(element->getBlockTag());
        elementNew->setNativeFlag(element->getNativeFlag());
        elementNew->setTag(element->getTag());
        elementNew->enable(element->isEnable());
        mesh->appendElement(elementNew);

        return true;
    }

    bool OperatorFileFITKMeshExport::copyComponent(Interface::FITKComponentManager* managerComponent, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh)
    {
        // 检查参数
        if (managerComponent == nullptr) return ERROR(tr("Failed to get the component manager!"));
        if (idMesh < 1) return ERROR(tr("Invalid mesh ID!"));

        // 遍历组件数据
        int countComponent = managerComponent->getDataCount();
        for (int i = 0; i < countComponent; ++i)
        {
            // 获取组件数据
            Interface::FITKAbstractModelComponent* component = managerComponent->getDataByIndex(i);
            if (component == nullptr) continue;
            
            // 复制集合数据
            Interface::FITKModelSet* dataSet = dynamic_cast<Interface::FITKModelSet*>(component);
            if (dataSet != nullptr)
            {
                Interface::FITKModelEnum::FITKModelSetType type = dataSet->getModelSetType();
                if (type == Interface::FITKModelEnum::FMSNode) { if (!copyNodeSet(dataSet, listComponent, idMesh)) return ERROR(tr("Failed to copy the node set data!")); }
                else if (type == Interface::FITKModelEnum::FMSElem) { if (!copyElementSet(dataSet, listComponent, idMesh)) return ERROR(tr("Failed to copy the element set data!")); }
                else if (type == Interface::FITKModelEnum::FMSMIX) { if (!copyMixSet(dataSet, listComponent, idMesh)) return ERROR(tr("Failed to copy the mix set data!")); }
                else return ERROR(tr("Invalid model set type!"));
                continue;
            }

            // 复制表面数据
            Interface::FITKMeshSurface* dataSurface = dynamic_cast<Interface::FITKMeshSurface*>(component);
            if (dataSurface != nullptr)
            {
                Interface::FITKMeshSurface::MeshSurfaceType type = dataSurface->getMeshSurfaceType();
                if (type == Interface::FITKMeshSurface::SurNode) { if (!copyNodeSurface(dataSurface, listComponent, idMesh)) return ERROR(tr("Failed to copy the node surface data!")); }
                else if (type == Interface::FITKMeshSurface::SurEle) { if (!copyElementSurface(dataSurface, listComponent, idMesh)) return ERROR(tr("Failed to copy the element surface data!")); }
                else return ERROR(tr("Invalid mesh surface type!"));
                continue;
            }
        }

        return true;
    }

    bool OperatorFileFITKMeshExport::copyNodeSet(Interface::FITKModelSet* dataSet, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh)
    {
        // 检查参数
        if (dataSet == nullptr) return ERROR(tr("Failed to get the node set data!"));
        if (idMesh < 1) return ERROR(tr("Failed to get the mesh ID!"));

        // 获取Radioss节点组数据
        Radioss::FITKRadiossNodeGroup* dataNodeGroup = dynamic_cast<Radioss::FITKRadiossNodeGroup*>(dataSet);
        if (dataNodeGroup == nullptr) return ERROR(tr("Failed to get the node group data!"));
        int idNodeGroup = dataNodeGroup->getDataObjectID();

        // 创建节点集合数据
        Interface::FITKModelSet* dataNodeSet = new Interface::FITKModelSet();
        if (dataNodeSet == nullptr) return ERROR(tr("Failed to create the node set data!"));
        int idNodeSet = dataNodeSet->getDataObjectID();

        // 建立集合ID映射关系
        if (m_mapSetID.contains(idNodeGroup)) return ERROR(tr("The node group ID already exists!"));
        else m_mapSetID.insert(idNodeGroup, idNodeSet);

        // 复制节点集合数据
        dataNodeSet->setDataObjectName(dataNodeGroup->getDataObjectName());
        dataNodeSet->setModelSetType(Interface::FITKModelEnum::FMSNode);
        dataNodeSet->setInternal(dataNodeGroup->isInternal());
        dataNodeSet->setGenerated(dataNodeGroup->isGenerated());
        dataNodeSet->setAbsoluteMember(dataNodeGroup->getAbsoluteMember());
        dataNodeSet->setModel(idMesh);
        dataNodeSet->setColor(dataNodeGroup->getColor());

        // 追加节点集合数据
        listComponent.append(dataNodeSet);

        return true;
    }

    bool OperatorFileFITKMeshExport::copyElementSet(Interface::FITKModelSet* dataSet, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh)
    {
        // 检查参数
        if (dataSet == nullptr) return ERROR(tr("Failed to get the element set data!"));
        if (idMesh < 1) return ERROR(tr("Failed to get the mesh ID!"));

        // 获取Radioss单元组数据
        Radioss::FITKRadiossElementGroup* dataElementGroup = dynamic_cast<Radioss::FITKRadiossElementGroup*>(dataSet);
        if (dataElementGroup == nullptr) return ERROR(tr("Failed to get the element group data!"));
        int idElementGroup = dataElementGroup->getDataObjectID();

        // 创建单元集合数据
        Interface::FITKModelSet* dataElementSet = new Interface::FITKModelSet();
        if (dataElementSet == nullptr) return ERROR(tr("Failed to create the element set data!"));
        int idElementSet = dataElementSet->getDataObjectID();

        // 建立集合ID映射关系
        if (m_mapSetID.contains(idElementGroup)) return ERROR(tr("The element group ID already exists!"));
        else m_mapSetID.insert(idElementGroup, idElementSet);

        // 复制单元集合数据
        dataElementSet->setDataObjectName(dataElementGroup->getDataObjectName());
        dataElementSet->setModelSetType(Interface::FITKModelEnum::FMSElem);
        dataElementSet->setInternal(dataElementGroup->isInternal());
        dataElementSet->setGenerated(dataElementGroup->isGenerated());
        dataElementSet->setAbsoluteMember(dataElementGroup->getAbsoluteMember());
        dataElementSet->setModel(idMesh);
        dataElementSet->setColor(dataElementGroup->getColor());

        // 追加单元集合数据
        listComponent.append(dataElementSet);

        return true;
    }

    bool OperatorFileFITKMeshExport::copyMixSet(Interface::FITKModelSet* dataSet, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh)
    {
        // 检查参数
        if (dataSet == nullptr) return ERROR(tr("Failed to get the mix set data!"));
        if (idMesh < 1) return ERROR(tr("Failed to get the mesh ID!"));

        // 遍历子集合
        int countSubSet = dataSet->getDataCount();
        for (int i = 0; i < countSubSet; ++i)
        {
            // 获取子集合数据
            Interface::FITKModelSet* dataSubSet = dataSet->getDataByIndex(i);
            if (dataSubSet == nullptr) return ERROR(tr("Failed to get the sub set data!"));

            // 根据子集合类型复制数据
            Interface::FITKModelEnum::FITKModelSetType type = dataSubSet->getModelSetType();
            if (type == Interface::FITKModelEnum::FMSNode) { if (!copyNodeSet(dataSubSet, listComponent, idMesh)) return ERROR(tr("Failed to copy the sub node set data!")); }
            else if (type == Interface::FITKModelEnum::FMSElem) { if (!copyElementSet(dataSubSet, listComponent, idMesh)) return ERROR(tr("Failed to copy the sub element set data!")); }
            else if (type == Interface::FITKModelEnum::FMSMIX) { if (!copyMixSet(dataSubSet, listComponent, idMesh)) return ERROR(tr("Failed to copy the sub mix set data!")); }
            else return ERROR(tr("Invalid sub set type!"));
        }

        return true;
    }
    
    bool OperatorFileFITKMeshExport::copyNodeSurface(Interface::FITKMeshSurface* dataSurface, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh)
    {
        // 检查参数
        if (dataSurface == nullptr) return ERROR(tr("Failed to get the node surface data!"));
        if (idMesh < 1) return ERROR(tr("Failed to get the mesh ID!"));

        // 获取节点表面数据
        Interface::FITKMeshSurfaceNode* dataNodeSurfaceOld = dynamic_cast<Interface::FITKMeshSurfaceNode*>(dataSurface);
        if (dataNodeSurfaceOld == nullptr) return ERROR(tr("Failed to get the node surface data!"));

        // 创建节点表面数据
        Interface::FITKMeshSurfaceNode* dataSurfaceNodeNew = new Interface::FITKMeshSurfaceNode();
        if (dataSurfaceNodeNew == nullptr) return ERROR(tr("Failed to create the node surface data!"));

        // 复制节点表面数据
        int countSet = dataSurface->getSetCount();
        for (int i = 0; i < countSet; i++)
        {
            int idSet = dataNodeSurfaceOld->getSetIDAt(i);
            double value = dataNodeSurfaceOld->getValueAt(i);
            if (!m_mapSetID.contains(idSet)) return ERROR(tr("Failed to get the new node set ID by the old node set ID!"));
            else dataSurfaceNodeNew->addMeshSet(idMesh, m_mapSetID[idSet], value);
        }
        dataSurfaceNodeNew->setDataObjectName(dataNodeSurfaceOld->getDataObjectName());
        dataSurfaceNodeNew->enable(dataNodeSurfaceOld->isEnable());
        dataSurfaceNodeNew->isInternal(dataNodeSurfaceOld->isInternal());
        dataSurfaceNodeNew->setModel(idMesh);
        dataSurfaceNodeNew->setColor(dataNodeSurfaceOld->getColor());

        // 追加节点表面数据
        listComponent.append(dataSurfaceNodeNew);

        return true;
    }

    bool OperatorFileFITKMeshExport::copyElementSurface(Interface::FITKMeshSurface* dataSurface, QList<Interface::FITKAbstractModelComponent*>& listComponent, int idMesh)
    {
        // 检查参数
        if (dataSurface == nullptr) return ERROR(tr("Failed to get the node surface data!"));
        if (idMesh < 1) return ERROR(tr("Failed to get the mesh ID!"));

        // 获取单元表面数据
        Radioss::FITKRadiossSurfaceSeg* dataNodeSurfaceOld = dynamic_cast<Radioss::FITKRadiossSurfaceSeg*>(dataSurface);
        if (dataNodeSurfaceOld == nullptr) return ERROR(tr("Failed to get the node surface data!"));

        // 创建单元表面数据
        Interface::FITKMeshSurfaceElement* dataElementSurfaceNew = new Interface::FITKMeshSurfaceElement();
        if (dataElementSurfaceNew == nullptr) return ERROR(tr("Failed to create the element surface data!"));
        
        // 获取表面索引与单元ID列表映射关系
        QHash<int, QList<int>> mapFaceIndexElementIDs = {};
        for (int elementid : dataNodeSurfaceOld->getAllElement())
        {
            for (int faceindex : dataNodeSurfaceOld->getElementSurfaceIDs(elementid))
            {
                if (faceindex == 0) faceindex = FITKMeshIO::ESI_S1;
                else if (faceindex == 1) faceindex = FITKMeshIO::ESI_S2;
                else if (faceindex == 2) faceindex = FITKMeshIO::ESI_S3;
                else if (faceindex == 3) faceindex = FITKMeshIO::ESI_S4;
                else if (faceindex == 4) faceindex = FITKMeshIO::ESI_S5;
                else if (faceindex == 5) faceindex = FITKMeshIO::ESI_S6;
                else return ERROR(tr("Invalid face index"));

                if (mapFaceIndexElementIDs.contains(faceindex)) mapFaceIndexElementIDs[faceindex].append(elementid);
                else mapFaceIndexElementIDs.insert(faceindex, QList<int>() << elementid);
            }
        }
        
        // 复制单元表面数据
        for (int faceindex : mapFaceIndexElementIDs.keys())
        {
            Interface::FITKModelSet* dataSet = new Interface::FITKModelSet();
            if (dataSet == nullptr) return ERROR(tr("Failed to create the element set"));
            dataSet->setDataObjectName(QString("InternalSet-%1").arg(dataSet->getDataObjectID()));
            dataSet->setModelSetType(Interface::FITKModelEnum::FMSElem);
            dataSet->setModel(idMesh);
            dataSet->setAbsoluteMember(mapFaceIndexElementIDs.value(faceindex));
            dataSet->setInternal(true);
            dataSet->setColor(dataNodeSurfaceOld->getColor());
            listComponent.append(dataSet);
            dataElementSurfaceNew->addMeshSet(idMesh, dataSet->getDataObjectID(), faceindex);
        }
        dataElementSurfaceNew->setDataObjectName(dataNodeSurfaceOld->getDataObjectName());
        dataElementSurfaceNew->enable(dataNodeSurfaceOld->isEnable());
        dataElementSurfaceNew->isInternal(dataNodeSurfaceOld->isInternal());
        dataElementSurfaceNew->setModel(idMesh);
        dataElementSurfaceNew->setColor(dataNodeSurfaceOld->getColor());

        // 追加单元表面数据
        listComponent.append(dataElementSurfaceNew);

        return true;
    }
}


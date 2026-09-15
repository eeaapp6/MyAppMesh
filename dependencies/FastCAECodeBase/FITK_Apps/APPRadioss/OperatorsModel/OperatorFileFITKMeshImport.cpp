/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorFileFITKMeshImport.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementVertex.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementLine.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementTri.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementQuad.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementTet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementHex.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementWedge.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementPolygon.h"
#include "FITK_Interface/FITKInterfaceModel/FITKStructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceNode.h"
#include "FITK_Component/FITKMeshIO/FITKMeshIOInterface.h"
#include "FITK_Component/FITKMeshIO/FITKMeshIOEnum.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodeGroup.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossElementGroup.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include <QApplication>
#include <QFileDialog>
#include <QSet>

namespace ModelOper
{
    OperatorFileFITKMeshImport::OperatorFileFITKMeshImport()
    {
        m_mapNodeID = {};
        m_mapElemID = {};
        m_mapSetID = {};
    }

    OperatorFileFITKMeshImport::~OperatorFileFITKMeshImport()
    {
        for (QHash<int, int> hash : m_mapNodeID.values())
        {
            hash.clear();
            QHash<int, int>().swap(hash);
        }
        m_mapNodeID.clear();
        QHash<int, QHash<int, int>>().swap(m_mapNodeID);

        for (QHash<int, int> hash : m_mapElemID.values())
        {
            hash.clear();
            QHash<int, int>().swap(hash);
        }
        m_mapElemID.clear();
        QHash<int, QHash<int, int>>().swap(m_mapElemID);

        m_mapSetID.clear();
        QHash<int, int>().swap(m_mapSetID);
    }

    bool OperatorFileFITKMeshImport::execGUI()
    {
        // 2.执行界面操作

        // 判断是否正常触发
        if (_emitter == nullptr || _emitter->objectName() != "actionImportFITKMesh") return ERROR(tr("Failed to operate!"));

        // 获取全局设置
        AppFrame::FITKAppSettings* setting = FITKAPP->getAppSettings();
        if (setting == nullptr) return ERROR(tr("Failed to get the global setting!"));

        // 获取工作路径
        QString workdir = setting->getWorkingDir();
        if (workdir.isEmpty())
        {
            AppFrame::FITKMessageWarning(tr("Get the working directory failed! Please check if the working directory has been set successfully!"));
            workdir = QApplication::applicationDirPath();
        }
        AppFrame::FITKMessageNormal(tr("Current working directory: %1").arg(workdir));

        // 显示文件对话框
        QFileDialog dialog(FITKAPP->getGlobalData()->getMainWindow(), tr("Import FITKMesh File"), workdir, tr("FITKMesh File(*.fitkmesh)"));
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
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
        AppFrame::FITKMessageNormal(tr("The file will be opened to the following path: %1.").arg(filePath));

        // 设置参数
        this->setArgs("FilePath", filePath);

        return true;
    }
    
    bool OperatorFileFITKMeshImport::execProfession()
    {
        // 3.执行业务逻辑

        // 获取文件保存路径
        QString filePath = "";
        if (!this->argValue<QString>("FilePath", filePath)) return ERROR(tr("Failed to get the file path!"));

        // 导入开始信息提示
        AppFrame::FITKMessageNormal(tr("Start the import process!"));

        // 开启IO线程
        IO::FITKMeshIOInterface* interface = FITKAPP->getComponents()->getComponentTByName<IO::FITKMeshIOInterface>("FITKMeshIO");
        if (interface == nullptr) return ERROR(tr("Failed to get the component interface!"));
        interface->setRunInThread(true);
        interface->setFileName(filePath);
        connect(interface, &IO::FITKMeshIOInterface::ioThreadFinishedSig, this, &OperatorFileFITKMeshImport::ioThreadFinishedSlot);
        return interface->exec(1);
    }
    
    void OperatorFileFITKMeshImport::preArgs()
    {
        // 1.预处理参数
    }

    void OperatorFileFITKMeshImport::ioThreadFinishedSlot()
    {
        // 导入结束信息提示
        AppFrame::FITKMessageNormal(tr("End the import process!"));

        // 清除参数
        this->clearArgs();
        
        // 获取组件接口
        IO::FITKMeshIOInterface* interface = FITKAPP->getComponents()->getComponentTByName<IO::FITKMeshIOInterface>("FITKMeshIO");
        if (interface == nullptr)
        {
            AppFrame::FITKMessageError(tr("Failed to get the component interface!"));
            return;
        }

        // 判断结果
        if (interface->isSuccess()) AppFrame::FITKMessageNormal(tr("%1 file imported successfully!").arg(interface->getFileName()));
        else
        {
            AppFrame::FITKMessageError(tr("Failed to import %1 file!").arg(interface->getFileName()));
            return;
        }

        // 存储数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            ERROR(tr("Failed to get the current case date!"));
            return;
        }
        Radioss::FITKRadiossMeshModel* dataMeshModel = dataCase->getMeshModel();
        if (dataMeshModel == nullptr)
        {
            ERROR(tr("Failed to get the mesh model data!"));
            return;
        }
        if (!convert(dataMeshModel, interface))
        {
            AppFrame::FITKMessageError(tr("Failed to convert data!"));
            return;
        }

        // 清除接口数据
        disconnect(interface, &IO::FITKMeshIOInterface::ioThreadFinishedSig, this, &OperatorFileFITKMeshImport::ioThreadFinishedSlot);
        interface->clearStructuredMesh();
        interface->clearUnstructuredMesh();
        interface->clearComponentList();

        //更新渲染
        if (!updateUI())
        {
            AppFrame::FITKMessageError(tr("Failed to update the UI!"));
            return;
        }
    }

    bool OperatorFileFITKMeshImport::convert(Radioss::FITKRadiossMeshModel* dataMeshModel, IO::FITKMeshIOInterface* api)
    {
        // 检查参数
        if (dataMeshModel == nullptr) return ERROR(tr("Failed to get the mesh model data!"));
        if (api == nullptr) return ERROR(tr("Failed to get the component interface!"));

        // 获取Radioss网格模型ID
        int idMeshModel = dataMeshModel->getDataObjectID();

        // 获取数据
        QList<Interface::FITKUnstructuredMesh*> listUnstructuredMesh = api->getUnstructuredMeshList();
        QList<Interface::FITKAbstractModelComponent*> listComponent = api->getComponentList();

        // 获取管理器
        Radioss::FITKRadiossNodes* managerNode = dataMeshModel->getNodes();
        if (managerNode == nullptr) return ERROR(tr("Failed to get the node manager!"));
        Radioss::FITKRadiossPartManager* managerPart = dataMeshModel->getPartsManager();
        if (managerPart == nullptr) return ERROR(tr("Failed to get the part manager!"));
        Interface::FITKComponentManager* managerComponent = dataMeshModel->getComponentManager();
        if (managerComponent == nullptr) return ERROR(tr("Failed to get the component manager!"));

        // 转换非结构化网格数据
        for (Interface::FITKUnstructuredMesh* mesh : listUnstructuredMesh)
        {
            // 检查数据
            if (mesh == nullptr) return ERROR(tr("Failed to get the unstructured mesh data!"));

            // 转换节点数据
            if (!convertNode(managerNode, mesh)) return ERROR(tr("Failed to convert the node data!"));
            // 转换单元数据
            if (!convertElement(managerPart, mesh)) return ERROR(tr("Failed to convert the element data!"));
        }

        // 转换组件数据
        for (Interface::FITKAbstractModelComponent* component : listComponent)
        {
            // 检查数据
            if (component == nullptr) return ERROR(tr("Failed to get the model component data!"));

            // 转换集合数据
            Interface::FITKModelSet* dataSet = dynamic_cast<Interface::FITKModelSet*>(component);
            if (dataSet != nullptr)
            {
                Interface::FITKModelEnum::FITKModelSetType type = dataSet->getModelSetType();
                if (type == Interface::FITKModelEnum::FMSNode) { if (!convertNodeSet(managerComponent, dataSet, idMeshModel)) return ERROR(tr("Failed to convert the node set data!")); }
                else if (type == Interface::FITKModelEnum::FMSElem) { if (!convertElementSet(managerComponent, dataSet, idMeshModel)) return ERROR(tr("Failed to convert the element set data!")); }
                else if (type == Interface::FITKModelEnum::FMSMIX) { if (!convertMixSet(managerComponent, dataSet, idMeshModel)) return ERROR(tr("Failed to convert the mix set data!")); }
                else return ERROR(tr("Invalid model set type!"));
                continue;
            }

            // 转换表面数据
            Interface::FITKMeshSurface* dataSurface = dynamic_cast<Interface::FITKMeshSurface*>(component);
            if (dataSurface != nullptr)
            {
                Interface::FITKMeshSurface::MeshSurfaceType type = dataSurface->getMeshSurfaceType();
                if (type == Interface::FITKMeshSurface::SurNode) { if (!convertNodeSurface(managerComponent, dynamic_cast<Interface::FITKMeshSurfaceNode*>(dataSurface), idMeshModel)) return ERROR(tr("Failed to convert the node surface data!")); }
                else if (type == Interface::FITKMeshSurface::SurEle) { if (!convertElementSurface(managerComponent, dynamic_cast<Interface::FITKMeshSurfaceElement*>(dataSurface), idMeshModel)) return ERROR(tr("Failed to convert the element surface data!")); }
                else return ERROR(tr("Invalid mesh surface type!"));
                continue;
            }
        }
        
        return true;
    }

    bool OperatorFileFITKMeshImport::convertNode(Radioss::FITKRadiossNodes* managerNode, Interface::FITKUnstructuredMesh* mesh)
    {
        // 检查参数
        if (managerNode == nullptr) return ERROR(tr("Failed to get the node manager!"));
        if (mesh == nullptr) return ERROR(tr("Failed to get the unstructured mesh data!"));

        // 获取非结构化网格ID
        int idMesh = mesh->getDataObjectID();

        // 初始化映射表
        if (!m_mapNodeID.contains(idMesh)) m_mapNodeID.insert(idMesh, QHash<int, int>());

        // 转换节点数据
        int countNode = mesh->getNodeCount();
        for (int i = 0; i < countNode; i++)
        {
            // 获取节点数据
            Core::FITKNode* dataNode = mesh->getNodeAt(i);
            if (dataNode == nullptr) return ERROR(tr("Failed to get the node data!"));

            // 追加节点数据并构建新旧节点ID映射关系
            int idNodeOld = dataNode->getNodeID();
            double coord[3] = { 0.0, 0.0, 0.0 };
            dataNode->getCoor(coord);
            int idNodeNew = managerNode->addNode(coord[0], coord[1], coord[2], dataNode->getNativeFlag(), dataNode->getTag());
            if (m_mapNodeID[idMesh].contains(idNodeOld)) return ERROR(tr("Duplicate node ID!"));
            else m_mapNodeID[idMesh].insert(idNodeOld, idNodeNew);
        }

        return true;
    }

    bool OperatorFileFITKMeshImport::convertElement(Radioss::FITKRadiossPartManager* managerPart, Interface::FITKUnstructuredMesh* mesh)
    {
        // 检查参数
        if (managerPart == nullptr) return ERROR(tr("Failed to get the part manager!"));
        if (mesh == nullptr) return ERROR(tr("Failed to get the unstructured mesh data!"));

        // 获取非结构化网格ID
        int idMesh = mesh->getDataObjectID();

        // 创建部件数据
        Radioss::FITKRadiossPart* dataPart = new Radioss::FITKRadiossPart();
        if (dataPart == nullptr) return ERROR(tr("Failed to create the part data!"));

        // 获取部件ID
        int idPart = dataPart->getDataObjectID();

        // 设置部件名称
        dataPart->setDataObjectName(QString("Part-%1").arg(idPart));

        // 初始化映射表
        if (!m_mapElemID.contains(idMesh)) m_mapElemID.insert(idMesh, QHash<int, int>());
        
        // 转换单元数据
        int countElement = mesh->getElementCount();
        for (int i = 0; i < countElement; i++)
        {
            // 获取单元数据
            Interface::FITKAbstractElement* dataElement = mesh->getElementAt(i);
            if (dataElement == nullptr) return ERROR(tr("Failed to get the element data!"));

            // 根据单元类型创建新的单元对象
            Interface::FITKAbstractElement* elementNew = nullptr;
            Interface::FITKModelEnum::FITKEleType type = dataElement->getEleType();
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
            elementNew->setBlockTag(dataElement->getBlockTag());
            elementNew->setNativeFlag(dataElement->getNativeFlag());
            elementNew->setTag(dataElement->getTag());
            elementNew->enable(dataElement->isEnable());

            // 替换旧节点ID
            QList<int> nodeOldIDs = dataElement->getAllNodes(), nodeNewIDs = {};
            for (int nodeID : nodeOldIDs)
            {
                if (!m_mapNodeID[idMesh].contains(nodeID)) return ERROR(tr("Failed to get the new node ID by the old node ID!"));
                else nodeNewIDs.append(m_mapNodeID[idMesh][nodeID]);
            }
            elementNew->setNodeID(nodeNewIDs);

            // 追加单元数据并构建新旧单元ID映射关系
            int idElementOld = dataElement->getEleID();
            int idElementNew = dataPart->addElement(elementNew);
            if (m_mapElemID[idMesh].contains(idElementOld)) return ERROR(tr("Duplicate element ID!"));
            else m_mapElemID[idMesh].insert(idElementOld, idElementNew);
        }

        // 追加部件数据
        managerPart->appendDataObj(dataPart);

        return true;
    }

    bool OperatorFileFITKMeshImport::convertNodeSet(Interface::FITKComponentManager* managerComponent, Interface::FITKModelSet* dataSet, int idMeshModel)
    {
        // 检查参数
        if (managerComponent == nullptr) return ERROR(tr("Failed to get the component manager!"));
        if (dataSet == nullptr) return ERROR(tr("Failed to get the model node set data!"));
        if (idMeshModel < 1) return ERROR(tr("Failed to get the mesh model ID!"));

        // 创建节点集合数据
        Radioss::FITKRadiossNodeGroup* dataNodeGroup = new Radioss::FITKRadiossNodeGroup();
        if (dataNodeGroup == nullptr) return ERROR(tr("Failed to create the node group data!"));

        // 复制节点集合数据
        dataNodeGroup->setDataObjectName(QString("NodeSet-%1").arg(dataNodeGroup->getDataObjectID()));
        dataNodeGroup->setModelSetType(Interface::FITKModelEnum::FMSNode);
        dataNodeGroup->setInternal(dataSet->isInternal());
        dataNodeGroup->setGenerated(dataSet->isGenerated());
        int idMesh = dataSet->getAbsModelID();
        QList<int> memberAbsolute = dataSet->getAbsoluteMember(), members = {};
        for (int member : memberAbsolute)
        {
            if (!m_mapNodeID[idMesh].contains(member)) return ERROR(tr("Failed to get the new node ID by the old node ID!"));
            else members.append(m_mapNodeID[idMesh][member]);
        }
        dataNodeGroup->setAbsoluteMember(members);
        dataNodeGroup->setModel(idMeshModel);
        dataNodeGroup->setColor(dataSet->getColor());

        // 集合ID映射关系
        int idNodeGroup = dataNodeGroup->getDataObjectID();
        int idSet = dataSet->getDataObjectID();
        if (m_mapSetID.contains(idSet)) return ERROR(tr("Duplicate set ID!"));
        else m_mapSetID.insert(idSet, idNodeGroup);
        
        // 追加节点集合数据
        managerComponent->appendDataObj(dataNodeGroup);

        return true;
    }

    bool OperatorFileFITKMeshImport::convertElementSet(Interface::FITKComponentManager* managerComponent, Interface::FITKModelSet* dataSet, int idMeshModel)
    {
        // 检查参数
        if (managerComponent == nullptr) return ERROR(tr("Failed to get the component manager!"));
        if (dataSet == nullptr) return ERROR(tr("Failed to get the model element set data!"));
        if (idMeshModel < 1) return ERROR(tr("Failed to get the mesh model ID!"));

        // 创建单元集合数据
        Radioss::FITKRadiossElementGroup* dataElementGroup = new Radioss::FITKRadiossElementGroup();
        if (dataElementGroup == nullptr) return ERROR(tr("Failed to create the element group data!"));

        // 复制单元集合数据
        dataElementGroup->setDataObjectName(QString("ElementSet-%1").arg(dataElementGroup->getDataObjectID()));
        dataElementGroup->setModelSetType(Interface::FITKModelEnum::FMSElem);
        dataElementGroup->setInternal(dataSet->isInternal());
        dataElementGroup->setGenerated(dataSet->isGenerated());
        int idMesh = dataSet->getAbsModelID();
        QList<int> memberAbsolute = dataSet->getAbsoluteMember(), members = {};
        for (int member : memberAbsolute)
        {
            if (!m_mapElemID[idMesh].contains(member)) return ERROR(tr("Failed to get the new element ID by the old element ID!"));
            else members.append(m_mapElemID[idMesh][member]);
        }
        dataElementGroup->setAbsoluteMember(members);
        dataElementGroup->setModel(idMeshModel);
        dataElementGroup->setColor(dataSet->getColor());

        // 集合ID映射关系
        int idElementGroup = dataElementGroup->getDataObjectID();
        int idSet = dataSet->getDataObjectID();
        if (m_mapSetID.contains(idSet)) return ERROR(tr("Duplicate set ID!"));
        else m_mapSetID.insert(idSet, idElementGroup);

        // 追加单元集合数据
        managerComponent->appendDataObj(dataElementGroup);

        return true;
    }

    bool OperatorFileFITKMeshImport::convertMixSet(Interface::FITKComponentManager* managerComponent, Interface::FITKModelSet* dataSet, int idMeshModel)
    {
        // 检查参数
        if (managerComponent == nullptr) return ERROR(tr("Failed to get the component manager!"));
        if (dataSet == nullptr) return ERROR(tr("Failed to get the model mix set data!"));
        if (idMeshModel < 1) return ERROR(tr("Failed to get the mesh model ID!"));

        // 遍历子集合数据
        int count = dataSet->getDataCount();
        for (int i = 0; i < count; i++)
        {
            // 获取子集合数据
            Interface::FITKModelSet* dataSubSet = dataSet->getDataByIndex(i);
            if (dataSubSet == nullptr) return ERROR(tr("Failed to get the sub set data!"));

            // 初始化转换状态
            bool ok = true;

            // 转换子集合数据
            Interface::FITKModelEnum::FITKModelSetType type = dataSubSet->getModelSetType();
            if (type == Interface::FITKModelEnum::FMSNode) ok = convertNodeSet(managerComponent, dataSubSet, idMeshModel);
            else if (type == Interface::FITKModelEnum::FMSElem) ok = convertElementSet(managerComponent, dataSubSet, idMeshModel);
            else if (type == Interface::FITKModelEnum::FMSMIX) ok = convertMixSet(managerComponent, dataSubSet, idMeshModel);
            else return ERROR(tr("Invalid model set type!"));

            // 如果转换失败，返回错误信息
            if (!ok) return ERROR(tr("Failed to convert the model mix set data!"));
        }

        return true;
    }

    bool OperatorFileFITKMeshImport::convertNodeSurface(Interface::FITKComponentManager* managerComponent, Interface::FITKMeshSurfaceNode* dataSurface, int idMeshModel)
    {
        // 检查参数
        if (managerComponent == nullptr) return ERROR(tr("Failed to get the component manager!"));
        if (dataSurface == nullptr) return ERROR(tr("Failed to get the node surface data!"));
        if (idMeshModel < 1) return ERROR(tr("Failed to get the mesh model ID!"));

        // 创建节点表面数据
        Interface::FITKMeshSurfaceNode* dataSurfaceNode = new Interface::FITKMeshSurfaceNode();
        if (dataSurfaceNode == nullptr) return ERROR(tr("Failed to create the node surface data!"));

        // 初始化集合值映射表
        QHash<int, double> mapSetValue = {};
        
        // 建立集合值映射关系
        int countSet = dataSurface->getSetCount();
        for (int i = 0; i < countSet; i++)
        {
            Interface::FITKModelSet* dataSet = dataSurface->getSetAt(i);
            if (dataSet == nullptr) return ERROR(tr("Failed to get the model set data!"));
            double value = dataSurface->getValueAt(i);
            if (!getSetValueMap(dataSet, value, mapSetValue)) return ERROR(tr("Failed to get the set value map!"));
        }

        // 复制节点表面数据
        for (int idSet : mapSetValue.keys()) dataSurfaceNode->addMeshSet(idMeshModel, idSet, mapSetValue[idSet]);
        dataSurfaceNode->setDataObjectName(QString("NodeSurface-%1").arg(dataSurfaceNode->getDataObjectID()));
        dataSurfaceNode->enable(dataSurface->isEnable());
        dataSurfaceNode->isInternal(dataSurface->isInternal());
        dataSurfaceNode->setModel(idMeshModel);
        dataSurfaceNode->setColor(dataSurface->getColor());

        // 追加节点表面数据
        managerComponent->appendDataObj(dataSurfaceNode);

        return true;
    }

    bool OperatorFileFITKMeshImport::convertElementSurface(Interface::FITKComponentManager* managerComponent, Interface::FITKMeshSurfaceElement* dataSurface, int idMeshModel)
    {
        // 检查参数
        if (managerComponent == nullptr) return ERROR(tr("Failed to get the component manager!"));
        if (dataSurface == nullptr) return ERROR(tr("Failed to get the element surface data!"));
        if (idMeshModel < 1) return ERROR(tr("Failed to get the mesh model ID!"));

        // 创建单元表面数据
        Radioss::FITKRadiossSurfaceSeg* dataSurfaceElement = new Radioss::FITKRadiossSurfaceSeg();
        if (dataSurfaceElement == nullptr) return ERROR(tr("Failed to create the element surface data!"));
        
        // 初始化单元ID面索引映射表
        QHash<int, int> mapElementIDFaceIndex = {};
        int countSet = dataSurface->getSetCount();
        for (int i = 0; i < countSet; i++)
        {
            Interface::FITKModelSet* dataSet = dataSurface->getSetAt(i);
            if (dataSet == nullptr) return ERROR(tr("Failed to get the model set data!"));
            int index = dataSurface->getSurfaceIndexAt(i);
            if (!getElementIDFaceIndexMap(dataSet, mapElementIDFaceIndex, index)) return false;
        }
        
        // 复制单元表面数据
        for (int elementid : mapElementIDFaceIndex.keys()) dataSurfaceElement->addElementSurface(elementid, mapElementIDFaceIndex[elementid]);
        dataSurfaceElement->setDataObjectName(QString("ElementSurface-%1").arg(dataSurfaceElement->getDataObjectID()));
        dataSurfaceElement->enable(dataSurface->isEnable());
        dataSurfaceElement->isInternal(dataSurface->isInternal());
        dataSurfaceElement->setModel(idMeshModel);
        dataSurfaceElement->setColor(dataSurface->getColor());

        // 追加单元表面数据
        managerComponent->appendDataObj(dataSurfaceElement);
        
        return true;
    }

    bool OperatorFileFITKMeshImport::ERROR(const QString& message)
    {
        AppFrame::FITKMessageError(message);
        return false;
    }
    
    bool OperatorFileFITKMeshImport::updateUI()
    {
        // 初始化树形菜单事件操作器列表
        QStringList list = { "TreeAssemblyEvent", "TreeGroupEvent" };

        // 更新树形菜单
        for (QString name : list)
        {
            EventOper::TreeEventOperator* tree = FITKOPERREPO->getOperatorT<EventOper::TreeEventOperator>(name);
            if (tree == nullptr) return ERROR(tr("Failed to get the tree event operator: %1!").arg(name));
            tree->updateTree();
        }

        // 更新前处理可视化窗口显示对象，根据当前界面选项获取需要渲染的对象。
        EventOper::GraphEventOperator* graph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graph == nullptr) return ERROR(tr("Failed to get the graph preprocess operator!"));
        graph->updateAll();

        return true;
    }
    
    bool OperatorFileFITKMeshImport::getSetValueMap(Interface::FITKModelSet* dataSet, double value, QHash<int, double>& mapSetValue)
    {
        // 检查参数
        if (dataSet == nullptr) return ERROR(tr("Failed to get the model set data!"));

        // 建立集合值映射关系
        int countSubSet = dataSet->getDataCount();
        if (countSubSet > 0)
        {
            // 如果存在子集合，则递归获取子集合的集合值映射关系
            for (int i = 0; i < countSubSet; i++)
            {
                // 获取子集合数据
                Interface::FITKModelSet* dataSubSet = dataSet->getDataByIndex(i);
                if (dataSubSet == nullptr) return ERROR(tr("Failed to get the sub set data!"));

                // 递归获取子集合的集合值映射关系
                if (!getSetValueMap(dataSubSet, value, mapSetValue)) return ERROR(tr("Failed to get the set value map!"));
            }
        }
        else
        {
            // 如果不存在子集合，则获取当前集合的集合值映射关系
            int idSet = dataSet->getDataObjectID();

            if (!m_mapSetID.contains(idSet)) return ERROR(tr("Failed to get the new set ID by the old set ID!"));
            else mapSetValue.insert(m_mapSetID[idSet], value);
        }

        return true;
    }
    
    bool OperatorFileFITKMeshImport::getElementIDFaceIndexMap(Interface::FITKModelSet* dataSet, QHash<int, int>& mapElementIDFaceIndex, int faceindex)
    {
        // 检查参数
        if (dataSet == nullptr) return ERROR(tr("Failed to get the model set data!"));

        // 获取面索引
        if (faceindex == FITKMeshIO::EnumSurfaceIndex::ESI_S1) faceindex = 0;
        else if (faceindex == FITKMeshIO::EnumSurfaceIndex::ESI_S2) faceindex = 1;
        else if (faceindex == FITKMeshIO::EnumSurfaceIndex::ESI_S3) faceindex = 2;
        else if (faceindex == FITKMeshIO::EnumSurfaceIndex::ESI_S4) faceindex = 3;
        else if (faceindex == FITKMeshIO::EnumSurfaceIndex::ESI_S5) faceindex = 4;
        else if (faceindex == FITKMeshIO::EnumSurfaceIndex::ESI_S6) faceindex = 5;

        // 获取子集合数据
        int countSubSet = dataSet->getDataCount();
        if (countSubSet > 0)
        {
            // 如果存在子集合，则递归获取子集合的单元ID面索引映射关系
            for (int i = 0; i < countSubSet; i++)
            {
                Interface::FITKModelSet* dataSubSet = dataSet->getDataByIndex(i);
                if (dataSubSet == nullptr) return ERROR(tr("Failed to get the sub set data!"));
                if (!getElementIDFaceIndexMap(dataSubSet, mapElementIDFaceIndex, faceindex)) return ERROR(tr("Failed to get the element ID to face index map!"));
            }
        }
        else
        {
            // 如果不存在子集合，则获取当前集合的单元ID面索引映射关系
            if (dataSet->getModelSetType() != Interface::FITKModelEnum::FMSElem) return ERROR(tr("The model set type is not element set!"));
            QList<int> absoluteMember = dataSet->getAbsoluteMember();
            for (int member : absoluteMember) mapElementIDFaceIndex.insert(member, faceindex);
        }

        return true;
    }
}


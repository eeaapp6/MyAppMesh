/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshGenerateProcessorGmshExec.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"

#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"
#include "FITK_Interface/FITKInterfaceModel/FITKGeoToMeshMapper.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"

#include "FITK_Component/FITKGmshMshIO/FITKGmshMshIOInterface.h"
#include "FITK_Component/FITKGmshMshIO/FITKGmshMshData.h"

#include <QDir>
#include <QSet>

namespace Gmsh
{
    FITKMeshGenerateProcessorGmshExec::FITKMeshGenerateProcessorGmshExec()
    {

    }

    FITKMeshGenerateProcessorGmshExec::~FITKMeshGenerateProcessorGmshExec()
    {

    }

    void FITKMeshGenerateProcessorGmshExec::start(QStringList info)
    {
        Q_UNUSED(info);
        //获取数据
        QString file = this->getValueT<QString>("File");
        QVariant expCmdId = this->getValue("ExpGeoCmdId");//设置与几何映射的CmdId
        QVariantList dim = this->getValueT<QVariantList>("FilterDim");//需要过滤的维度
        bool filterUnusedNode = this->getValueT<bool>("FilterUnusedNode");//过滤没有使用的节点
        bool filterHTLElement = this->getValueT<bool>("FilterHighToLowCells");//过滤高维单元包含的低维单元，该设置与几何映射有冲突
        Interface::FITKUnstructuredMesh* mesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(_dataObject.value("Mesh"));
        Interface::FITKComponentManager* comp = dynamic_cast<Interface::FITKComponentManager*>(_dataObject.value("ComponentManager"));
        //获取读取接口
        AppFrame::FITKCmponents* componentMge = FITKAPP->getComponents();
        if (!componentMge) return;
        Gmsh::FITKGmshMshIOInterface* fitkAbaIO = dynamic_cast<Gmsh::FITKGmshMshIOInterface*>(componentMge->getComponentByName("GmshMshIO"));
        if (!fitkAbaIO) return;
        //设置参数
        QList<int> filterDim;
        for (QVariant d : dim)
        {
            bool ok = false;
            int v = d.toInt(&ok);
            if (!ok) continue;
            filterDim.append(v);
        }
        fitkAbaIO->setFileName(file);
        fitkAbaIO->setDataObject("ComponentManager", comp);
        fitkAbaIO->setDataObject("MeshData", mesh);
        //设置读取信息
        Gmsh::ReaderGmshMshConfig readConfig;
        readConfig._filterDim = filterDim;//设置读取时过滤掉的单元维度。默认为空链表，所有维度都不过滤
        readConfig._isFilterUnusedNode = filterUnusedNode;//设置是否过滤掉未使用的节点。默认为false
        readConfig._isFilterHighToLowCells = filterHTLElement;//设置是否过滤高维单元包含的低维单元。默认为false
        readConfig._isExtractData = expCmdId.isNull();//设置读取的Msh文件数据是否提取到mesh和comp数据结构中。默认为true
        fitkAbaIO->setReaderConfig(readConfig);
        //不在线程中运行并执行
        fitkAbaIO->runInThread(false);
        fitkAbaIO->exec(1);
        //几何映射信息
        if (!expCmdId.isNull())
            startGridMapGeoInfo(fitkAbaIO->getGmshMshData(), expCmdId.toInt());
        fitkAbaIO->clearGmshMshData();
    }
    /**
     * @brief    几何与网格映射
     *           1、获取网格数据（用于存储网格数据与几何映射数据）
     *           2、获取几何数据（用于确定几何虚拓扑数据）
     *           3、插入网格节点数据（这一步无关联，放到哪里都行）
     *           4、建立网格与几何映射关系
     */
    void FITKMeshGenerateProcessorGmshExec::startGridMapGeoInfo(FITKGmshMshData* data, int geoId)
    {
        if (!data || data->_array_dim.isEmpty()) return;
        int nodeIDMax = this->getValueT<int>("MeshNodeMaxID");
        int elementIDMax = this->getValueT<int>("MeshElementMaxID");
        //获取网格数据
        Interface::FITKUnstructuredMesh* mesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(_dataObject.value("Mesh"));
        if (!mesh) return;
        Interface::FITKGeoToMeshMapper* geoToMeshMap = mesh->getGeoToMeshMapper();
        if (!geoToMeshMap) return;
        nodeIDMax += mesh->getNodeMaxID(), elementIDMax += mesh->getElementMaxID();
        int meshEleCount = mesh->getElementCount();
        //获取几何数据和虚拓扑管理器
        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoData) return;
        Interface::FITKAbsGeoCommand* cmd = geoData->getDataByID(geoId);
        if (!cmd) return;
        Interface::FITKVirtualTopoManager* virTopoManager = cmd->getVirtualTopoManager();
        if (!virTopoManager) return;
        Interface::FITKShapeVirtualTopoManager* solidTopoManager = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        Interface::FITKShapeVirtualTopoManager* faceTopoManager = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        Interface::FITKShapeVirtualTopoManager* edgeTopoManager = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        Interface::FITKShapeVirtualTopoManager* pointTopoManager = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSPoint);
        if (!solidTopoManager || !faceTopoManager || !edgeTopoManager || !pointTopoManager) return;
        //创建拓扑查询工具
        Interface::FITKVirtualTopoMapper topoMapper;
        //插入网格节点
        int nCount = data->_node_coordX.size();
        for (int nIndex = 0; nIndex < nCount; ++nIndex) {
            int nodeID = nodeIDMax + nIndex + 1;//节点ID
            mesh->addNode(nodeID, data->_node_coordX[nIndex], data->_node_coordY[nIndex], data->_node_coordZ[nIndex]);
        }
        /**
         * @brief    建立网格与几何映射关系
         *           1、创建节点与网格单元之间的映射
         *           2、获取维度与各维度网格数据映射
         *           3、创建并存入三维网格单元数据，并插入节点与网格单元之间的映射，存入三维网格单元与几何体之间的映射
         *           4、判断二维网格单元数据是否是某个体单元的边界面，是：存入二维网格单元与几何体之间的映射；否：创建并存入二维网格单元数据，并插入节点与网格单元之间的映射，存入二维网格单元与几何面之间的映射
         *           5、判断一维网格单元数据是否是某个体、面单元的边界边。
         *           6、记录网格节点与几何节点的映射
         */
        QVector<QSet<int>> node_element_mapping(nCount);//节点与单元Index的映射
        //维度与各维度网格数据映射
        QList<int> setIndex_dim[4]{};//0-0维 1-1维 2-2维 3-3维
        const int dimCount = data->_array_dim.size();
        for (int i = 0; i < dimCount; ++i) {
            int dim = data->_array_dim[i];
            if (dim < 0 || dim > 3) continue;
            setIndex_dim[dim].append(i);
        }
        //三维网格单元映射
        for (int i = 0; i < setIndex_dim[3].size(); ++i) {
            int solidID = solidTopoManager->getIDByIndex(i);
            int setIndex = setIndex_dim[3].at(i);
            const QList<int>& elementList = data->_array[setIndex];
            for (int elementIndex : elementList) {
                //获取单元的节点ID
                const QList<int>& nodeList = data->_element[elementIndex];
                QList<int> nodeIDs;
                nodeIDs.reserve(nodeList.size());
                for (int nIndex : nodeList) {
                    int nodeID = nodeIDMax + nIndex + 1;
                    nodeIDs.append(nodeID);
                }
                //创建并添加单元
                int elemID = elementIDMax + elementIndex + 1;//单元ID
                Interface::FITKModelEnum::FITKEleType eType = Interface::FITKModelEnum::FITKEleType(data->_element_type[elementIndex]);
                Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(eType);
                if (!element)
                    continue;
                element->setEleID(elemID);
                element->setNodeID(nodeIDs);
                element->setTag(solidID);
                mesh->appendElement(element);
                //插入节点与网格单元之间的映射
                for (int nIndex : nodeList)
                    node_element_mapping[nIndex].insert(meshEleCount);
                ++meshEleCount;
                //插入映射
                geoToMeshMap->insertMapOfGeoSolid(solidID, elemID);
            }
        }
        //二维网格单元映射
        for (int i = 0; i < setIndex_dim[2].size(); ++i) {
            int faceID = faceTopoManager->getIDByIndex(i);
            int setIndex = setIndex_dim[2].at(i);
            const QList<int>& elementList = data->_array[setIndex];
            for (int elementIndex : elementList) {
                //获取单元的节点ID
                const QList<int>& nodeList = data->_element[elementIndex];
                QList<int> nodeIDs;
                nodeIDs.reserve(nodeList.size());
                for (int index = 0; index < nodeList.size(); ++index) {
                    int nIndex = nodeList[index];
                    int nodeID = nodeIDMax + nIndex + 1;
                    nodeIDs.append(nodeID);
                }
                //判断是否是需要存储的独立单元
                QSet<int> intersection;
                if (!nodeList.isEmpty())
                {
                    intersection = node_element_mapping[nodeList.first()];
                    for (int k = 1; k < nodeList.size(); ++k) {
                        intersection.intersect(node_element_mapping[nodeList[k]]);
                        if (intersection.isEmpty()) break;
                    }
                }
                int eleID = 0;
                int faceIndex = 0;
                if (intersection.isEmpty())
                {
                    //创建并添加单元
                    int elemID = elementIDMax + elementIndex + 1;//单元ID
                    Interface::FITKModelEnum::FITKEleType eType = Interface::FITKModelEnum::FITKEleType(data->_element_type[elementIndex]);
                    Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(eType);
                    if (!element) continue;
                    element->setEleID(elemID);
                    element->setNodeID(nodeIDs);
                    element->setTag(faceID);
                    mesh->appendElement(element);
                    //插入节点与网格单元之间的映射
                    for (int nIndex : nodeList)
                        node_element_mapping[nIndex].insert(meshEleCount);
                    ++meshEleCount;

                    eleID = elemID;
                }
                else
                {
                    int eIndex = *intersection.constBegin();
                    Interface::FITKAbstractElement* element = mesh->getElementAt(eIndex);
                    if (!element) continue;
                    eleID = element->getEleID();
                    faceIndex = element->getFaceIndex(nodeIDs);
                }
                //插入映射
                geoToMeshMap->insertMeshEleOfGeoFace(faceID, eleID, faceIndex);
            }
        }
        //一维网格单元映射
        for (int i = 0; i < setIndex_dim[1].size(); ++i) {
            Interface::FITKAbsVirtualTopo* lineTopo = edgeTopoManager->getDataByIndex(i);
            if (!lineTopo) continue;
            bool isWireEdge = false;
            topoMapper.mapTopo(lineTopo, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
            if (topoMapper.length() <= 0)
                isWireEdge = true;
            int edgeID = lineTopo->getDataObjectID();
            int setIndex = setIndex_dim[1].at(i);
            const QList<int>& elementList = data->_array[setIndex];
            for (int elementIndex : elementList) {
                //获取单元的节点ID
                const QList<int>& nodeList = data->_element[elementIndex];
                QList<int> nodeIDs;
                nodeIDs.reserve(nodeList.size());
                for (int index = 0; index < nodeList.size(); ++index) {
                    int nIndex = nodeList[index];
                    int nodeID = nodeIDMax + nIndex + 1;
                    nodeIDs.append(nodeID);
                }
                //判断是否是需要存储的独立单元
                QSet<int> intersection;
                if (!nodeList.isEmpty())
                {
                    intersection = node_element_mapping[nodeList.first()];
                    for (int k = 1; k < nodeList.size(); ++k) {
                        intersection.intersect(node_element_mapping[nodeList[k]]);
                        if (intersection.isEmpty()) break;
                    }
                }
                int eleID = 0;
                int edgeIndex = 0;
                if (intersection.isEmpty())
                {
                    //创建并添加单元
                    int elemID = elementIDMax + elementIndex + 1;//单元ID
                    Interface::FITKModelEnum::FITKEleType eType = Interface::FITKModelEnum::FITKEleType(data->_element_type[elementIndex]);
                    Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(eType);
                    if (!element) continue;
                    element->setEleID(elemID);
                    element->setNodeID(nodeIDs);
                    element->setTag(edgeID);
                    mesh->appendElement(element);
                    //插入节点与网格单元之间的映射
                    for (int nIndex : nodeList)
                        node_element_mapping[nIndex].insert(meshEleCount);
                    ++meshEleCount;

                    eleID = elemID;
                }
                else
                {
                    int eIndex = *intersection.constBegin();
                    Interface::FITKAbstractElement* element = mesh->getElementAt(eIndex);
                    if (!element) continue;
                    eleID = element->getEleID();
                    edgeIndex = element->getEdgeIndex(nodeIDs);
                }
                //插入映射
                if (isWireEdge)
                    geoToMeshMap->insertMeshEleOfGeoWireEdge(edgeID, eleID);
                else
                    geoToMeshMap->insertMeshEleOfGeoEdge(edgeID, eleID, edgeIndex);
            }
        }
        //0维网格单元映射
        for (int i = 0; i < setIndex_dim[0].size(); ++i) {
            int pointID = pointTopoManager->getIDByIndex(i);
            int setIndex = setIndex_dim[0].at(i);
            const QList<int>& elementList = data->_array[setIndex];
            for (int elementIndex : elementList) {
                //获取单元的节点ID
                const QList<int>& nodeList = data->_element[elementIndex];
                QList<int> nodeIDs;
                nodeIDs.reserve(nodeList.size());
                for (int nIndex : nodeList) {
                    int nodeID = nodeIDMax + nIndex + 1;
                    nodeIDs.append(nodeID);
                }
                if (nodeIDs.isEmpty()) continue;
                //插入映射
                geoToMeshMap->insertMeshVertexOfGeoNode(pointID, nodeIDs[0]);
            }
        }

    }

}

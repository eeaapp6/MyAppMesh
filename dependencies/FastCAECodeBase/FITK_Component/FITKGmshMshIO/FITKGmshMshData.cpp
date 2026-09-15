/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshMshData.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

namespace Gmsh
{
    void FITKGmshMshData::init(Interface::FITKUnstructuredMesh * meshData, Interface::FITKComponentManager * componentMgr)
    {
        //初始化接口 将数据结构中meshData转化到Gmsh mshData文件数据结构中
        Q_UNUSED(meshData);
        Q_UNUSED(componentMgr);
    }

    void FITKGmshMshData::update(Interface::FITKUnstructuredMesh * meshData, Interface::FITKComponentManager * componentMgr)
    {
        //更新接口 将Gmsh mshData文本数据更新到数据结构meshData里
        if (!meshData) return;
        if (_node_coordX.size() != _node_coordY.size() || _node_coordX.size() != _node_coordZ.size()) return;
        if (_element_type.size() != _element.size()) return;
        int nodeIDMax = meshData->getNodeMaxID();
        int elementIDMax = meshData->getElementMaxID();
        //添加节数据
        for (int nIndex = 0; nIndex < _node_coordX.size(); ++nIndex)
        {
            int nodeID = nodeIDMax + nIndex + 1;
            //添加节点
            meshData->addNode(nodeID, _node_coordX[nIndex], _node_coordY[nIndex], _node_coordZ[nIndex]);
        }
        //添加单元数据
        for (int eIndex = 0; eIndex < _element.size(); ++eIndex)
        {
            int elemID = elementIDMax + eIndex + 1;
            Interface::FITKModelEnum::FITKEleType eType = Interface::FITKModelEnum::FITKEleType(_element_type[eIndex]);
            QList<int>& nodeList = _element[eIndex];
            QList<int> nodeIDs;
            for (int i = 0; i < nodeList.size(); ++i)
            {
                int nodeID = nodeIDMax + nodeList[i] + 1;
                nodeIDs.append(nodeID);
            }
            //创建并添加单元
            Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(eType);
            if (!element) continue;
            element->setEleID(elemID);
            element->setNodeID(nodeIDs);
            meshData->appendElement(element);
        }
        //添加集合
        if (componentMgr == nullptr) return;
        for (int setIndex = 0; setIndex < _array.size(); ++setIndex)
        {
            QList<int>& elementList = _array[setIndex];
            QList<int> elemIDs;
            for (int i = 0; i < elementList.size(); ++i)
            {
                int elemID = elementIDMax + elementList[i] + 1;
                elemIDs.append(elemID);
            }
            if (elemIDs.isEmpty()) continue;
            //创建并添加集合
            QString name = componentMgr->checkName("GmshSet-1");
            Interface::FITKModelSet* setData = new Interface::FITKModelSet(Interface::FITKModelEnum::FITKModelSetType::FMSElem);
            setData->setModel(meshData->getDataObjectID());
            setData->setDataObjectName(name);
            setData->setAbsoluteMember(elemIDs);
            componentMgr->appendDataObj(setData);
        }
    }

}

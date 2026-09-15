/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossMeshModel.h"
#include "FITKRadiossNodesParts.h" 
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"


namespace Radioss
{   
    FITKRadiossMeshModel::FITKRadiossMeshModel()
        : Interface::FITKAbstractModel()
    {
        _nodes = new FITKRadiossNodes(); // Initialize nodes
        _partsManager = new FITKRadiossPartManager(); // Initialize parts manager
        _componentManager = new Interface::FITKComponentManager(); // Initialize component manager
        _subSetManager = new FITKRadiossSubSetManager(); // Initialize sub-set manager
        _meshTopo = new FITKRadiossMeshTopo(this); // Initialize mesh topology
    }
    FITKRadiossMeshModel::~FITKRadiossMeshModel()
    {
        if(_nodes) delete _nodes;
        if(_partsManager) delete _partsManager;
        if(_componentManager) delete _componentManager;
        if(_subSetManager) delete _subSetManager;
        if(_meshTopo) delete _meshTopo;
    }

    Interface::FITKModelEnum::AbsModelType FITKRadiossMeshModel::getAbsModelType()
    {
        return Interface::FITKModelEnum::AMTMesh;
    }

    void FITKRadiossMeshModel::getNodeBndBox(double *bndBox) const
    {
        if(bndBox == nullptr || _nodes == nullptr) return;
        _nodes->getNodeBndBox(bndBox);
    }


    FITKRadiossPartManager *Radioss::FITKRadiossMeshModel::getPartsManager() const
    {
        return _partsManager;
    }

    Interface::FITKComponentManager *FITKRadiossMeshModel::getComponentManager() const
    {
        return _componentManager;
    }

    FITKRadiossSubSetManager *FITKRadiossMeshModel::getSubSetManager() const
    {
        return _subSetManager;
    }

    FITKRadiossMeshTopo *FITKRadiossMeshModel::getMeshTopo() const
    {
        return _meshTopo;
    }

    void FITKRadiossMeshModel::clearData()
    {
        if (_nodes) _nodes->removeAllNode();
        if (_partsManager) _partsManager->clear();
        if (_subSetManager) _subSetManager->clear();
        if (_componentManager) _componentManager->clear();
    }

    FITKRadiossNodes *Radioss::FITKRadiossMeshModel::getNodes() const
    {
        return _nodes;
    }





    NodeElements::NodeElements(Core::FITKNode* node)
        :_node(node)
    {

    }

    void NodeElements::appendElement(Interface::FITKAbstractElement* element)
    {
        if (element == nullptr) return;
        if (_elements.contains(element)) return;
        _elements.append(element);
        connect(element, &Interface::FITKAbstractElement::elementDestroiedSignal, this, &NodeElements::onElementDestroiedSlot);
    }
    Core::FITKNode* NodeElements::getNode() const
    {
        return _node;
    }
    QList<Interface::FITKAbstractElement*> NodeElements::getElements() const
    {
        return _elements;
    }
    QList<int> NodeElements::getElementIDs() const
    {
        QList<int> ids;
        for (Interface::FITKAbstractElement* ele : _elements)
        {
            if (ele)
                ids.append(ele->getEleID());
        }
        return ids;
    }
    void NodeElements::onElementDestroiedSlot(Interface::FITKAbstractElement* ele)
    {
        if (ele == nullptr) return;
        _elements.removeAll(ele);
    }
  
    


    FITKRadiossMeshTopo::FITKRadiossMeshTopo(FITKRadiossMeshModel* mesh)
        : _radiossMesh(mesh)
    {
    }

    FITKRadiossMeshTopo::~FITKRadiossMeshTopo()
    {
        qDeleteAll(_nodeElementHash);
        _nodeElementHash.clear();
    }   

    bool FITKRadiossMeshTopo::updateAllMeshTopo()
    {
        if (_radiossMesh == nullptr) return false;
        qDeleteAll(_nodeElementHash);
        _nodeElementHash.clear();
        //切断节点销毁信号与槽的连接
        FITKRadiossNodes* nodes = _radiossMesh->getNodes();
        if (nodes == nullptr) return false;
        int nodeCount = nodes->getNodeCount();
        for (int i = 0; i < nodeCount; ++i)
        {
            Core::FITKNode* node = nodes->getNodeAt(i);
            if (node == nullptr) continue;
            disconnect(node, &Core::FITKNode::nodeDestroiedSignal, this, &FITKRadiossMeshTopo::onNodeDestroiedSlot);
        }
        //重新建立节点-单元映射关系
        FITKradioss:FITKRadiossPartManager* partsManager = _radiossMesh->getPartsManager();
        if (partsManager == nullptr) return false;
        int partCount = partsManager->getDataCount();
        for (int i = 0; i < partCount; ++i)
        {
            FITKRadiossPart* part = partsManager->getDataByIndex(i);
            if (part == nullptr) continue;
            this->addPartTopo(part);
        }
    }

    void FITKRadiossMeshTopo::addPartTopo(Interface::FITKElementList *eleList)
    {
        if (eleList == nullptr || _radiossMesh == nullptr) return;
        FITKRadiossNodes* nodes = _radiossMesh->getNodes();
        int eleCount = eleList->getElementCount();
        for (int j = 0; j < eleCount; ++j)
        {
            Interface::FITKAbstractElement* ele = eleList->getElementAt(j);
            if (ele == nullptr) continue;
            this->addElementTopo(nodes, ele);
        }
    }

    bool FITKRadiossMeshTopo::addElementTopo(Interface::FITKNodeList* nodeList, Interface::FITKAbstractElement* element)
    {
        if (element == nullptr || _radiossMesh == nullptr || nodeList == nullptr) return false;
        const int nNode = element->getNodeCount();
        for (int i = 0; i < nNode; i++)
        {
            const int nodeID = element->getNodeID(i);
            if (nodeID < 0) continue;
            Core::FITKNode* node = nodeList->getNodeByID(nodeID);
            if (node == nullptr) continue;
            NodeElements* ne = nullptr;
            if (!_nodeElementHash.contains(nodeID))
            {
                ne = new NodeElements(node);
                _nodeElementHash.insert(nodeID, ne);
                ne->appendElement(element);
                connect(node, &Core::FITKNode::nodeDestroiedSignal, this, &FITKRadiossMeshTopo::onNodeDestroiedSlot);
            }
            else
            {
                ne = _nodeElementHash.value(nodeID);
                if (ne)
                   ne->appendElement(element);
            }
        }
        return true;
    }

    QList<Interface::FITKAbstractElement *> FITKRadiossMeshTopo::getNodeElements(const int nodeID) const
    {
        NodeElements* ne = _nodeElementHash.value(nodeID);
        if (ne)
            return ne->getElements();
        return QList<Interface::FITKAbstractElement *>();
    }

    void FITKRadiossMeshTopo::onNodeDestroiedSlot(Core::FITKNode* node)
    {
        if (node == nullptr) return;
        int id = node->getNodeID();
        NodeElements* ne = _nodeElementHash.value(id);
        if (ne) delete ne;
        _nodeElementHash.remove(id);
    }

} // namespace Radioss
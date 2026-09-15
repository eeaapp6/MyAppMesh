/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKUnstructuredMeshTopo.h"
#include "FITKUnstructuredMesh.h"
#include "FITKAbstractElement.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"

namespace Interface
{
    NodeElements::NodeElements(Core::FITKNode* node)
        :_node(node)
    {

    }

    void NodeElements::appendElement(FITKAbstractElement* element)
    {
        if (element == nullptr) return;
        if (_elements.contains(element)) return;
        _elements.append(element);
        connect(element, &FITKAbstractElement::elementDestroiedSignal, this, &NodeElements::onElementDestroiedSlot);    
    }
    Core::FITKNode* NodeElements::getNode() const
    {
        return _node;
    }
    QList<FITKAbstractElement*> NodeElements::getElements() const
    {
        return _elements;
    }
    QList<int> NodeElements::getElementIDs() const
    {
        QList<int> ids;
        for (FITKAbstractElement* ele : _elements)
        {
            if (ele)
                ids.append(ele->getEleID());
        }
        return ids;
    }
    void NodeElements::onElementDestroiedSlot(FITKAbstractElement* ele)
    {
        if (ele == nullptr) return;
        _elements.removeAll(ele);
    }
  






    FITKUnstructuredMeshTopo::FITKUnstructuredMeshTopo(FITKUnstructuredMesh *mesh)
        :_mesh(mesh)
    {
    }

    FITKUnstructuredMeshTopo::~FITKUnstructuredMeshTopo()
    {
        QList<NodeElements*> elelist = _nodeElementHash.values();
        for (NodeElements* ne : elelist)
        {
            if (ne) delete ne;
        }
        _nodeElementHash.clear();
    }
    
    bool FITKUnstructuredMeshTopo::buildUnstructuredMeshTopo()
    {
        if (_mesh == nullptr) return false;

        const int nEle = _mesh->getElementCount();
        for (int i = 0; i < nEle; i++)
        {
            Interface::FITKAbstractElement* ele = _mesh->getElementAt(i);
            if (ele == nullptr) continue;   
            this->addElementTopo(ele);
        }
        
        return true;
    }

    bool FITKUnstructuredMeshTopo::addElementTopo(FITKAbstractElement * element)
    {
        if (element == nullptr) return false;
        const int nNode = element->getNodeCount();
        for (int i = 0; i < nNode; i++)
        {
            const int nodeID = element->getNodeID(i);
            if (nodeID < 0) continue;
            Core::FITKNode* node = _mesh->getNodeByID(nodeID);
            if (node == nullptr) continue;
            NodeElements* ne = nullptr;
            if (!_nodeElementHash.contains(nodeID))
            {
                ne = new NodeElements(node);
                _nodeElementHash.insert(nodeID, ne);
                connect(node, &Core::FITKNode::nodeDestroiedSignal, this, &FITKUnstructuredMeshTopo::onNodeDestroiedSlot);
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

    QList<FITKAbstractElement *> FITKUnstructuredMeshTopo::getNodeElements(const int nodeID) const
    {
        NodeElements* ne = _nodeElementHash.value(nodeID);
        if (ne)
            return ne->getElements();
        return QList<FITKAbstractElement *>();
    }

    void FITKUnstructuredMeshTopo::onNodeDestroiedSlot(Core::FITKNode* node)
    {
        if (node == nullptr) return;
        int id = node->getNodeID();
        if (_nodeElementHash.contains(id))
        {
            NodeElements* ne = _nodeElementHash.value(id);
            if (ne) delete ne;
            _nodeElementHash.remove(id);
        }
    }
}
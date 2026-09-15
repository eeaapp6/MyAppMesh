/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossNodesParts.h"
#include "FITK_Kernel/FITKCore/FITKColorMap.hpp"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementVertex.h"
#include <QSet>

namespace Radioss
{

    QList<int> FITKRadiossSubSet::getPartIDs()
    {
        if(!isCombination())
            return m_partIDs;

        QList<int> partIDs{};
        for (int i = 0; i < getDataCount(); ++i)
        {
            FITKRadiossSubSet* subset = this->getDataByIndex(i);
            if (!subset)continue;
            partIDs += subset->getPartIDs();
        }
        QSet<int> uniqueIDs = QSet<int>::fromList(partIDs);

        return uniqueIDs.toList();
    }
    
    bool FITKRadiossSubSet::addPartID(int partID)
    {
        if (m_partIDs.contains(partID))
        {
            return false;
        }
        m_partIDs.append(partID);
        return true;
    }
    
    bool FITKRadiossSubSet::removePartID(int partID)
    {
        if (isCombination()) 
            return false;
        return m_partIDs.removeOne(partID);
    }
    
    bool FITKRadiossSubSet::containsPartID(int partID)
    {
        if(!isCombination())
            return m_partIDs.contains(partID);

        QList<int> partIDs = this->getPartIDs();
        return partIDs.contains(partID);
    }
    
    void FITKRadiossSubSet::clearPartIDs()
    {
        m_partIDs.clear();
        if (!isCombination())return;
      
        for (int i = 0; i < getDataCount(); ++i) 
        {
            FITKRadiossSubSet* subset = this->getDataByIndex(i);
            if (!subset)continue;
            this->removeDataObjWithoutRelease(subset);
        }

    }
    
    int FITKRadiossSubSet::getPartIDCount()
    {
        if (!isCombination())
            return m_partIDs.size();

        QList<int> partIDs = this->getPartIDs();
        return partIDs.size();
    }

    bool FITKRadiossSubSet::isCombination()
    {
        return this->getDataCount() > 0;
    }
    
    int FITKRadiossPart::MAXELEID = 0;
    
    FITKRadiossPart::FITKRadiossPart()
    {
        static int PART_ID = 0;
        m_color = Core::GetcAColorFromColorMap(PART_ID++);
    }
    
    Interface::FITKModelEnum::AbsModelType FITKRadiossPart::getAbsModelType()
    {
        return Interface::FITKModelEnum::AMTunstructuredMesh;
    }
    
    // FITKRadiossPart class implementations
    void FITKRadiossPart::setPropertyID(int propertyID)
    {
        m_propertyID = propertyID;
    }
    
    int FITKRadiossPart::getPropertyID() const
    {
        return m_propertyID;
    }
    
    void FITKRadiossPart::setMaterialID(int materialID)
    {
        m_materialID = materialID;
    }
    
    int FITKRadiossPart::getMaterialID() const
    {
        return m_materialID;
    }
    
    void FITKRadiossPart::setSubsetID(int subsetID)
    {
        m_subsetID = subsetID;
    }
    
    int FITKRadiossPart::getSubsetID() const
    {
        return m_subsetID;
    }
    
    void FITKRadiossPart::setThickness(double thickness)
    {
        m_thickness = thickness;
    }
    
    double FITKRadiossPart::getThickness() const
    {
        return m_thickness;
    }
    
    void FITKRadiossPart::setColor(const QColor &color)
    {
        m_color = color;
    
    }
    
    QColor FITKRadiossPart::getColor() const
    {
        return m_color;
    }
    
    int FITKRadiossPart::addElement(Interface::FITKAbstractElement *element)
    {
        if(element == nullptr) return -1;
        // Assign a unique element ID
        int eleID = ++MAXELEID;
        element->setEleID(eleID);
        // Add the element to the part
        this->appendElement(element);
        return eleID;
    }

    void FITKRadiossPart::addElement(const int id, Interface::FITKAbstractElement * element)
    {
        if (id < 0 || element == nullptr) return;
        element->setEleID(id);
        
        this->appendElement(element);

        MAXELEID = id > MAXELEID ? id : MAXELEID;
    }
    
    QList<int> FITKRadiossPart::getPartElementIDs() const
    {
        QList<int> eleIDs;
        for (Interface::FITKAbstractElement* element : _elementList)
        {
            if (element == nullptr) continue;    
            eleIDs.append(element->getEleID());
        }
        return eleIDs;
    }

    void FITKRadiossPart::transformationSPHModel()
    {
        //获取节点ID
        QList<int> nodeIDs = this->getAllNodeIDs();
        //移除所有单元
        this->removeAllElement();
        //创建SPH粒子单元
        for (int nID : nodeIDs)
        {
            Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::FITKEleType::Vertex1);
            if (!element) continue;
            element->setNodeID({ nID });
            this->addElement(element);
        }
    }
    
    //QList<int> FITKRadiossPart::getPartNodeIDs() const
    //{
    //    QList<int> nodeIDs;
    //    //获取全部节点ID
    //    for (Interface::FITKAbstractElement* element : _elementList)
    //    {
    //        if (element == nullptr) continue;    
    //        nodeIDs.append(element->getAllNodes()); 
    //    }
    //    //去重
    //    QSet<int> uniqueNodeIDs = QSet<int>::fromList(nodeIDs);
    //    //排序
    //    nodeIDs = uniqueNodeIDs.values();
    //    qSort(nodeIDs);
    //    return nodeIDs;
    //}
    
    QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> FITKRadiossPartManager::getElementByID(int eleID)
    {
        QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> result{nullptr, nullptr};
        for (int i = 0; i < this->getDataCount(); ++i)
        {
            FITKRadiossPart* part = getDataByIndex(i);
            if(part == nullptr) continue;
            Interface::FITKAbstractElement* element = part->getElementByID(eleID);
            if (element == nullptr )continue;
    
            result.first = part;
            result.second = element;
        }
        return result;
    }
    
} // namespace Radioss
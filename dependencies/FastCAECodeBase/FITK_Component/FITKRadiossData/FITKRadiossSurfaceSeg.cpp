/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossSurfaceSeg.h"
#include "FITKRadiossMeshModel.h"
#include "FITKRadiossNodesParts.h"
#include "FITKRadiossElementGroup.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include <QSet>

namespace Radioss
{
    FITKRadiossSurfaceSeg::FITKRadiossSurfaceSeg()
        : FITKAbstractRadiossSurface()
    {
       // this->isInternal(false);
    }

    FITKAbstractRadiossSurface::RadiossSurfaceType FITKRadiossSurfaceSeg::getRadiossSurfaceType()
    {
        return FITKRadiossSurfaceSeg::RadiossSurfaceType::RST_SEG;
    }

    void FITKRadiossSurfaceSeg::addSurface(QList<int> surfNodesIDs)
    {
        FITKRadiossMeshModel* meshModel = this->getRadiossMeshModel();
        if(meshModel == nullptr) return;
        FITKRadiossPartManager* partManager = meshModel->getPartsManager();
        if(partManager == nullptr) return;
        FITKRadiossMeshTopo* meshTopo = meshModel->getMeshTopo();
        if(meshTopo == nullptr) return;

        QList<Interface::FITKAbstractElement*> elementList = meshTopo->getNodeElements(surfNodesIDs.first());
        QSet<Interface::FITKAbstractElement*> intersection = elementList.toSet();
        for (int i = 1; i < surfNodesIDs.size(); i++)
        {
            const int nodeID = surfNodesIDs.at(i);
            QList<Interface::FITKAbstractElement*> nodeElementList = meshTopo->getNodeElements(nodeID);
            // 求交集,找到包含所有节点的单元
            QSet<Interface::FITKAbstractElement*> set2 = nodeElementList.toSet();
            intersection &= set2;
        }

        if (intersection.isEmpty()) return;

        // 取第一个元素作为代表
        Interface::FITKAbstractElement* elementPtr = intersection.values().first();
        if (elementPtr == nullptr) return;
        int faceIndex = elementPtr->getFaceIndexNorUnSen(surfNodesIDs, true);
        int eleID = elementPtr->getEleID(); 
        this->addElementSurface(eleID, faceIndex);
    }

    QList< QList<int> > FITKRadiossSurfaceSeg::getSurface(int element, const bool & lowerOrder)
    {
        QList< QList<int> > res;
        QList<int> faceIndexList = _elementSurface.values(element);
        if(faceIndexList.isEmpty()) return res;
         FITKRadiossMeshModel* meshModel = this->getRadiossMeshModel();
        if(meshModel == nullptr) return res;
        FITKRadiossPartManager* partManager = meshModel->getPartsManager();
        if(partManager == nullptr) return res;
        //找到对应的单元
        Interface::FITKAbstractElement* elementPtr = partManager->getElementByID(element).second;
        if(elementPtr == nullptr) return res; 

        for (int i = 0; i < faceIndexList.size(); i++)
        {
            int faceIndex = faceIndexList.at(i);
            QList<int> faceNodes = elementPtr->getFace(faceIndex, lowerOrder);
            res.append(faceNodes);
        }
        return res;
    }

    QList<int> FITKRadiossSurfaceSeg::getAllElement() const
    {
        return _elementSurface.uniqueKeys();
    }


    FITKRadiossSurfaceRadIOInfo * FITKRadiossSurfaceSeg::getRadIOInfo()
    {
        if (!_radIOInfo)
            _radIOInfo = new FITKRadiossSurfaceRadIOInfo();
        return _radIOInfo;
    }

    void FITKRadiossSurfaceSeg::transformRadIOInfo(FITKRadiossMeshModel * model)
    {
        if (_radIOInfo == nullptr || model == nullptr) return;
        if (_radIOInfo->_type == RST_None|| _radIOInfo->_radiossIDs.isEmpty()) {
            delete _radIOInfo;
            _radIOInfo = nullptr;
            return;
        }
        FITKRadiossPartManager* partMgr = model->getPartsManager();
        if (!partMgr)return;
        Interface::FITKComponentManager* setMgr = model->getComponentManager();
        if (!setMgr)return;

        switch(_radIOInfo->_type)
        {
        case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_PART:
        {
            for (auto id:_radIOInfo->_radiossIDs) {
                FITKRadiossPart* part = partMgr->getDataByID(id);
                if (!part)continue;
                int count = part->getElementCount();
                for (int i = 0; i < count; ++i)
                {
                    Interface::FITKAbstractElement* ele = part->getElementAt(i);
                    if (!ele || ele->getNodeCount() < 3)continue;
                    if (ele->getElementDim() != 2) continue;
                    this->addElementSurface(ele->getEleID(), 0);
                }
            }
            break;
        }
        case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_SURF:
        {
            for (auto id : _radIOInfo->_radiossIDs) {
                FITKRadiossSurfaceSeg* surfSet = dynamic_cast<FITKRadiossSurfaceSeg*>(setMgr->getDataByID(id));
                if (!surfSet)continue;
                for (auto eleId : surfSet->getAllElement())
                {
                    this->addElementSurface(eleId, 0);
                }
            }
            break;
        }
        case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_GRSHEL:
        case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_GRSH3N:
        {
            for (auto id : _radIOInfo->_radiossIDs) {
                FITKRadiossElementGroup* eleSet =dynamic_cast<FITKRadiossElementGroup*>( setMgr->getDataByID(id));
                if (!eleSet)continue;
                for (auto eleId: eleSet->getMember()) 
                {
                    this->addElementSurface(eleId, 0);
                }
            }
            break;
        }
        default:
            break;
        }
        this->setModel(model);

        //删除处理过的RadIO信息
        if (_radIOInfo)
        {
            delete _radIOInfo;
            _radIOInfo = nullptr;
        }
    }

} // namespace Radioss

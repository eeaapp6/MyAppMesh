/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossNodeGroup.h"
#include "FITKRadiossMeshModel.h"

#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRadiossSurface.h"
#include <QSet>

namespace Radioss
{
    FITKRadiossNodeGroupRadIOInfo::FITKRadiossNodeGroupRadIOInfo()
    {
    }

    FITKRadiossNodeGroupRadIOInfo::~FITKRadiossNodeGroupRadIOInfo()
    {
    }

    void FITKRadiossNodeGroupRadIOInfo::setRadiossKeyWord(const QString &keyWords)
    {
        _radiossKeyWord = keyWords;
    }

    void FITKRadiossNodeGroupRadIOInfo::setRadiossIDs(const QList<int> &ids)
    {
        _radiossIDs = ids;
    }

    FITKRadiossNodeGroup::FITKRadiossNodeGroup()
    {
        this->setModelSetType(Interface::FITKModelEnum::FITKModelSetType::FMSNode);
    }

    FITKRadiossNodeGroup::~FITKRadiossNodeGroup()
    {
        if(_radIOInfo) delete _radIOInfo;
    }

    FITKRadiossNodeGroupRadIOInfo* FITKRadiossNodeGroup::getRadIOInfo()
    {
        // Lazy initialization
        if(!_radIOInfo)
            _radIOInfo = new FITKRadiossNodeGroupRadIOInfo();
        return _radIOInfo;
    }
    void FITKRadiossNodeGroup::transformRadIOInfo(FITKRadiossMeshModel* model)
    {
        if(_radIOInfo == nullptr|| model == nullptr) return;

        FITKRadiossPartManager* partMgr = model->getPartsManager();
        if (!partMgr)return;
        Interface::FITKComponentManager* setMgr= model->getComponentManager();
        if (!setMgr)return;

        //根据关键字转换
        QString keyWords = _radIOInfo->_radiossKeyWord.toUpper();
        if (keyWords.isEmpty())return;
        QList<int> nodeList{};

        if(keyWords == "/GRNOD/NODE")
        {
            nodeList = _radIOInfo->_radiossIDs;
        }
        else if(keyWords == "/GRNOD/PART")
        {
            QSet<int> all;
            QList<int> partIDList = _radIOInfo->_radiossIDs;
            for (auto partID : partIDList) {
                FITKRadiossPart* part = partMgr->getDataByID(partID);
                if (!part)continue;
                all.unite(part->getAllNodeIDs().toSet());
            }
            nodeList = all.toList();
        }
        else if(keyWords == "/GRNOD/SURF")
        {
            QSet<int> all;
            QList<int> surfIDList = _radIOInfo->_radiossIDs;
            for (auto surfID : surfIDList) {
                FITKAbstractModelComponent* data = setMgr->getDataByID(surfID);
                FITKAbstractRadiossSurface* surf = dynamic_cast<FITKAbstractRadiossSurface*>(data);
                if (!surf)continue;
                all.unite(surf->getSurfaceNodeIDs().toSet());
            }
            nodeList = all.toList();
        }

        this->setModel(model);
        this->setAbsoluteMember(nodeList);

        //删除无用的RadIO信息
        if(_radIOInfo)
        {
            delete _radIOInfo;
            _radIOInfo = nullptr;
        }

    }
} // namespace Radioss

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossElementGroup.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

#include "FITKRadiossMeshModel.h"

#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include <QSet>

namespace Radioss
{
    FITKRadiossElementGroupRadIOInfo::FITKRadiossElementGroupRadIOInfo(FITKRadiossElementGroup * eleGroup)
    {
        _eleGroup = eleGroup;
    }
    void Radioss::FITKRadiossElementGroupRadIOInfo::setRadiossKeyWord(const QString & keyWords)
    {
        if (!_eleGroup)return;
        _radiossKeyWord = keyWords;
        //设置元素组类型
        QString groupType = _radiossKeyWord.mid(1, 6);
        bool ok = false;
        Core::FITKEnumTransfer<FITKRadiossElementGroup::ElementGroupType> et1;
        for (int i = FITKRadiossElementGroup::ElementGroupType::GRQUAD; i <= FITKRadiossElementGroup::ElementGroupType::GRTRIA; ++i)
        {
            QString str = et1.toString((FITKRadiossElementGroup::ElementGroupType)i, ok);
            if (groupType == str) {
                _eleGroup->setElementGroupType((FITKRadiossElementGroup::ElementGroupType)i);
                break;
            }
        }
        //设置元素来源方式类型
        if (_radiossKeyWord.mid(0, 7) == _radiossKeyWord.mid(7))
        {
            _elementWayType = ElementWayType::Equal;
            return;
        }
        _elementWayType = ElementWayType::SameProp;

        QString wayType = _radiossKeyWord.mid(8);
        if (wayType.isEmpty())return;
 
        Core::FITKEnumTransfer<ElementWayType> et2;
        for (int i = ElementWayType::BOX; i <= ElementWayType::SUBSET; ++i) 
        {
            QString str = et2.toString((ElementWayType)i, ok);
            if (wayType == str) {
                _elementWayType = (ElementWayType)i;
                return;
            }
        }
    }


    FITKRadiossElementGroup::FITKRadiossElementGroup()
        : Interface::FITKModelSet()
    {
        this->setModelSetType(Interface::FITKModelEnum::FITKModelSetType::FMSElem);
        this->setInternal(false);
        this->setGenerated(false);
    }

    FITKRadiossElementGroup::~FITKRadiossElementGroup()
    {
        if (_radIOInfo) delete _radIOInfo;
    }

    QString FITKRadiossElementGroup::getElementGroupKeyWords()
    {
        Core::FITKEnumTransfer<ElementGroupType> et;
        bool ok = false;
        QString str = et.toString(this->_elementGroupType, ok);
        return "/" + str;
    }

    FITKRadiossElementGroup::ElementGroupType FITKRadiossElementGroup::getElementGroupType() const
    {
        return _elementGroupType;
    }

    void Radioss::FITKRadiossElementGroup::setElementGroupType(const ElementGroupType &type)
    {
        _elementGroupType = type;
    }

    FITKRadiossElementGroupRadIOInfo * FITKRadiossElementGroup::getRadIOInfo()
    {
        if (!_radIOInfo)
            _radIOInfo = new FITKRadiossElementGroupRadIOInfo(this);
        return _radIOInfo;
    }

    void FITKRadiossElementGroup::transformRadIOInfo(FITKRadiossMeshModel * model)
    {
        if (_radIOInfo == nullptr || model == nullptr) return;

        FITKRadiossPartManager* partMgr = model->getPartsManager();
        if (!partMgr)return;
        Interface::FITKComponentManager* setMgr = model->getComponentManager();
        if (!setMgr)return;

        //根据关键字转换
        FITKRadiossElementGroupRadIOInfo::ElementWayType wayType = _radIOInfo->_elementWayType;
        QList<int> eleIDList{};

        if (wayType == FITKRadiossElementGroupRadIOInfo::ElementWayType::SameProp)
        {
            eleIDList= _radIOInfo->_radiossIDs;
        }
        else if (wayType == FITKRadiossElementGroupRadIOInfo::ElementWayType::PART)
        {
            QSet<int> all;
            QList<int> partIDList = _radIOInfo->_radiossIDs;
            for (auto partID : partIDList) {
                FITKRadiossPart* part = partMgr->getDataByID(partID);
                if (!part)continue;
                all.unite(part->getPartElementIDs().toSet());
            }
            eleIDList = all.toList();
        }
        else if (wayType == FITKRadiossElementGroupRadIOInfo::ElementWayType::Equal)
        {
            ;
        }
        //其余类型 todo

        this->setModel(model);
        this->setAbsoluteMember(eleIDList);

        //删除无用的RadIO信息
        if (_radIOInfo)
        {
            delete _radIOInfo;
            _radIOInfo = nullptr;
        }

    }

}


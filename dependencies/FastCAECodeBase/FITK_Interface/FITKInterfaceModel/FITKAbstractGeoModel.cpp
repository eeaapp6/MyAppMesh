/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractGeoModel.h"

namespace Interface
{
    FITKAbstractGeoModel::FITKAbstractGeoModel()
    {
        //创建对象
        _compManager = new FITKGeoComponentManager;
        //记录父对象id
        _compManager->setParentDataIDM(this->getDataObjectID());
    }

    FITKAbstractGeoModel::~FITKAbstractGeoModel()
    {
        if (_compManager) delete _compManager;
    }

    int FITKAbstractGeoModel::getShapeCount(const Interface::FITKModelEnum::FITKModelSetType type)
    {
        return -1;
    }

    void FITKAbstractGeoModel::getTriangleMeshVS(FITKAbstractDataObject* mesh, int id, void * additional /*= nullptr*/)
    {

    }

    bool FITKAbstractGeoModel::writeSTLFile(QString folderPath)
    {
        return false;
    }

    Interface::FITKGeoComponentManager* FITKAbstractGeoModel::getGeoComponentManager()
    {
        //组件管理器
        return _compManager;
    }

    bool FITKAbstractGeoModel::getBoundaryBox(double* min, double* max)
    {
        if (min == nullptr || max == nullptr) return false;
        //赋予错误值
        for (int i = 0; i<3; ++i)
        {
            min[i] = 9e66;
            max[i] = -9e66;
        }

        return false;
    }

    void FITKAbstractGeoModel::setAbsDataID(const int id)
    {
        FITKAbstractModel::setAbsDataID(id);
        if (_compManager) _compManager->setParentDataIDM(id);
    }

    FITKGeoComponent::FITKGeoComponent(const FITKModelEnum::FITKModelSetType type /*= FITKModelEnum::FMSSurface*/)
        :_type(type)
    {
    }

    void FITKGeoComponent::setMember(QList<int> & member)
    {
        _member = member;
    }

    void FITKGeoComponent::appendMember(const int mem)
    {
        //已经包含则不追加
        if (_member.contains(mem)) return;
        _member.append(mem);
    }

    void FITKGeoComponent::clear()
    {
        _member.clear();
    }

    void FITKGeoComponent::removeMember(const int mem)
    {
        //移除
        _member.removeOne(mem);
    }

    QList<int> FITKGeoComponent::getMember() const
    {
        return _member;
    }

    Interface::FITKModelEnum::FITKModelSetType FITKGeoComponent::getGeoType() const
    {
        return _type;
    }

    void FITKGeoComponentManager::appendDataObj(Core::FITKAbstractDataObject* item)
    {
        //错误判断
        FITKGeoComponent* comp = dynamic_cast<FITKGeoComponent*>(item);
        if (comp == nullptr) return;
        //设置父对象ID
        const int modelID = this->getMParentDataIDM();
        comp->setModel(modelID);
        //数据存储
        Core::FITKAbstractDataManager<FITKGeoComponent>::appendDataObj(item);
    }

    QList<FITKGeoComponent*> FITKGeoComponentManager::getGeoComponent(const FITKModelEnum::FITKModelSetType type /*= FITKModelEnum::FMSSurface*/)
    {
        const int n = this->getDataCount();
        QList<FITKGeoComponent*> comps;
        for (int i = 0; i < n; ++i)
        {
            //过滤空指针与模型类型
            FITKGeoComponent* comp = this->getDataByIndex(i);
            if(comp == nullptr) continue;
            if(type != comp->getGeoType()) continue;
            comps.append(comp);
        }
        return comps;
    }

    QList<int> FITKGeoComponentManager::getAllComponentMember(const FITKModelEnum::FITKModelSetType type /*= FITKModelEnum::FMSSurface*/)
    {
        QList<int> members{};
        //获取所有组件
        QList<FITKGeoComponent*> comps = this->getGeoComponent(type);
        for (FITKGeoComponent* comp : comps)
        {
            //获取成员
            QList<int> mem = comp->getMember();
            for (int m : mem)
            {
                //记录组件中的成员
                if(members.contains(m)) continue;
                members.append(m);
            }
        }
        return members;
    }

}
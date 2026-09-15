/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKComponentManager.h"
#include "FITKModelSet.h"
#include "FITKMeshSurface.h"
#include "FITKAssElement.h"

namespace Interface
{
    QList<FITKModelSet*> FITKComponentManager::getModelSet(FITKModelEnum::FITKModelSetType t)
    {
        QList<FITKModelSet*> sets;
        //获取数据个数
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKModelSet* set = dynamic_cast<FITKModelSet*>(this->getDataByIndex(i));
            //空指针不操作
            if (set == nullptr) continue;
            //类型一致则记录
            if (set->getModelSetType() == t)
                sets.append(set);
            //复合集合展开
            if (!set->isCombination()) continue;
            const int nSub = set->getDataCount();
            for (int j = 0; j < nSub; ++j)
            {
                FITKModelSet* subSet = set->getDataByIndex(j);
                if (subSet == nullptr) continue;
                //类型一致则记录
                if (subSet->getModelSetType() == t)
                    sets.append(subSet);
            }

        }
        return sets;
    }

    QList<FITKModelSet*> FITKComponentManager::getUnInternalModelSet(FITKModelEnum::FITKModelSetType t)
    {
        //获取类型符合要求的集合
        QList<FITKModelSet*> sets = this->getModelSet(t);
        QList<FITKModelSet*> unSets;
        for (auto s : sets)
        {
            if (s == nullptr) continue;
            //内部集合标记判断
            bool isIn = s->isInternal();
            if (isIn) continue;
            unSets.append(s);
        }
        return unSets;
    }

    bool FITKComponentManager::isSetUsedBySurface(FITKModelSet* set)
    {
        if (!set)
        {
            return false;
        }

        int setId = set->getDataObjectID();

        QList<FITKModelSurface*> surfs = getAllSurface();
        for (FITKModelSurface* surf : surfs)
        {
            int nSets = surf->getSetCount();
            for (int i = 0; i < nSets; i++)
            {
                int id = surf->getSetIDAt(i);
                if (id == setId)
                {
                    return true;
                }
            }
        }

        return false;
    }

    QList<FITKMeshSurface*> FITKComponentManager::getAllMeshSurface()
    {
        QList<FITKMeshSurface*> surfs;

        //获取数据个数
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKMeshSurface* surf = dynamic_cast<FITKMeshSurface*>(this->getDataByIndex(i));
            //空指针不操作
            if (surf == nullptr) continue;
            surfs.append(surf);
        }
        return surfs;
    }

    QList<FITKGeometrySurface*> FITKComponentManager::getAllGeometrySurface()
    {
        QList<FITKGeometrySurface*> surfs;

        //获取数据个数
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKGeometrySurface* surf = dynamic_cast<FITKGeometrySurface*>(this->getDataByIndex(i));
            //空指针不操作
            if (surf == nullptr) continue;
            surfs.append(surf);
        }
        return surfs;
    }

    QList<FITKMeshSurface*> FITKComponentManager::getMeshSurface(FITKMeshSurface::MeshSurfaceType t)
    {
        QList<FITKMeshSurface*> surfs;
        //获取数据个数
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKMeshSurface* surf = dynamic_cast<FITKMeshSurface*>(this->getDataByIndex(i));
            //空指针不操作
            if (surf == nullptr) continue;
            //类型一致则记录
            if (surf->getMeshSurfaceType() == t)
                surfs.append(surf);
        }
        return surfs;
    }

    QList<FITKModelSurface*> FITKComponentManager::getModelSurface(FITKModelSurface::ModelSurfaceType t) {
        QList<FITKModelSurface*> surfs;
        //获取数据个数
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKModelSurface* surf = dynamic_cast<FITKModelSurface*>(this->getDataByIndex(i));
            //空指针不操作
            if (surf == nullptr) continue;
            //类型一致则记录
            if (surf->getModelSurfaceType() == t)
                surfs.append(surf);
        }
        return surfs;
    }

    QList<FITKModelSurface*> FITKComponentManager::getModelSurface(QList<FITKModelSurface::ModelSurfaceType> types)
    {
        QList<FITKModelSurface*> surfs;
        //获取数据个数
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKModelSurface* surf = dynamic_cast<FITKModelSurface*>(this->getDataByIndex(i));
            //空指针不操作
            if (surf == nullptr) continue;
            //类型一致则记录
            if (types.contains(surf->getModelSurfaceType()))
                surfs.append(surf);
        }
        return surfs;
    }

    QList<FITKMeshSurface*> FITKComponentManager::getUnInternalMeshSurface(FITKMeshSurface::MeshSurfaceType t)
    {
        //获取类型符合要求的集合
        QList<FITKMeshSurface*> surfs = this->getMeshSurface(t);
        QList<FITKMeshSurface*> unSurfs;
        for (auto s : surfs)
        {
            if (s == nullptr) continue;
            //内部集合标记判断
            bool isIn = s->isInternal();
            if (isIn) continue;
            unSurfs.append(s);
        }
        return unSurfs;
    }

    bool FITKComponentManager::isModelSet(int index)
    {
        auto set = this->getAsModelSet(index);
        return set != nullptr;
    }

    FITKModelSet* FITKComponentManager::getAsModelSet(int index)
    {
        return dynamic_cast<FITKModelSet*>(this->getDataByIndex(index));
    }

    FITKMeshSurface* FITKComponentManager::getAsMeshSurf(int index)
    {
        return dynamic_cast<FITKMeshSurface*>(this->getDataByIndex(index));
    }

    FITKModelSurface * FITKComponentManager::getSurf(int index)
    {
        return dynamic_cast<FITKModelSurface*>(this->getDataByIndex(index));
    }

    QList<Interface::FITKModelSet*> FITKComponentManager::getAllSet(bool rec)
    {
        QList<Interface::FITKModelSet*> sets;
        const int n = getDataCount();
        //不递归
        if (!rec)
        {
            for (int i = 0; i < n; i++)
            {
                auto s = this->getAsModelSet(i);
                if (s == nullptr) continue;
                sets.append(s);
            }
            return sets;
        }

        for (int i = 0; i < n; i++)
        {
            auto s = this->getAsModelSet(i);
            if (s == nullptr) continue;
            //复合集合取出子集合
            if (s->isCombination())
            {
                const int nsub = s->getDataCount();
                for (int i = 0; i < nsub; ++i)
                {
                    Interface::FITKModelSet* sub = s->getDataByIndex(i);
                    if (sub)
                        sets.append(sub);
                }
            }
            //不是复合集合
            else if (s)
                sets.append(s);
        }
        return sets;
    }

    QList<FITKModelSurface*> FITKComponentManager::getAllSurface()
    {
        QList<Interface::FITKModelSurface*> surf;
        const int n = getDataCount();
        for (int i = 0; i < n; i++)
        {
            auto s = this->getSurf(i);
            if (s)
                surf.append(s);
        }
        return surf;
    }

    void FITKComponentManager::appendDataObj(FITKAbstractDataObject* item)
    {
        //是表面，直接添加
        FITKModelSurface* surf = dynamic_cast<FITKModelSurface*>(item);
        if (surf)
        {
            Core::FITKAbstractDataManager<FITKAbstractModelComponent>::appendDataObj(item);
            return;
        }
        //装配体单元集合直接添加
        FITKAssemblyElementSet* ele = dynamic_cast<FITKAssemblyElementSet*>(item);
        if (ele)
        {
            Core::FITKAbstractDataManager<FITKAbstractModelComponent>::appendDataObj(item);
            return;
        }


        //强制类型判断
        FITKModelSet* set = dynamic_cast<FITKModelSet*>(item);
        if (set == nullptr) return;
        const QString name = set->getDataObjectName();
        //已经存在的set
        FITKModelSet* existSet = dynamic_cast<FITKModelSet*> (this->getDataByName(name));
        if (existSet == nullptr)
            Core::FITKAbstractDataManager<FITKAbstractModelComponent>::appendDataObj(item);
        else
            //已经存在的则合并
            mergeSameNameSet(existSet, set);
    }

    void FITKComponentManager::insertDataObj(int index, FITKAbstractDataObject* item)
    {
        //是表面，直接添加
        FITKMeshSurface* surf = dynamic_cast<FITKMeshSurface*>(item);
        if (surf)
        {
            Core::FITKAbstractDataManager<FITKAbstractModelComponent>::insertDataObj(index, item);
            return;
        }
        //装配体单元集合直接添加
        FITKAssemblyElementSet* ele = dynamic_cast<FITKAssemblyElementSet*>(item);
        if (ele)
        {
            Core::FITKAbstractDataManager<FITKAbstractModelComponent>::appendDataObj(item);
            return;
        }

        //强制类型判断
        FITKModelSet* set = dynamic_cast<FITKModelSet*>(item);
        if (set == nullptr) return;
        const QString name = set->getDataObjectName();
        //已经存在的set
        FITKModelSet* existSet = dynamic_cast<FITKModelSet*> (this->getDataByName(name));
        if (existSet == nullptr)
            Core::FITKAbstractDataManager<FITKAbstractModelComponent>::insertDataObj(index, item);
        else
            //已经存在的则合并
            mergeSameNameSet(existSet, set);
    }

    FITKAbstractModelComponent* FITKComponentManager::getDataByID(const int id)
    {
        //首先遍历最外层
        FITKAbstractModelComponent* comp =
            Core::FITKAbstractDataManager<FITKAbstractModelComponent>::getDataByID(id);
        if (comp) return comp;

        for (FITKAbstractDataObject* data : _dataList)
        {
            FITKModelSet* set = dynamic_cast<FITKModelSet*>(data);
            if (set == nullptr) continue;
            //不是复合集合已经在最外层遍历过
            if (!set->isCombination()) continue;
            const int nsub = set->getDataCount();
            //查找set的子集合
            comp = set->getDataByID(id);
            if (comp) return comp;
        }
        //没有找到
        return nullptr;
    }

    FITKModelSet* FITKComponentManager::getModelSetByMember(FITKModelEnum::FITKModelSetType t,
        const int modelID, const QList<int> &member)
    {
        if (member.isEmpty()) return nullptr;
        QList<FITKModelSet*> sets = this->getModelSet(t);
        //查找相同成员的集合
        for (FITKModelSet* set : sets)
        {
            if (set == nullptr) continue;
            if (set->getAbsModelID() != modelID) continue;
            QList<int> absmem = set->getAbsoluteMember();
            if (absmem == member) return set;
        }
        //没有找到，创建新的集合
        FITKModelSet* set = new FITKModelSet(t);
        set->setInternal(true);
        set->setModel(modelID);
        for (int i : member) set->appendMember(i);
        set->setDataObjectName(this->checkName("Set-1"));
        this->appendDataObj(set);
        return set;
    }

    FITKModelSet* FITKComponentManager::getSingleNodeSet(const int modelID, const int memID)
    {
        //全部节点集合
        QList<FITKModelSet*> sets = this->getModelSet(FITKModelEnum::FITKModelSetType::FMSNode);
        for (auto set : sets)
        {
            //父对象判断
            if (set->getAbsModelID() != modelID) continue;
            QList<int> mem = set->getMember();
            //只有一个成员
            if (mem.size() == 1 && mem.at(0) == memID)
                return set;
        }
        return nullptr;
    }

    void FITKComponentManager::mergeSameNameSet(FITKModelSet* existSet, FITKModelSet* newSet)
    {
        if (existSet == nullptr || newSet == nullptr) return;
        //已经存在的是复合集合，直接追加
        if (existSet->isCombination())
        {
            existSet->appendDataObj(newSet);
            return;
        }
        //创建新的，并合并已有集合与新集合
        FITKModelSet* comSet = new FITKModelSet;
        //        comSet->setModelSetType(FITKModelEnum::FITKModelSetType::FMSComb);
        comSet->setModelSetType(FITKModelEnum::FITKModelSetType::FMSMIX);
        comSet->setDataObjectName(existSet->getDataObjectName());
        comSet->setModel(this->getMParentDataIDM());

        //从原有管理器移除，追加到set中
        int index = this->getDataIndex(existSet);
        this->removeDataObjWithoutRelease(existSet);
        comSet->appendDataObj(existSet);
        comSet->appendDataObj(newSet);
        //新的复合集合追加到管理器
        Core::FITKAbstractDataManager<FITKAbstractModelComponent>::insertDataObj(index, comSet);
    }

}




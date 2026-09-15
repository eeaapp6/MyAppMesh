/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKComponentCreator.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceNode.h"

namespace Interface
{
    FITKComponentCreator::FITKComponentCreator(FITKComponentManager* manager)
        :_compManager(manager)
    {

    }

    void FITKComponentCreator::setName(const QString& name)
    {
        _name = name;
    }

    void FITKComponentCreator::isInternal(const bool internal)
    {
        _internal = internal;
    }

    FITKModelSet* FITKComponentCreator::createNodeSet(const int &parentID, const QList<int> & member, bool addCompManager)
    {
        //创建集合，不区分类型
        auto set = createSet(parentID, member, addCompManager);
        //设置为节点类型
        if (set)
            set->setModelSetType(Interface::FITKModelEnum::FITKModelSetType::FMSNode);
        return set;
    }

    FITKModelSet* FITKComponentCreator::createEleSet(const int &parentID, const QList<int> & member, bool addCompManager)
    {
        //创建集合，不区分类型
        auto set = createSet(parentID, member, addCompManager);
        //设置为单元类型
        if (set)
            set->setModelSetType(Interface::FITKModelEnum::FITKModelSetType::FMSElem);
        return set;
    }

    FITKModelSet* FITKComponentCreator::createGeometryVertexSet(const int &parentID, const QList<int> & member, bool addCompManager)
    {
        //创建集合，不区分类型
        auto set = createSet(parentID, member, addCompManager);
        //设置为几何类型
        if (set)
            set->setModelSetType(Interface::FITKModelEnum::FITKModelSetType::FMSPoint);
        return set;
    }

    FITKModelSet* FITKComponentCreator::createGeometryEdgeSet(const int &parentID, const QList<int> & member, bool addCompManager)
    {
        //创建集合，不区分类型
        auto set = createSet(parentID, member, addCompManager);
        //设置为几何类型
        if (set)
            set->setModelSetType(Interface::FITKModelEnum::FITKModelSetType::FMSEdge);
        return set;
    }

    FITKModelSet* FITKComponentCreator::createGeometryFaceSet(const int &parentID, const QList<int> & member, bool addCompManager)
    {
        //创建集合，不区分类型
        auto set = createSet(parentID, member, addCompManager);
        //设置为几何类型
        if (set)
            set->setModelSetType(Interface::FITKModelEnum::FITKModelSetType::FMSSurface);
        return set;
    }

    FITKModelSet* FITKComponentCreator::createGeometrySolidSet(const int &parentID, const QList<int> & member, bool addCompManager)
    {
        //创建集合，不区分类型
        auto set = createSet(parentID, member, addCompManager);
        //设置为几何类型
        if (set)
            set->setModelSetType(Interface::FITKModelEnum::FITKModelSetType::FMSSolid);
        return set;
    }

    FITKModelSet * FITKComponentCreator::editNodeSet(const int & parentID, const QList<int>& member, int originId)
    {
        //错误判断
        if (_compManager == nullptr) return nullptr;
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(parentID);
        if (model == nullptr) return nullptr;
        if (member.isEmpty()) return nullptr;
        //创建集合
        Interface::FITKModelSet* set = dynamic_cast<Interface::FITKModelSet*>(_compManager->getDataByID(originId));
        if (set == nullptr) return nullptr;
        set->setModel(parentID);
        set->setAbsoluteMember(member);
        return set;
    }

    FITKModelSet * FITKComponentCreator::editEleSet(const int & parentID, const QList<int>& member, int originId)
    {
        //错误判断
        if (_compManager == nullptr) return nullptr;
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(parentID);
        if (model == nullptr) return nullptr;
        if (member.isEmpty()) return nullptr;
        //创建集合
        Interface::FITKModelSet* set = dynamic_cast<Interface::FITKModelSet*>(_compManager->getDataByID(originId));
        if (set == nullptr) return nullptr;
        set->setModel(parentID);
        set->setAbsoluteMember(member);
        return set;
    }

    FITKMeshSurface* FITKComponentCreator::createEleSurface(QHash<Interface::FITKModelSet*, int >& setSurfaceIndex)
    {
        if (_compManager == nullptr || setSurfaceIndex.isEmpty()) return nullptr;

        QList<Interface::FITKModelSet*> sets = setSurfaceIndex.keys();
        FITKMeshSurfaceElement* surf = new FITKMeshSurfaceElement;
        surf->setModel(_compManager->getMParentDataIDM());
        surf->setDataObjectName(_name);

        for (Interface::FITKModelSet* set : sets)
        {
            if (set == nullptr) continue;
            const int surfIndex = setSurfaceIndex.value(set);
            if (surfIndex < 0) continue;

            surf->addMeshSet(set->getAbstractModel(), set, surfIndex);
        }
        _compManager->appendDataObj(surf);
        return surf;

    }

    FITKMeshSurface* FITKComponentCreator::editEleSurface(const QHash<Interface::FITKModelSet*, int >& setSurfaceIndex, int originId)
    {
        if (_compManager == nullptr || setSurfaceIndex.isEmpty()) return nullptr;
        FITKMeshSurfaceElement* surf = dynamic_cast<FITKMeshSurfaceElement*>(_compManager->getDataByID(originId));
        if (surf == nullptr) return nullptr;
        QList<Interface::FITKModelSet*> sets = setSurfaceIndex.keys();
        /*FITKMeshSurfaceElement* surf = new FITKMeshSurfaceElement;
        surf->setModel(_compManager->getMParentDataIDM());
        surf->setDataObjectName(_name);*/

        surf->clearMeshSet();
        for (Interface::FITKModelSet* set : sets)
        {
            if (set == nullptr) continue;
            const int surfIndex = setSurfaceIndex.value(set);
            if (surfIndex < 0) continue;

            surf->addMeshSet(set->getAbstractModel(), set, surfIndex);
        }
        return surf;

    }

    FITKMeshSurface * FITKComponentCreator::createNodeSurface(QHash<Interface::FITKModelSet*, int>& setSurfaceIndex)
    {
        if (_compManager == nullptr || setSurfaceIndex.isEmpty()) return nullptr;

        QList<Interface::FITKModelSet*> sets = setSurfaceIndex.keys();
        FITKMeshSurfaceNode* surf = new FITKMeshSurfaceNode;
        surf->setModel(_compManager->getMParentDataIDM());
        surf->setDataObjectName(_name);

        for (Interface::FITKModelSet* set : sets)
        {
            if (set == nullptr) continue;
            const int surfIndex = setSurfaceIndex.value(set);
            if (surfIndex < 0) continue;

            surf->addMeshSet(set->getAbstractModel(), set, surfIndex);
        }
        _compManager->appendDataObj(surf);
        return surf;
    }

    FITKMeshSurface * FITKComponentCreator::editNodeSurface(const QHash<Interface::FITKModelSet*, int>& setSurfaceIndex, int originId)
    {
        if (_compManager == nullptr || setSurfaceIndex.isEmpty()) return nullptr;
        FITKMeshSurfaceNode* surf = dynamic_cast<FITKMeshSurfaceNode*>(_compManager->getDataByID(originId));
        if (surf == nullptr) return nullptr;
        QList<Interface::FITKModelSet*> sets = setSurfaceIndex.keys();
        /*FITKMeshSurfaceElement* surf = new FITKMeshSurfaceElement;
        surf->setModel(_compManager->getMParentDataIDM());
        surf->setDataObjectName(_name);*/

        surf->clearMeshSet();
        for (Interface::FITKModelSet* set : sets)
        {
            if (set == nullptr) continue;
            const int surfIndex = setSurfaceIndex.value(set);
            if (surfIndex < 0) continue;

            surf->addMeshSet(set->getAbstractModel(), set, surfIndex);
        }
        return surf;
    }

    FITKModelSet* FITKComponentCreator::createSet(const int &parentID, const QList<int> & member, bool addCompManager)
    {
        //错误判断
        if (_compManager == nullptr) return nullptr;
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(parentID);
        if (model == nullptr) return nullptr;
        if (member.isEmpty()) return nullptr;
        //创建集合
        Interface::FITKModelSet* set = new Interface::FITKModelSet;
        set->setModel(parentID);
        set->setAbsoluteMember(member);
        //添加到管理器
        if (addCompManager)
        {
            set->setDataObjectName(_name);
            set->setInternal(_internal);
            _compManager->appendDataObj(set);
        }

        return set;
    }

}





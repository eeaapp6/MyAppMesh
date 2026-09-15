/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKVirtualTopoManager.h"
#include <QMutexLocker>
#include <QDebug>
namespace Interface
{
    FITKVirtualTopoManager::FITKVirtualTopoManager()
    {
        // Added by ChengHaotian 2024/09/11 优化工程文件读写效率，初始化所有类型虚拓扑形状管理器。
        _shapeVirtualTopos.insert(FITKGeoEnum::VTopoShapeType::VSPoint, new FITKShapeVirtualTopoManager);
        _shapeVirtualTopos.insert(FITKGeoEnum::VTopoShapeType::VSEdge, new FITKShapeVirtualTopoManager);
        _shapeVirtualTopos.insert(FITKGeoEnum::VTopoShapeType::VSWire, new FITKShapeVirtualTopoManager);
        _shapeVirtualTopos.insert(FITKGeoEnum::VTopoShapeType::VSFace, new FITKShapeVirtualTopoManager);
        _shapeVirtualTopos.insert(FITKGeoEnum::VTopoShapeType::VSShell, new FITKShapeVirtualTopoManager);
        _shapeVirtualTopos.insert(FITKGeoEnum::VTopoShapeType::VSSolid, new FITKShapeVirtualTopoManager);
        _shapeVirtualTopos.insert(FITKGeoEnum::VTopoShapeType::VSLump, new FITKShapeVirtualTopoManager);
        _shapeVirtualTopos.insert(FITKGeoEnum::VTopoShapeType::VSAssembly, new FITKShapeVirtualTopoManager);
    }

    FITKVirtualTopoManager::~FITKVirtualTopoManager()
    {
        this->clear();

        QList<FITKShapeVirtualTopoManager*> sms = _shapeVirtualTopos.values();
        for (FITKShapeVirtualTopoManager* sm : sms)
        {
            if (sm)
            {
                delete sm;
            }
        }

        _shapeVirtualTopos.clear();
    }

    void FITKVirtualTopoManager::clear()
    {
        QMutexLocker locker(&_mutex);
        QList<FITKShapeVirtualTopoManager*> sms = _shapeVirtualTopos.values();
        for (FITKShapeVirtualTopoManager* sm : sms)
        {
            if (sm)
            {
                sm->clear();
            }
        }
        // _shapeVirtualTopos.clear();
        _rootTopoObjs.clear();
    }

    void FITKVirtualTopoManager::clearRootTopos()
    {
        _rootTopoObjs.clear();
    }

    void FITKVirtualTopoManager::setRootObj(FITKAbsVirtualTopo* topoObj)
    {
        QMutexLocker locker(&_mutex);

        if (topoObj == nullptr) return;
        //记录根节点iD
        _rootTopoObjs.clear();
        _rootTopoObjs.push_back(topoObj->getDataObjectID());
    }

    void FITKVirtualTopoManager::addRootObj(FITKAbsVirtualTopo* topoObj)
    {
        if (topoObj == nullptr) return;
        _rootTopoObjs.push_back(topoObj->getDataObjectID());
    }

    FITKAbsVirtualTopo* FITKVirtualTopoManager::getRootObj(int index)
    {
        QMutexLocker locker(&_mutex);

        if (_rootTopoObjs.count() <= index) return nullptr;

        int rootObjId = _rootTopoObjs[index];

        //获取全部类型
        QList<FITKGeoEnum::VTopoShapeType > types = _shapeVirtualTopos.keys();
        qSort(types);

        //排序后选择最大的类型开始搜索
        for (int i = types.size() - 1; i >= 0; i--)
        {
            FITKGeoEnum::VTopoShapeType t = types.at(i);
            FITKShapeVirtualTopoManager* sm = _shapeVirtualTopos.value(t);
            FITKAbsVirtualTopo* topo = sm->getDataByID(rootObjId);
            if (topo != nullptr)  return topo;
        }

        return nullptr;
    }

    int FITKVirtualTopoManager::getNumberOfRootObjs()
    {
        return _rootTopoObjs.count();
    }

    void FITKVirtualTopoManager::appendVirtualTopoObj(FITKAbsVirtualTopo* tobj)
    {
        QMutexLocker locker(&_mutex);

        if (tobj == nullptr) return;
        //根据类型判断追加到哪个管理器
        Interface::FITKGeoEnum::VTopoShapeType t = tobj->getShapeType();
        FITKShapeVirtualTopoManager*mgr = _shapeVirtualTopos.value(t);
        if (mgr == nullptr)
        {
            //不存在则创建
            mgr = new FITKShapeVirtualTopoManager;
            _shapeVirtualTopos[t] = mgr;
        }
        mgr->appendDataObj(tobj);
    }

    void FITKVirtualTopoManager::appendVirtualTopoObj(FITKAbsVirtualTopo* tobj, Interface::FITKGeoEnum::VTopoShapeType st)
    {
        if (tobj == nullptr) return;
        //根据类型判断追加到哪个管理器 
        FITKShapeVirtualTopoManager* mgr = _shapeVirtualTopos.value(st);
        //if (mgr == nullptr)
        //{
        //    //不存在则创建
        //    mgr = new FITKShapeVirtualTopoManager;
        //    _shapeVirtualTopos[st] = mgr;
        //}
        mgr->appendDataObj(tobj);
    }

    void FITKVirtualTopoManager::appendVirtualTopoObjs(const QList< FITKAbsVirtualTopo*>& tobjs)
    {
        QMutexLocker locker(&_mutex);
        for (auto tobj : tobjs)
        {
            if (tobj == nullptr) return;
            //根据类型判断追加到哪个管理器
            Interface::FITKGeoEnum::VTopoShapeType t = tobj->getShapeType();
            if (t == Interface::FITKGeoEnum::VTopoShapeType::VSNone)
            {
                qDebug() << "Unable to resolve type tobjID :" << tobj->getDataObjectID();
                continue;
            }
            FITKShapeVirtualTopoManager*mgr = _shapeVirtualTopos.value(t);
            //if (mgr == nullptr)
            //{
            //    //不存在则创建
            //    mgr = new FITKShapeVirtualTopoManager;
            //    _shapeVirtualTopos[t] = mgr;
            //}
            mgr->appendDataObj(tobj);
        }

    }

    void FITKVirtualTopoManager::appendVirtualTopoObjs(Interface::FITKGeoEnum::VTopoShapeType st, const QList<Core::FITKAbstractDataObject*>& tobj)
    {
        FITKShapeVirtualTopoManager* mgr = _shapeVirtualTopos.value(st);
        //if (mgr == nullptr)
        //{
        //    mgr = new FITKShapeVirtualTopoManager;
        //    _shapeVirtualTopos[st] = mgr;
        //}
        mgr->appendDataObj(tobj);
    }

    FITKAbsVirtualTopo* FITKVirtualTopoManager::getVirtualTopo(void* shape, FITKGeoEnum::VTopoShapeType type)
    {
        QMutexLocker locker(&_mutex);

        //类型区分
        FITKShapeVirtualTopoManager* manager = _shapeVirtualTopos.value(type);
        if (manager == nullptr) return nullptr;

        //遍历子对象
        const int n = manager->getDataCount();
        for (int i = n - 1; i > -1; --i)
        {
            FITKAbsVirtualTopo* tvtp = manager->getDataByIndex(i);
            if (tvtp == nullptr) continue;
            //额外类型判断
            if (tvtp->getShapeType() != type) continue;

            FITKAbsVirtualTopoShape* tshape = tvtp->getShape();
            if (tshape == nullptr) continue;
            //形状一致性判断
            if (tshape->isSameShape(shape)) return tvtp;
        }
        return nullptr;

    }

    FITKShapeVirtualTopoManager* FITKVirtualTopoManager::getShapeVirtualTopoManager(FITKGeoEnum::VTopoShapeType type)
    {
        QMutexLocker locker(&_mutex);
        if (!_shapeVirtualTopos.contains(type)) return nullptr;
        return _shapeVirtualTopos.value(type);
    }

    int FITKVirtualTopoManager::getShapeVirtualTopoCount(FITKGeoEnum::VTopoShapeType type)
    {
        QMutexLocker locker(&_mutex);

        if (!_shapeVirtualTopos.contains(type)) return 0;
        FITKShapeVirtualTopoManager* m = _shapeVirtualTopos.value(type);
        //不存在，返回 0
        if (m == nullptr) return 0;
        return m->getDataCount();
    }

    int FITKVirtualTopoManager::getAllShapeVirtualTopoCount()
    {
        QMutexLocker locker(&_mutex);

        QList<FITKGeoEnum::VTopoShapeType > types = _shapeVirtualTopos.keys();

        int c = 0;
        //遍历全部子对象
        for (auto t : types)
        {
            if (!_shapeVirtualTopos.contains(t)) continue;
            FITKShapeVirtualTopoManager* m = _shapeVirtualTopos.value(t);
            if (m == nullptr) continue;

            c += m->getDataCount();
        }
        return c;
    }

    QList<FITKGeoEnum::VTopoShapeType> FITKVirtualTopoManager::getTopoTypes()
    {
        // 获取字典键列表。
        return _shapeVirtualTopos.keys();
    }

    FITKAbsVirtualTopo* FITKVirtualTopoManager::findDownInternal(int destId, FITKGeoEnum::VTopoShapeType srcType)
    {
        for (int i = srcType; i >= FITKGeoEnum::VTopoShapeType::VSPoint; i--)
        {
            FITKShapeVirtualTopoManager* sMgr = _shapeVirtualTopos[(FITKGeoEnum::VTopoShapeType)i];
            if (!sMgr)
            {
                continue;
            }

            FITKAbsVirtualTopo* vTopo = sMgr->getDataByID(destId);
            if (vTopo)
            {
                return vTopo;
            }
        }

        return nullptr;
    }

    FITKAbsVirtualTopo* FITKVirtualTopoManager::findTopo(int destId)
    {
        for (int i = FITKGeoEnum::VTopoShapeType::VSPoint; i <= FITKGeoEnum::VTopoShapeType::VSAssembly; i++)
        {
            FITKShapeVirtualTopoManager* sMgr = _shapeVirtualTopos[(FITKGeoEnum::VTopoShapeType)i];
            if (!sMgr)
            {
                continue;
            }

            FITKAbsVirtualTopo* vTopo = sMgr->getDataByID(destId);
            if (vTopo)
            {
                return vTopo;
            }
        }

        return nullptr;
    }
}


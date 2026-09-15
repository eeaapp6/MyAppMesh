/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKDataRepo.h"
#include "FITKAbstractDataManagerHelper.h"

namespace Core
{
    //静态变量初始化
    FITKDataRepo* FITKDataRepo::_instance = nullptr;
    QMutex FITKDataRepo::m_mutex;

    int FITKDataRepo::getDataCount()
    {                                              
        //线程安全
        QMutexLocker locker(&m_mutex);
        return _repoPrivate.getDataCount();
    }

    Core::FITKAbstractDataObject* FITKDataRepo::getDataByID(const int id)
    {
        //线程安全
        QMutexLocker locker(&m_mutex);
        return _repoPrivate.getDataByID(id);
    }

    void FITKDataRepo::resetRepo( QList<int> save /*= { -1 }*/)
    {
//        QMutexLocker locker(&m_mutex);
 
        //获取保留的最大ID
        int maxid =0;
        QList<FITKAbstractDataObject*> releaseObjs;
        //遍历全部，记录需要删除的对象
        const int n = _repoPrivate.getDataCount();
        for (int i = 0; i < n; ++i)
        {
            auto v = _repoPrivate.getDataByIndex(i);
            if (v == nullptr) continue;
            const int id = v->getDataObjectID();
            //记录最大ID
            if (save.contains(id))
            {
                maxid = id > maxid ? id : maxid;
                continue;
            }
            releaseObjs.append(v);
            
        }
        //删除
        for (FITKAbstractDataObject* obj : releaseObjs)
            _repoPrivate.removeDataObj(obj);
        //设置最大ID
        FITKAbstractDataObject::ResetMaxID(maxid);

    }

    QListIterator<Core::FITKAbstractDataObject*> FITKDataRepo::getIterator()
    {
        return _repoPrivate.getIterator();
    }

    void FITKDataRepo::initialize()
    {
        //初始化
    }

    void FITKDataRepo::finalize()
    {
//        _repoPrivate.clear();
    }

    void FITKDataRepo::addDataObj(FITKAbstractDataObject* obj)
    {
        //追加
        QMutexLocker locker(&m_mutex);
        _repoPrivate.appendDataObj(obj);
    }

    void FITKDataRepo::removeDataObj(FITKAbstractDataObject* obj)
    {
//        QMutexLocker locker(&m_mutex);
        _repoPrivate.remveFromListOnly(obj);
    }

    void FITKDataRepoPrivate::appendDataObj(FITKAbstractDataObject* item)
    {
        
        FITKAbstractDataManager<FITKAbstractDataObject>::appendDataObj(item);
        
    }

    void FITKDataRepoPrivate::remveFromListOnly(FITKAbstractDataObject* obj)
    {
        while (_dataList.contains(obj))
            _dataList.removeOne(obj);
    }

    FITKDataRepoDeleator::~FITKDataRepoDeleator()
    {
        if (FITKDataRepo::_instance)
            delete FITKDataRepo::_instance;
        FITKDataRepo::_instance = nullptr;
    }
    static FITKDataRepoDeleator DELEATOR_;
}




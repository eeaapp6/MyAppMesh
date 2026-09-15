/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraphObjRepo.h"
#include "FITKAbstractGraphObject.h"

namespace Core
{
    FITKGraphObjRepo* FITKGraphObjRepo::_instance = nullptr;
    QMutex FITKGraphObjRepo::m_mutex;

    int FITKGraphObjRepo::getGraphObjCount()
    {
        QMutexLocker locker(&m_mutex);
        return _repoPrivate.getGraphObjCount();
    }

    FITKAbstractGraphObject* FITKGraphObjRepo::getGraphObjAt(const int index)
    {
        QMutexLocker locker(&m_mutex);
        return _repoPrivate.getGraphObjAt(index);
    }

    QListIterator<Core::FITKAbstractGraphObject*> FITKGraphObjRepo::getIterator()
    {
        QMutexLocker locker(&m_mutex);
        return _repoPrivate.getIterator();
    }

    void FITKGraphObjRepo::clear()
    {
        QList<FITKAbstractGraphObject*> releaseObjs;
        {
            QMutexLocker locker(&m_mutex);
            const int count = _repoPrivate.getGraphObjCount();
            releaseObjs.reserve(count);
            for (int i = 0; i < count; ++i)
            {
                releaseObjs.append(_repoPrivate.getGraphObjAt(i));
            }
            _repoPrivate.clear();
        }

        for (FITKAbstractGraphObject* obj : releaseObjs)
        {
            delete obj;
        }
    }

    bool FITKGraphObjRepo::isContains(FITKAbstractGraphObject* obj)
    {
        QMutexLocker locker(&m_mutex);
        const int n = _repoPrivate.getGraphObjCount();
        for (int i = 0; i < n; ++i)
        {
            if (_repoPrivate.getGraphObjAt(i) == obj)
                return true;
        }
        return false;
    }

    void FITKGraphObjRepo::initialize()
    {
    }

    void FITKGraphObjRepo::finalize()
    {
        this->clear();
    }

    void FITKGraphObjRepo::addGraphObj(FITKAbstractGraphObject* obj)
    {
        QMutexLocker locker(&m_mutex);
        _repoPrivate.appendGraphObj(obj);
    }

    void FITKGraphObjRepo::removeGraphObj(FITKAbstractGraphObject* obj)
    {
        QMutexLocker locker(&m_mutex);
        _repoPrivate.remveFromListOnly(obj);
    }

    void FITKGraphObjRepoPrivate::appendGraphObj(FITKAbstractGraphObject* item)
    {
        if (item == nullptr || _graphObjList.contains(item))
            return;
        _graphObjList.append(item);
    }

    void FITKGraphObjRepoPrivate::remveFromListOnly(FITKAbstractGraphObject* obj)
    {
        while (_graphObjList.contains(obj))
            _graphObjList.removeOne(obj);
    }

    int FITKGraphObjRepoPrivate::getGraphObjCount() const
    {
        return _graphObjList.size();
    }

    FITKAbstractGraphObject* FITKGraphObjRepoPrivate::getGraphObjAt(const int index)
    {
        if (index < 0 || index >= _graphObjList.size())
            return nullptr;
        return _graphObjList.at(index);
    }

    QListIterator<Core::FITKAbstractGraphObject*> FITKGraphObjRepoPrivate::getIterator()
    {
        return QListIterator<Core::FITKAbstractGraphObject*>(_graphObjList);
    }

    void FITKGraphObjRepoPrivate::clear()
    {
        _graphObjList.clear();
    }

    FITKGraphObjRepoDeleator::~FITKGraphObjRepoDeleator()
    {
        if (FITKGraphObjRepo::_instance)
            delete FITKGraphObjRepo::_instance;
        FITKGraphObjRepo::_instance = nullptr;
    }

    static FITKGraphObjRepoDeleator DELEATOR_;
}
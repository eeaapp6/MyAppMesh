/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAbstractDataManagerPrivate.cpp
 * @brief 本文件中的全部函数均为虚函数，子类会override覆盖，
 *         故全部参数与返回值均无实际含义
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-11-29
 * 
 */
#include "FITKAbstractDataManagerPrivate.h"

Core::FITKAbstractDataManagerPrivate::~FITKAbstractDataManagerPrivate()
{
}

void Core::FITKAbstractDataManagerPrivate::appendDataObj(FITKAbstractDataObject* item)
{
}

int Core::FITKAbstractDataManagerPrivate::getDataCount() const
{
    //错误值
    return -1;
}

bool Core::FITKAbstractDataManagerPrivate::isContains(FITKAbstractDataObject* obj)
{
    //错误值
    return false;
}

void Core::FITKAbstractDataManagerPrivate::clear()
{
}

void Core::FITKAbstractDataManagerPrivate::insertDataObj(int index, FITKAbstractDataObject* item)
{
}

int Core::FITKAbstractDataManagerPrivate::getDataIndex(FITKAbstractDataObject* obj) const
{
    //错误值
    return -1;
}

Core::FITKAbstractDataObject* Core::FITKAbstractDataManagerPrivate::getDataObjectByName(const QString& name, bool CompSens /*= false*/)
{
    //错误值
    return nullptr;
}

QList<Core::FITKAbstractDataObject*> Core::FITKAbstractDataManagerPrivate::getDataObjectsByName(const QString& name, bool CompSens /*= false*/)
{
    //空链表
    return QList<Core::FITKAbstractDataObject*>();
}

void Core::FITKAbstractDataManagerPrivate::removeDataObj(FITKAbstractDataObject* obj)
{
}

void Core::FITKAbstractDataManagerPrivate::removeDataObjWithoutRelease(FITKAbstractDataObject* obj)
{
}

void Core::FITKAbstractDataManagerPrivate::removeDataByIndex(int index)
{
}

void Core::FITKAbstractDataManagerPrivate::removeDataByID(int id)
{
}

void Core::FITKAbstractDataManagerPrivate::removeDataByName(const QString &name, bool compSens /*= false*/)
{
}

QString Core::FITKAbstractDataManagerPrivate::checkName(const QString& name)
{
    //错误值
    return QString();
}

int Core::FITKAbstractDataManagerPrivate::getIndexByID(int dataId)
{
    //错误值
    return -1;
}

int Core::FITKAbstractDataManagerPrivate::getIDByIndex(int dataIndex)
{
    //错误值
    return -1;
}

void Core::FITKAbstractDataManagerPrivate::sortObject(ObjectSortType type)
{
}

bool Core::FITKAbstractDataManagerPrivate::isUsedDataObject(const QList<int>& data)
{
    return false;
}

bool Core::FITKAbstractDataManagerPrivate::IsUsedDataObjInManagers(const QList<int>& dataIDs, const QList<FITKAbstractDataManagerPrivate*>& mgrs)
{
    for (FITKAbstractDataManagerPrivate* pMgr : mgrs)
    {
        if (pMgr && pMgr->isUsedDataObject(dataIDs))
            return true;
    }
    return false;
}

QListIterator<Core::FITKAbstractDataObject*> Core::FITKAbstractDataManagerPrivate::getIterator()
{
    QList<FITKAbstractDataObject*> das;
    return QListIterator<FITKAbstractDataObject*>(das);
}

void Core::FITKAbstractDataManagerPrivate::sortObject(std::function<bool(FITKAbstractDataObject*, FITKAbstractDataObject*)> function)
{
}

void Core::FITKAbstractDataManagerPrivate::appendDataObj(const QList<FITKAbstractDataObject*> & items)
{
}


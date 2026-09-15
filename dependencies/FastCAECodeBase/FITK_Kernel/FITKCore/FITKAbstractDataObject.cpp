/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractDataObject.h"
#include "FITKDataRepo.h"
#include <QMutex>
#include <QMutexLocker>

namespace Core
{
    static FITKID ID = 0;
    static QMutex MUTEX;

    FITKAbstractDataObject::FITKAbstractDataObject(const int pid  )
    {
        QMutexLocker locker(&MUTEX);
        //自动分配ID
        _absDataID = FITKAbstractDataIDAlter::GetAValidDataID();
        ID = ID > _absDataID ? ID : _absDataID;
        _parentDataID = pid;
        //存储到全局数据仓库
        FITKDataRepo::getInstance()->addDataObj(this);
    }

    FITKAbstractDataObject::~FITKAbstractDataObject()
    {
        //触发对象释放信号
        emit dataObjectDestoried(this);
        //移出全局数据仓库
        FITKDataRepo::getInstance()->removeDataObj(this);
    }

    int FITKAbstractDataObject::getDataObjectID() const
    {
        return _absDataID;
    }


    void FITKAbstractDataObject::setParentDataID(const int id)
    {
        _parentDataID = id;
    }

    int FITKAbstractDataObject::getParentDataID() const
    {
        return _parentDataID;
    }

    QString FITKAbstractDataObject::serialize(const int & label /*= -1*/)
    {
        return QString();
    }

    bool FITKAbstractDataObject::deserialize(const QString& text, const int &label /*= -1*/)
    {
        return false;
    }

    QVariant FITKAbstractDataObject::getUserData(int userData)
    {
        return _userData.value(userData);
    }

    void FITKAbstractDataObject::setUserData(int ud, QVariant va)
    {
        //键值重复直接覆盖
        _userData.insert(ud, va);
    }

    QList<int> FITKAbstractDataObject::getUserDataKeys()
    {
        return _userData.uniqueKeys();
    }

    bool FITKAbstractDataObject::copy(FITKAbstractDataObject* obj)
    {
        //拷贝父对象
        _parentDataID = obj->getParentDataID();
        //拷贝用户数据
        QList<int > ks = obj->getUserDataKeys();
        for (int k : ks)
        {
            QVariant v = obj->getUserData(k);
            this->setUserData(k, v);
        }

        return true;
    }

    FITKAbstractDataObject* FITKAbstractDataObject::getParentObject()
    {
        return FITKDATAREPO->getDataByID(_parentDataID);
    }

    bool FITKAbstractDataObject::isUsedDataObject(const QList<int> & dataIDs)
    {
        return false;
    }

    void FITKAbstractDataObject::ResetMaxID(const int maxid /*=0*/)
    {
        ID = maxid;
    }

    int FITKAbstractDataObject::GetMaxID()
    {
        return ID;
    }

    void FITKAbstractDataObject::setAbsDataID(const int id)
    {
        _absDataID = id;
        //自动调大ID
        ID = _absDataID > ID ? _absDataID : ID;
    }

    bool FITKAbstractDataIDAlter::modifyDataID(FITKAbstractDataObject* d, const int id)
    {
        if (d == nullptr) return false;
        //判断ID是否被占用
        FITKAbstractDataObject* dobj = FITKDATAREPO->getDataByID(id);
        //id被占用
        if (dobj != nullptr)
        {
            int validID = this->GetAValidDataID();
            FITKAbstractDataIDAlter alter;
            alter.modifyDataID(dobj, validID);
            //重置最大ID
            FITKAbstractDataObject::ResetMaxID(++validID);
        } 
        //修改原始数据ID
        d->setAbsDataID(id);
        int maxID = FITKAbstractDataObject::GetMaxID();
        maxID = maxID > id ? maxID : id;
        //重置最大ID
        FITKAbstractDataObject::ResetMaxID(maxID);
        return true;
    }

    int FITKAbstractDataIDAlter::GetAValidDataID()
    {
        int id = FITKAbstractDataObject::GetMaxID();
        //数据仓库不存在这个ID，否则自增，一直找到没有被占用的ID
//         while (FITKDATAREPO->getDataByID(id) != nullptr)
//             ++id;
//         return id;
        return id + 1;
    }

}

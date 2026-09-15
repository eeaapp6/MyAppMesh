/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGUIRepo.h"
#include "FITKAbstractGUIObject.h"
#include "FITKAbstractGUI.h"
#include <QMutexLocker>

namespace Core
{
    //静态变量初始化
    FITKGUIRepo* FITKGUIRepo::_instance = nullptr;
    QMutex FITKGUIRepo::m_mutex;

    FITKAbstractGUIObject* FITKGUIRepo::getFITKGUIObject(const int id)
    {
        //不存在则返回空指针
        return _idObjectHash.value(id);
    }

    void FITKGUIRepo::closeGUIObject(const int objID)
    {
        FITKAbstractGUIObject* guiObj = _idObjectHash.value(objID);
        //delete会关闭
        if (guiObj != nullptr)
            delete guiObj;
    }

    void FITKGUIRepo::closeGUIObject(FITKAbstractGUIObject* obj)
    {
        if (!isContains(obj)) return;
        //delete会关闭
        delete obj;
    }

    void FITKGUIRepo::closeAllDialog( const QList<FITKDialog*> & except)
    {
        QList<FITKDialog*> dlgs = this->getGUIObjects<FITKDialog>();
        //没有需要特别处理的对象
        if(except.isEmpty())
        {
            for (auto d : dlgs)
                delete d;
        }
        else
        {
            for (auto d : dlgs)
            {
                //d是除外的对象
                if(except.contains(d))continue;
                delete d;
            }
        }
    }

    void FITKGUIRepo::showAllDialog(bool isshow, const QList<FITKDialog*>& except /*= QList<FITKDialog*>()*/)
    {
        QList<FITKDialog*> dlgs = this->getGUIObjects<FITKDialog>();
        //没有需要特别处理的对象
        if (except.isEmpty())
        {
            for (auto d : dlgs)
                if (d) d->setVisible(isshow);
        }
        else
        {
            for (auto d : dlgs)
            {
                if(!d) continue;
                //d是除外的对象
                if (except.contains(d))continue;
                d->setVisible(isshow);
            }
        }
    }

    QList<FITKAbstractGUIObject*> FITKGUIRepo::fileterGUIObject(const QString& key, const QVariant& va)
    {
        QList<FITKAbstractGUIObject*> fobjs;

        QList<FITKAbstractGUIObject*> guiObjs = _idObjectHash.values();
        //遍历对象
        for (FITKAbstractGUIObject* Obj:guiObjs)
        {
            if(Obj == nullptr) continue;
            if(!Obj->isContainsInfo(key)) continue;
            //筛选对象
            if (va == Obj->getAdditionalInfo(key))
                fobjs.append(Obj);
        }
        return fobjs;

    }

    bool FITKGUIRepo::isContains(FITKAbstractGUIObject* obj)
    {
        if (obj == nullptr) return false;
        const int id = obj->getGUIObjectID();
        return _idObjectHash.contains(id);
    }

    void FITKGUIRepo::setActiveDialog(const int & id)
    {
        QMutexLocker locker(&m_mutex);
        _activeID = id;
    }

    void FITKGUIRepo::insertGUIObject(FITKAbstractGUIObject* obj)
    {
        if (obj == nullptr) return;
        const int id = obj->getGUIObjectID();
        //纳入到管理
        _idObjectHash.insert(id, obj);
    }

    void FITKGUIRepo::removeGUIObject(FITKAbstractGUIObject* obj)
    {
        //FITKAbstractGUIObject析构调用
        if (obj == nullptr) return;
        const int id = obj->getGUIObjectID();
        //不能重复delete
        _idObjectHash.remove(id);
    }

    void FITKGUIRepo::initialize()
    {
    }

    void FITKGUIRepo::finalize()
    {

    }

    FITKGUIRepoDeleator::~FITKGUIRepoDeleator()
    {
        if (FITKGUIRepo::_instance)
            delete FITKGUIRepo::_instance;
        FITKGUIRepo::_instance = nullptr;
    }

    static FITKGUIRepoDeleator _DELEATOR__;

}




/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractGUIObject.h"
#include "FITKGUIRepo.h"
#include "FITKScriptTextRepo.h"

namespace Core
{
    FITKAbstractGUIObject::FITKAbstractGUIObject()
    {
        //自动分配ID
        static int ID = 0;
        _guiID = ++ID;
        //加入仓库
        FITKGUIREPO->insertGUIObject(this);
    }

    FITKAbstractGUIObject::~FITKAbstractGUIObject()
    {
        //从仓库移除
        FITKGUIREPO->removeGUIObject(this);
    }

    int FITKAbstractGUIObject::getGUIObjectID() const
    {
        return _guiID;
    }

    void FITKAbstractGUIObject::saveScript(const QStringList& script)
    {
        FITKScriptTextRepo::getInstance()->appendScript(script);
    }

    void FITKAbstractGUIObject::saveScript(const QString& script)
    {
        FITKScriptTextRepo::getInstance()->appendScript(script);
    }

    bool FITKAbstractGUIObject::isContainsInfo(const QString& key)
    {
        return _additionalInfo.contains(key);
    }

    void FITKAbstractGUIObject::setAdditionalInfo(const QString& key, const QVariant& va)
    {
        _additionalInfo.insert(key, va);
    }

    void FITKAbstractGUIObject::removeAdditionalInfo(const QString& key)
    {
        _additionalInfo.remove(key);
    }

    void FITKAbstractGUIObject::clearAdditionalInfo()
    {
        _additionalInfo.clear();
    }

    QVariant FITKAbstractGUIObject::getAdditionalInfo(const QString& key)
    {
        return _additionalInfo.value(key);
    }

}

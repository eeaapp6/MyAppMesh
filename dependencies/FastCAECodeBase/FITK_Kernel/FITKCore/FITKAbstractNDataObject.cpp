/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractNDataObject.h"

namespace Core
{
    //记录名称
    FITKAbstractNDataObject::FITKAbstractNDataObject(const QString & name)
        :m_dataObjName(name)
    {
    }

    FITKAbstractNDataObject::~FITKAbstractNDataObject()
    {
    }

    void FITKAbstractNDataObject::setDataObjectName(const QString & name)
    {
        //记录名称
        m_dataObjName = name;
    }

    QString FITKAbstractNDataObject::getDataObjectName()
    {
        //返回名称
        return m_dataObjName;
    }

    bool FITKAbstractNDataObject::copy(FITKAbstractDataObject* obj)
    {
        FITKAbstractNDataObject* nobj = dynamic_cast<FITKAbstractNDataObject*>(obj);
        if (nobj == nullptr) return  false;
        m_dataObjName = nobj->getDataObjectName();
        return FITKAbstractDataObject::copy(obj);
    }

}




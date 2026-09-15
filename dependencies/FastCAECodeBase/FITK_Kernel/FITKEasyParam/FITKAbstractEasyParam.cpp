/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractEasyParam.h"
#include "FITKCore/FITKLibInfo.h"

FITKLIBINFOREGISTER(FITKEasyParam, FITKEasyParamVersion);

namespace Core 
{
    FITKAbstractEasyParam::~FITKAbstractEasyParam()
    {
        _data.clear();
    }

    FITKAbstractEasyParam::FITKEasyParamType Core::FITKAbstractEasyParam::getParamType()
    {
        return  Core::FITKAbstractEasyParam::FITKEasyParamType::FEPNone;
    }

    void FITKAbstractEasyParam::copyParaData(FITKAbstractEasyParam * data)
    {
        if (data == nullptr)return;
        QList<QString> title = data->getDataTitle();
        for (QString t : title) {
            QVariant value = data->getData(t);
            this->setData(value, t);
        }
    }

    QVariant FITKAbstractEasyParam::getData(QString title)
    {
        return _data.value(title);
    }

    void FITKAbstractEasyParam::setData(QVariant data, QString title)
    {
        _data.insert(title, data);
    }

    QList<QString> FITKAbstractEasyParam::getDataTitle()
    {
        return _data.keys();
    }
}
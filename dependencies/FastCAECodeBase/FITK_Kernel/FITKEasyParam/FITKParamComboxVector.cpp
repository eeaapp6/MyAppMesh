/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKParamComboxVector.h"
#include "FITKParamCombox.h"

namespace Core
{
    FITKParamComboxVector::FITKParamComboxVector()
    {

    }

    FITKParamComboxVector::~FITKParamComboxVector()
    {
        clear();
    }

    FITKAbstractEasyParam::FITKEasyParamType FITKParamComboxVector::getParamType()
    {
        return FITKAbstractEasyParam::FITKEasyParamType::FEPComboxVector;
    }

    void FITKParamComboxVector::copyParaData(FITKAbstractEasyParam * data)
    {
        FITKAbstractEasyParam::copyParaData(data);
        //判断同一类型数据对象
        if (!data || data->getParamType() != this->getParamType())return;
        FITKParamComboxVector* oldData = dynamic_cast<FITKParamComboxVector*>(data);
        if (!oldData)return;
        //设置名称
        this->setDataObjectName(data->getDataObjectName());
        //拷贝参数
        for (auto d : oldData->getDatas()) {
            if (!d)continue;
            FITKParamCombox* newData = new FITKParamCombox();
            newData->copyParaData(d);
            pushData(newData);
        }
    }

    int FITKParamComboxVector::getCount()
    {
        return _value.size();
    }

    void FITKParamComboxVector::setDatas(QList<FITKParamCombox*> values)
    {
        clear();
        _value = values;
    }

    void FITKParamComboxVector::pushData(FITKParamCombox * value)
    {
        if (value == nullptr)return;
        _value.push_back(value);
    }

    FITKParamCombox* FITKParamComboxVector::getData(int index)
    {
        if (index < 0 || index >= _value.size())return nullptr;
        return _value[index];
    }

    QList<FITKParamCombox*> FITKParamComboxVector::getDatas()
    {
        return _value;
    }

    void FITKParamComboxVector::clear()
    {
        for (auto element : _value) {
            if (element != nullptr) {
                delete element;
                element = nullptr;
            }
        }
        _value.clear();
    }
}


/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKParamDoubleList.h"
#include "FITKParamDouble.h"

namespace Core
{
    FITKParamDoubleList::FITKParamDoubleList()
    {

    }

    FITKParamDoubleList::~FITKParamDoubleList()
    {
        clear();
    }

    FITKAbstractEasyParam::FITKEasyParamType FITKParamDoubleList::getParamType()
    {
        return FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList;
    }

    void FITKParamDoubleList::copyParaData(FITKAbstractEasyParam* data)
    {
        FITKAbstractEasyParam::copyParaData(data);

        //判断同一类型数据对象
        if (!data || data->getParamType() != this->getParamType())return;
        FITKParamDoubleList* oldData = dynamic_cast<FITKParamDoubleList*>(data);
        if (!oldData)return;
        //设置名称
        this->setDataObjectName(data->getDataObjectName());
        //拷贝参数
        for (auto d : oldData->getDoubleDatas()) {
            if (!d)continue;
            FITKParamDouble* newData = new FITKParamDouble();
            newData->copyParaData(d);
            pushDoubleData(newData);
        }
    }

    int FITKParamDoubleList::getCount()
    {
        return _value.size();
    }

    void FITKParamDoubleList::setValue(int index, double value)
    {
        if (index < 0 || index >= _value.size())return;
        FITKParamDouble* dV = _value[index];
        if (dV) {
            dV->setValue(value);
        }
    }

    void FITKParamDoubleList::setValue(QList<double> value)
    {
        clear();
        for (double v : value) {
            FITKParamDouble* dV = new FITKParamDouble();
            dV->setValue(v);
            _value.append(dV);
        }
    }

    void FITKParamDoubleList::setDoubleDatas(QList<FITKParamDouble*> values)
    {
        clear();
        _value = values;
    }

    void FITKParamDoubleList::pushDoubleData(FITKParamDouble * value)
    {
        if (!value)return;
        _value.append(value);
    }

    double FITKParamDoubleList::getValue(int index)
    {
        if (index < 0 || index >= _value.size())return 0.0;

        FITKParamDouble* dV = _value[index];
        if (dV) {
            return dV->getValue();
        }
        return 0.0;
    }

    FITKParamDouble* FITKParamDoubleList::getDoubleData(int index)
    {
        if (index < 0 || index >= _value.size())return nullptr;
        return _value[index];
    }

    QList<FITKParamDouble*> FITKParamDoubleList::getDoubleDatas()
    {
        return _value;
    }

    void FITKParamDoubleList::setRange(double* range)
    {
        for (auto v : _value) {
            if (!v)continue;
            v->setRange(range);
        }
    }

    void FITKParamDoubleList::getRange(double* range)
    {
        if (_value[0]) {
            _value[0]->getRange(range);
        }
    }

    void FITKParamDoubleList::clear()
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
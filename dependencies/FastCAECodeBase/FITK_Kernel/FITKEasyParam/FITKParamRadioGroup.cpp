/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKParamRadioGroup.h"
#include "FITKParameter.h"

namespace Core
{
    FITKParamRadioGroup::FITKParamRadioGroup()
    {

    }

    FITKParamRadioGroup::~FITKParamRadioGroup()
    {
        if (_subValue)delete _subValue;
        for (FITKRadioGroupValue v : _value)
        {
            if (v._value)delete v._value;
        }
    }

    void FITKParamRadioGroup::copyParaData(FITKAbstractEasyParam * data)
    {
        FITKAbstractEasyParam::copyParaData(data);
        //判断同一类型数据对象
        if (!data || data->getParamType() != this->getParamType())return;
        FITKParamRadioGroup* oldData = dynamic_cast<FITKParamRadioGroup*>(data);
        if (!oldData)return;
        //设置名称
        this->setDataObjectName(data->getDataObjectName());
        //子数据拷贝
        if (oldData->_subValue)
        {
            this->_subValue = new FITKParameter;
            this->_subValue->copyPara(oldData->_subValue);
        }
        //拷贝参数
        this->_currentIndex = oldData->_currentIndex;
        for (FITKRadioGroupValue v : oldData->_value)
        {
            FITKParameter* newValue = nullptr;
            if (v._value)
            {
                newValue = new FITKParameter;
                newValue->copyPara(v._value);
            }
            this->appendValue(v._name, newValue, v._labelValue);
        }
    }

    FITKAbstractEasyParam::FITKEasyParamType FITKParamRadioGroup::getParamType()
    {
        return FITKAbstractEasyParam::FITKEasyParamType::FEPRadioGroup;
    }

    void FITKParamRadioGroup::appendValue(QString name, FITKParameter* value, QList<bool> label)
    {
        FITKRadioGroupValue v;
        v._name = name;
        v._value = value;
        if (_subValue && label.size() == _subValue->getDataCount())
            v._labelValue = label;
        _value.append(v);
    }

    void FITKParamRadioGroup::setSubValue(FITKParameter* subValue)
    {
        if (_subValue) delete _subValue;
        _subValue = subValue;
    }

    QList<FITKRadioGroupValue> FITKParamRadioGroup::getRadioValues()
    {
        return _value;
    }

    QList<FITKAbstractEasyParam*> FITKParamRadioGroup::getSubValues()
    {
        if (_subValue == nullptr)return QList<FITKAbstractEasyParam*>();
        return _subValue->getParameter();
    }

    FITKParameter* FITKParamRadioGroup::getSubValuePara()
    {
        return _subValue;
    }

    QList<bool> FITKParamRadioGroup::getCurrentValueSubState()
    {
        if (_currentIndex < 0 || _currentIndex >= _value.size()) return  QList<bool>();

        FITKRadioGroupValue v = _value.at(_currentIndex);
        return v._labelValue;
    }

    void FITKParamRadioGroup::setCurrentIndex(int index)
    {
        if ((index < 0) || (index >= _value.size()))return;
        _currentIndex = index;
    }

    void FITKParamRadioGroup::setCurrentValue(QString name)
    {
        for (int i = 0; i < _value.size(); ++i)
        {
            if (_value.at(i)._name != name)continue;
            _currentIndex = i;
            return;
        }
        _currentIndex = -1;
    }

    QString FITKParamRadioGroup::getCurrentValue()
    {
        if (_currentIndex < 0 || _currentIndex >= _value.size()) return QString();
        return _value[_currentIndex]._name;
    }

    FITKRadioGroupValue FITKParamRadioGroup::getCurrentRadioGroupValue()
    {
        if (_currentIndex < 0 || _currentIndex >= _value.size()) return FITKRadioGroupValue();
        return _value[_currentIndex];
    }

    int FITKParamRadioGroup::getCurrentValueIndex()
    {
        return _currentIndex;
    }
}


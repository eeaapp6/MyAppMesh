/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKParamBoolGroup.h"
#include "FITKParameter.h"

namespace Core
{
    FITKParamBoolGroup::FITKParamBoolGroup()
    {

    }

    FITKParamBoolGroup::~FITKParamBoolGroup()
    {
        if (_valueGroup)delete _valueGroup;
    }

    FITKAbstractEasyParam::FITKEasyParamType FITKParamBoolGroup::getParamType()
    {
        return FITKAbstractEasyParam::FITKEasyParamType::FEPBoolGroup;
    }

    void FITKParamBoolGroup::copyParaData(FITKAbstractEasyParam* data)
    {
        FITKAbstractEasyParam::copyParaData(data);
        //判断同一类型数据对象
        if (!data || data->getParamType() != this->getParamType())return;
        FITKParamBoolGroup* oldData = dynamic_cast<FITKParamBoolGroup*>(data);
        if (!oldData)return;
        //设置名称
        this->setDataObjectName(data->getDataObjectName());
        //拷贝参数
        this->_value = oldData->_value;
        if (oldData->_valueGroup)
        {
            //如果数据组存在，清空
            if (_valueGroup)delete _valueGroup;
            _valueGroup = new FITKParameter;
            _valueGroup->copyPara(oldData->_valueGroup);
        }
    }

    void FITKParamBoolGroup::setValue(bool value)
    {
        _value = value;
        emit sigDataChange();
    }
    bool FITKParamBoolGroup::getValue()
    {
        return _value;
    }

    void FITKParamBoolGroup::setValueGroup(FITKParameter* g)
    {
        _valueGroup = g;
    }

    FITKParameter* FITKParamBoolGroup::getValueGroup()
    {
        if (_valueGroup) return _valueGroup;
        return nullptr;
    }
}


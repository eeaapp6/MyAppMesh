/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKParamDouble.h"

namespace Core
{
    FITKParamDouble::FITKParamDouble()
    {

    }

    FITKParamDouble::~FITKParamDouble()
    {

    }

    FITKAbstractEasyParam::FITKEasyParamType FITKParamDouble::getParamType()
    {
        return FITKAbstractEasyParam::FITKEasyParamType::FEPDouble;
    }

    void FITKParamDouble::copyParaData(FITKAbstractEasyParam* data)
    {
        FITKAbstractEasyParam::copyParaData(data);
        //判断同一类型数据对象
        if (!data || data->getParamType() != this->getParamType())return;
        FITKParamDouble* oldData = dynamic_cast<FITKParamDouble*>(data);
        if (!oldData)return;
        //设置名称
        this->setDataObjectName(data->getDataObjectName());
        //拷贝参数
        this->_value = oldData->_value;
        this->_range[0] = oldData->_range[0];
        this->_range[1] = oldData->_range[1];
    }

    void FITKParamDouble::setValue(double value)
    {
        if (value < _range[0] || value>_range[1])return;
        _value = value;
        emit sigDataChange();
    }

    double FITKParamDouble::getValue()
    {
        return _value;
    }

    void FITKParamDouble::setRange(double * range)
    {
        _range[0] = range[0];
        _range[1] = range[1];
    }

    void FITKParamDouble::getRange(double * range)
    {
        range[0] = _range[0];
        range[1] = _range[1];
    }
}


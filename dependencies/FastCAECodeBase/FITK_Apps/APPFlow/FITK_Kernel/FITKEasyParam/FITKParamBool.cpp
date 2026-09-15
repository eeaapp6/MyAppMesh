/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKParamBool.h"

namespace Core
{
    FITKParamBool::FITKParamBool()
    {

    }

    FITKParamBool::~FITKParamBool()
    {

    }

    FITKAbstractEasyParam::FITKEasyParamType FITKParamBool::getParamType()
    {
        return FITKAbstractEasyParam::FITKEasyParamType::FEPBool;
    }

    void FITKParamBool::copyParaData(FITKAbstractEasyParam* data)
    {
        FITKAbstractEasyParam::copyParaData(data);
        //判断同一类型数据对象
        if (!data || data->getParamType() != this->getParamType())return;
        FITKParamBool* oldData = dynamic_cast<FITKParamBool*>(data);
        if (!oldData)return;
        //设置名称
        this->setDataObjectName(data->getDataObjectName());
        //拷贝参数
        this->_value = oldData->_value;
    }

    void FITKParamBool::setValue(bool value)
    {
        _value = value;
        emit sigDataChange();
    }
    bool FITKParamBool::getValue()
    {
        return _value;
    }
}


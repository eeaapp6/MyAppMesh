/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKParamCombox.h"

namespace Core
{
    FITKParamCombox::FITKParamCombox()
    {

    }

    FITKParamCombox::~FITKParamCombox()
    {

    }

    FITKAbstractEasyParam::FITKEasyParamType FITKParamCombox::getParamType()
    {
        return FITKAbstractEasyParam::FITKEasyParamType::FEPCombox;
    }

    void FITKParamCombox::copyParaData(FITKAbstractEasyParam* data)
    {
        FITKAbstractEasyParam::copyParaData(data);
        //判断同一类型数据对象
        if (!data || data->getParamType() != this->getParamType())return;
        FITKParamCombox* oldData = dynamic_cast<FITKParamCombox*>(data);
        if (!oldData)return;
        //设置名称
        this->setDataObjectName(data->getDataObjectName());
        //拷贝参数
        this->_value = oldData->_value;
        this->_index = oldData->_index;
    }

    void FITKParamCombox::setValue(QStringList value)
    {
        _value = value;
    }
    QStringList FITKParamCombox::getValue()
    {
        return _value;
    }

    QString FITKParamCombox::getCurrentValue()
    {
        if (_value.size() <= _index || _index < 0)return QString();
        return _value.at(_index);
    }

    void FITKParamCombox::setIndex(int index)
    {
        if (_value.size() <= index || index < 0)return;
        _index = index;
        emit sigDataChange();
    }
    int FITKParamCombox::getIndex()
    {
        return _index;
    }
}


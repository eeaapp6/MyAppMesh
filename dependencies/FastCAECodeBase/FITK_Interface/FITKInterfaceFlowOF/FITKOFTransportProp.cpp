/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTransportProp.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKOFTransportModel.h"

namespace Interface
{
    FITKOFTransportPhase::FITKOFTransportPhase()
    {
    }

    FITKOFTransportPhase::~FITKOFTransportPhase()
    {
        if (_additional) delete _additional;
        if (_transportModel) delete _transportModel;
    }

    QString FITKOFTransportPhase::getPhaseName()
    {
        //获取材料相位名称
        return _name;
    }

    Core::FITKParameter* FITKOFTransportPhase::getPhaseAdditionalData()
    {
        //获取相位数据
        return _additional;
    }

    FITKAbsOFTransportModel* FITKOFTransportPhase::getTransportModel()
    {
        //获取运输模型
        return _transportModel;
    }

    void FITKOFTransportPhase::setPhaseName(QString name)
    {
        //设置相位名称
        _name = name;
    }

    void FITKOFTransportPhase::setPhaseAdditionalData(Core::FITKParameter* data)
    {
        //设置相位的其余数据
        if (_additional != data && _additional) delete _additional;
        _additional = data;
    }

    void FITKOFTransportPhase::setTransportModel(FITKAbsOFTransportModel* modelData)
    {
        //设置运输模型
        if (_transportModel != modelData && _transportModel) delete _transportModel;
        _transportModel = modelData;
    }

    FITKOFTransportProp::FITKOFTransportProp()
    {
        _additionalTrans = new Core::FITKParameter;
    }

    FITKOFTransportProp::~FITKOFTransportProp()
    {
        if (_additionalTrans) delete _additionalTrans;
        for (FITKOFTransportPhase* phase : _phases)
            if (phase) delete phase;
    }

    int FITKOFTransportProp::getPhasesCount()
    {
        //获取相位数量
        return _phases.size();
    }

    FITKOFTransportPhase* FITKOFTransportProp::getPhase(int index)
    {
        //获取相位
        if (index < 0 || index >= _phases.size())return nullptr;
        return _phases.at(index);
    }

    Core::FITKParameter* FITKOFTransportProp::getTransportAdditionalData()
    {
        //获取传输附加数据
        return _additionalTrans;
    }

    void FITKOFTransportProp::appendPhase(FITKOFTransportPhase* phase)
    {
        //添加相位数据
        if (!phase)return;
        _phases.append(phase);
    }

    void FITKOFTransportProp::setTransportAdditionalData(Core::FITKParameter* tranAddData)
    {
        //设置传输附加数据
        if (_additionalTrans != tranAddData && _additionalTrans) delete _additionalTrans;
        _additionalTrans = tranAddData;
    }
}

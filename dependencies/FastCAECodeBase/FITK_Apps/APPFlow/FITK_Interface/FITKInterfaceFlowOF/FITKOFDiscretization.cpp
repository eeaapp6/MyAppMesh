/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFDiscretization.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamRadioGroup.h"


namespace Interface
{
    FITKOFDiscretization::FITKOFDiscretization()
    {
        //创建数据
        _timeOption = new Core::FITKParameter;
        _gradients = new Core::FITKParameter;
        _interpolation = new Core::FITKParameter;
    }

    FITKOFDiscretization::~FITKOFDiscretization()
    {
        //销毁数据
        this->clearData();
    }

    void FITKOFDiscretization::onlyKeepConvection(QStringList nameList)
    {
        //移除除solver变量
        for (int i = _convection.size() - 1; i >= 0; --i)
        {
            QString name = _convection.at(i).first;
            if (nameList.contains(name))continue;
            if (_convection.at(i).second) delete _convection.at(i).second;
            _convection.removeAt(i);
        }
    }

    Core::FITKParameter* FITKOFDiscretization::getTimeOption()
    {
        return _timeOption;
    }

    int FITKOFDiscretization::getConvectionCount()
    {
        return _convection.size();
    }

    QString FITKOFDiscretization::getConvectionVName(int index)
    {
        if (index < 0 || index >= _convection.size())return QString();
        return _convection.at(index).first;
    }

    Core::FITKParameter* FITKOFDiscretization::getConvectionVPara(int index)
    {
        if (index < 0 || index >= _convection.size())return nullptr;
        return _convection.at(index).second;
    }

    Core::FITKParameter* FITKOFDiscretization::getGradients()
    {
        return _gradients;
    }

    Core::FITKParameter* FITKOFDiscretization::getInterpolation()
    {
        return _interpolation;
    }

    void FITKOFDiscretization::insertConvection(QString name, Core::FITKParameter* variable)
    {
        if (!variable)return;
        _convection.append(QPair<QString, Core::FITKParameter*>(name, variable));
    }

    void FITKOFDiscretization::clearConvection()
    {
        for (QList<QPair<QString, Core::FITKParameter*>>::iterator iter = _convection.begin(); iter != _convection.end(); ++iter)
        {
            if (iter->second)delete iter->second;
        }
        _convection.clear();
    }

    void FITKOFDiscretization::initGradients()
    {
        //初始化Gradients
        if (!_gradients)return;
        //Gradient数据
        Core::FITKParamRadioGroup* gradient = _gradients->createRadioGroupParameter("Gradient");
        //创建修饰Gradient的value
        Core::FITKParameter* decorated = new Core::FITKParameter;
        Core::FITKParamRadioGroup* limitScheme = decorated->createRadioGroupParameter("Limiting Scheme");
        limitScheme->appendValue("none", nullptr);
        Core::FITKParameter* vCellLimited = new Core::FITKParameter;
        vCellLimited->createDoubleParameter("Cell Limited", 1.0, 1.0, 0.0);
        limitScheme->appendValue("Cell Limited", vCellLimited);
        Core::FITKParameter* vFaceLimited = new Core::FITKParameter;
        vFaceLimited->createDoubleParameter("Face Limited", 1.0, 1.0, 0.0);
        limitScheme->appendValue("Face Limited", vFaceLimited);
        limitScheme->setCurrentValue("none");
        gradient->setSubValue(decorated);
        //设置选择项
        Core::FITKParameter* vGauss = new Core::FITKParameter;
        vGauss->createComboxParameter("Gauss", QStringList() << "Linear" << "Cubic" << "MidPoint", 0);
        gradient->appendValue("Gauss", vGauss, QList<bool>() << true);
        gradient->appendValue("Least Squares", nullptr, QList<bool>() << true);
        gradient->appendValue("Fourth", nullptr, QList<bool>() << true);
        gradient->setCurrentValue("Gauss");

        //Surface Normal Gradient
        Core::FITKParamRadioGroup* surfGrad = _gradients->createRadioGroupParameter("Surface Normal Gradient");
        //设置选择项
        surfGrad->appendValue("Corrected", nullptr);
        surfGrad->appendValue("Uncorrected", nullptr);
        surfGrad->appendValue("Orthogonal", nullptr);
        surfGrad->appendValue("Face Corrected", nullptr);
        Core::FITKParameter* vLimited = new Core::FITKParameter;
        vLimited->createDoubleParameter("Limited", 0.5);
        surfGrad->appendValue("Limited", vLimited);
        Core::FITKParameter* vLinearFit = new Core::FITKParameter;
        vLinearFit->createDoubleParameter("Linear Fit", 1.0);
        surfGrad->appendValue("Linear Fit", vLinearFit);
        Core::FITKParameter* vQuadraticFit = new Core::FITKParameter;
        vQuadraticFit->createDoubleParameter("Quadratic Fit", 1.0);
        surfGrad->appendValue("Quadratic Fit", vQuadraticFit);
        surfGrad->setCurrentValue("Corrected");
    }

    void FITKOFDiscretization::initInterpolation()
    {
        //初始化Interpolation
        if (!_interpolation)return;
        _interpolation->createComboxParameter("Scheme", QStringList() << "Linear" << "Cubic" << "MidPoint", 0);
    }

    void FITKOFDiscretization::initAppendTurbulenceConvection(QString name)
    {
        //添加变量
        Core::FITKParameter* variable = new Core::FITKParameter;
        this->insertConvection(name, variable);
        Core::FITKParamRadioGroup* convectionType = variable->createRadioGroupParameter("Convection Type");
        //创建修饰convectionType的value
        Core::FITKParameter* decorated = new Core::FITKParameter;
        decorated->createBoolParameter("VectorSpecific", true);
        decorated->createBoolParameter("Bounded", true);
        decorated->createBoolParameter("SkewCorrected", false);
        convectionType->setSubValue(decorated);
        //设置选择项
        convectionType->appendValue("upwind", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("linearUpwind", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("linear", nullptr, QList<bool>() << false << true << true);
        Core::FITKParameter* vlimitedLinear = new Core::FITKParameter;
        vlimitedLinear->createDoubleParameter("limitedLinear", 1.0);
        convectionType->appendValue("limitedLinear", vlimitedLinear, QList<bool>() << false << true << true);
        convectionType->appendValue("LUST", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("cubic", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("vanLeer", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("MUSCL", nullptr, QList<bool>() << false << true << true);
        convectionType->appendValue("QUICK", nullptr, QList<bool>() << false << true << true);
        Core::FITKParameter* vDEShybrid = new Core::FITKParameter;
        vDEShybrid->createDoubleParameter("DEShybrid", 10.0);
        vDEShybrid->createDoubleParameter("DEShybrid", 1.0);
        convectionType->appendValue("DEShybrid", vDEShybrid, QList<bool>() << false << true << true);
        //设置初始值
        convectionType->setCurrentValue("upwind");
    }

    void FITKOFDiscretization::clearData()
    {
        if (_timeOption) delete _timeOption;
        _timeOption = nullptr;
        if (_gradients) delete _gradients;
        _gradients = nullptr;
        if (_interpolation) delete _interpolation;
        _interpolation = nullptr;
        for (QList<QPair<QString, Core::FITKParameter*>>::iterator iter = _convection.begin(); iter != _convection.end(); ++iter)
        {
            if (iter->second)delete iter->second;
        }
        _convection.clear();
    }
}




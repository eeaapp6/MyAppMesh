/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFRadiation.h"
#include "FITKOFRadiationModel.h"
#include "FITKOFRadiationAbsorptionModel.h"
#include "FITKOFRadiationScatterModel.h"
#include "FITKOFRadiationSunDirection.h"
#include "FITKOFRadiationSunLoad.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{
    FITKOFRadiationSolarLoad::FITKOFRadiationSolarLoad()
    {
        _radiationSolarLoadParams = new Core::FITKParameter;
    }

    FITKOFRadiationSolarLoad::~FITKOFRadiationSolarLoad()
    {
        //数据释放
        if (_radiationSolarLoadParams) delete _radiationSolarLoadParams;
        if (_sunLoad) delete _sunLoad;
        if (_sunDirection) delete _sunDirection;
    }

    Core::FITKParameter * FITKOFRadiationSolarLoad::getRadiationSolarLoadParams()
    {
        //获取太阳能负载参数
        return _radiationSolarLoadParams;
    }

    FITKOFAbsRadiationSunDirection * FITKOFRadiationSolarLoad::getRadiationSunDirection()
    {
        //获取太阳辐射方向
        return _sunDirection;
    }

    FITKOFAbsRadiationSunLoad * FITKOFRadiationSolarLoad::getRadiationSunLoad()
    {
        //获取太阳辐射载荷参数
        return _sunLoad;
    }

    void FITKOFRadiationSolarLoad::setRadiationSunDirection(FITKOFAbsRadiationSunDirection * sunDirection)
    {
        //设置太阳辐射方向
        if (_sunDirection != sunDirection && _sunDirection) delete _sunDirection;
        _sunDirection = sunDirection;
    }

    void FITKOFRadiationSolarLoad::setRadiationSunLoad(FITKOFAbsRadiationSunLoad * sunLoad)
    {
        //设置太阳辐射载荷参数
        if (_sunLoad != sunLoad && _sunLoad) delete _sunLoad;
        _sunLoad = sunLoad;
    }

    FITKOFRadiation::FITKOFRadiation()
    {
        _radiationParams = new Core::FITKParameter;
        _solarLoad = new FITKOFRadiationSolarLoad;
    }

    FITKOFRadiation::~FITKOFRadiation()
    {
        //数据释放
        if (_radiationParams) delete _radiationParams;
        if (_radiationModel) delete _radiationModel;
        if (_absorptionModel) delete _absorptionModel;
        if (_scatterModel) delete _scatterModel;
        if (_solarLoad) delete _solarLoad;
    }

    Core::FITKParameter * FITKOFRadiation::getRadiationParam()
    {
        //获取辐射参数
        return _radiationParams;
    }

    FITKOFAbsRadiationModel * FITKOFRadiation::getRadiationModel()
    {
        //获取辐射模型
        return _radiationModel;
    }

    FITKOFAbsRadiationAbsorptionModel * FITKOFRadiation::getAbsorptionModel()
    {
        //获取吸收系数模型
        return _absorptionModel;
    }

    FITKOFAbsRadiationScatterModel * FITKOFRadiation::getScatterModel()
    {
        //获取散射模型
        return _scatterModel;
    }

    FITKOFRadiationSolarLoad * FITKOFRadiation::getSolarLoad()
    {
        //获取太阳辐射数据
        return _solarLoad;
    }

    void FITKOFRadiation::setRadiationModel(FITKOFAbsRadiationModel * radiationModel)
    {
        //设置辐射模型
        if (_radiationModel != radiationModel && _radiationModel) delete _radiationModel;
        _radiationModel = radiationModel;
    }

    void FITKOFRadiation::setAbsorptionModel(FITKOFAbsRadiationAbsorptionModel * absorptionModel)
    {
        //设置吸收系数模型
        if (_absorptionModel != absorptionModel && _absorptionModel) delete _absorptionModel;
        _absorptionModel = absorptionModel;
    }

    void FITKOFRadiation::setScatterModel(FITKOFAbsRadiationScatterModel * scatterModel)
    {
        //设置散射模型
        if (_scatterModel != scatterModel && _scatterModel) delete _scatterModel;
        _scatterModel = scatterModel;
    }
}

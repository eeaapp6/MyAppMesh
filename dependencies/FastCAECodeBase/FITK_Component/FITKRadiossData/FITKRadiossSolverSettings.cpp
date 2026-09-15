/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossSolverSettings.h"

namespace Radioss
{

    void FITKRadiossSolverRunExeTimeParam::setFinalTime(const double time)
    {
        //设置运行最终时间
        _finalTime = time;
    }

    double FITKRadiossSolverRunExeTimeParam::getFinalTime() const
    {
        //获取运行最终时间
        return _finalTime;
    }

    void FITKRadiossSolverGlobalTimeParam::setScaleFactor(const double s)
    {
        //设置时间停止的缩放因子
        _scaleFactor = s;
    }

    double FITKRadiossSolverGlobalTimeParam::getScaleFactor() const
    {
        //获取时间停止的缩放因子
        return _scaleFactor;
    }

    void FITKRadiossSolverGlobalTimeParam::setEnableAdditionalCards(const bool state)
    {
        //设置启用用户输入GTS收敛的容差与周期数
        _isAdditionalCards = state;
    }

    bool FITKRadiossSolverGlobalTimeParam::isEnableAdditionalCards() const
    {
        //是否启用用户输入GTS收敛的容差与周期数
        return _isAdditionalCards;
    }

    void FITKRadiossSolverGlobalTimeParam::setToleranceGTS(const double v)
    {
        //设置GTS收敛的容差
        _toleranceGTS = v;
    }

    double FITKRadiossSolverGlobalTimeParam::getToleranceGTS() const
    {
        //获取GTS收敛的容差
        return _toleranceGTS;
    }

    void FITKRadiossSolverGlobalTimeParam::setNumCycles(const int n)
    {
        //设置周期数
        _numCycles = n;
    }

    int FITKRadiossSolverGlobalTimeParam::getNumCycles() const
    {
        //获取周期数
        return _numCycles;
    }

    void FITKRadiossSolverNodalTimeStepControlParam::setTimeStepControlType(const NodalTimeStepControlType type)
    {
        _controlType = type;
    }

    FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType FITKRadiossSolverNodalTimeStepControlParam::getTimeStepControlType() const
    {
        return _controlType;
    }

    void FITKRadiossSolverNodalTimeStepControlParam::setMinimumTimeStep(const double time)
    {
        _minTimeStep = time;
    }

    double FITKRadiossSolverNodalTimeStepControlParam::getMinimumTimeStep() const
    {
        return _minTimeStep;
    }

    void FITKRadiossSolverNodalTimeStepControlParam::setScaleFactor(const double s)
    {
        _scaleFactor = s;
    }

    double FITKRadiossSolverNodalTimeStepControlParam::getScaleFactor() const
    {
        return _scaleFactor;
    }

    void FITKRadiossSolverNodalTimeStepControlParam::setInitialAddMassRatio(const double r)
    {
        _initAddMassRatio = r;
    }

    double FITKRadiossSolverNodalTimeStepControlParam::getInitialAddMassRatio() const
    {
        return _initAddMassRatio;
    }

    void FITKRadiossSolverElementalTimeStepControlParam::setTimeStepControlType(const ElementalTimeStepControlType type)
    {
        _controlType = type;
    }

    FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType FITKRadiossSolverElementalTimeStepControlParam::getTimeStepControlType() const
    {
        return _controlType;
    }

    void FITKRadiossSolverElementalTimeStepControlParam::setMinimumTimeStep(const double time)
    {
        _minTimeStep = time;
    }

    double FITKRadiossSolverElementalTimeStepControlParam::getMinimumTimeStep() const
    {
        return _minTimeStep;
    }

    void FITKRadiossSolverElementalTimeStepControlParam::setNumberOfAddCards(const bool addCards)
    {
        _numberOfAddCards = addCards;
    }

    bool FITKRadiossSolverElementalTimeStepControlParam::getNumberOfAddCards() const
    {
        return _numberOfAddCards;
    }

    void FITKRadiossSolverElementalTimeStepControlParam::setMinAspectRatio(const double value)
    {
        _minAspectRatio = value;
    }

    double FITKRadiossSolverElementalTimeStepControlParam::getMinAspectRatio() const
    {
        return _minAspectRatio;
    }

    void FITKRadiossSolverElementalTimeStepControlParam::setMinVolumeRatio(const double value)
    {
        _minVolumeRatio = value;
    }

    double FITKRadiossSolverElementalTimeStepControlParam::getMinVolumeRatio() const
    {
        return _minVolumeRatio;
    }

    void FITKRadiossSolverElementalTimeStepControlParam::setScaleFactor(const double s)
    {
        _scaleFactor = s;
    }

    double FITKRadiossSolverElementalTimeStepControlParam::getScaleFactor() const
    {
        return _scaleFactor;
    }

    void FITKRadiossSolverAdaptiveDynamicRelaxation::setEnableDynamicRelaxation(const bool isEnable)
    {
        _enableDynamicRelaxation = isEnable;
    }

    bool FITKRadiossSolverAdaptiveDynamicRelaxation::getEnableDynamicRelaxation() const
    {
        return _enableDynamicRelaxation;
    }

    void FITKRadiossSolverAdaptiveDynamicRelaxation::setEnableStartTime(const bool isEnable)
    {
        _enableStartTime = isEnable;
    }

    bool FITKRadiossSolverAdaptiveDynamicRelaxation::isEnableStartTime() const
    {
        return _enableStartTime;
    }

    void FITKRadiossSolverAdaptiveDynamicRelaxation::setStartTime(const double time)
    {
        _startTime = time;
    }

    double FITKRadiossSolverAdaptiveDynamicRelaxation::getStartTime() const
    {
        return _startTime;
    }

    void FITKRadiossSolverAdaptiveDynamicRelaxation::setEnableStopTime(const bool isEnable)
    {
        _enableStopTime = isEnable;
    }

    bool FITKRadiossSolverAdaptiveDynamicRelaxation::isEnableStopTime() const
    {
        return _enableStopTime;
    }

    void FITKRadiossSolverAdaptiveDynamicRelaxation::setStopTime(const double time)
    {
        _stopTime = time;
    }

    double FITKRadiossSolverAdaptiveDynamicRelaxation::getStopTime() const
    {
        return _stopTime;
    }

    void FITKRadiossSolverAMSElementalTimeStepControlParam::setEnableCriticalTimeStepScaleFactor(const bool isEnable)
    {
        _isEnableCriticalTimeStepScaleFactor = isEnable;
    }

    bool FITKRadiossSolverAMSElementalTimeStepControlParam::isEnableCriticalTimeStepScaleFactor() const
    {
        return _isEnableCriticalTimeStepScaleFactor;
    }

    void FITKRadiossSolverAMSElementalTimeStepControlParam::setCriticalTimeStepScaleFactor(const double value)
    {
        _criticalTimeStepScaleFactor = value;
    }
    double FITKRadiossSolverAMSElementalTimeStepControlParam::getCriticalTimeStepScaleFactor() const
    {
        return _criticalTimeStepScaleFactor;
    }

    void FITKRadiossSolverAMSElementalTimeStepControlParam::setEnableMinTimeStep(const bool isEnable)
    {
        _isEnableMinTimeStep = isEnable;
    }
    bool FITKRadiossSolverAMSElementalTimeStepControlParam::isEnableMinTimeStep() const
    {
        return _isEnableMinTimeStep;
    }

    void FITKRadiossSolverAMSElementalTimeStepControlParam::setMinTimeStep(const double value)
    {
        _minTimeStep = value;
    }
    double FITKRadiossSolverAMSElementalTimeStepControlParam::getMinTimeStep() const
    {
        return _minTimeStep;
    }

    void FITKRadiossSolverAMSElementalTimeStepControlParam::setEnableAMSConvergenceTolerance(const bool isEnable)
    {
        _isEnableAMSConvergenceTolerance = isEnable;
    }
    bool FITKRadiossSolverAMSElementalTimeStepControlParam::isEnableAMSConvergenceTolerance() const
    {
        return _isEnableAMSConvergenceTolerance;
    }

    void FITKRadiossSolverAMSElementalTimeStepControlParam::setAMSConvergenceTolerance(const double value)
    {
        _AMSConvergenceTolerance = value;
    }
    double FITKRadiossSolverAMSElementalTimeStepControlParam::getAMSConvergenceTolerance() const
    {
        return _AMSConvergenceTolerance;
    }

    void FITKRadiossSolverAMSElementalTimeStepControlParam::setEnableMaxIterationsConjugateGradient(const bool isEnable)
    {
        _isEnableMaxIterationsConjugateGradient = isEnable;
    }
    bool FITKRadiossSolverAMSElementalTimeStepControlParam::isEnableMaxIterationsConjugateGradient() const
    {
        return _isEnableMaxIterationsConjugateGradient;
    }

    void FITKRadiossSolverAMSElementalTimeStepControlParam::setMaxIterationsConjugateGradient(const double value)
    {
        _maxIterationsConjugateGradient = value;
    }
    double FITKRadiossSolverAMSElementalTimeStepControlParam::getMaxIterationsConjugateGradient() const
    {
        return _maxIterationsConjugateGradient;
    }

    void FITKRadiossSolverAMSElementalTimeStepControlParam::setEnableNumberOfAdditionalOutputs(const bool isEnable)
    {
        _isEnableNumberOfAdditionalOutputs = isEnable;
    }
    bool FITKRadiossSolverAMSElementalTimeStepControlParam::isEnableNumberOfAdditionalOutputs() const
    {
        return _isEnableNumberOfAdditionalOutputs;
    }

    void FITKRadiossSolverAMSElementalTimeStepControlParam::setNumberOfAdditionalOutputs(const double value)
    {
        _numberOfAdditionalOutputs = value;
    }
    double FITKRadiossSolverAMSElementalTimeStepControlParam::getNumberOfAdditionalOutputs() const
    {
        return _numberOfAdditionalOutputs;
    }

    void FITKRadiossSolverComputationControlParam::setStopComputationWhenNegativeVolume(const bool isStop)
    {
        _stopComputationWhenNegativeVolume = isStop;
    }

    bool FITKRadiossSolverComputationControlParam::getStopComputationWhenNegativeVolume() const
    {
        return _stopComputationWhenNegativeVolume;
    }

    void FITKRadiossSolverComputationControlParam::setParallelArithmetic(const bool isParallel)
    {
        _parallelArithmetic = isParallel;
    }

    bool FITKRadiossSolverComputationControlParam::getParallelArithmetic() const
    {
        return _parallelArithmetic;
    }

    void FITKRadiossSolverConvergenceCriteriaParam::setEnableState(const bool state)
    {
        _enableState = state;
    }

    bool FITKRadiossSolverConvergenceCriteriaParam::getEnableState() const
    {
        return _enableState;
    }

    void FITKRadiossSolverConvergenceCriteriaParam::setEnergyErrorRatio(const double value)
    {
        _energyErrorRatio = value;
    }
    double FITKRadiossSolverConvergenceCriteriaParam::getEnergyErrorRatio() const
    {
        return _energyErrorRatio;
    }

    void FITKRadiossSolverConvergenceCriteriaParam::setTotalMassRatio(const double value)
    {
        _totalMassRatio = value;
    }
    double FITKRadiossSolverConvergenceCriteriaParam::getTotalMassRatio() const
    {
        return _totalMassRatio;
    }

    void FITKRadiossSolverConvergenceCriteriaParam::setNodalMassRatio(const double value)
    {
        _nodalMassRatio = value;
    }
    double FITKRadiossSolverConvergenceCriteriaParam::getNodalMassRatio() const
    {
        return _nodalMassRatio;
    }

    void FITKRadiossSolverConvergenceCriteriaParam::setWriteTimeHistoryFile(const bool state)
    {
        _writeTimeHistoryFile = state;
    }
    bool FITKRadiossSolverConvergenceCriteriaParam::getWriteTimeHistoryFile() const
    {
        return _writeTimeHistoryFile;
    }

    void FITKRadiossSolverConvergenceCriteriaParam::setWriteAnimationFile(const bool state)
    {
        _writeAnimationFile = state;
    }
    bool FITKRadiossSolverConvergenceCriteriaParam::getWriteAnimationFile() const
    {
        return _writeAnimationFile;
    }

    void FITKRadiossSolverConvergenceCriteriaParam::setEnergyError(const bool state)
    {
        _energyError = state;
    }
    bool FITKRadiossSolverConvergenceCriteriaParam::getEnergyError() const
    {
        return _energyError;
    }




    FITKRadiossSolverSettings::FITKRadiossSolverSettings()
    {
        _runExeTimeParam = new FITKRadiossSolverRunExeTimeParam;
        _globalTimeParam = new FITKRadiossSolverGlobalTimeParam;
        _nodalTimeStepControlParam = new FITKRadiossSolverNodalTimeStepControlParam;
        _elementalTimeStepControlParam = new FITKRadiossSolverElementalTimeStepControlParam;
        _adaptiveDynamicRelaxation = new FITKRadiossSolverAdaptiveDynamicRelaxation;
        _AMSElementalTimeStepControlParam = new FITKRadiossSolverAMSElementalTimeStepControlParam;
        _computationControlParam = new FITKRadiossSolverComputationControlParam;
        _convergenceCriteriaParam = new FITKRadiossSolverConvergenceCriteriaParam;
        _globalSPHParam = new FITKRadiossSolverSPHGlobal;
        _globalSPHParam->enable(false);
    }

    FITKRadiossSolverSettings::~FITKRadiossSolverSettings()
    {
        if (_runExeTimeParam) delete _runExeTimeParam;
        if (_globalTimeParam) delete _globalTimeParam;
        if (_nodalTimeStepControlParam) delete _nodalTimeStepControlParam;
        if (_elementalTimeStepControlParam) delete _elementalTimeStepControlParam;
        if (_adaptiveDynamicRelaxation) delete _adaptiveDynamicRelaxation;
        if (_AMSElementalTimeStepControlParam) delete _AMSElementalTimeStepControlParam;
        if (_computationControlParam) delete _computationControlParam;
        if (_convergenceCriteriaParam) delete _convergenceCriteriaParam;
        if (_globalSPHParam) delete _globalSPHParam;
    }

    FITKRadiossSolverRunExeTimeParam * FITKRadiossSolverSettings::getRunExeTimeParam()
    {
        return _runExeTimeParam;
    }

    FITKRadiossSolverGlobalTimeParam * FITKRadiossSolverSettings::getGlobalTimeParam()
    {
        return _globalTimeParam;
    }

    FITKRadiossSolverNodalTimeStepControlParam * FITKRadiossSolverSettings::getNodalTimeStepControlParam()
    {
        return _nodalTimeStepControlParam;
    }

    FITKRadiossSolverElementalTimeStepControlParam * FITKRadiossSolverSettings::getElementalTimeStepControlParam()
    {
        return _elementalTimeStepControlParam;
    }

    FITKRadiossSolverAdaptiveDynamicRelaxation * FITKRadiossSolverSettings::getAdaptiveDynamicRelaxation()
    {
        return _adaptiveDynamicRelaxation;
    }

    FITKRadiossSolverAMSElementalTimeStepControlParam * FITKRadiossSolverSettings::getAMSElementalTimeStepControlParam()
    {
        return _AMSElementalTimeStepControlParam;
    }

    FITKRadiossSolverComputationControlParam * FITKRadiossSolverSettings::getComputationControlParam()
    {
        return _computationControlParam;
    }

    FITKRadiossSolverConvergenceCriteriaParam * FITKRadiossSolverSettings::getConvergenceCriteriaParam()
    {
        return _convergenceCriteriaParam;
    }

    FITKRadiossSolverSPHGlobal * FITKRadiossSolverSettings::getGlobalSPHParam()
    {
        return _globalSPHParam;
    }

    void FITKRadiossSolverSettings::clearParam()
    {
        if (_convergenceCriteriaParam)
            _convergenceCriteriaParam->setEnableState(false);
        if (_globalSPHParam)
            _globalSPHParam->enable(false);
    }

    void FITKRadiossSolverSPHGlobal::setAsort(const double value)
    {
        m_asort = value;
    }

    double FITKRadiossSolverSPHGlobal::getAsort() const
    {
        return m_asort;
    }

    void FITKRadiossSolverSPHGlobal::setMaxsph(const int value)
    {
        m_Maxsph = value;
    }

    int FITKRadiossSolverSPHGlobal::getMaxsph() const
    {
        return m_Maxsph;
    }

    void FITKRadiossSolverSPHGlobal::setLneigh(const int value)
    {
        m_Lneigh = value;
    }

    int FITKRadiossSolverSPHGlobal::getLneigh() const
    {
        return m_Lneigh;
    }

    void FITKRadiossSolverSPHGlobal::setNneigh(const int value)
    {
        m_Nneigh = value;
    }

    int FITKRadiossSolverSPHGlobal::getNneigh() const
    {
        return m_Nneigh;
    }

    void FITKRadiossSolverSPHGlobal::setIsol2sph(const int value)
    {
        m_Isol2sph = value;
    }

    int FITKRadiossSolverSPHGlobal::getIsol2sph() const
    {
        return m_Isol2sph;
    }

} // namespace Radioss


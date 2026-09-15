/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataSolverSettingsAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"

namespace Interface
{
    bool FITKRadiossDataSolverSettingsAdaptor::adaptR()
    {
        _settingData = dynamic_cast<Radioss::FITKRadiossSolverSettings*>(_dataObj);
        if (_reader == nullptr || _settingData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        Radioss::FITKRadiossSolverRunExeTimeParam* runExeTimeParam = _settingData->getRunExeTimeParam();
        if (!runExeTimeParam)return false;
        Radioss::FITKRadiossSolverGlobalTimeParam* globalTimeParam = _settingData->getGlobalTimeParam();
        if (!globalTimeParam)return false;
        Radioss::FITKRadiossSolverNodalTimeStepControlParam* nodalTimeStepParam = _settingData->getNodalTimeStepControlParam();
        if (!nodalTimeStepParam)return false;
        Radioss::FITKRadiossSolverElementalTimeStepControlParam* elementalTimeStepParam = _settingData->getElementalTimeStepControlParam();
        if (!elementalTimeStepParam)return false;
        Radioss::FITKRadiossSolverAdaptiveDynamicRelaxation* adaptiveRelaxation = _settingData->getAdaptiveDynamicRelaxation();
        if (!adaptiveRelaxation)return false;
        Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* amsTimeStepParam = _settingData->getAMSElementalTimeStepControlParam();
        if (!amsTimeStepParam)return false;
        Radioss::FITKRadiossSolverComputationControlParam* computationParam = _settingData->getComputationControlParam();
        if (!computationParam)return false;
        Radioss::FITKRadiossSolverConvergenceCriteriaParam* convergenceParam = _settingData->getConvergenceCriteriaParam();
        if (!convergenceParam)return false;
        double doubleValue = 0.0;
        int intValue = 0;
        bool boolValue = false;
        std::string strValue;
        // 读取运行时间参数
        if (!h5tools->readGroupDouble(*_h5Group, "RunExeTimeParam_FinalTime", &doubleValue)) return false;
        runExeTimeParam->setFinalTime(doubleValue);
        // 读取全局时间参数
        if (!h5tools->readGroupDouble(*_h5Group, "GlobalTimeParam_ScaleFactor", &doubleValue)) return false;
        globalTimeParam->setScaleFactor(doubleValue);
        if (!h5tools->readGroupBool(*_h5Group, "GlobalTimeParam_EnableAdditionalCards", &boolValue)) return false;
        globalTimeParam->setEnableAdditionalCards(boolValue);
        if (!h5tools->readGroupDouble(*_h5Group, "GlobalTimeParam_ToleranceGTS", &doubleValue)) return false;
        globalTimeParam->setToleranceGTS(doubleValue);
        if (!h5tools->readGroupint(*_h5Group, "GlobalTimeParam_NumCycles", &intValue)) return false;
        globalTimeParam->setNumCycles(intValue);
        // 读取节点时间步长控制参数
        if (!h5tools->readGroupStr(*_h5Group, "NodalTimeStep_ControlType", strValue)) return false;
        Core::FITKEnumTransfer<Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType> nodalTimeTypeTransfer;
        bool isValid = false;
        Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType nodalControlType =
            nodalTimeTypeTransfer.fromString(QString::fromStdString(strValue), isValid);
        if (!isValid) return false;
        nodalTimeStepParam->setTimeStepControlType(nodalControlType);
        if (!h5tools->readGroupDouble(*_h5Group, "NodalTimeStep_MinTimeStep", &doubleValue)) return false;
        nodalTimeStepParam->setMinimumTimeStep(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "NodalTimeStep_ScaleFactor", &doubleValue)) return false;
        nodalTimeStepParam->setScaleFactor(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "NodalTimeStep_InitAddMassRatio", &doubleValue)) return false;
        nodalTimeStepParam->setInitialAddMassRatio(doubleValue);
        // 读取单元时间步长控制参数
        if (!h5tools->readGroupStr(*_h5Group, "ElementalTimeStep_ControlType", strValue)) return false;
        Core::FITKEnumTransfer<Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType> elementalTypeTransfer;
        Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType elementalControlType =
            elementalTypeTransfer.fromString(QString::fromStdString(strValue), isValid);
        if (!isValid) return false;
        elementalTimeStepParam->setTimeStepControlType(elementalControlType);
        if (!h5tools->readGroupDouble(*_h5Group, "ElementalTimeStep_MinTimeStep", &doubleValue)) return false;
        elementalTimeStepParam->setMinimumTimeStep(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "ElementalTimeStep_ScaleFactor", &doubleValue)) return false;
        elementalTimeStepParam->setScaleFactor(doubleValue);
        if (!h5tools->readGroupBool(*_h5Group, "ElementalTimeStep_AddCards", &boolValue)) return false;
        elementalTimeStepParam->setNumberOfAddCards(boolValue);
        if (!h5tools->readGroupDouble(*_h5Group, "ElementalTimeStep_MinAspectRatio", &doubleValue)) return false;
        elementalTimeStepParam->setMinAspectRatio(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "ElementalTimeStep_MinVolumeRatio", &doubleValue)) return false;
        elementalTimeStepParam->setMinVolumeRatio(doubleValue);
        // 读取自适应动态松弛参数
        if (!h5tools->readGroupBool(*_h5Group, "AdaptiveRelaxation_Enable", &boolValue)) return false;
        adaptiveRelaxation->setEnableDynamicRelaxation(boolValue);
        if (!h5tools->readGroupBool(*_h5Group, "AdaptiveRelaxation_EnableStartTime", &boolValue)) return false;
        adaptiveRelaxation->setEnableStartTime(boolValue);
        if (!h5tools->readGroupDouble(*_h5Group, "AdaptiveRelaxation_StartTime", &doubleValue)) return false;
        adaptiveRelaxation->setStartTime(doubleValue);
        if (!h5tools->readGroupBool(*_h5Group, "AdaptiveRelaxation_EnableStopTime", &boolValue)) return false;
        adaptiveRelaxation->setEnableStopTime(boolValue);
        if (!h5tools->readGroupDouble(*_h5Group, "AdaptiveRelaxation_StopTime", &doubleValue)) return false;
        adaptiveRelaxation->setStopTime(doubleValue);
        // 读取AMS单元时间步长控制参数
        if (!h5tools->readGroupBool(*_h5Group, "AMSTimeStep_EnableCriticalScale", &boolValue)) return false;
        amsTimeStepParam->setEnableCriticalTimeStepScaleFactor(boolValue);
        if (!h5tools->readGroupDouble(*_h5Group, "AMSTimeStep_CriticalScale", &doubleValue)) return false;
        amsTimeStepParam->setCriticalTimeStepScaleFactor(doubleValue);
        if (!h5tools->readGroupBool(*_h5Group, "AMSTimeStep_EnableMinTimeStep", &boolValue)) return false;
        amsTimeStepParam->setEnableMinTimeStep(boolValue);
        if (!h5tools->readGroupDouble(*_h5Group, "AMSTimeStep_MinTimeStep", &doubleValue)) return false;
        amsTimeStepParam->setMinTimeStep(doubleValue);
        if (!h5tools->readGroupBool(*_h5Group, "AMSTimeStep_EnableConvergenceTol", &boolValue)) return false;
        amsTimeStepParam->setEnableAMSConvergenceTolerance(boolValue);
        if (!h5tools->readGroupDouble(*_h5Group, "AMSTimeStep_ConvergenceTol", &doubleValue)) return false;
        amsTimeStepParam->setAMSConvergenceTolerance(doubleValue);
        if (!h5tools->readGroupBool(*_h5Group, "AMSTimeStep_EnableMaxIterations", &boolValue)) return false;
        amsTimeStepParam->setEnableMaxIterationsConjugateGradient(boolValue);
        if (!h5tools->readGroupDouble(*_h5Group, "AMSTimeStep_MaxIterations", &doubleValue)) return false;
        amsTimeStepParam->setMaxIterationsConjugateGradient(doubleValue);
        if (!h5tools->readGroupBool(*_h5Group, "AMSTimeStep_EnableAdditionalOutputs", &boolValue)) return false;
        amsTimeStepParam->setEnableNumberOfAdditionalOutputs(boolValue);
        if (!h5tools->readGroupDouble(*_h5Group, "AMSTimeStep_AdditionalOutputs", &doubleValue)) return false;
        amsTimeStepParam->setNumberOfAdditionalOutputs(doubleValue);
        // 读取计算控制参数
        if (!h5tools->readGroupBool(*_h5Group, "Computation_StopOnNegativeVolume", &boolValue)) return false;
        computationParam->setStopComputationWhenNegativeVolume(boolValue);
        if (!h5tools->readGroupBool(*_h5Group, "Computation_ParallelArithmetic", &boolValue)) return false;
        computationParam->setParallelArithmetic(boolValue);
        // 读取收敛标准参数
        if (!h5tools->readGroupBool(*_h5Group, "Convergence_StopComputation", &boolValue)) return false;
        convergenceParam->setEnableState(boolValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Convergence_EnergyErrorRatio", &doubleValue)) return false;
        convergenceParam->setEnergyErrorRatio(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Convergence_TotalMassRatio", &doubleValue)) return false;
        convergenceParam->setTotalMassRatio(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Convergence_NodalMassRatio", &doubleValue)) return false;
        convergenceParam->setNodalMassRatio(doubleValue);
        if (!h5tools->readGroupBool(*_h5Group, "Convergence_WriteTimeHistory", &boolValue)) return false;
        convergenceParam->setWriteTimeHistoryFile(boolValue);
        if (!h5tools->readGroupBool(*_h5Group, "Convergence_WriteAnimation", &boolValue)) return false;
        convergenceParam->setWriteAnimationFile(boolValue);
        if (!h5tools->readGroupBool(*_h5Group, "Convergence_EnergyError", &boolValue)) return false;
        convergenceParam->setEnergyError(boolValue);
        // 读取全局SPH参数
        Radioss::FITKRadiossSolverSPHGlobal* globalSPHParam = _settingData->getGlobalSPHParam();
        if (!globalSPHParam) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "SPHGlobal_Asort", &doubleValue)) return false;
        globalSPHParam->setAsort(doubleValue);
        if (!h5tools->readGroupint(*_h5Group, "SPHGlobal_Maxsph", &intValue)) return false;
        globalSPHParam->setMaxsph(intValue);
        if (!h5tools->readGroupint(*_h5Group, "SPHGlobal_Lneigh", &intValue)) return false;
        globalSPHParam->setLneigh(intValue);
        if (!h5tools->readGroupint(*_h5Group, "SPHGlobal_Nneigh", &intValue)) return false;
        globalSPHParam->setNneigh(intValue);
        if (!h5tools->readGroupint(*_h5Group, "SPHGlobal_Isol2sph", &intValue)) return false;
        globalSPHParam->setIsol2sph(intValue);
        return true;
    }

    bool FITKRadiossDataSolverSettingsAdaptor::adaptW()
    {
        _settingData = dynamic_cast<Radioss::FITKRadiossSolverSettings*>(_dataObj);
        if (_writer == nullptr || _settingData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //获取数据
        // 写入运行时间参数
        Radioss::FITKRadiossSolverRunExeTimeParam* runExeTimeParam = _settingData->getRunExeTimeParam();
        if (!runExeTimeParam)return false;
        h5tools->writeGroupAttrDouble(*_h5Group, "RunExeTimeParam_FinalTime", runExeTimeParam->getFinalTime());
        // 写入全局时间参数
        Radioss::FITKRadiossSolverGlobalTimeParam* globalTimeParam = _settingData->getGlobalTimeParam();
        if (!globalTimeParam)return false;
        h5tools->writeGroupAttrDouble(*_h5Group, "GlobalTimeParam_ScaleFactor", globalTimeParam->getScaleFactor());
        h5tools->writeGroupAttrBool(*_h5Group, "GlobalTimeParam_EnableAdditionalCards", globalTimeParam->isEnableAdditionalCards());
        h5tools->writeGroupAttrDouble(*_h5Group, "GlobalTimeParam_ToleranceGTS", globalTimeParam->getToleranceGTS());
        h5tools->writeGroupAttrInt(*_h5Group, "GlobalTimeParam_NumCycles", globalTimeParam->getNumCycles());
        // 写入节点时间步长控制参数
        Radioss::FITKRadiossSolverNodalTimeStepControlParam* nodalTimeStepParam = _settingData->getNodalTimeStepControlParam();
        if (!nodalTimeStepParam)return false;
        Core::FITKEnumTransfer<Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType> nodalTimeTypeTransfer;
        bool isValid = false;
        QString nodalTimeTypeStr = nodalTimeTypeTransfer.toString(nodalTimeStepParam->getTimeStepControlType(), isValid);
        if (!isValid) return false;
        h5tools->writeGroupAttrStr(*_h5Group, "NodalTimeStep_ControlType", nodalTimeTypeStr.toStdString());
        h5tools->writeGroupAttrDouble(*_h5Group, "NodalTimeStep_MinTimeStep", nodalTimeStepParam->getMinimumTimeStep());
        h5tools->writeGroupAttrDouble(*_h5Group, "NodalTimeStep_ScaleFactor", nodalTimeStepParam->getScaleFactor());
        h5tools->writeGroupAttrDouble(*_h5Group, "NodalTimeStep_InitAddMassRatio", nodalTimeStepParam->getInitialAddMassRatio());
        // 写入单元时间步长控制参数
        Radioss::FITKRadiossSolverElementalTimeStepControlParam* elementalTimeStepParam = _settingData->getElementalTimeStepControlParam();
        if (!elementalTimeStepParam)return false;
        Core::FITKEnumTransfer<Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType> elementalTypeTransfer;
        QString elementalTypeStr = elementalTypeTransfer.toString(elementalTimeStepParam->getTimeStepControlType(), isValid);
        if (!isValid) return false;
        h5tools->writeGroupAttrStr(*_h5Group, "ElementalTimeStep_ControlType", elementalTypeStr.toStdString());
        h5tools->writeGroupAttrDouble(*_h5Group, "ElementalTimeStep_MinTimeStep", elementalTimeStepParam->getMinimumTimeStep());
        h5tools->writeGroupAttrDouble(*_h5Group, "ElementalTimeStep_ScaleFactor", elementalTimeStepParam->getScaleFactor());
        h5tools->writeGroupAttrBool(*_h5Group, "ElementalTimeStep_AddCards", elementalTimeStepParam->getNumberOfAddCards());
        h5tools->writeGroupAttrDouble(*_h5Group, "ElementalTimeStep_MinAspectRatio", elementalTimeStepParam->getMinAspectRatio());
        h5tools->writeGroupAttrDouble(*_h5Group, "ElementalTimeStep_MinVolumeRatio", elementalTimeStepParam->getMinVolumeRatio());
        // 写入自适应动态松弛参数
        Radioss::FITKRadiossSolverAdaptiveDynamicRelaxation* adaptiveRelaxation = _settingData->getAdaptiveDynamicRelaxation();
        if (!adaptiveRelaxation)return false;
        h5tools->writeGroupAttrBool(*_h5Group, "AdaptiveRelaxation_Enable", adaptiveRelaxation->getEnableDynamicRelaxation());
        h5tools->writeGroupAttrBool(*_h5Group, "AdaptiveRelaxation_EnableStartTime", adaptiveRelaxation->isEnableStartTime());
        h5tools->writeGroupAttrDouble(*_h5Group, "AdaptiveRelaxation_StartTime", adaptiveRelaxation->getStartTime());
        h5tools->writeGroupAttrBool(*_h5Group, "AdaptiveRelaxation_EnableStopTime", adaptiveRelaxation->isEnableStopTime());
        h5tools->writeGroupAttrDouble(*_h5Group, "AdaptiveRelaxation_StopTime", adaptiveRelaxation->getStopTime());
        // 写入AMS单元时间步长控制参数
        Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* amsTimeStepParam = _settingData->getAMSElementalTimeStepControlParam();
        if (!amsTimeStepParam)return false;
        h5tools->writeGroupAttrBool(*_h5Group, "AMSTimeStep_EnableCriticalScale", amsTimeStepParam->isEnableCriticalTimeStepScaleFactor());
        h5tools->writeGroupAttrDouble(*_h5Group, "AMSTimeStep_CriticalScale", amsTimeStepParam->getCriticalTimeStepScaleFactor());
        h5tools->writeGroupAttrBool(*_h5Group, "AMSTimeStep_EnableMinTimeStep", amsTimeStepParam->isEnableMinTimeStep());
        h5tools->writeGroupAttrDouble(*_h5Group, "AMSTimeStep_MinTimeStep", amsTimeStepParam->getMinTimeStep());
        h5tools->writeGroupAttrBool(*_h5Group, "AMSTimeStep_EnableConvergenceTol", amsTimeStepParam->isEnableAMSConvergenceTolerance());
        h5tools->writeGroupAttrDouble(*_h5Group, "AMSTimeStep_ConvergenceTol", amsTimeStepParam->getAMSConvergenceTolerance());
        h5tools->writeGroupAttrBool(*_h5Group, "AMSTimeStep_EnableMaxIterations", amsTimeStepParam->isEnableMaxIterationsConjugateGradient());
        h5tools->writeGroupAttrDouble(*_h5Group, "AMSTimeStep_MaxIterations", amsTimeStepParam->getMaxIterationsConjugateGradient());
        h5tools->writeGroupAttrBool(*_h5Group, "AMSTimeStep_EnableAdditionalOutputs", amsTimeStepParam->isEnableNumberOfAdditionalOutputs());
        h5tools->writeGroupAttrDouble(*_h5Group, "AMSTimeStep_AdditionalOutputs", amsTimeStepParam->getNumberOfAdditionalOutputs());
        // 写入计算控制参数
        Radioss::FITKRadiossSolverComputationControlParam* computationParam = _settingData->getComputationControlParam();
        if (!computationParam)return false;
        h5tools->writeGroupAttrBool(*_h5Group, "Computation_StopOnNegativeVolume", computationParam->getStopComputationWhenNegativeVolume());
        h5tools->writeGroupAttrBool(*_h5Group, "Computation_ParallelArithmetic", computationParam->getParallelArithmetic());
        // 写入收敛标准参数
        Radioss::FITKRadiossSolverConvergenceCriteriaParam* convergenceParam = _settingData->getConvergenceCriteriaParam();
        if (!convergenceParam)return false;
        h5tools->writeGroupAttrBool(*_h5Group, "Convergence_StopComputation", convergenceParam->getEnableState());
        h5tools->writeGroupAttrDouble(*_h5Group, "Convergence_EnergyErrorRatio", convergenceParam->getEnergyErrorRatio());
        h5tools->writeGroupAttrDouble(*_h5Group, "Convergence_TotalMassRatio", convergenceParam->getTotalMassRatio());
        h5tools->writeGroupAttrDouble(*_h5Group, "Convergence_NodalMassRatio", convergenceParam->getNodalMassRatio());
        h5tools->writeGroupAttrBool(*_h5Group, "Convergence_WriteTimeHistory", convergenceParam->getWriteTimeHistoryFile());
        h5tools->writeGroupAttrBool(*_h5Group, "Convergence_WriteAnimation", convergenceParam->getWriteAnimationFile());
        h5tools->writeGroupAttrBool(*_h5Group, "Convergence_EnergyError", convergenceParam->getEnergyError());
        // 全局SPH参数
        Radioss::FITKRadiossSolverSPHGlobal* globalSPHParam = _settingData->getGlobalSPHParam();
        if (!globalSPHParam)return false;
        h5tools->writeGroupAttrDouble(*_h5Group, "SPHGlobal_Asort", globalSPHParam->getAsort());
        h5tools->writeGroupAttrInt(*_h5Group, "SPHGlobal_Maxsph", globalSPHParam->getMaxsph());
        h5tools->writeGroupAttrInt(*_h5Group, "SPHGlobal_Lneigh", globalSPHParam->getLneigh());
        h5tools->writeGroupAttrInt(*_h5Group, "SPHGlobal_Nneigh", globalSPHParam->getNneigh());
        h5tools->writeGroupAttrInt(*_h5Group, "SPHGlobal_Isol2sph", globalSPHParam->getIsol2sph());
        return true;
    }
}

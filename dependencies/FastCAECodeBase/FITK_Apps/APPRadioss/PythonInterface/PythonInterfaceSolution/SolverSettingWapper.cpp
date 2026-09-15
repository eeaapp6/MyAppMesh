/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SolverSettingWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace
{
    QString sendMessageToServer(const QString& message, bool isToServer = true)
    {
        if (isToServer)
        {
            HttpPython::FITKHttpCommons::sendMessageToServer(message);
        }
        return message;
    }
}

SolverSetting* SolverSettingWapper::new_SolverSetting()
{
    //创建SolverSetting实例
    return new SolverSetting();
}

void SolverSettingWapper::delete_SolverSetting(SolverSetting* obj)
{
    //释放SolverSetting实例
    if (obj != nullptr)
    {
        delete obj;
    }
}

QString SolverSettingWapper::setRunExeTime(SolverSetting* obj, double time)
{
    //设置运行执行时间
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));

    Radioss::FITKRadiossSolverRunExeTimeParam* param = solverSettings->getRunExeTimeParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Run Exe Time failed!");
        return sendMessageToServer(QString("Failed to set run execution time"));
    }
    param->setFinalTime(time);
    return sendMessageToServer(QString("Run execution time set successfully"));
}

QString SolverSettingWapper::setScaleFactor(SolverSetting* obj, double factor)
{
    //设置时间停止的缩放因子
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverGlobalTimeParam* param = solverSettings->getGlobalTimeParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Scale Factor failed!");
        return sendMessageToServer(QString("Failed to set scale factor"));
    }
    param->setScaleFactor(factor);
    return sendMessageToServer(QString("Scale factor set successfully"));
}

QString SolverSettingWapper::setEnableAdditionalCards(SolverSetting* obj, bool state)
{
    //设置启用用户输入GTS收敛的容差与周期数
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverGlobalTimeParam* param = solverSettings->getGlobalTimeParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Enable Additional Cards failed!");
        return sendMessageToServer(QString("Failed to set enable additional cards"));
    }
    param->setEnableAdditionalCards(state);
    return sendMessageToServer(QString("Enable additional cards set successfully"));
}

QString SolverSettingWapper::setToleranceGTS(SolverSetting* obj, double v)
{
    //设置GTS收敛的容差
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverGlobalTimeParam* param = solverSettings->getGlobalTimeParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Tolerance GTS failed!");
        return sendMessageToServer(QString("Failed to set tolerance GTS"));
    }
    param->setToleranceGTS(v);
    return sendMessageToServer(QString("Tolerance GTS set successfully"));
}

QString SolverSettingWapper::setNumCycles(SolverSetting* obj, int cycles)
{
    //设置周期数
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverGlobalTimeParam* param = solverSettings->getGlobalTimeParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Num Cycles failed!");
        return sendMessageToServer(QString("Failed to set num cycles"));
    }
    param->setNumCycles(cycles);
    return sendMessageToServer(QString("Num cycles set successfully"));
}

QString SolverSettingWapper::setNodalTimeStepControlType(SolverSetting* obj, int type)
{
    //设置节点时间步控制类型
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverNodalTimeStepControlParam* param = solverSettings->getNodalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Nodal Time Step Control Type failed!");
        return sendMessageToServer(QString("Failed to set nodal time step control type"));
    }
    param->setTimeStepControlType(static_cast<Radioss::FITKRadiossSolverNodalTimeStepControlParam::NodalTimeStepControlType>(type));
    return sendMessageToServer(QString("Nodal time step control type set successfully"));
}

QString SolverSettingWapper::setNodalMinimumTimeStep(SolverSetting* obj, double time)
{
    //设置节点最小时间步长
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverNodalTimeStepControlParam* param = solverSettings->getNodalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Nodal Minimum Time Step failed!");
        return sendMessageToServer(QString("Failed to set nodal minimum time step"));
    }
    param->setMinimumTimeStep(time);
    return sendMessageToServer(QString("Nodal minimum time step set successfully"));
}

QString SolverSettingWapper::setNodalScaleFactor(SolverSetting* obj, double s)
{
    //设置节点缩放因子
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverNodalTimeStepControlParam* param = solverSettings->getNodalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Nodal Scale Factor failed!");
        return sendMessageToServer(QString("Failed to set nodal scale factor"));
    }
    param->setScaleFactor(s);
    return sendMessageToServer(QString("Nodal scale factor set successfully"));
}

QString SolverSettingWapper::setInitialAddMassRatio(SolverSetting* obj, double r)
{
    //设置初始添加质量比
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverNodalTimeStepControlParam* param = solverSettings->getNodalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Initial Add Mass Ratio failed!");
        return sendMessageToServer(QString("Failed to set initial add mass ratio"));
    }
    param->setInitialAddMassRatio(r);
    return sendMessageToServer(QString("Initial add mass ratio set successfully"));
}

QString SolverSettingWapper::setElementalTimeStepControlType(SolverSetting* obj, int type)
{
    //设置单元时间步控制类型
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverElementalTimeStepControlParam* param = solverSettings->getElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Elemental Time Step Control Type failed!");
        return sendMessageToServer(QString("Failed to set elemental time step control type"));
    }
    param->setTimeStepControlType(static_cast<Radioss::FITKRadiossSolverElementalTimeStepControlParam::ElementalTimeStepControlType>(type));
    return sendMessageToServer(QString("Elemental time step control type set successfully"));
}

QString SolverSettingWapper::setElementalMinimumTimeStep(SolverSetting* obj, double time)
{
    //设置单元最小时间步长
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverElementalTimeStepControlParam* param = solverSettings->getElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Elemental Minimum Time Step failed!");
        return sendMessageToServer(QString("Failed to set elemental minimum time step"));
    }
    param->setMinimumTimeStep(time);
    return sendMessageToServer(QString("Elemental minimum time step set successfully"));
}

QString SolverSettingWapper::setElementalScaleFactor(SolverSetting* obj, double s)
{
    //设置单元缩放因子
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverElementalTimeStepControlParam* param = solverSettings->getElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Elemental Scale Factor failed!");
        return sendMessageToServer(QString("Failed to set elemental scale factor"));
    }
    param->setScaleFactor(s);
    return sendMessageToServer(QString("Elemental scale factor set successfully"));
}

QString SolverSettingWapper::setNumberOfAddCards(SolverSetting* obj, bool addCards)
{
    //设置GTS收敛的容差与周期数
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverElementalTimeStepControlParam* param = solverSettings->getElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Number Of Add Cards failed!");
        return sendMessageToServer(QString("Failed to set number of add cards"));
    }
    param->setNumberOfAddCards(addCards);
    return sendMessageToServer(QString("Number of add cards set successfully"));
}

QString SolverSettingWapper::setMinAspectRatio(SolverSetting* obj, double value)
{
    //设置最小长宽比
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverElementalTimeStepControlParam* param = solverSettings->getElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Min Aspect Ratio failed!");
        return sendMessageToServer(QString("Failed to set minimum aspect ratio"));
    }
    param->setMinAspectRatio(value);
    return sendMessageToServer(QString("Minimum aspect ratio set successfully"));
}

QString SolverSettingWapper::setMinVolumeRatio(SolverSetting* obj, double value)
{
    //设置最小体积比
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverElementalTimeStepControlParam* param = solverSettings->getElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Min Volume Ratio failed!");
        return sendMessageToServer(QString("Failed to set minimum volume ratio"));
    }
    param->setMinVolumeRatio(value);
    return sendMessageToServer(QString("Minimum volume ratio set successfully"));
}

QString SolverSettingWapper::setEnableDynamicRelaxation(SolverSetting* obj, bool isEnable)
{
    //设置启用动态松弛
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAdaptiveDynamicRelaxation* param = solverSettings->getAdaptiveDynamicRelaxation();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Enable Dynamic Relaxation failed!");
        return sendMessageToServer(QString("Failed to set enable dynamic relaxation"));
    }
    param->setEnableDynamicRelaxation(isEnable);
    return sendMessageToServer(QString("Enable dynamic relaxation set successfully"));
}

QString SolverSettingWapper::setEnableStartTime(SolverSetting* obj, bool isEnable)
{
    //设置启用每次迭代开始时间
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAdaptiveDynamicRelaxation* param = solverSettings->getAdaptiveDynamicRelaxation();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Enable Start Time failed!");
        return sendMessageToServer(QString("Failed to set enable start time"));
    }
    param->setEnableStartTime(isEnable);
    return sendMessageToServer(QString("Enable start time set successfully"));
}

QString SolverSettingWapper::setStartTime(SolverSetting* obj, double time)
{
    //设置每次迭代开始时间
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAdaptiveDynamicRelaxation* param = solverSettings->getAdaptiveDynamicRelaxation();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Start Time failed!");
        return sendMessageToServer(QString("Failed to set start time"));
    }
    param->setStartTime(time);
    return sendMessageToServer(QString("Start time set successfully"));
}

QString SolverSettingWapper::setEnableStopTime(SolverSetting* obj, bool isEnable)
{
    //设置启用每次迭代停止时间
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAdaptiveDynamicRelaxation* param = solverSettings->getAdaptiveDynamicRelaxation();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Enable Stop Time failed!");
        return sendMessageToServer(QString("Failed to set enable stop time"));
    }
    param->setEnableStopTime(isEnable);
    return sendMessageToServer(QString("Enable stop time set successfully"));
}

QString SolverSettingWapper::setStopTime(SolverSetting* obj, double time)
{
    //设置每次迭代停止时间
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAdaptiveDynamicRelaxation* param = solverSettings->getAdaptiveDynamicRelaxation();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Stop Time failed!");
        return sendMessageToServer(QString("Failed to set stop time"));
    }
    param->setStopTime(time);
    return sendMessageToServer(QString("Stop time set successfully"));
}

QString SolverSettingWapper::setEnableCriticalTimeStepScaleFactor(SolverSetting* obj, bool isEnable)
{
    //设置启用Critical Time Step Scale Factor
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* param = solverSettings->getAMSElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Enable Critical Time Step Scale Factor failed!");
        return sendMessageToServer(QString("Failed to set enable critical time step scale factor"));
    }
    param->setEnableCriticalTimeStepScaleFactor(isEnable);
    return sendMessageToServer(QString("Enable critical time step scale factor set successfully"));
}

QString SolverSettingWapper::setCriticalTimeStepScaleFactor(SolverSetting* obj, double value)
{
    //设置Critical Time Step Scale Factor
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* param = solverSettings->getAMSElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Critical Time Step Scale Factor failed!");
        return sendMessageToServer(QString("Failed to set critical time step scale factor"));
    }
    param->setCriticalTimeStepScaleFactor(value);
    return sendMessageToServer(QString("Critical time step scale factor set successfully"));
}

QString SolverSettingWapper::setEnableMinTimeStep(SolverSetting* obj, bool isEnable)
{
    //设置启用最小时间步长
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* param = solverSettings->getAMSElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Enable Min Time Step failed!");
        return sendMessageToServer(QString("Failed to set enable min time step"));
    }
    param->setEnableMinTimeStep(isEnable);
    return sendMessageToServer(QString("Enable min time step set successfully"));
}

QString SolverSettingWapper::setAMSMinTimeStep(SolverSetting* obj, double value)
{
    //设置AMS最小时间步长
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* param = solverSettings->getAMSElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set AMS Min Time Step failed!");
        return sendMessageToServer(QString("Failed to set AMS min time step"));
    }
    param->setMinTimeStep(value);
    return sendMessageToServer(QString("AMS min time step set successfully"));
}

QString SolverSettingWapper::setEnableAMSConvergenceTolerance(SolverSetting* obj, bool isEnable)
{
    //设置启用AMS收敛的容差
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* param = solverSettings->getAMSElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Enable AMS Convergence Tolerance failed!");
        return sendMessageToServer(QString("Failed to set enable AMS convergence tolerance"));
    }
    param->setEnableAMSConvergenceTolerance(isEnable);
    return sendMessageToServer(QString("Enable AMS convergence tolerance set successfully"));
}

QString SolverSettingWapper::setAMSConvergenceTolerance(SolverSetting* obj, double value)
{
    //设置AMS收敛的容差
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* param = solverSettings->getAMSElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set AMS Convergence Tolerance failed!");
        return sendMessageToServer(QString("Failed to set AMS convergence tolerance"));
    }
    param->setAMSConvergenceTolerance(value);
    return sendMessageToServer(QString("AMS convergence tolerance set successfully"));
}

QString SolverSettingWapper::setEnableMaxIterationsConjugateGradient(SolverSetting* obj, bool isEnable)
{
    //设置启用最大迭代次数共轭梯度
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* param = solverSettings->getAMSElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Enable Max Iterations Conjugate Gradient failed!");
        return sendMessageToServer(QString("Failed to set enable max iterations conjugate gradient"));
    }
    param->setEnableMaxIterationsConjugateGradient(isEnable);
    return sendMessageToServer(QString("Enable max iterations conjugate gradient set successfully"));
}

QString SolverSettingWapper::setMaxIterationsConjugateGradient(SolverSetting* obj, double value)
{
    //设置最大迭代次数共轭梯度
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* param = solverSettings->getAMSElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Max Iterations Conjugate Gradient failed!");
        return sendMessageToServer(QString("Failed to set max iterations conjugate gradient"));
    }
    param->setMaxIterationsConjugateGradient(value);
    return sendMessageToServer(QString("Max iterations conjugate gradient set successfully"));
}

QString SolverSettingWapper::setEnableNumberOfAdditionalOutputs(SolverSetting* obj, bool isEnable)
{
    //设置启用额外输出的数量
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* param = solverSettings->getAMSElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Enable Number Of Additional Outputs failed!");
        return sendMessageToServer(QString("Failed to set enable number of additional outputs"));
    }
    param->setEnableNumberOfAdditionalOutputs(isEnable);
    return sendMessageToServer(QString("Enable number of additional outputs set successfully"));
}

QString SolverSettingWapper::setNumberOfAdditionalOutputs(SolverSetting* obj, double value)
{
    //设置额外输出的数量
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverAMSElementalTimeStepControlParam* param = solverSettings->getAMSElementalTimeStepControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Number Of Additional Outputs failed!");
        return sendMessageToServer(QString("Failed to set number of additional outputs"));
    }
    param->setNumberOfAdditionalOutputs(value);
    return sendMessageToServer(QString("Number of additional outputs set successfully"));
}

QString SolverSettingWapper::setStopComputationWhenNegativeVolume(SolverSetting* obj, bool isStop)
{
    //设置当出现负体积时停止计算
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverComputationControlParam* param = solverSettings->getComputationControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Stop Computation When Negative Volume failed!");
        return sendMessageToServer(QString("Failed to set stop computation when negative volume"));
    }
    param->setStopComputationWhenNegativeVolume(isStop);
    return sendMessageToServer(QString("Stop computation when negative volume set successfully"));
}

QString SolverSettingWapper::setParallelArithmetic(SolverSetting* obj, bool isParallel)
{
    //设置并行算法
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverComputationControlParam* param = solverSettings->getComputationControlParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Parallel Arithmetic failed!");
        return sendMessageToServer(QString("Failed to set parallel arithmetic"));
    }
    param->setParallelArithmetic(isParallel);
    return sendMessageToServer(QString("Parallel arithmetic set successfully"));
}

QString SolverSettingWapper::setConvergenceEnableState(SolverSetting* obj, bool state)
{
    //设置收敛标准启用状态
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverConvergenceCriteriaParam* param = solverSettings->getConvergenceCriteriaParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Convergence Enable State failed!");
        return sendMessageToServer(QString("Failed to set convergence enable state"));
    }
    param->setEnableState(state);
    return sendMessageToServer(QString("Convergence enable state set successfully"));
}

QString SolverSettingWapper::setEnergyErrorRatio(SolverSetting* obj, double value)
{
    //设置能量误差比
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverConvergenceCriteriaParam* param = solverSettings->getConvergenceCriteriaParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Energy Error Ratio failed!");
        return sendMessageToServer(QString("Failed to set energy error ratio"));
    }
    param->setEnergyErrorRatio(value);
    return sendMessageToServer(QString("Energy error ratio set successfully"));
}

QString SolverSettingWapper::setTotalMassRatio(SolverSetting* obj, double value)
{
    //设置总质量比
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverConvergenceCriteriaParam* param = solverSettings->getConvergenceCriteriaParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Total Mass Ratio failed!");
        return sendMessageToServer(QString("Failed to set total mass ratio"));
    }
    param->setTotalMassRatio(value);
    return sendMessageToServer(QString("Total mass ratio set successfully"));
}

QString SolverSettingWapper::setNodalMassRatio(SolverSetting* obj, double value)
{
    //设置节点质量比
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverConvergenceCriteriaParam* param = solverSettings->getConvergenceCriteriaParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Nodal Mass Ratio failed!");
        return sendMessageToServer(QString("Failed to set nodal mass ratio"));
    }
    param->setNodalMassRatio(value);
    return sendMessageToServer(QString("Nodal mass ratio set successfully"));
}

QString SolverSettingWapper::setWriteTimeHistoryFile(SolverSetting* obj, bool state)
{
    //设置写入时间历史文件
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverConvergenceCriteriaParam* param = solverSettings->getConvergenceCriteriaParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Write Time History File failed!");
        return sendMessageToServer(QString("Failed to set write time history file"));
    }
    param->setWriteTimeHistoryFile(state);
    return sendMessageToServer(QString("Write time history file set successfully"));
}

QString SolverSettingWapper::setWriteAnimationFile(SolverSetting* obj, bool state)
{
    //设置写入动画文件
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverConvergenceCriteriaParam* param = solverSettings->getConvergenceCriteriaParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Write Animation File failed!");
        return sendMessageToServer(QString("Failed to set write animation file"));
    }
    param->setWriteAnimationFile(state);
    return sendMessageToServer(QString("Write animation file set successfully"));
}

QString SolverSettingWapper::setEnergyError(SolverSetting* obj, bool state)
{
    //设置能量误差
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverConvergenceCriteriaParam* param = solverSettings->getConvergenceCriteriaParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Energy Error failed!");
        return sendMessageToServer(QString("Failed to set energy error"));
    }
    param->setEnergyError(state);
    return sendMessageToServer(QString("Energy error set successfully"));
}

QString SolverSettingWapper::setGlobalSPHEnableState(SolverSetting* obj, bool state)
{
    //设置全局SPH参数启用状态
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverSPHGlobal* param = solverSettings->getGlobalSPHParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set Global SPH Enable State failed!");
        return sendMessageToServer(QString("Failed to set global SPH enable state"));
    }
    param->enable(state);
    return sendMessageToServer(QString("Global SPH enable state set successfully"));
}

QString SolverSettingWapper::setSPHAsort(SolverSetting* obj, double value)
{
    //设置附近粒子的安全系数
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverSPHGlobal* param = solverSettings->getGlobalSPHParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set SPH Asort failed!");
        return sendMessageToServer(QString("Failed to set SPH asort"));
    }
    param->setAsort(value);
    return sendMessageToServer(QString("SPH asort set successfully"));
}

QString SolverSettingWapper::setSPHMaxsph(SolverSetting* obj, int value)
{
    //设置幽灵粒子的最大数量
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverSPHGlobal* param = solverSettings->getGlobalSPHParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set SPH Maxsph failed!");
        return sendMessageToServer(QString("Failed to set SPH maxsph"));
    }
    param->setMaxsph(value);
    return sendMessageToServer(QString("SPH maxsph set successfully"));
}

QString SolverSettingWapper::setSPHLneigh(SolverSetting* obj, int value)
{
    //设置邻居粒子参数SPH计算的最大数量
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverSPHGlobal* param = solverSettings->getGlobalSPHParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set SPH Lneigh failed!");
        return sendMessageToServer(QString("Failed to set SPH lneigh"));
    }
    param->setLneigh(value);
    return sendMessageToServer(QString("SPH lneigh set successfully"));
}

QString SolverSettingWapper::setSPHNneigh(SolverSetting* obj, int value)
{
    //设置邻居粒子的最大数量
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverSPHGlobal* param = solverSettings->getGlobalSPHParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set SPH Nneigh failed!");
        return sendMessageToServer(QString("Failed to set SPH nneigh"));
    }
    param->setNneigh(value);
    return sendMessageToServer(QString("SPH nneigh set successfully"));
}

QString SolverSettingWapper::setSPHIsol2sph(SolverSetting* obj, int value)
{
    //设置粒子活化类型
    if (obj == nullptr) return sendMessageToServer(QString("Invalid SolverSetting object"));
    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr) return sendMessageToServer(QString("Invalid solver settings"));
    Radioss::FITKRadiossSolverSPHGlobal* param = solverSettings->getGlobalSPHParam();
    if (param == nullptr)
    {
        AppFrame::FITKMessageError("Set SPH Isol2sph failed!");
        return sendMessageToServer(QString("Failed to set SPH isol2sph"));
    }
    param->setIsol2sph(value);
    return sendMessageToServer(QString("SPH isol2sph set successfully"));
}

QString SolverSettingWapper::clearSolverSettingParam(SolverSetting* obj)
{
    //清理求解器设置参数
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("SolverSetting object is null!");
        return sendMessageToServer(QString("Clear SolverSetting Param failed!"));
    }

    Radioss::FITKRadiossSolverSettings* solverSettings = obj->getSolverSettings();
    if (solverSettings == nullptr)
    {
        AppFrame::FITKMessageError("Clear SolverSetting Param failed!");
        return sendMessageToServer(QString("Clear SolverSetting Param failed!"));
    }

    solverSettings->clearParam();
    return sendMessageToServer(QString("Clear SolverSetting Param succeeded!"));
}


Radioss::FITKRadiossSolverSettings* SolverSetting::getSolverSettings()
{
    //获取求解器设置
    Radioss::FITKRadiossCase* radiossCase =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (radiossCase == nullptr)
    {
        return nullptr;
    }

    return radiossCase->getSolverSettings();
}


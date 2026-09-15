/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   SolverSettingWapper.h
 * @brief  求解器设置相关的python接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-15
 *********************************************************************/
#ifndef _SOLVER_SETTING_WAPPER_H___
#define _SOLVER_SETTING_WAPPER_H___

#include <QObject>
#include <QString>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss {
    class FITKRadiossSolverSettings;
}

/**
 * @brief  求解器设置接口封装，供装饰器装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-15
 */
class SolverSetting
{
public:
    /**
     * @brief    默认构造函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    explicit SolverSetting() = default;
    /**
     * @brief    析构函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    virtual ~SolverSetting() = default;

    /**
     * @brief    获取求解器设置
     * @return   FITKRadiossSolverSettings指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    Radioss::FITKRadiossSolverSettings* getSolverSettings();
};

/**
 * @brief  求解器设置相关PYTHON接口装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-15
 */
class PythonInterfaceSolutionAPI SolverSettingWapper : public QObject
{
    Q_OBJECT
public slots:
    /**
     * @brief    创建SolverSetting实例
     * @return   SolverSetting指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    SolverSetting* new_SolverSetting();

    /**
     * @brief    释放SolverSetting实例
     * @param[i] obj SolverSetting对象指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    void delete_SolverSetting(SolverSetting* obj);

    /**
     * @brief    设置运行最终时间
     * @param[i] obj SolverSetting对象指针
     * @param[i] time 最终时间
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setRunExeTime(SolverSetting* obj, double time);

    /**
     * @brief    设置缩放因子
     * @param[i] obj SolverSetting对象指针
     * @param[i] factor 缩放因子
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setScaleFactor(SolverSetting* obj, double factor);

    /**
     * @brief    设置启用附加卡片
     * @param[i] obj SolverSetting对象指针
     * @param[i] state 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnableAdditionalCards(SolverSetting* obj, bool state);

    /**
     * @brief    设置GTS容差
     * @param[i] obj SolverSetting对象指针
     * @param[i] v 容差值
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setToleranceGTS(SolverSetting* obj, double v);

    /**
     * @brief    设置循环次数
     * @param[i] obj SolverSetting对象指针
     * @param[i] cycles 循环次数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setNumCycles(SolverSetting* obj, int cycles);

    /**
     * @brief    设置节点时间步控制类型
     * @param[i] obj SolverSetting对象指针
     * @param[i] type 控制类型
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setNodalTimeStepControlType(SolverSetting* obj, int type);

    /**
     * @brief    设置节点最小时间步
     * @param[i] obj SolverSetting对象指针
     * @param[i] time 最小时间步
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setNodalMinimumTimeStep(SolverSetting* obj, double time);

    /**
     * @brief    设置节点缩放因子
     * @param[i] obj SolverSetting对象指针
     * @param[i] s 缩放因子
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setNodalScaleFactor(SolverSetting* obj, double s);

    /**
     * @brief    设置初始附加质量比
     * @param[i] obj SolverSetting对象指针
     * @param[i] r 质量比
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setInitialAddMassRatio(SolverSetting* obj, double r);

    /**
     * @brief    设置单元时间步控制类型
     * @param[i] obj SolverSetting对象指针
     * @param[i] type 控制类型
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setElementalTimeStepControlType(SolverSetting* obj, int type);

    /**
     * @brief    设置单元最小时间步
     * @param[i] obj SolverSetting对象指针
     * @param[i] time 最小时间步
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setElementalMinimumTimeStep(SolverSetting* obj, double time);

    /**
     * @brief    设置单元缩放因子
     * @param[i] obj SolverSetting对象指针
     * @param[i] s 缩放因子
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setElementalScaleFactor(SolverSetting* obj, double s);

    /**
     * @brief    设置附加卡片数量
     * @param[i] obj SolverSetting对象指针
     * @param[i] addCards 是否添加卡片
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setNumberOfAddCards(SolverSetting* obj, bool addCards);

    /**
     * @brief    设置最小长宽比
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 长宽比
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setMinAspectRatio(SolverSetting* obj, double value);

    /**
     * @brief    设置最小体积比
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 体积比
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setMinVolumeRatio(SolverSetting* obj, double value);

    /**
     * @brief    设置启用动态松弛
     * @param[i] obj SolverSetting对象指针
     * @param[i] isEnable 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnableDynamicRelaxation(SolverSetting* obj, bool isEnable);

    /**
     * @brief    设置启用开始时间
     * @param[i] obj SolverSetting对象指针
     * @param[i] isEnable 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnableStartTime(SolverSetting* obj, bool isEnable);

    /**
     * @brief    设置开始时间
     * @param[i] obj SolverSetting对象指针
     * @param[i] time 开始时间
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setStartTime(SolverSetting* obj, double time);

    /**
     * @brief    设置启用停止时间
     * @param[i] obj SolverSetting对象指针
     * @param[i] isEnable 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnableStopTime(SolverSetting* obj, bool isEnable);

    /**
     * @brief    设置停止时间
     * @param[i] obj SolverSetting对象指针
     * @param[i] time 停止时间
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setStopTime(SolverSetting* obj, double time);

    /**
     * @brief    设置启用临界时间步缩放因子
     * @param[i] obj SolverSetting对象指针
     * @param[i] isEnable 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnableCriticalTimeStepScaleFactor(SolverSetting* obj, bool isEnable);

    /**
     * @brief    设置临界时间步缩放因子
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 缩放因子
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setCriticalTimeStepScaleFactor(SolverSetting* obj, double value);

    /**
     * @brief    设置启用最小时间步
     * @param[i] obj SolverSetting对象指针
     * @param[i] isEnable 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnableMinTimeStep(SolverSetting* obj, bool isEnable);

    /**
     * @brief    设置AMS最小时间步
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 最小时间步
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setAMSMinTimeStep(SolverSetting* obj, double value);

    /**
     * @brief    设置启用AMS收敛容差
     * @param[i] obj SolverSetting对象指针
     * @param[i] isEnable 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnableAMSConvergenceTolerance(SolverSetting* obj, bool isEnable);

    /**
     * @brief    设置AMS收敛容差
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 容差值
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setAMSConvergenceTolerance(SolverSetting* obj, double value);

    /**
     * @brief    设置启用共轭梯度最大迭代次数
     * @param[i] obj SolverSetting对象指针
     * @param[i] isEnable 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnableMaxIterationsConjugateGradient(SolverSetting* obj, bool isEnable);

    /**
     * @brief    设置共轭梯度最大迭代次数
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 迭代次数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setMaxIterationsConjugateGradient(SolverSetting* obj, double value);

    /**
     * @brief    设置启用附加输出数量
     * @param[i] obj SolverSetting对象指针
     * @param[i] isEnable 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnableNumberOfAdditionalOutputs(SolverSetting* obj, bool isEnable);

    /**
     * @brief    设置附加输出数量
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 输出数量
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setNumberOfAdditionalOutputs(SolverSetting* obj, double value);

    /**
     * @brief    设置负体积时停止计算
     * @param[i] obj SolverSetting对象指针
     * @param[i] isStop 是否停止
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setStopComputationWhenNegativeVolume(SolverSetting* obj, bool isStop);

    /**
     * @brief    设置并行运算
     * @param[i] obj SolverSetting对象指针
     * @param[i] isParallel 是否并行
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setParallelArithmetic(SolverSetting* obj, bool isParallel);

    /**
     * @brief    设置收敛启用状态
     * @param[i] obj SolverSetting对象指针
     * @param[i] state 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setConvergenceEnableState(SolverSetting* obj, bool state);

    /**
     * @brief    设置能量误差比
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 误差比
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnergyErrorRatio(SolverSetting* obj, double value);

    /**
     * @brief    设置总质量比
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 质量比
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setTotalMassRatio(SolverSetting* obj, double value);

    /**
     * @brief    设置节点质量比
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 质量比
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setNodalMassRatio(SolverSetting* obj, double value);

    /**
     * @brief    设置写入时间历史文件
     * @param[i] obj SolverSetting对象指针
     * @param[i] state 是否写入
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setWriteTimeHistoryFile(SolverSetting* obj, bool state);

    /**
     * @brief    设置写入动画文件
     * @param[i] obj SolverSetting对象指针
     * @param[i] state 是否写入
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setWriteAnimationFile(SolverSetting* obj, bool state);

    /**
     * @brief    设置能量误差
     * @param[i] obj SolverSetting对象指针
     * @param[i] state 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setEnergyError(SolverSetting* obj, bool state);

    /**
     * @brief    设置全局SPH参数启用状态
     * @param[i] obj SolverSetting对象指针
     * @param[i] state 是否启用
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-07-14
     */
    QString setGlobalSPHEnableState(SolverSetting* obj, bool state);

    /**
     * @brief    设置附近粒子的安全系数
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 安全系数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-07-14
     */
    QString setSPHAsort(SolverSetting* obj, double value);

    /**
     * @brief    设置幽灵粒子的最大数量
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 最大数量
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-07-14
     */
    QString setSPHMaxsph(SolverSetting* obj, int value);

    /**
     * @brief    设置邻居粒子参数SPH计算的最大数量
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 最大数量
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-07-14
     */
    QString setSPHLneigh(SolverSetting* obj, int value);

    /**
     * @brief    设置邻居粒子的最大数量
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 最大数量
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-07-14
     */
    QString setSPHNneigh(SolverSetting* obj, int value);

    /**
     * @brief    设置粒子活化类型
     * @param[i] obj SolverSetting对象指针
     * @param[i] value 活化类型
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-07-14
     */
    QString setSPHIsol2sph(SolverSetting* obj, int value);

    /**
     * @brief    清理求解器设置参数
     * @param[i] obj SolverSetting对象指针
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-07-14
     */
    QString clearSolverSettingParam(SolverSetting* obj);

private:
};

#endif // _SOLVER_SETTING_WAPPER_H___
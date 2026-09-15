/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossSolverSettings.h
 * @brief  Radioss 求解设置
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-05
 *********************************************************************/
#ifndef __FITKRADIOSSSOLVERSETTINGS_H__
#define __FITKRADIOSSSOLVERSETTINGS_H__

#include "FITKRadiossDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKAbstractRadiossData.h"

namespace Radioss
{
    /**
     * @brief  求解运行时间参数
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class FITKRadiossDataAPI FITKRadiossSolverRunExeTimeParam : public Core::FITKAbstractDataObject
    {
    public:
        explicit FITKRadiossSolverRunExeTimeParam() = default;
        /**
         * @brief    设置运行最终时间
         * @param[i] time 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setFinalTime(const double time);
        /**
         * @brief    获取运行最终时间
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        double getFinalTime() const;

    private:
        /**
         * @brief  最终时间
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-05
         */
        double _finalTime{ 0.001 };
    };
    /**
     * @brief  求解全局时间参数
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class FITKRadiossDataAPI FITKRadiossSolverGlobalTimeParam : public Core::FITKAbstractDataObject
    {
    public:
        explicit FITKRadiossSolverGlobalTimeParam() = default;

        /**
         * @brief    设置时间停止的缩放因子
         * @param[i] s
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setScaleFactor(const double s);
        /**
         * @brief    获取时间停止的缩放因子
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        double getScaleFactor() const;
        /**
         * @brief    设置启用用户输入GTS收敛的容差与周期数
         * @param[i] state
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setEnableAdditionalCards(const bool state);
        /**
         * @brief    是否启用用户输入GTS收敛的容差与周期数
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        bool isEnableAdditionalCards() const;
        /**
         * @brief    设置GTS收敛的容差
         * @param[i] v
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setToleranceGTS(const double v);
        /**
         * @brief    获取GTS收敛的容差
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        double getToleranceGTS() const;
        /**
         * @brief    设置周期数
         * @param[i] n
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setNumCycles(const int n);
        /**
         * @brief    获取周期数
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        int getNumCycles() const;

    private:
        /**
         * @brief  时间停止的缩放因子
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        double _scaleFactor{ 0.9 };
        /**
         * @brief  是否用户输入GTS收敛的容差与周期数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        bool _isAdditionalCards{ false };
        /**
         * @brief  对GTS收敛的容差
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        double _toleranceGTS{ 10e-3 };
        /**
         * @brief  周期数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        int _numCycles{ 20 };
    };
    /**
     * @brief  求解节点时间步长控制参数
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class FITKRadiossDataAPI FITKRadiossSolverNodalTimeStepControlParam : public Core::FITKAbstractDataObject
    {
        Q_OBJECT
    public:
        /**
         * @brief  时间步控制类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        enum NodalTimeStepControlType
        {
            NodalTimeStep_SolverDefault = 0,
            NodalTimeStep_CST = 1,
            NodalTimeStep_SET = 2,
            NodalTimeStep_STOP = 3,
        };
        Q_ENUM(NodalTimeStepControlType);

    public:
        explicit FITKRadiossSolverNodalTimeStepControlParam() = default;

        void setTimeStepControlType(const NodalTimeStepControlType type);
        NodalTimeStepControlType getTimeStepControlType() const;

        void setMinimumTimeStep(const double time);
        double getMinimumTimeStep() const;

        void setScaleFactor(const double s);
        double getScaleFactor() const;

        void setInitialAddMassRatio(const double r);
        double getInitialAddMassRatio() const;

    private:
        NodalTimeStepControlType _controlType{ NodalTimeStepControlType::NodalTimeStep_SolverDefault };
        double _minTimeStep{ 0 };
        double _scaleFactor{ 0 };
        double _initAddMassRatio{ 0 };
    };
    /**
     * @brief  求解单元时间步长控制参数
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class FITKRadiossDataAPI FITKRadiossSolverElementalTimeStepControlParam : public Core::FITKAbstractDataObject
    {
        Q_OBJECT
    public:
        /**
         * @brief  时间步控制类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        enum ElementalTimeStepControlType
        {
            ElementalTimeStep_SolverDefault = 0,
            ElementalTimeStep_CST = 1,
            ElementalTimeStep_DEL = 2,
            ElementalTimeStep_STOP = 3,
        };
        Q_ENUM(ElementalTimeStepControlType);

    public:
        explicit FITKRadiossSolverElementalTimeStepControlParam() = default;
        
        void setTimeStepControlType(const ElementalTimeStepControlType type);
        ElementalTimeStepControlType getTimeStepControlType() const;

        void setMinimumTimeStep(const double time);
        double getMinimumTimeStep() const;

        void setScaleFactor(const double s);
        double getScaleFactor() const;

        void setNumberOfAddCards(const bool addCards);
        bool getNumberOfAddCards() const;

        void setMinAspectRatio(const double value);
        double getMinAspectRatio() const;

        void setMinVolumeRatio(const double value);
        double getMinVolumeRatio() const;

    private:
        ElementalTimeStepControlType _controlType{ ElementalTimeStepControlType::ElementalTimeStep_SolverDefault };
        double _minTimeStep{ 0 };
        double _scaleFactor{ 0 };
        bool _numberOfAddCards{ false };
        double _minAspectRatio{ 0.2 };
        double _minVolumeRatio{ 0.05 };
    };
    /**
     * @brief  求解自适应动态松弛
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class FITKRadiossDataAPI FITKRadiossSolverAdaptiveDynamicRelaxation : public Core::FITKAbstractDataObject
    {
    public:
        explicit FITKRadiossSolverAdaptiveDynamicRelaxation() = default;
        
        void setEnableDynamicRelaxation(const bool isEnable);
        bool getEnableDynamicRelaxation() const;

        void setEnableStartTime(const bool isEnable);
        bool isEnableStartTime() const;

        void setStartTime(const double time);
        double getStartTime() const;

        void setEnableStopTime(const bool isEnable);
        bool isEnableStopTime() const;

        void setStopTime(const double time);
        double getStopTime() const;

    private:
        bool _enableDynamicRelaxation{ false };
        bool _enableStartTime{ false };
        double _startTime{ 0.0 };
        bool _enableStopTime{ false };
        double _stopTime{ 0.0 };
    };
    /**
     * @brief  求解AMS单元时间步长控制参数
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class FITKRadiossDataAPI FITKRadiossSolverAMSElementalTimeStepControlParam : public Core::FITKAbstractDataObject
    {
    public:
        explicit FITKRadiossSolverAMSElementalTimeStepControlParam() = default;

        void setEnableCriticalTimeStepScaleFactor(const bool isEnable);
        bool isEnableCriticalTimeStepScaleFactor() const;

        void setCriticalTimeStepScaleFactor(const double value);
        double getCriticalTimeStepScaleFactor() const;

        void setEnableMinTimeStep(const bool isEnable);
        bool isEnableMinTimeStep() const;

        void setMinTimeStep(const double value);
        double getMinTimeStep() const;

        void setEnableAMSConvergenceTolerance(const bool isEnable);
        bool isEnableAMSConvergenceTolerance() const;

        void setAMSConvergenceTolerance(const double value);
        double getAMSConvergenceTolerance() const;

        void setEnableMaxIterationsConjugateGradient(const bool isEnable);
        bool isEnableMaxIterationsConjugateGradient() const;

        void setMaxIterationsConjugateGradient(const double value);
        double getMaxIterationsConjugateGradient() const;

        void setEnableNumberOfAdditionalOutputs(const bool isEnable);
        bool isEnableNumberOfAdditionalOutputs() const;

        void setNumberOfAdditionalOutputs(const double value);
        double getNumberOfAdditionalOutputs() const;

    private:
        bool _isEnableCriticalTimeStepScaleFactor{ false };
        double _criticalTimeStepScaleFactor{ 0.9 };
        bool _isEnableMinTimeStep{ false };
        double _minTimeStep{ 0.0 };
        bool _isEnableAMSConvergenceTolerance{ false };
        double _AMSConvergenceTolerance{ 0.0001 };
        bool _isEnableMaxIterationsConjugateGradient{ false };
        double _maxIterationsConjugateGradient{ 1000 };
        bool _isEnableNumberOfAdditionalOutputs{ false };
        double _numberOfAdditionalOutputs{ 0.0 };
    };

    /**
     * @brief  求解计算控制参数
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class FITKRadiossDataAPI FITKRadiossSolverComputationControlParam : public Core::FITKAbstractDataObject
    {
    public:
        explicit FITKRadiossSolverComputationControlParam() = default;
        
        void setStopComputationWhenNegativeVolume(const bool isStop);
        bool getStopComputationWhenNegativeVolume() const;

        void setParallelArithmetic(const bool isParallel);
        bool getParallelArithmetic() const;

    private:
        bool _stopComputationWhenNegativeVolume{ true };
        bool _parallelArithmetic{ true };
    };
    /**
     * @brief  求解收敛标准参数
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class FITKRadiossDataAPI FITKRadiossSolverConvergenceCriteriaParam : public Core::FITKAbstractDataObject
    {
    public:
        explicit FITKRadiossSolverConvergenceCriteriaParam() = default;

        void setEnableState(const bool state);
        bool getEnableState() const;

        void setEnergyErrorRatio(const double value);
        double getEnergyErrorRatio() const;

        void setTotalMassRatio(const double value);
        double getTotalMassRatio() const;

        void setNodalMassRatio(const double value);
        double getNodalMassRatio() const;

        void setWriteTimeHistoryFile(const bool state);
        bool getWriteTimeHistoryFile() const;

        void setWriteAnimationFile(const bool state);
        bool getWriteAnimationFile() const;

        void setEnergyError(const bool state);
        bool getEnergyError() const;

    private:
        bool _enableState{ false };
        double _energyErrorRatio{ 0.99 };
        double _totalMassRatio{ 0.05 };
        double _nodalMassRatio{ 0.0 };
        bool _writeTimeHistoryFile{ true };
        bool _writeAnimationFile{ true };
        bool _energyError{ true };
    };
    /**
     * @brief  全局SPH参数设置
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-10-29
     */
    class FITKRadiossDataAPI FITKRadiossSolverSPHGlobal : public Core::FITKAbstractDataObject
    {
        FITKCLASS(Radioss, FITKRadiossSolverSPHGlobal); //!< FITK类标识
        RadiossKeyWord(FITKRadiossSolverSPHGlobal, /SPHGLO)
    public:
        explicit FITKRadiossSolverSPHGlobal() = default;
        /**
         * @brief    设置附近粒子的安全系数
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        void setAsort(const double value);
        /**
         * @brief    获取附近粒子的安全系数
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        double getAsort() const;
        /**
         * @brief    设置幽灵粒子的最大数量
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        void setMaxsph(const int value);
        /**
         * @brief    获取幽灵粒子的最大数量
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        int getMaxsph() const;
        /**
         * @brief    设置邻居粒子参数SPH计算的最大数量
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        void setLneigh(const int value);
        /**
         * @brief    获取邻居粒子参数SPH计算的最大数量
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        int getLneigh() const;
        /**
         * @brief    设置邻居粒子的最大数量
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        void setNneigh(const int value);
        /**
         * @brief    获取邻居粒子的最大数量
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        int getNneigh() const;
        /**
         * @brief    设置粒子活化类型
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        void setIsol2sph(const int value);
        /**
         * @brief    获取粒子活化类型
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        int getIsol2sph() const;

    private:
        /**
         * @brief  Security coefficient on search for neighbors, so that for each particle more than the actual neighbors are found.
         *         找到附近粒子的安全系数0.25
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-29
         */
        double m_asort{ 0.25 };
        /**
         * @brief  Maximum number of ghost particles allowed to be created at one time.
         *         幽灵粒子的最大数量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-29
         */
        int m_Maxsph{ 0 };
        /**
         * @brief  	Maximum number of neighbors to be taken into account for the SPH approximation.
         *          邻居粒子参数SPH计算的最大数量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-29
         */
        int m_Lneigh{ 120 };
        /**
         * @brief  	Maximum number of neighbors
         *          邻居粒子的最大数量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-29
         */
        int m_Nneigh{ 0 };
        /**
         * @brief  	Particle activation process method for Sol2SPH
         *          粒子活化类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-29
         */
        int m_Isol2sph{ 0 };
    };
    /**
     * @brief  Radioss 求解设置
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-05
     */
    class FITKRadiossDataAPI FITKRadiossSolverSettings : public Core::FITKAbstractDataObject
    {
        FITKCLASS(Radioss, FITKRadiossSolverSettings);
    public:
        /**
         * @brief 构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-05
         */
        explicit FITKRadiossSolverSettings();
        
        /**
         * @brief 析构函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-05
         */
        virtual ~FITKRadiossSolverSettings();
        /**
         * @brief    获取运行时时间参数
         * @return   FITKRadiossSolverRunExeTimeParam *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        FITKRadiossSolverRunExeTimeParam* getRunExeTimeParam();
        /**
         * @brief    获取全局时间参数
         * @return   FITKRadiossSolverGlobalTimeParam *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        FITKRadiossSolverGlobalTimeParam* getGlobalTimeParam();
        /**
         * @brief    获取节点时间步长控制参数
         * @return   FITKRadiossSolverNodalTimeStepControlParam *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        FITKRadiossSolverNodalTimeStepControlParam* getNodalTimeStepControlParam();
        /**
         * @brief    获取单元时间步长控制参数
         * @return   FITKRadiossSolverElementalTimeStepControlParam *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        FITKRadiossSolverElementalTimeStepControlParam* getElementalTimeStepControlParam();
        /**
         * @brief    获取自适应动态松弛
         * @return   FITKRadiossSolverAdaptiveDynamicRelaxation *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        FITKRadiossSolverAdaptiveDynamicRelaxation* getAdaptiveDynamicRelaxation();
        /**
         * @brief    获取AMS单元时间步长控制参数
         * @return   FITKRadiossSolverAMSElementalTimeStepControlParam *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        FITKRadiossSolverAMSElementalTimeStepControlParam* getAMSElementalTimeStepControlParam();
        /**
         * @brief    获取计算控制参数
         * @return   FITKRadiossSolverComputationControlParam *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        FITKRadiossSolverComputationControlParam* getComputationControlParam();
        /**
         * @brief    获取收敛标准参数
         * @return   FITKRadiossSolverConvergenceCriteriaParam *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        FITKRadiossSolverConvergenceCriteriaParam* getConvergenceCriteriaParam();
        /**
         * @brief    获取全局SPH参数
         * @return   FITKRadiossSolverSPHGlobal *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        FITKRadiossSolverSPHGlobal* getGlobalSPHParam();
        /**
         * @brief    清理参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        void clearParam();

    private:
        FITKRadiossSolverRunExeTimeParam* _runExeTimeParam{ nullptr };
        FITKRadiossSolverGlobalTimeParam* _globalTimeParam{ nullptr };
        FITKRadiossSolverNodalTimeStepControlParam* _nodalTimeStepControlParam{ nullptr };
        FITKRadiossSolverElementalTimeStepControlParam* _elementalTimeStepControlParam{ nullptr };
        FITKRadiossSolverAdaptiveDynamicRelaxation* _adaptiveDynamicRelaxation{ nullptr };
        FITKRadiossSolverAMSElementalTimeStepControlParam* _AMSElementalTimeStepControlParam{ nullptr };
        FITKRadiossSolverComputationControlParam* _computationControlParam{ nullptr };
        FITKRadiossSolverConvergenceCriteriaParam* _convergenceCriteriaParam{ nullptr };
        FITKRadiossSolverSPHGlobal* _globalSPHParam{ nullptr };

    };


} // namespace Radioss

#endif // __FITKRADIOSSSOLVERSETTINGS_H__
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file   FITKAbaAbstractStep.h
 * @brief  分析步抽象类、分析步类型枚举类等
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-26
 *
 */
#ifndef FITKABAABSTRACTSTEP_H
#define FITKABAABSTRACTSTEP_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"


namespace Interface {

    /**
     * @brief  分析步抽象类。定义分析步描述、时间、几何非线性三个属性，时间和几何非线性的接口可由
     * 具体分析步重写。如模态分析步时间固定为0.0，显式动力学分析步默认打开几何非线性
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-26
     */
    class FITKInterfaceStructuralAPI FITKAbaAbstractStep : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
    public:
        /**
        * @brief  分析步类型枚举类
        * @author LiJin (1003680421@qq.com)
        * @date   2024-03-26
        */
        enum FITKAbaStepType
        {
            Init,
            Buckle,
            Frequency,
            DynamicExplicit,
            GeneralStatic,
            DynamicTempDispExplicit,
            CoupledTempDisplacement,
        };
        Q_ENUM(FITKAbaStepType);
        /**
         * @brief  时间增量步类型，包括自动、固定
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        enum TimeIncrementType
        {
            Automatic,
            Fixed
        };
        Q_ENUM(TimeIncrementType)
            /**
             * @brief  稳定增量评估枚举类
             * @author LiJin (1003680421@qq.com)
             * @date   2024-03-26
             */
            enum StableIncrementEstimator
        {
            Global,
            ElementByElement
        };
        Q_ENUM(StableIncrementEstimator);

        enum IncrementSizeSelection
        {
            UserDefinedTimeIncrement,
            UseElementByElementTimeIncrementEstimator,
        };
        Q_ENUM(IncrementSizeSelection);
        /**
         * @brief  最大时间增量枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        enum MaxTimeIncrement
        {
            Unlimited,
            ValueMaxTime,
        };
        Q_ENUM(MaxTimeIncrement);
        /**
         * @brief  质量缩放枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        enum MassScaling
        {
            FromPreviousStep,
            Definitions
        };
        Q_ENUM(MassScaling);
        /**
         * @brief  特征值求解枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        enum Eigensolver
        {
            Lanczos,
            Subspace,
            AMS
        };
        Q_ENUM(Eigensolver);
        /**
         * @brief  特征值个数请求枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        enum NumberOfEigenvaluesRequsted
        {
            AllFrequencyRange,
            ValueNumberOfEigen,
        };
        Q_ENUM(NumberOfEigenvaluesRequsted);
        /**
         * @brief  声固耦合应用
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        enum AcousticStructuralCouplingWhereApplicable
        {
            Include,
            Exclude,
            Project
        };
        Q_ENUM(AcousticStructuralCouplingWhereApplicable);
        /**
         * @brief  块个数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        enum BlockSize
        {
            DefaultBlock,
            ValueBlock,
        };
        Q_ENUM(BlockSize);
        /**
         * @brief  BlockLanczox分析步最大个数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        enum MaximumNumberOfBlockLanczosStep
        {
            DefaultMaximum,
            ValueMaximum,
        };
        Q_ENUM(MaximumNumberOfBlockLanczosStep);
        /**
         * @brief  矩阵存储
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        enum MatrixStorage
        {
            UseSolverDefault,
            Unsymmetric,
            Symmetric
        };
        Q_ENUM(MatrixStorage);
        /**
         * @brief  特征向量正规化
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        enum NormalizeEnginevectors
        {
            Mass,
            Displacement,
        };
        Q_ENUM(NormalizeEnginevectors);
        /**
         * @brief  响应类型
         * @author libaojun
         * @date   2024-10-31
         */
        enum ResponseType
        {
            SteadyState,
            Transient,
        };
        Q_ENUM(ResponseType);
        /**
         * @brief  自动稳定化方法
         * @author libaojun
         * @date   2024-10-31
         */
        enum AutomaticStabilizationMethod
        {
            None,
            SpecifyDissipatedEnergyFraction,
            SpecifyDampingFraction,
            UseDampingFactorsFromPreviousStep,
        };
        Q_ENUM(AutomaticStabilizationMethod);
        /**
         * @brief  蠕变/膨胀/粘弹性积分
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        enum CreepSwellingViscoelasticIntegration
        {
            ExplicitOrImplicit,
            Explicit
        };
        Q_ENUM(CreepSwellingViscoelasticIntegration);

        /**
         * @brief  求解技术方法
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        enum SolutionTechniqueMethod {
            FullNewton,
            Separated
        };
        Q_ENUM(SolutionTechniqueMethod);

        /**
         * @brief  转换严重不连续迭代
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        enum ConvertIterationsMethod {
            PropagateFromPreviousStep,
            Off,
            On
        };
        Q_ENUM(ConvertIterationsMethod);

        /**
         * @brief  默认载荷随时间变化方法
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        enum LoadVarWithTimeMethod {
            Instantaneous,
            RampLinerlyOverStep
        };
        Q_ENUM(LoadVarWithTimeMethod);


        /**
         * @brief  每次增量开始时先前状态推断方法
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        enum ExtrapolationOfPreviousStateMethod {
            NoneExtrapolation,
            Linear,
            Parabolic
        };
        Q_ENUM(ExtrapolationOfPreviousStateMethod);


    public:
        /**
         * @brief  构造函数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaAbstractStep() = default;
        /**
         * @brief  析构函数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaAbstractStep() = default;
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取分析步类型
         * @param[in]
         * @return FITKAbaStepType枚举类型值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        virtual FITKAbaStepType getStepType() const = 0;
        /**
         * @brief  是否为线性摄动分析步，便于inp输出时判断是否为Step关键字输出perturbation参数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        virtual bool isPerturbation() const;
        /**
         * @brief  获取分析步描述
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        QString getDecription() const;
        /**
         * @brief  设置分析步描述
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setDescription(const QString& description);
        /**
         * @brief  获取几何非线性状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        virtual bool getNlgeom() const;
        /**
         * @brief  设置几何非线性状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setNlgeom(bool stat);
        /**
         * @brief  获取分析步时间
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        virtual double getTime() const;
        /**
         * @brief  设置分析步时间
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setTime(double time);
        /**
         * @brief  是否激活
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        bool isActive() const;
        /**
         * @brief  设置分析步是否激活
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        void setActive(bool stat);
        /**
         * @brief  获取重启动写出频率
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        int getRestartWriteFrequency() const;
        /**
         * @brief  设置重启动写出频率
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        void setRestartWritFrequency(int frequency);
        /**
         * @brief  获取重启动写出间隔数目
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        int getRestartWriteIntervals() const;
        /**
         * @brief  设置重启动写出间隔数目
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        void setRestartWriteIntervals(int num);
        /**
         * @brief  获取重启动写出是否覆盖
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        bool getOverlayState() const;
        /**
         * @brief  设置重启动写出是否覆盖
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        void setOverlayState(bool stat);
        /**
         * @brief  获取重启动写出时间标记
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        bool getTimeMarkState() const;
        /**
         * @brief  设置重启动写出时间标记
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        void setTimeMarkState(bool stat);

    protected:
        /**
         * @brief  几何非线性状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        bool m_nlgeom{ false };
        /**
         * @brief  分析步时间
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        double m_time{ 1.0 };
        /**
         * @brief  分析步描述
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        QString m_description{};
        /**
         * @brief  分析步激活状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        bool m_active{ true };
        /**
         * @brief  重启动输出频率，每m_restartFrequency个增量步输出一次
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        int m_restartFrequency{ 0 };
        /**
         * @brief  重启动输出间隔，整个分析步的输出次数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        int m_restartNumberInterval{ 0 };
        /**
         * @brief  重启动输出是否覆盖
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        int m_restartOverlay{ false };
        /**
         * @brief
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        int m_restartTimeMarks{ false };
    };
}


#endif // FITKABAABSTRACTSTEP_H

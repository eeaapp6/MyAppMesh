/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKAbaStepCoupledTempDisp.h
 * @brief 热-位移耦合分析步
 * @author libaojun
 * @date 2025-10-31
 */
#ifndef FITKABASTEPCOUPLEDTEMPDISP_H
#define FITKABASTEPCOUPLEDTEMPDISP_H

#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractStep.h"

namespace Interface {
    /**
     * @brief  热-位移耦合分析步
     * @author libaojun
     * @date   2025-10-31
     */
    class FITKInterfaceStructuralAPI FITKAbaStepCoupledTempDisp : public FITKAbaAbstractStep
    {
        FITKCLASS(Interface, FITKAbaStepCoupledTempDisp)
    public:
        /**
         * @brief Construct a new FITKAbaStepCoupledTempDisp object
         * @author libaojun
         * @date 2025-10-31
         */
        explicit FITKAbaStepCoupledTempDisp() = default;
        /**
         * @brief Destroy the FITKAbaStepCoupledTempDisp object
         * @author libaojun
         * @date 2025-10-31
         */
        virtual ~FITKAbaStepCoupledTempDisp() = default;
        /**
         * @brief  复制数据对象,深拷贝，数据id不拷贝
         * @param[i] obj 被拷贝的对象
         * @return true
         * @return false
         * @author libaojun
         * @date 2025-10-31
         */
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取分析步类型
         * @return FITKAbaStepType枚举类型值
         * @author libaojun
         * @date   2025-10-31
         */
        FITKAbaAbstractStep::FITKAbaStepType getStepType() const override;

        /**
         * @brief 设置响应类型
         * @param type 响应类型
         * @author libaojun
         * @date 2025-10-31
         */
        void setResponseType(FITKAbaAbstractStep::ResponseType type);
        /**
         * @brief Get the Response Type object
         * @author libaojun
         * @date 2025-10-31
         * @return FITKAbaAbstractStep::ResponseType
         */
        FITKAbaAbstractStep::ResponseType getResponseType() const;
        ///**
        // * @brief Set the Time Period object
        // * @author libaojun
        // * @date 2025-10-31
        // * @param period
        // */
        //void setTimePeriod(double period);
        ///**
        // * @brief Get the Time Period object
        // * @author libaojun
        // * @date 2025-10-31
        // * @return double
        // */
        //double getTimePeriod() const;
        /**
         * @brief Set the Automatic Stabilization Method object
         * @author libaojun
         * @date 2025-10-31
         * @param method
         */
        void setAutomaticStabilizationMethod(FITKAbaAbstractStep::AutomaticStabilizationMethod method);
        /**
         * @brief Get the Automatic Stabilization Method object
         * @author libaojun
         * @date 2025-10-31
         * @return FITKAbaAbstractStep::AutomaticStabilizationMethod
         */
        FITKAbaAbstractStep::AutomaticStabilizationMethod getAutomaticStabilizationMethod() const;
        /**
         * @brief 获取阻尼系数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double getDampingFactor() const;
        /**
         * @brief 设置阻尼系数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        void setDampingFactor(double factor);
        /**
         * @brief 是否指定自适应稳定最大比
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        bool isMaxRatioOfStabilization() const;
        /**
         * @brief 是否指定自适应稳定最大比
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        void setIsMaxRatioOfStabilization(bool active);
        /**
         * @brief 获取自适应稳定最大比
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double getMaxRatioOfStabilization() const;
        /**
         * @brief 设置自适应稳定最大比
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        void setMaxRatioOfStabilization(double ratio);
        /**
         * @brief Set the Include Creep Swelling Vis Behavior object
         * @author libaojun
         * @date 2025-10-31
         * @param include
         */
        void setIncludeCreepSwellingVisBehavior(bool include);
        /**
         * @brief Get the Include Creep Swelling Vis Behavior object
         * @author libaojun
         * @date 2025-10-31
         * @return bool
         */
        bool getIncludeCreepSwellingVisBehavior() const;
        /**
         * @brief  是否设置每增量允许的最大温度变化
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        bool isMaxAllowTempChangePerIncrement() const;
        void setIsMaxAllowTempChangePerIncrement(bool active);
        /**
         * @brief Set the Max Allow Temp Change Per Increment object
         * @author libaojun
         * @date 2025-10-31
         * @param value
         */
        void setMaxAllowTempChangePerIncrement(double value);
        /**
         * @brief Get the Max Allow Temp Change Per Increment object
         * @author libaojun
         * @date 2025-10-31
         * @return double
         */
        double getMaxAllowTempChangePerIncrement() const;
        /**
         * @brief  每增量允许的最大温度变化
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        bool isCreepSwellingViscoelasticStrainErrorTolerance() const;
        void setIsCreepSwellingViscoelasticStrainErrorTolerance(bool active);
        double getCreepSwellingViscoelasticStrainErrorTolerance() const;
        void setCreepSwellingViscoelasticStrainErrorTolerance(double value);
        /**
         * @brief  蠕变/膨胀/粘弹性积分
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        CreepSwellingViscoelasticIntegration getCreepSwellingViscoelasticIntegration() const;
        void setCreepSwellingViscoelasticIntegration(CreepSwellingViscoelasticIntegration method);

        /**
         * @brief  获取时间增量步类型
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        TimeIncrementType getIncrementType() const;
        /**
         * @brief  设置时间增量步类型
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        void setIncrementType(TimeIncrementType type);
        /**
         * @brief  获取最大时间增量步数目
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        int getMaximumNumberOfIncrements() const;
        /**
         * @brief  设置最大时间增量步数目
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        void setMaximumNumberOfIncrements(int num);
        /**
         * @brief  获取初始时间增量步大小
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double getInitIncrementSize() const;
        /**
         * @brief  设置初始时间增量步大小
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        void setInitIncrementSize(double value);
        /**
         * @brief  获取最小时间增量步大小
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double getMinIncrementSize() const;
        /**
         * @brief  设置最小时间增量步大小
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        void setMinIncrementSize(double value);
        /**
         * @brief  获取最大时间增量步大小
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double getMaxIncrementSize() const;
        /**
         * @brief  设置最大时间增量步大小
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        void setMaxIncrementSize(double value);
        /**
         * @brief  获取固定时间增量步大小
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double getFixedIncrementSize() const;
        /**
         * @brief  设置固定时间增量步大小
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        void setFixedIncrementSize(double value);

        /**
         * @brief  求解器矩阵存储方式
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        MatrixStorage getMatrixStorage() const;
        void setMatrixStorage(MatrixStorage method);
        /**
         * @brief  求解技术方法
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        SolutionTechniqueMethod getSolutionTechniqueMethod() const;
        void setSolutionTechniqueMethod(SolutionTechniqueMethod method);
        /**
         * @brief  转换严重不连续迭代方式
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        ConvertIterationsMethod getConvertIterationsMethod() const;
        void setConvertIterationsMethod(ConvertIterationsMethod method);
        /**
         * @brief  默认载荷随时间变化方法
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        LoadVarWithTimeMethod getLoadVarWithTimeMethod() const;
        void setLoadVarWithTimeMethod(LoadVarWithTimeMethod method);
        /**
         * @brief  每次增量开始时先前状态推断方法
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        ExtrapolationOfPreviousStateMethod getExtrapolationOfPreviousStateMethod() const;
        void setExtrapolationOfPreviousStateMethod(ExtrapolationOfPreviousStateMethod method);

    private:
        /**
         * @brief  响应类型
         * @author libaojun
         * @date   2025-10-31
         */
        FITKAbaAbstractStep::ResponseType m_responseType{ FITKAbaAbstractStep::ResponseType::Transient };
        ///**
        // * @brief  分析步时间周期
        // * @author libaojun
        // * @date   2025-10-31
        // */
        //double m_timePeriod{ 1.0 };

        /**
         * @brief  自动稳定化方法
         * @author libaojun
         * @date   2025-10-31
         */
        FITKAbaAbstractStep::AutomaticStabilizationMethod m_autoStabMethod{ FITKAbaAbstractStep::AutomaticStabilizationMethod::None };
        /**
         * @brief 阻尼系数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double m_DampingFactor{ 0.0002 };
        /**
         * @brief 是否指定自适应稳定最大比
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        bool m_IsMaxRatioOfStabilization{ true };
        /**
         * @brief 自适应稳定最大比
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double m_MaxRatioOfStabilization{ 0.05 };
        /**
         * @brief  是否包含蠕变、膨胀和粘性行为
         * @author libaojun
         * @date   2025-10-31
         */
        bool m_includeCreepSwellingVisBehavior{ false };

        /**
         * @brief  时间增量步类型
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        TimeIncrementType m_IncrementType{ TimeIncrementType::Automatic };
        /**
         * @brief  最大时间增量步数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        int m_MaxNumOfIncrements{ 100 };
        /**
         * @brief  初始时间增量步
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double m_InitIncrementSize{ 1 };
        /**
         * @brief  最小时间增量步
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double m_MinIncrementSize{ 1e-5 };
        /**
         * @brief  最大时间增量步
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double m_MaxIncrementSize{ 1 };
        /**
         * @brief  固定时间增量步
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double m_FixedIncrementSize{ 1 };
        /**
         * @brief  是否设置每增量允许的最大温度变化
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        bool m_IsMaxAllowTempChangePerIncrement{ false };
        /**
         * @brief  每增量允许的最大温度变化
         * @author libaojun
         * @date   2025-10-31
         */
        double m_maxAllowTempChangePerIncrement{ 0. };

        /**
         * @brief  是否设置每增量允许的最大温度变化
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        bool m_IsCreepSwellingViscoelasticStrainErrorTolerance{ false };
        /**
         * @brief  每增量允许的最大温度变化
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        double m_CreepSwellingViscoelasticStrainErrorTolerance{ 0. };
        /**
         * @brief  蠕变/膨胀/粘弹性积分
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        CreepSwellingViscoelasticIntegration m_CreepSwellingViscoelasticIntegration{ CreepSwellingViscoelasticIntegration::ExplicitOrImplicit };
        /**
         * @brief  求解器矩阵存储方式
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        MatrixStorage m_MatrixStorage{ MatrixStorage::Unsymmetric };
        /**
         * @brief  求解技术方法
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        SolutionTechniqueMethod m_SolutionTechnique{ SolutionTechniqueMethod::FullNewton };
        /**
         * @brief  转换严重不连续迭代方式
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        ConvertIterationsMethod m_ConvertIterations{ ConvertIterationsMethod::PropagateFromPreviousStep };
        /**
         * @brief  默认载荷随时间变化方法
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        LoadVarWithTimeMethod m_DefaultLoadVarWithTime{ LoadVarWithTimeMethod::Instantaneous };
        /**
         * @brief  每次增量开始时先前状态推断方法
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2025-11-06
         */
        ExtrapolationOfPreviousStateMethod m_ExtrapolationOfPreviousState{ ExtrapolationOfPreviousStateMethod::Linear };

    };
}

#endif // FITKABASTEPCOUPLEDTEMPDISP_H
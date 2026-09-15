/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaStepDynamicExplicit.h
 * @brief  显式动力学分析步
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-26
 * 
 */
#ifndef FITKABASTEPDYNAMICEXPLICIT_H
#define FITKABASTEPDYNAMICEXPLICIT_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractStep.h"
#include "FITKAbaMassScalingDefinition.h"


namespace Interface {
    /**
     * @brief  显式动力学分析步类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-26
     */
    class FITKInterfaceStructuralAPI FITKAbaStepDynamicExplicit: public FITKAbaAbstractStep
    {
        FITKCLASS(Interface, FITKAbaStepDynamicExplicit);
    public:
        /**
         * @brief  显式动力学分析步类，时间默认为1.0，几何非线性默认为true
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        FITKAbaStepDynamicExplicit();
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        ~FITKAbaStepDynamicExplicit() = default;
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取分析步类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaAbstractStep::FITKAbaStepType getStepType() const override;
        /**
         * @brief  是否包括绝热效应
         * @param[in]  
         * @return bool值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        bool getIncludeAdiabaticHeatingEffects() const;
        /**
         * @brief  设置绝热效应
         * @param[in]  stat 状态
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setIncludeAdiabaticHeatingEffects(bool stat);
        /**
         * @brief  获取时间增量步类型，自动或固定
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        FITKAbaAbstractStep::TimeIncrementType getIncrementType() const;
        /**
         * @brief  设置时间增量类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setIncrementType(TimeIncrementType type);
        /**
         * @brief  获取稳定时间增量步估计类型，全局或单元by单元
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        FITKAbaAbstractStep::StableIncrementEstimator getStableIncrementEstimator() const;
        /**
         * @brief  设置稳定时间增量步估计类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setStableIncrementEstimator(FITKAbaAbstractStep::StableIncrementEstimator type);
        /**
         * @brief  获取improve Dt method状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        bool getImproveDtMethod() const;
        /**
         * @brief  设置improve Dt method状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setImproveDtMethod(bool stat);
        /**
         * @brief  获取最大时间增量步类型，无限制或通过指定值控制
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        FITKAbaAbstractStep::MaxTimeIncrement getMaxTimeIncrement() const;
        /**
         * @brief  设置最大时间增量步类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setMaxTimeIncrement(FITKAbaAbstractStep::MaxTimeIncrement type);
        /**
         * @brief  获取最大时间增量步，当通过指定值控制时有效
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        double getMaxTimeIncrementValue() const;
        /**
         * @brief  设置最大时间增量步数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setMaxTimeIncrementValue(double value);
        /**
         * @brief  获取时间缩放系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        double getTimeScalingFactor() const;
        /**
         * @brief  设置时间缩放系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setTimeScalingFactor(double factor);
        /**
         * @brief  增量步定义方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaAbstractStep::IncrementSizeSelection getIncrementSizeSelection() const;
        /**
         * @brief  设置增量步定义方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setIncrementSizeSelection(FITKAbaAbstractStep::IncrementSizeSelection type);
        /**
         * @brief  获取用户定义的时间增量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        double getUserDefinedTimeIncrement() const;
        /**
         * @brief  设置用户定义的时间增量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setUserDefinedTimeIncrement(double inc);
        /**
         * @brief  获取质量缩放类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        FITKAbaAbstractStep::MassScaling getMassScaling() const;
        /**
         * @brief  创建质量缩放定义
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaMassScalingDefinition* createMassScalingDefinition(int index = -1);
        /**
         * @brief  插入质量缩放定义
         * @param[in]  index 要插入的索引位置
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-09
         */
        void insertMassScalingDefinition(int index, const FITKAbaMassScalingDefinition& define);
        /**
         * @brief  移除第index个质量缩放定义
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void removeMassScalingDefinition(int index);
        /**
         * @brief  获取定义的质量缩放个数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        int getNumberOfMassScalingDefinition() const;
        /**
         * @brief  获取第index个质量缩放定义
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaMassScalingDefinition* getMassScalingDefinition(int index);
        /**
         * @brief  设置质量缩放类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setMassScaling(MassScaling type);
        /**
         * @brief  获取线性体积粘性系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        double getLinearBulkViscosityParameter() const;
        /**
         * @brief  设置线性体积粘性系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setLinearBulkViscosityParameter(double value);
        /**
         * @brief  获取二次体积粘性系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        double getQudraticBulkViscosityParameter() const;
        /**
         * @brief  设置二次体积粘性系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-26
         */
        void setQudraticBulkViscosityParameter(double value);

    private:
        /**
         * @brief  是否包含绝热效应
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        bool m_includeAdiabaticHeatingEffects{false};
        /**
         * @brief  时间增量步类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        FITKAbaAbstractStep::TimeIncrementType m_incrementType{ FITKAbaAbstractStep::TimeIncrementType::Automatic};
        /**
         * @brief  稳定时间增量步评估方式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        FITKAbaAbstractStep::StableIncrementEstimator m_stableIncrementEstimator{ FITKAbaAbstractStep::StableIncrementEstimator::Global};
        /**
         * @brief  增量步定义方式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaAbstractStep::IncrementSizeSelection m_incrementSizeSelection{ FITKAbaAbstractStep::IncrementSizeSelection::UseElementByElementTimeIncrementEstimator};
        /**
         * @brief  用户定义的时间增量
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        double m_userDefinedTimeIncrement{0.0};
        /**
         * @brief  是否使用improvedDtMethod
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool m_improvedDtMethod{true};
        /**
         * @brief  最大时间增量，无限制或指定值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        FITKAbaAbstractStep::MaxTimeIncrement m_maxTimeIncrement{ FITKAbaAbstractStep::MaxTimeIncrement::Unlimited};
        /**
         * @brief  最大时间增量步
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_maxTimeIncrementValue{0.0};
        /**
         * @brief  时间缩放系数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_timeScalingFactor{1.0};
        /**
         * @brief  质量缩放类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        FITKAbaAbstractStep::MassScaling m_massScalingType{ FITKAbaAbstractStep::MassScaling::FromPreviousStep};
        /**
         * @brief  线性体积粘性系数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_linearBulkViscosityParameter{0.06};
        /**
         * @brief  二次体积粘性系数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_quadraticBulkViscosityParameter{1.2};
        /**
         * @brief  质量缩放定义
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        QVector<FITKAbaMassScalingDefinition> m_massScaleingDefinitions{};
    };
}


#endif // FITKABASTEPDYNAMICEXPLICIT_H

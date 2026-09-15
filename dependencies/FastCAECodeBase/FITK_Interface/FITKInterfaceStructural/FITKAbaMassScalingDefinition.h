/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMassScalingDefinition.h
 * @brief  定义质量缩放，用于显式计算分析步
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-25
 * 
 */
#ifndef FITKABAMASSSCALINGDEFINITION_H
#define FITKABAMASSSCALINGDEFINITION_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKBoundaryLocation.h"
#include <QObject>

namespace Interface {

    class FITKInterfaceStructuralAPI FITkMassScalarEnum : public QObject
    {
        Q_OBJECT
    public:
        /**
    * @brief  质量缩放目标
    * @author LiJin (1003680421@qq.com)
    * @date   2024-04-25
    */
        enum MassScalingObjective
        {
            SemiAutomaticMassScaling,
            AutomaticMassScaling,
            ReinitializeMass,
            DisaleMassScalingThroughoutStep
        };
        Q_ENUM(MassScalingObjective);
        /**
         * @brief  质量缩放作用区域
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum MassScalingApplicationRegion
        {
            WholeModel,
            Set
        };
        Q_ENUM(MassScalingApplicationRegion);
        /**
         * @brief  质量缩放作用时机
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum MassScalingApplicationScale
        {
            AtBeginningOfStep,
            ThroughoutStep
        };
        Q_ENUM(MassScalingApplicationScale);
        /**
         * @brief  质量缩放作用条件
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum MassScalingScaleElementMass
        {
            IfBelowMinimumTarget,
            UniformlyToSatisfyTarget,
            NonuniformlyToEqualTarget
        };
        Q_ENUM(MassScalingScaleElementMass);
        /**
         * @brief  质量缩放频率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum MassScalingFrequency
        {
            EvertyIncrements,
            AtEqualIntervals
        };
        Q_ENUM(MassScalingFrequency);
    };

   
    /**
     * @brief  质量缩放定义，用于显式瞬态分析步
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-25
     */
    class FITKInterfaceStructuralAPI FITKAbaMassScalingDefinition
    {
    public:
        FITKAbaMassScalingDefinition();
        FITKAbaMassScalingDefinition(const FITKAbaMassScalingDefinition& msd);
        FITKAbaMassScalingDefinition& operator=(const FITKAbaMassScalingDefinition& msd);
        ~FITKAbaMassScalingDefinition();
        /**
         * @brief  获取Mass scaling objective
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITkMassScalarEnum::MassScalingObjective getMassScalingObjectvie() const;
        /**
         * @brief  设置Mass scaling objective
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setMassScalingObjective(FITkMassScalarEnum::MassScalingObjective value);
        /**
         * @brief  质量缩放作用区域
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITkMassScalarEnum::MassScalingApplicationRegion getMassScalingApplicationRegion() const;
        /**
         * @brief  设置质量缩放作用区域
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setMassScalingApplicationRegion(FITkMassScalarEnum::MassScalingApplicationRegion region);
        /**
         * @brief  在分析步开始时应用质量缩放还是整个分析步中应用
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITkMassScalarEnum::MassScalingApplicationScale getMassScalingApplicationScale() const;
        /**
         * @brief  设置质量缩放应用与分析步开始还是整个分析步
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setMassScalingApplicationScale(FITkMassScalarEnum::MassScalingApplicationScale scale);
        FITKBoundaryLocation* getLocation();
        /**
         * @brief  是否通过系数进行缩放
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool getScaleByFactor() const;
        /**
         * @brief  设置是否通过系数进行缩放
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setScaleByFactor(bool stat);
        /**
         * @brief  获取缩放系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        double getScalerFactor() const;
        /**
         * @brief  设置缩放系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setScalerFactor(double factor);
        /**
         * @brief  质量缩放是否作用于目标时间增量步
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool getScalerToTargetTimeIncrement() const;
        /**
         * @brief  设置质量缩放是否作用于目标时间增量步
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setScalerToTargetTimeIncrement(bool stat);
        /**
         * @brief  获取质量缩放作用的目标增量步
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        double getTargetIncrement() const;
        /**
         * @brief  设置质量缩放作用的目标增量步
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setTargetIncrement(double inc);
        /**
         * @brief  获取质量缩放作用的条件
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITkMassScalarEnum::MassScalingScaleElementMass getScaleElementMass() const;
        /**
         * @brief  设置质量缩放作用的条件
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setScaleElementMass(FITkMassScalarEnum::MassScalingScaleElementMass type);
        /**
         * @brief  质量缩放作用频率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITkMassScalarEnum::MassScalingFrequency getMassScalingFrequency() const;
        /**
         * @brief  设置质量缩放作用频率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setMassScalingFrequency(FITkMassScalarEnum::MassScalingFrequency type);
        /**
         * @brief  获取每多少个增量步进行质量缩放
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        int getEveryIncrementNumber() const;
        /**
         * @brief  设置每多少个增量步进行质量缩放
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setEveryIncrementNumber(int num);
        /**
         * @brief  获取质量缩放的间隔数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        int getNumberOfIntervals() const;
        /**
         * @brief  设置质量缩放的间隔数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setNumberOfInvtervals(int num);
        /**
         * @brief  获取自动质量缩放类型轧制流入比率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-09
         */
        double getFeedRate() const;
        /**
         * @brief  设置自动质量缩放轧制流入比率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-09
         */
        void setFeedRate(double newFeedRate);
        /**
         * @brief  获取自动质量缩放轧制拉伸的单元长度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-09
         */
        double getExtrudedElementLength() const;
        /**
         * @brief  设置自动质量缩放轧制拉伸的单元长度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-09
         */
        void setExtrudedElementLength(double newExtrudedElementLength);
        /**
         * @brief  获取自动质量缩放轧制横截面上的节点数目
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-09
         */
        int getNodesInCrossSection() const;
        /**
         * @brief  设置自动质量缩放轧制横截面上的节点数目
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-09
         */
        void setNodesInCrossSection(int newNodesInCrossSection);

    private:

        /**
         * @brief  质量缩放目标
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITkMassScalarEnum::MassScalingObjective m_Objectvie{ FITkMassScalarEnum::MassScalingObjective::SemiAutomaticMassScaling};
        /**
         * @brief  质量缩放作用区域
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITkMassScalarEnum::MassScalingApplicationRegion m_application{ FITkMassScalarEnum::MassScalingApplicationRegion::WholeModel};
        /**
         * @brief  质量缩放作用时机
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITkMassScalarEnum::MassScalingApplicationScale m_applicationScale{ FITkMassScalarEnum::MassScalingApplicationScale::ThroughoutStep};
        /**
         * @brief  质量缩放作用的区域
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-03
         */
        FITKBoundaryLocation *m_location{ nullptr };
        /**
         * @brief  是否通过系数进行缩放
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool m_scaleByFactor{true};
        /**
         * @brief  缩放系数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        double m_scaleFactor{0.0};
        /**
         * @brief  是否作用于目标时间增量步
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool m_scaleToTargetTimeIncrement{false};
        /**
         * @brief  目标时间增量步
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        double m_targetIncrement{0};
        /**
         * @brief  质量缩放作用条件
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITkMassScalarEnum::MassScalingScaleElementMass m_scaleElementMass{ FITkMassScalarEnum::MassScalingScaleElementMass::IfBelowMinimumTarget};
        /**
         * @brief  自动质量缩放，轧制流入比率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-09
         */
        double m_FeedRate{ 0.0 };
        /**
         * @brief  自动质量缩放，轧制拉伸的单元长度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-09
         */
        double m_extrudedElementLength{ 0.0 };
        /**
         * @brief  自动质量缩放，轧制横截面上的节点数目
         * @author LiJin (1003680421@qq.com)
         * @date   2024-07-09
         */
        int m_NodesInCrossSection{ 0 };
        /**
         * @brief  质量缩放作用频率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITkMassScalarEnum::MassScalingFrequency m_frequency{ FITkMassScalarEnum::MassScalingFrequency::EvertyIncrements};
        /**
         * @brief  每多少个增量步进行缩放
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        int m_evertyIncrementsNumber{1};
        /**
         * @brief  质量缩放作用间隔数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        int m_atNumberOfIntervals{1};
    };
}


#endif // FITKABAMASSSCALINGDEFINITION_H

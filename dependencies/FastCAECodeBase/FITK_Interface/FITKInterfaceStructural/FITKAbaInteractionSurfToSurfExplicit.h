/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaInteractionSurfToSurfExplicit.h
 * @brief  显式面面接触相互作用
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-17
 * 
 */
#ifndef FITKABAINTERACTIONSURFTOSURFEXPLICIT_H
#define FITKABAINTERACTIONSURFTOSURFEXPLICIT_H
#include "FITKAbaAbstractInteraction.h"


namespace Interface {
    

    /**
     * @brief  显式面面接触相互作用
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-17
     */
    class FITKInterfaceStructuralAPI FITKAbaInteractionSurfToSurfExplicit: public FITKAbaAbstractInteraction
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbaInteractionSurfToSurfExplicit)
    public:
        /**
         * @brief  力学约束公式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        enum MachanicalConstraintFormulation
        {
            KinematicContactMethod,
            PenaltyContactMethod
        };
        Q_ENUM(MachanicalConstraintFormulation);
        /**
         * @brief  权重系数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        enum WeightingFactor
        {
            UseAnalaysisDefault,
            Specify
        };
        Q_ENUM(WeightingFactor);
        /**
         * @brief  初始间隙
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        enum InitialClearance
        {
            NotSpecify,
            UniformValueAcrossSlaveSurf,
            ComputedForSingleThreadedBolt,
            SpecifyForSingleThreadedBolt
        };
        Q_ENUM(InitialClearance);

    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaInteractionSurfToSurfExplicit() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaInteractionSurfToSurfExplicit() = default;
        /**
         * @brief  获取相互作用类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        FITKAbaInteractionType getInteractionType() const override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  数据在分析步中是否发生修改
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool isModifiedInStep(int stepId) const override;
        /**
         * @brief  数据在分析步中的激活状态发生变化时的响应函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void activeStateChanged() override;
        /**
         * @brief  获取力学约束公式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        MachanicalConstraintFormulation getMachanicalConstraintFormulation() const;
        /**
         * @brief  设置力学约束公式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setMachanicalConstraintFormulation(MachanicalConstraintFormulation newMachanicalConsForm);
        /**
         * @brief  获取滑移公式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        SlidingFormulation getSlidingFormulation() const;
        /**
         * @brief  设置滑移公式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setSlidingFormulation(SlidingFormulation newSliding);
        /**
         * @brief  获取初始间隙定义方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        InitialClearance getInitialClearance() const;
        /**
         * @brief  设置初始间隙定义方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setInitialClearance(InitialClearance newInitialClearance);
        /**
         * @brief  获取一致的初始间隙值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        double getUniformClearance() const;
        /**
         * @brief  设置一致的初始间隙值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setUniformClearance(double newUniformClearance);
        /**
         * @brief  获取接触属性Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        int getContactPropertyId(int stepId = 0);
        /**
         * @brief  设置接触属性Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setContactPropertyId(int newContactPropertyId, int stepId = 0);
        /**
         * @brief  设置权重系数方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        WeightingFactor getWeightingFactor() const;
        /**
         * @brief  设置权重系数方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setWeightingFactor(WeightingFactor newWeightingFactor);
        /**
         * @brief  获取权重系数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        double getWeightingFactorValue() const;
        /**
         * @brief  设置权重系数值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setWeightingFactorValue(double newWeightingFactorValue);
    private:
        /**
         * @brief  获取分析步中的接触属性id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        int getContactPorpertyIdInStep(int stepId);

    private:
        /**
         * @brief  力学约束公式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        MachanicalConstraintFormulation m_machanicalConsForm{ MachanicalConstraintFormulation::KinematicContactMethod };
        /**
         * @brief  滑移方式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        SlidingFormulation m_sliding{ SlidingFormulation::FinitSliding };
        /**
         * @brief  初始间隙定义方式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        InitialClearance m_initialClearance{ InitialClearance::NotSpecify };
        /**
         * @brief  一致的初始间隙值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        double m_uniformClearance{ 0.0 };
        /**
         * @brief  权重系数定义方式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        WeightingFactor m_weightingFactor{ WeightingFactor::UseAnalaysisDefault };
        /**
         * @brief  指定的权重系数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        double m_weightingFactorValue{ 0.0 };
        /**
         * @brief  接触属性Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        int m_contactPropertyId{ 0 };
        /**
         * @brief  后续分析步修改的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        QMap<int, int> m_modifiedDatas{};
    };
}


#endif // FITKABAINTERACTIONSURFTOSURFEXPLICIT_H

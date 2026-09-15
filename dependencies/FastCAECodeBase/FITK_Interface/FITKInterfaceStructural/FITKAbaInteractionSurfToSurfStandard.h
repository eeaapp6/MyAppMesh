/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file   FITKAbaInteractionSurfToSurfStandard.h
 * @brief  隐式面面接触
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-25
 *
 */
#ifndef FITKABAINTERACTIONSURFTOSURFSTANDARD_H
#define FITKABAINTERACTIONSURFTOSURFSTANDARD_H
#include "FITKAbaAbstractInteraction.h"


namespace Interface {

    /**
     * @brief  隐式面面接触类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-25
     */
    class FITKInterfaceStructuralAPI FITKAbaInteractionSurfToSurfStandard :
        public FITKAbaAbstractInteraction
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKAbaInteractionSurfToSurfStandard)
    public:
        /**
         * @brief  接触离散方法
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum ContactDiscretizationMethod
        {
            SurfaceToSurface,
            NodeToSurface
        };
        Q_ENUM(ContactDiscretizationMethod);
        /**
         * @brief  Contact tracking
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum ContactTracking
        {
            TwoConfigurationsPath,
            SingleConfigurationState
        };
        Q_ENUM(ContactTracking);
        /**
         * @brief  从面调整
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum SlaveAdjustment
        {
            NoAdjustment,
            AdjustOnlyToRemoveOverclosure,
            SepecifyToleranceForAdjustmentZone,
            AdjustSlaveNodesInSet
        };
        Q_ENUM(SlaveAdjustment);
        /**
         * @brief  Use supplementary contact pairs
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum UseSupplementaryContactPairs
        {
            Selectively,
            Never,
            Always
        };
        Q_ENUM(UseSupplementaryContactPairs);
        /**
         * @brief  初始容差
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum InitialClearance
        {
            NotSpecified,
            UniformValueAcrossSlaveSurface,
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
        FITKAbaInteractionSurfToSurfStandard() = default;
        /**
         * @brief  析构函数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaInteractionSurfToSurfStandard() = default;
        /**
         * @brief  获取相互作用类型
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaInteractionType getInteractionType() const override;
        /**
         * @brief  复制对象
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  在分析步中是否被调整
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool isModifiedInStep(int stepId) const override;
        /**
         * @brief  激活状态发生变化时的响应函数
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void activeStateChanged() override;
        /**
         * @brief  获取滑移方程
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        SlidingFormulation getSlidingFormulation() const;
        /**
         * @brief  设置滑移方程
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setSlidingFormulation(SlidingFormulation newSlidingFormulation);
        /**
         * @brief  接触离散方法
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        ContactDiscretizationMethod getDiscretizationMethod() const;
        /**
         * @brief  设置接触离散方法
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setDiscretizationMethod(ContactDiscretizationMethod newDiscretizationMethod);
        /**
         * @brief  获取是否排除壳/膜单元厚度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool getExcludeShellMembranceElementThickness() const;
        /**
         * @brief  设置是否排除壳/膜单元厚度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setExcludeShellMembranceElementThickness(bool newExcludeShellMembranceElementThickness);
        /**
         * @brief  获取Contact tracking
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        ContactTracking getContactTracking() const;
        /**
         * @brief  设置Contact tracking
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setContactTracking(ContactTracking newContactTracking);
        /**
         * @brief  获取从面调整方式
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        SlaveAdjustment getSlaveAdjustment() const;
        /**
         * @brief  设置从面调整方式
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setSlaveAdjustment(SlaveAdjustment newSlaveAdjustment);
        /**
         * @brief  获取从面调整容差
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        double getAdjustTolerance() const;
        /**
         * @brief  设置从面调整容差
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setAdjustTolerance(double newAdjustTolerance);
        /**
         * @brief  获取调整的节点集合
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        int getAdjustNodeSetId() const;
        /**
         * @brief  设置调整的节点集合
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setAdjustNodeSetId(int newAdjustNodeSetId);
        /**
         * @brief  获取是否绑定调整的面
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool getTieAdjustSurfaces() const;
        /**
         * @brief  设置是否绑定调整的面
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setTieAdjustSurfaces(bool newTieAdjustSurfaces);
        /**
         * @brief  获取Use supplementary contact pairs
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        UseSupplementaryContactPairs getUseSupplementaryContactPairs() const;
        /**
         * @brief  设置Use supplymentary contact pairs
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setUseSupplementaryContactPairs(UseSupplementaryContactPairs newUseSupplementaryContactPairs);
        /**
         * @brief  获取初始间隙类型
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        InitialClearance getInitialClearance() const;
        /**
         * @brief  设置初始间隙类型
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setInitialClearance(InitialClearance newInitialClearance);
        /**
         * @brief  获取一致的间隙容差
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        double getUniformClearance() const;
        /**
         * @brief  设置一致的间隙容差
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setUniformClearance(double newUniformClearance);
        /**
         * @brief  获取在分析步中的接触属性Id
         * @param[in]  stepId分析步id
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        int getInteractionPropertyId(int stepId = 0);
        /**
         * @brief  设置分析步中的接触属性Id
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setInteractionPropertyId(int newInteractionPropertyId, int stepId = 0);
        /**
         * @brief  设置主表面平滑度
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-12-16
         */
        double getSmoothDegreeOfMasterSurface() const;
        void setSmoothDegreeOfMasterSurface(double val);
        /**
         * @brief  设置表面平滑
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-12-16
         */
        bool getSurfaceSmoothing() const;
        void setSurfaceSmoothing(bool val);
        /**
         * @brief  限制绑定到子集中的从属节点
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-12-16
         */
        bool getLimitBonding() const;
        void setLimitBonding(bool val);
        int getLimitBondingNodeSetId() const;
        void setLimitBondingNodeSetId(int val);
    private:
        /**
         * @brief  获取分析步中的接触属性Id
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        int getInteractionPorpertyIdInStep(int stepId);

    private:
        /**
         * @brief  主表面平滑度
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-12-16
         */
        double m_SmoothDegreeOfMasterSurface{ 0.2 };
        /**
         * @brief  表面平滑
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-12-16
         */
        bool m_SurfaceSmoothing{ false };
        /**
         * @brief  限制绑定到子集中的从属节点
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-12-16
         */
        bool m_LimitBonding{ false };
        int m_LimitBondingNodeSetId{ 0 };
        /**
         * @brief  滑移公式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        SlidingFormulation m_slidingFormulation{ SlidingFormulation::FinitSliding };
        /**
         * @brief  离散方法
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        ContactDiscretizationMethod m_discretizationMethod{ ContactDiscretizationMethod::SurfaceToSurface };
        /**
         * @brief  是否排除壳/膜单元厚度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool m_excludeShellMembranceElementThickness{ false };
        /**
         * @brief  接触跟踪
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        ContactTracking m_contactTracking{ ContactTracking::TwoConfigurationsPath };
        /**
         * @brief  从面调整方式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        SlaveAdjustment m_slaveAdjustment{ SlaveAdjustment::NoAdjustment };
        /**
         * @brief  调整容差
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        double m_adjustTolerance{ 0.0 };
        /**
         * @brief  调整节点集合Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        int m_adjustNodeSetId{ 0 };
        /**
         * @brief  是否绑定从面
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool m_tieAdjustSurfaces{ false };
        /**
         * @brief  Use supplementary contact pairs
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        UseSupplementaryContactPairs m_useSupplementaryContactPairs{ UseSupplementaryContactPairs::Selectively };
        /**
         * @brief  初始间隙
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        InitialClearance m_initialClearance{ InitialClearance::NotSpecified };
        /**
         * @brief  一致的间隙值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        double m_uniformClearance{ 0.0 };
        /**
         * @brief  接触属性Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        int m_InteractionPropertyId{ 0 };
        /**
         * @brief  后续分析步中调整的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        QMap<int, int> m_modifiedDatas{};
    };
}


#endif // FITKABAINTERACTIONSURFTOSURFSTANDARD_H

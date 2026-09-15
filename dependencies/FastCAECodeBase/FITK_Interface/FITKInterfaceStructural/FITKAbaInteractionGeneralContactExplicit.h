/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaInterfaceGeneralContactExplicit.h
 * @brief  显式分析通用接触
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-22
 * 
 */
#ifndef FITKABAINTERACTIONGENERALCONTACTEXPLICIT_H
#define FITKABAINTERACTIONGENERALCONTACTEXPLICIT_H
#include "FITKAbaAbstractInteraction.h"
#include <QPair>

namespace Interface {
    
    class FITKInterfaceStructuralAPI GeneralContactExplicitData: public FITKInteractionLocation
    {
        Q_OBJECT
    public:
        /**
         * @brief  通用接触包括面对方式枚举
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        enum IncludeSurfacePairs
        {
            AllWithSelf,
            SelectedSurfacePairs
        };
        Q_ENUM(IncludeSurfacePairs);

    public:
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  包括面对方式
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        IncludeSurfacePairs getIncludeSurfacePairs() const;
        /**
         * @brief  设置包括面对方式
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setIncludeSurfacePairs(IncludeSurfacePairs type);
        /**
         * @brief  设置接触面对的数目
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-16
         */
        void setNumberOfIncludeSurfPairs(int num);
        void addIncludeSurfPair();
        /**
         * @brief  获取接触面对的数目
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-16
         */
        int getNumberOfIncludeSurfPairs() const;
        /**
         * @brief  通过索引获取接触对
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-16
         */
        QPair<Interface::FITKBoundaryLocation*, Interface::FITKBoundaryLocation*> getIncludeSurfPair(int index);
        /**
         * @brief  通过索引移除接触对
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-16
         */
        void removeIncludeSurfPair(int index);
        /**
         * @brief  清空所有包含的接触对
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-16
         */
        void celarIncludeSurfPairs();
        /**
         * @brief  获取接触属性id
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        int getContactPropertyId() const;
        /**
         * @brief  设置接触属性id
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setContactPropertyId(int id);
    private:
        IncludeSurfacePairs m_includeSurfPairs{ IncludeSurfacePairs::AllWithSelf };
        int m_contactPropertyID{ 0 };
    };
    /**
     * @brief  显式分析
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-22
     */
    class FITKInterfaceStructuralAPI FITKAbaInteractionGeneralContactExplicit: public FITKAbaAbstractInteraction
    {
        FITKCLASS(Interface, FITKAbaInteractionGeneralContactExplicit)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaInteractionGeneralContactExplicit() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaInteractionGeneralContactExplicit() = default;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  数据在分析步中是否被修改
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool isModifiedInStep(int stepId) const override;
        /**
         * @brief  数据在分析步中激活状态发生变化时的响应函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void activeStateChanged() override;
        /**
         * @brief  获取相互作用类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        FITKAbaInteractionType getInteractionType() const override;
        /**
         * @brief  包括面对方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        GeneralContactExplicitData::IncludeSurfacePairs getIncludeSurfacePairs(int stepId = 0);
        /**
         * @brief  设置包括面对方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setIncludeSurfacePairs(GeneralContactExplicitData::IncludeSurfacePairs type, int stepId = 0);
        /**
         * @brief  设置接触面对的数目
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-16
         */
        void setNumberOfIncludeSurfPairs(int num, int stepId = 0);
        /**
         * @brief  获取接触面对的数目
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-16
         */
        int getNumberOfIncludeSurfPairs( int stepId = 0) ;
        /**
         * @brief  通过索引获取接触对
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-16
         */
        QPair<Interface::FITKBoundaryLocation*, Interface::FITKBoundaryLocation*> getIncludeSurfPair(int index, int stepId = 0);
        /**
         * @brief  通过索引移除接触对
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-16
         */
        void removeIncludeSurfPair(int index,  int stepId = 0);
        /**
         * @brief  清空所有包含的接触对
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-16
         */
        void celarIncludeSurfPairs( int stepId = 0);
        /**
         * @brief  获取接触属性id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        int getContactPropertyId(int stepId = 0);
        /**
         * @brief  设置接触属性id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setContactPropertyId(int id, int stepId = 0);

    private:
        /**
         * @brief  获取分析步数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        GeneralContactExplicitData& getStepData(int stepId);
    private:
        /**
         * @brief  创建数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        GeneralContactExplicitData m_data;
        /**
         * @brief  后续分析步修改的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        QMap<int, GeneralContactExplicitData*> m_modifiedDatas{};
    };
}


#endif // FITKABAINTERACTIONGENERALCONTACTEXPLICIT_H

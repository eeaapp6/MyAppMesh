/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAbstractInteraction.h
 * @brief  相互作用基类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-17
 * 
 */
#ifndef FITKABAABSTRACTINTERACTION_H
#define FITKABAABSTRACTINTERACTION_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKInteractionLocation.h"
#include "FITKAbaStepRelatedDataObject.h"


namespace Interface {
    
    /**
     * @brief  相互作用基类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-17
     */
    class FITKInterfaceStructuralAPI FITKAbaAbstractInteraction: public FITKInteractionLocation,
        public FITKAbaStepRelatedDataObject
    {
        Q_OBJECT
    public:
        /**
        * @brief  相互作用类型枚举
        * @author LiJin (1003680421@qq.com)
        * @date   2024-04-17
        */
        enum FITKAbaInteractionType
        {
            GeneralContactExplicit,
            SurfToSurfContactExplicit,
            SurfToSurfContactStandard,
            SurfaceFilmCondition,
        };
        Q_ENUM(FITKAbaInteractionType);
        /**
         * @brief  滑移公式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        enum SlidingFormulation
        {
            FinitSliding,
            SmallSliding
        };
        Q_ENUM(SlidingFormulation);

    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaAbstractInteraction() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaAbstractInteraction() = default;
        /**
         * @brief  获取相互作用类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        virtual FITKAbaInteractionType getInteractionType() const = 0;
        /**
         * @brief  是否成功获取分析步管理器
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool hasGetStepManager() override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  在创建所在的分析步中是否有效，当分析步替换时调用
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool isValidInCreatedStep() override;
        /**
         * @brief  获取相互作用的激活状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        bool getActiveState() const;
        /**
         * @brief  设置相互作用的激活状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        void setActiveState(bool stat);
    protected:
        /**
         * @brief  激活状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        bool m_activeState{ true };
    };
}


#endif // FITKABAABSTRACTINTERACTION_H

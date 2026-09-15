/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaStepGeneralStatic.h
 * @brief  Static,General分析步
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-26
 * 
 */
#ifndef FITKABASTEPGENERALSTATIC_H
#define FITKABASTEPGENERALSTATIC_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractStep.h"


namespace Interface {
    /**
     * @brief  Static,General类型分析步
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-26
     */
    class FITKInterfaceStructuralAPI FITKAbaStepGeneralStatic: public FITKAbaAbstractStep
    {
        FITKCLASS(Interface, FITKAbaStepGeneralStatic)
    public:
        /**
         * @brief  默认构造
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaStepGeneralStatic() = default;
        /**
         * @brief  默认析构
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        ~FITKAbaStepGeneralStatic() = default;
        /**
         * @brief  获取分析步类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaStepType getStepType() const override;
        /**
         * @brief  获取时间增量步类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        TimeIncrementType getIncrementType() const;
        /**
         * @brief  设置时间增量步类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        void setIncrementType(TimeIncrementType type);
        /**
         * @brief  获取最大时间增量步数目
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        int getMaximumNumberOfIncrements() const;
        /**
         * @brief  设置最大时间增量步数目
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        void setMaximumNumberOfIncrements(int num);
        /**
         * @brief  获取初始时间增量步大小
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double getInitIncrementSize() const;
        /**
         * @brief  设置初始时间增量步大小
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        void setInitIncrementSize(double value);
        /**
         * @brief  获取最小时间增量步大小
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double getMinIncrementSize() const;
        /**
         * @brief  设置最小时间增量步大小
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        void setMinIncrementSize(double value);
        /**
         * @brief  获取最大时间增量步大小
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double getMaxIncrementSize() const;
        /**
         * @brief  设置最大时间增量步大小
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        void setMaxIncrementSize(double value);
        /**
         * @brief  获取固定时间增量步大小
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double getFixedIncrementSize() const;
        /**
         * @brief  设置固定时间增量步大小
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        void setFixedIncrementSize(double value);
    private:
        /**
         * @brief  时间增量步类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        TimeIncrementType m_incrementType{TimeIncrementType::Automatic};
        /**
         * @brief  最大时间增量步数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        int m_maxNumOfIncrements{100};
        /**
         * @brief  初始时间增量步
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_initIncrementSize{1};
        /**
         * @brief  最小时间增量步
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_minIncrementSize{1e-5};
        /**
         * @brief  最大时间增量步
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_maxIncrementSize{1};
        /**
         * @brief  固定时间增量步
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_fixedIncrementSize{1};
    };
}


#endif // FITKABASTEPGENERALSTATIC_H

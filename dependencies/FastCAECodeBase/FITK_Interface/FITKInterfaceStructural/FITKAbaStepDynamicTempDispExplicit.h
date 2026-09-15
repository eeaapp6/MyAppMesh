/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************
 * @file   FITKAbaStepDynamicTempDispExplicit.h
 * @brief  ABAQUS温度位移耦合显式动力学分析步定义
 * @author libaojun
 * @date   2025-04-26
 ******************************************************************/

#ifndef __FITKABASTEPDYNAMICTEMPDISPEXPLICIT_H__
#define __FITKABASTEPDYNAMICTEMPDISPEXPLICIT_H__

#include "FITKAbaStepDynamicExplicit.h"

namespace Interface
{
    /**
     * @brief ABAQUS温度位移耦合显式动力学分析步类
     * @details 用于定义ABAQUS中的温度位移耦合显式动力学分析步,
     *          继承自显式动力学分析步基类
     */
    class FITKInterfaceStructuralAPI FITKAbaStepDynamicTempDispExplicit: public FITKAbaStepDynamicExplicit
    {
     
        public:
            /**
             * @brief  默认构造函数
             * @author Libaojun
             * @date   2025-04-26
             */
            explicit FITKAbaStepDynamicTempDispExplicit() = default;
            /**
             * @brief  默认析构函数
             * @author Libaojun
             * @date   2025-04-26
             */
            virtual ~FITKAbaStepDynamicTempDispExplicit() = default;

            /**
             * @brief  获取分析步类型
             * @param[in]  
             * @return 分析步类型枚举值
             * @author Libaojun
             * @date   2025-04-26
             */
            virtual FITKAbaStepType getStepType() const override;
            /**
             * @brief  是否包括绝热效应
             * @param[in]  
             * @return bool值
             * @author LiJin (1003680421@qq.com)
             * @date   2025-04-26
             */
            bool getIncludeAdiabaticHeatingEffects() const = delete;
            /**
             * @brief  设置绝热效应
             * @param[in]  stat 状态
             * @return 
             * @author LiJin (1003680421@qq.com)
             * @date   2025-04-26
             */
            void setIncludeAdiabaticHeatingEffects(bool stat) = delete;
            
    };

}
#endif // __FITKABASTEPDYNAMICTEMPDISPEXPLICIT_H__
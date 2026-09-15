/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaInteractionContactBehavior.h
 * @brief  相互作用接触属性抽象行为
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABAINTERACTIONCONTACTBEHAVIOR_H
#define FITKABAINTERACTIONCONTACTBEHAVIOR_H

#include "FITKInterfaceStructuralAPI.h"
#include <QObject>


namespace Interface {

    class FITKInterfaceStructuralAPI FITKAbaInteractionContactBehaviorEnum : public QObject
    {
        Q_OBJECT
    public:
        /**
          * @brief  接触行为类型枚举类
          * @author LiJin (1003680421@qq.com)
          * @date   2024-04-15
          */
        enum FITKAbaContactBehaviorType
        {
            TangentialBehavior,
            NormalHehavior,
            GeometricProperty,
            ThermalConductance,
        };
        Q_ENUM(FITKAbaContactBehaviorType);
        /**
         * @brief  摩擦公式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        enum FrictionFormulation
        {
            Frictionless,
            Penalty
        };
        Q_ENUM(FrictionFormulation);
    };
 
    /**
     * @brief  接触属性行为抽象类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaInteractionContactBehavior  
    {
        

    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaInteractionContactBehavior() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        virtual ~FITKAbaInteractionContactBehavior() = default;
        /**
         * @brief  获取接触属性行为类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        virtual FITKAbaInteractionContactBehaviorEnum::FITKAbaContactBehaviorType getBehaviorType() const = 0;
    };
}


#endif // FITKABAINTERACTIONCONTACTBEHAVIOR_H

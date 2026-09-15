/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaInteractionContactNormalBehavior.h
 * @brief  接触法向行为
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-25
 * 
 */
#ifndef FITKABAINTERACTIONCONTACTNORMALBEHAVIOR_H
#define FITKABAINTERACTIONCONTACTNORMALBEHAVIOR_H

#include "FITKAbaInteractionContactBehavior.h"
#include "FITKInterfaceStructuralAPI.h"
#include <QObject>

namespace Interface {


    class FITKInterfaceStructuralAPI FITKAbaInteractionContactEnum : public QObject
    {
        Q_OBJECT
    public:

        /**
         * @brief  接触法向行为Pressure Overclosure类型枚举
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum PressureOverclosure
        {
            HardContact,
            Exponential,
            Linear,
            Tabular,
            ScaleFactor_GeneralContactExplicit
        };
        Q_ENUM(PressureOverclosure);
        /**
         * @brief  接触法向行为Constraint Enforcement Method类型枚举
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        enum ConstraintEnformentMethod
        {
            Default,
            AugmentedLagrange_Standard,
            Penalty_Standard,
            Direct_Standard
        };
        Q_ENUM(ConstraintEnformentMethod);

        enum MaximumStiffness
        {
            Infinite,
            Specify
        };
        Q_ENUM(MaximumStiffness)
    };

    /**
     * @brief  接触法向行为类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-25
     */
    class FITKInterfaceStructuralAPI FITKAbaInteractionContactNormalBehavior: 
        public FITKAbaInteractionContactBehavior
    {
      
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaInteractionContactNormalBehavior() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaInteractionContactNormalBehavior() = default;
        /**
         * @brief  获取接触行为类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaInteractionContactBehaviorEnum::FITKAbaContactBehaviorType getBehaviorType() const override;
        /**
         * @brief  获取Pressure overclosuure类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaInteractionContactEnum::PressureOverclosure getPressureOverclosure() const;
        /**
         * @brief  设置Pressure overclosuure类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setPressureOverclosure(FITKAbaInteractionContactEnum::PressureOverclosure newPressureOverclosure);
        /**
         * @brief  获取Constraint Enforment Method
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaInteractionContactEnum::ConstraintEnformentMethod getConstraintEnforcementMethod() const;
        /**
         * @brief  设置Constraint Enforment Method
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setConstraintEnforcementMethod(FITKAbaInteractionContactEnum::ConstraintEnformentMethod newConstraintEnforcementMethod);
        /**
         * @brief  获取allow separation after contact
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool getAllowSeparationAfterContact() const;
        /**
         * @brief  设置allow separation after contact
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        void setAllowSeparationAfterContact(bool newAllowSeparationAfterContact);
        double getPressureWhenClearance0() const;
        void setPressureWhenClearance0(double pressure);
        double getClearanceWhenPressure0() const;
        void setClearanceWhenPressure0(double clearance);

    private:
        /**
         * @brief  pressure overclosure
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaInteractionContactEnum::PressureOverclosure m_pressureOverclosure{ FITKAbaInteractionContactEnum::PressureOverclosure::HardContact};
        /**
         * @brief  constraint enforce method
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        FITKAbaInteractionContactEnum::ConstraintEnformentMethod m_constraintEnforcementMethod{ FITKAbaInteractionContactEnum::ConstraintEnformentMethod::Default};
        /**
         * @brief  是否 allow separation after contact
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-25
         */
        bool m_allowSeparationAfterContact{true};
        double m_pressureOfClearance0{ 0.0 };
        double m_clearanceOfPressure0{ 0.0 };
        FITKAbaInteractionContactEnum::MaximumStiffness m_MaximunStiffnessType{ FITKAbaInteractionContactEnum::Infinite };
        double m_maximumStiffnessSpecifyValue{ 0.0 };
    };
}


#endif // FITKABAINTERACTIONCONTACTNORMALBEHAVIOR_H

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAbstractConstraint.h
 * @brief  相互作用约束抽象类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-17
 * 
 */
#ifndef FITKABAABSTRACTCONSTRAINT_H
#define FITKABAABSTRACTCONSTRAINT_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKInteractionLocation.h"


namespace Interface {
   
    /**
     * @brief  约束基类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-17
     */
    class FITKInterfaceStructuralAPI FITKAbaAbstractConstraint: public FITKInteractionLocation
    {
        Q_OBJECT
    public:
        /**
        * @brief  约束类型枚举
        * @author LiJin (1003680421@qq.com)
        * @date   2024-04-17
        */
        enum FITKAbaConstraintType
        {
            Tie,
            RigidBody,
            Coupling
        };
        Q_ENUM(FITKAbaConstraintType);
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaAbstractConstraint() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaAbstractConstraint() = default;
        /**
         * @brief  获取约束类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        virtual FITKAbaConstraintType getConstraintType() const = 0;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool getActiveState() const;
        /**
         * @brief  设置激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setActiveState(bool state);
    private:
        /**
         * @brief  激活状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool m_active{true};
    };
}


#endif // FITKABAABSTRACTCONSTRAINT_H

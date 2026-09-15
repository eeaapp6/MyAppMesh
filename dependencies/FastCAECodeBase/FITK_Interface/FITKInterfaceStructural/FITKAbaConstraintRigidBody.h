/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConstraintRigidBody.h
 * @brief  刚体约束
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-28
 * 
 */
#ifndef FITKABACONSTRAINTRIGIDBODY_H
#define FITKABACONSTRAINTRIGIDBODY_H
#include "FITKAbaAbstractConstraint.h"


namespace Interface {
    /**
     * @brief  刚体约束类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-28
     */
    class FITKInterfaceStructuralAPI FITKAbaConstraintRigidBody : public FITKAbaAbstractConstraint
    {
        FITKCLASS(Interface, FITKAbaConstraintRigidBody)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaConstraintRigidBody() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaConstraintRigidBody() = default;
        /**
         * @brief  获取约束类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaConstraintType getConstraintType() const override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取adjust point状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool getAdjustPoint() const;
        /**
         * @brief  设置adjust point状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setAdjustPoint(bool state);
        /**
         * @brief  获取 constraint selected region to be isothermal状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool getConstraintSelectedRegionToBeIsothermal() const;
        /**
         * @brief  设置 constraint selected region to be isothermal状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setConstraintSelectedRegionToBeIsothermal(bool state);
        FITKBoundaryLocation* getReferencePoint();
        void setReferencePointNone();
        FITKBoundaryLocation* getBody();
        void setBodyNone();
        FITKBoundaryLocation* getPin();
        void setPinNone();
        FITKBoundaryLocation* getTie();
        void setTieNone();
        FITKBoundaryLocation* getAnalyticalSurface();
        void setAnalyticalSurfaceNone();
    private:
        /**
         * @brief  adjust point状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool m_adjustPoint{false};
        /**
         * @brief  constraint selected region to be isothermal状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool m_constraintIsothermal{false};
    };
}


#endif // FITKABACONSTRAINTRIGIDBODY_H

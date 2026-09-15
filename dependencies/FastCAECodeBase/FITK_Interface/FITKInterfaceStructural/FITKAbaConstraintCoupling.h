/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConstraintCoupling.h
 * @brief  相互作用耦合约束
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-17
 * 
 */
#ifndef FITKABACONSTRAINTCOUPLING_H
#define FITKABACONSTRAINTCOUPLING_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractConstraint.h"


namespace Interface {
    
    /**
     * @brief  相互作用耦合类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-17
     */
    class FITKInterfaceStructuralAPI FITKAbaConstraintCoupling: public FITKAbaAbstractConstraint
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbaConstraintCoupling);
    public:
        /**
     * @brief  耦合类型枚举
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-17
     */
        enum CouplingType
        {
            Kinematic,
            ContinuumDistributing,
            StructualDistributing
        };
        Q_ENUM(CouplingType);
        /**
         * @brief  权重方法
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        enum WeightingMethod
        {
            Uniform,
            Linear,
            Quadratic,
            Cubic
        };
        Q_ENUM(WeightingMethod);
        /**
         * @brief  影响半径
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        enum InfluenceRadius
        {
            ToOutermostPointOnTheRegion,
            Specify
        };
        Q_ENUM(InfluenceRadius);
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaConstraintCoupling() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaConstraintCoupling() = default;
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
         * @brief  获取耦合类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        CouplingType couplingType() const;
        /**
         * @brief  设置耦合类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setCouplingType(CouplingType newCouplingType);
        /**
         * @brief  U1耦合状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool getU1() const;
        /**
         * @brief  设置U1耦合状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setU1(bool newU1);
        /**
         * @brief  U2耦合状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool getU2() const;
        /**
         * @brief  设置U1耦合状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setU2(bool newU2);
        /**
         * @brief  U3耦合状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool getU3() const;
        /**
         * @brief  设置U1耦合状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setU3(bool newU3);
        /**
         * @brief  UR1耦合状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool getUR1() const;
        /**
         * @brief  设置UR1耦合状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setUR1(bool newUR1);
        /**
         * @brief  UR2耦合状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool getUR2() const;
        /**
         * @brief  设置UR2耦合状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setUR2(bool newUR2);
        /**
         * @brief  UR3耦合状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool getUR3() const;
        /**
         * @brief  设置UR3耦合状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setUR3(bool newUR3);
        /**
         * @brief  耦合权重方法
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        WeightingMethod getWeightingMethod() const;
        /**
         * @brief  设置耦合权重方法
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setWeightingMethod(WeightingMethod newWeightingMethod);
        /**
         * @brief  获取影响半径方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        InfluenceRadius getInfluenceRadius() const;
        /**
         * @brief  设置影响半径方式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setInfluenceRadius(InfluenceRadius newInfluenceRadius);
        /**
         * @brief  获取影响半径值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        double getInfluenceRadiusValue() const;
        /**
         * @brief  设置影响半径值
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setInfluenceRadiusValue(double newInfluenceRadiusValue);
        /**
         * @brief  获取adjust centrol points状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool getAdjustCentrolPoints() const;
        /**
         * @brief  设置adjust centrol points状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setAdjustCentrolPoints(bool newAdjustCentrolPoints);
        /**
         * @brief  获取坐标系Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        int getCsysId() const;
        /**
         * @brief  设置坐标系Id
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        void setCsysId(int newCsysId);

    private:
        /**
         * @brief  耦合类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        CouplingType m_couplingType{CouplingType::Kinematic};
        /**
         * @brief  U1耦合状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool m_U1{true};
        /**
         * @brief  U2耦合状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool m_U2{true};
        /**
         * @brief  U3耦合状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool m_U3{true};
        /**
         * @brief  UR1耦合状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool m_UR1{true};
        /**
         * @brief  UR2耦合状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool m_UR2{true};
        /**
         * @brief  UR3耦合状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool m_UR3{true};
        /**
         * @brief  耦合权重方法
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        WeightingMethod m_weightingMethod{WeightingMethod::Uniform};
        /**
         * @brief  影响半径方式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        InfluenceRadius m_influenceRadius{InfluenceRadius::ToOutermostPointOnTheRegion};
        /**
         * @brief  影响半径
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        double m_influenceRadiusValue{0.0};
        /**
         * @brief  adjust central points
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        bool m_adjustCentrolPoints{false};
        /**
         * @brief  坐标系Id
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-17
         */
        int m_csysId{0};
    };
}


#endif // FITKABACONSTRAINTCOUPLING_H

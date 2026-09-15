/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaProfileL.h
 * @brief  截面形状L
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABAPROFILEL_H
#define FITKABAPROFILEL_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaProfile.h"


namespace Interface {
    /**
     * @brief  L型截面类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaProfileL: public FITKAbaProfile
    {
        FITKCLASS(Interface, FITKAbaProfileL)
    public:
        /**
         * @brief  获取截面形状类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaProfileShape getShape() const override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取参数A
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getA() const;
        /**
         * @brief  获取参数B
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getB() const;
        /**
         * @brief  获取参数T1
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getT1() const;
        /**
         * @brief  获取参数T2
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getT2() const;
        /**
         * @brief  设置参数A
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setA(double value);
        /**
         * @brief  设置参数B
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setB(double value);
        /**
         * @brief  设置参数T1
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setT1(double t1);
        /**
         * @brief  设置参数T2
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setT2(double t2);
    private:
        /**
         * @brief  截面形状参数A
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_a{0.0};
        /**
         * @brief  截面形状参数B
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_b{0.0};
        /**
         * @brief  截面形状参数T1
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_t1{0.0};
        /**
         * @brief  截面形状参数T2
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_t2{0.0};
    };
}


#endif // FITKABAPROFILEL_H

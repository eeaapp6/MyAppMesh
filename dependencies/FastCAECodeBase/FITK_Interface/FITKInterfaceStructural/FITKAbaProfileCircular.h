/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaProfileCircular.h
 * @brief  截面形状Circular
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABAPROFILECIRCULAR_H
#define FITKABAPROFILECIRCULAR_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaProfile.h"


namespace Interface {
    /**
     * @brief  圆型截面类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaProfileCircular: public FITKAbaProfile
    {
        FITKCLASS(Interface, FITKAbaProfileCircular)
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
         * @brief  获取半径
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getR() const;
        /**
         * @brief  设置半径
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setR(double value);
    private:
        /**
         * @brief  半径
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_r{0.0};
    };
}


#endif // FITKABAPROFILECIRCULAR_H

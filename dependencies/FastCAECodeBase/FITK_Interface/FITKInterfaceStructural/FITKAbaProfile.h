/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaProfile.h
 * @brief  结构模板（Abaqus）截面形状
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-21
 * 
 */
#ifndef FITKABAPROFILE_H
#define FITKABAPROFILE_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"


namespace Interface {
   
    /**
     * @brief  截面形状抽象类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-21
     */
    class FITKInterfaceStructuralAPI FITKAbaProfile: public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
    public:
        /**
     * @brief  截面形状枚举类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-21
     */
        enum FITKAbaProfileShape
        {
            Box,
            Pipe,
            Circular,
            Rectangular,
            I,
            L,
        };
        Q_ENUM(FITKAbaProfileShape);
        
    public:
        /**
         * @brief  获取截面形状类型，便于类型动态转换
         * @param[in]  void
         * @return 截面形状枚举类型值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        virtual FITKAbaProfileShape getShape() const = 0;
    };

}


#endif // FITKABAPROFILE_H

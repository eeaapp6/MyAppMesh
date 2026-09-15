/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaSectionAssignSolidHomogenous.h
 * @brief  均匀材料实体截面属性赋予类型
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-25
 * 
 */
#ifndef FITKABASECTIONASSIGNSOLIDHOMOGENOUS_H
#define FITKABASECTIONASSIGNSOLIDHOMOGENOUS_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbastractSectionAssign.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"


namespace Interface {
    /**
     * @brief  均匀材料实体截面属性赋予类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-25
     */
    class FITKInterfaceStructuralAPI FITKAbaSectionAssignSolidHomogenous: public FITKAbaAbastractSectionAssign
    {
        FITKCLASS(Interface, FITKAbaSectionAssignSolidHomogenous)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaSectionAssignSolidHomogenous() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaSectionAssignSolidHomogenous() = default;
        /**
         * @brief  获取截面属性赋予类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaSectionAssignType getSectionAssignType() const override;
    };
}


#endif // FITKABASECTIONASSIGNSOLIDHOMOGENOUS_H

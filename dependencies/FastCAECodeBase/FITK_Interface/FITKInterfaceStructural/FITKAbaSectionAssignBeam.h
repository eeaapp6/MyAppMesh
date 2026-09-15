/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaSectionAssignBeam.h
 * @brief  Beam类型的截面属性赋予
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-28
 * 
 */
#ifndef FITKABASECTIONASSIGNBEAM_H
#define FITKABASECTIONASSIGNBEAM_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbastractSectionAssign.h"


namespace Interface {
    /**
     * @brief  Beam类型截面属性赋予
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-28
     */
    class FITKInterfaceStructuralAPI FITKAbaSectionAssignBeam: public FITKAbaAbastractSectionAssign
    {
        FITKCLASS(Interface, FITKAbaSectionAssignBeam)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaSectionAssignBeam() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaSectionAssignBeam() = default;
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


#endif // FITKABASECTIONASSIGNBEAM_H

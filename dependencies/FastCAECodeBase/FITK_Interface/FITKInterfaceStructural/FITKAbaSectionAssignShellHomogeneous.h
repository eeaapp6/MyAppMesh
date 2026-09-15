/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaSectionAssignShellHomogeneous.h
 * @brief  均匀介质壳截面属性赋予类型
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-25
 * 
 */
#ifndef FITKABASECTIONASSIGNSHELLHOMOGENEOUS_H
#define FITKABASECTIONASSIGNSHELLHOMOGENEOUS_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbastractSectionAssign.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITKAbaSectionAssignShell.h"


namespace Interface {
    /**
     * @brief  均匀介质壳截面属性赋予类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-25
     */
    class FITKInterfaceStructuralAPI FITKAbaSectionAssignShellHomogeneous: public FITKAbaAbastractSectionAssign
    {
        FITKCLASS(Interface, FITKAbaSectionAssignShellHomogeneous)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaSectionAssignShellHomogeneous() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaSectionAssignShellHomogeneous() = default;
        /**
         * @brief  获取截面属性赋予类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaSectionAssignType getSectionAssignType() const override;
        /**
         * @brief  获取壳偏移方式
         * @param[in]  void
         * @return ShellOffset枚举类型值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        FITKShellAssignEnum::ShellOffset getShellOffset() const;
        /**
         * @brief  设置壳偏移方式
         * @param[in]  ShellOffset枚举类型值
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setShellOffset(FITKShellAssignEnum::ShellOffset type);
        /**
         * @brief  壳厚度赋予方式
         * @param[in]  
         * @return ShellThicknessAssignment枚举类型值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        FITKShellAssignEnum::ShellThicknessAssignment getThicknessAssignment() const;
        /**
         * @brief  设置壳厚度赋予方式
         * @param[in]  ShellThicknessAssignment枚举类型值
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setThicknessAssignment(FITKShellAssignEnum::ShellThicknessAssignment type);
        /**
         * @brief  壳偏移量，当壳偏移方式为SpecifyValue时需要
         * @param[in]  
         * @return double型壳偏移量
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getShellOffsetValue() const;
        /**
         * @brief  设置壳偏移量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setShellOffsetValue(double value);

    private:
        /**
         * @brief  壳厚度赋予类型，FromSection 或者 FromGeometry
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        FITKShellAssignEnum::ShellThicknessAssignment m_thicknessAssignment{ FITKShellAssignEnum::ShellThicknessAssignment::FromSection};
        /**
         * @brief  壳偏移类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        FITKShellAssignEnum::ShellOffset m_shellOffset{ FITKShellAssignEnum::ShellOffset::MiddleSurface};
        /**
         * @brief  壳偏移尺寸
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_shellOffsetValue{0.0};
    };

}


#endif // FITKABASECTIONASSIGNSHELLHOMOGENEOUS_H

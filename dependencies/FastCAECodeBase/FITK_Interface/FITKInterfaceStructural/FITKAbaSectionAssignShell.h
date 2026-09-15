/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaSectionAssignShell.h
 * @brief  Shell类型SectionAssignment枚举类定义
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-25
 * 
 */
#ifndef FITKABASECTIONASSIGNSHELL_H
#define FITKABASECTIONASSIGNSHELL_H

#include "FITKInterfaceStructuralAPI.h"
#include <QObject>

namespace Interface {

    class FITKInterfaceStructuralAPI FITKShellAssignEnum : public QObject
    {
        Q_OBJECT
    public:
        /**
        * @brief  壳厚度的赋值方式，包括FromSection和FromGeometry
        * @author LiJin (1003680421@qq.com)
        * @date   2024-03-25
        */
        enum ShellThicknessAssignment
        {
            FromSection,
            FromGeometry,
        };
        Q_ENUM(ShellThicknessAssignment);
        /**
         * @brief  壳偏移方式，包括中面、顶面、底面、指定值和根据几何
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        enum ShellOffset
        {
            MiddleSurface,
            TopSurface,
            BottomSurface,
            SpecifyValue,
            SFFromGeometry,
        };
        Q_ENUM(ShellOffset);
    };
   

}

#endif // FITKABASECTIONASSIGNSHELL_H

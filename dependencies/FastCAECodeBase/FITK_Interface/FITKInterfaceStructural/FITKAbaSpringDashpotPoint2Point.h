/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaSpringDashpotPoint2Point.h
 * @brief  两点弹簧类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-19
 * 
 */
#ifndef FITKABASPRINGDASHPOTPOINT2POINT_H
#define FITKABASPRINGDASHPOTPOINT2POINT_H
#include "FITKAbaAbstractSpringDashpot.h"


namespace Interface {
    class FITKInterfaceStructuralAPI FITKAbaSpringDashpotPoint2Point: public FITKAbaAbstractSpringDashpot
    {
        FITKCLASS(Interface, FITKAbaSpringDashpotPoint2Point)
    public:
        FITKAbaSpringDashpotPoint2Point() = default;
        ~FITKAbaSpringDashpotPoint2Point() = default;
        FITKAbaAbstractSpringDashpot::SpringDashpotType getSpringDashpotType() const override;
        bool copy(Core::FITKAbstractDataObject* obj) override;
    };
}


#endif // FITKABASPRINGDASHPOTPOINT2POINT_H

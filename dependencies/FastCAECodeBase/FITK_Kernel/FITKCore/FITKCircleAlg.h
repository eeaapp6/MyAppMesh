/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKCircleAlg.h
 * @brief 圆相关算法声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-20
 *
 */
#ifndef _FITK_CIRCLE_ALG_H___
#define _FITK_CIRCLE_ALG_H___

#include "FITKVec3D.h"
#include <QPair>

namespace Core
{
    /**
     * @brief 根据两点与半径计算圆心位置
     * @param[i]  p1             圆上的一个点
     * @param[i]  p2             圆上第二点
     * @param[i]  another        附加点，用于确定圆所在平面，与P1 p2不共线
     * @param[i]  r              半径
     * @return QPair<FITKPoint, FITKPoint>
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-20
     */
    FITKCoreAPI QPair<FITKPoint, FITKPoint> GetCircleCenter(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& another,
                                                const double r);

    /**
     * @brief 获取圆弧上对应的弦中心点
     * @param[i]  p1             圆上的一个点
     * @param[i]  p2             圆上另外一点
     * @param[i]  center         圆心，上述三点不共线
     * @param[i]  r              半径
     * @return FITKPoint
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-20
     */
    FITKCoreAPI FITKPoint GetMidPointOnArc(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& center, const double r);

    /**
     * @brief 空间三个点计算圆
     * @param[i]  p1 p2 p3       圆上的3个点
     * @param[o]  normal         圆所在平面法向
     * @param[o]  center         圆心
     * @param[o]  r              半径
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-20
     */
    FITKCoreAPI void GetCircleBy3Points(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& p3,
                              FITKPoint& center, FITKVec3D& normal, double& r);

}


#endif // _FITK_CIRCLE_ALG_H___
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKTriangleAlg.h
 * @brief  声明三角形相关算法接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-10-24
 *********************************************************************/
#ifndef _FITKTRIANGLEALG_H__
#define _FITKTRIANGLEALG_H__

#include "FITKVec3D.h"

namespace Core
{
    /**
     * @brief  通过三点获取面积
     * @param[i]  p1 三角形顶点p1
     * @param[i]  p2 三角形顶点p2
     * @param[i]  p3 三角形顶点p3
     * @return double 返回面积
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-10-24
     */
    double FITKCoreAPI GetTriangleArea(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& p3);
    /**
     * @brief  获取三角形三边长
     * @param[i]  p1 三角形顶点p1
     * @param[i]  p2 三角形顶点p2
     * @param[i]  p3 三角形顶点p3
     * @param[o]  length 三角形三边长(p1-p2, p2-p3, p3-p1)
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-10-24
     */
    void FITKCoreAPI GetTriEdgeLength(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& p3, double* length);
    /**
     * @brief  判断一个点是否在三角形内部-点在边上也是内部
     * @param[i]  p1 三角形顶点p1
     * @param[i]  p2 三角形顶点p2
     * @param[i]  p3 三角形顶点p3
     * @param[i]  piont 需要判断的点
     * @return true-在内部或者边上， false-在外部
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-10-24
     */
    bool FITKCoreAPI IsPointInsideTriangle(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& p3, const FITKPoint& piont);


    /**
     * @brief  通过四个点获取四面体体积，输入的三角形有顺序，如果顶点在三角形法向一侧体积为正值，否则为负值
     * @param[i]  p1 底面三角形顶点p1
     * @param[i]  p2 底面三角形顶点p2
     * @param[i]  p3 底面三角形顶点p3
     * @param[i]  tip 四面体顶点
     * @return double 返回体积，包含正负值
     * @author Libaojun (libaojunqd@foxmail.com)
     * @date   2024-11-13
     */
    double FITKCoreAPI GetTet4Volume(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& p3, const FITKPoint& tip);
}



#endif

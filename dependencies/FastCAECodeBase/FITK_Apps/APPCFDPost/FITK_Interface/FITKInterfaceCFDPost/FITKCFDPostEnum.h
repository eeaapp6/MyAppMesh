/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostEnum.h
 * @brief  后处理枚举定义头文件
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-13
 *********************************************************************/
#ifndef _FITKCFDPostEnum_H
#define _FITKCFDPostEnum_H

namespace Interface
{
    enum class FITKPostDataType
    {
        Post_None,
        Post_Steady,
        Post_UnSteady,
        Post_Isosurface,   
        Post_ClipPlane,
        Post_ClipSphere,
        Post_ClipCylinder,
        Post_ClipBox,
        Post_SlicePlane,
        Post_SliceSphere,
        Post_SliceCylinder,
        Post_SliceBox,
        Post_StreamLine,
        Post_Glyph,
        Post_Threshold,
        Post_Deform,
        Post_Probe,
        Post_Ruler,
    };

    enum class FITKPostVariableType {
        Post_Error,            //错误值
        Post_Scalar,           //标量
        Post_Vector,           //矢量
        Post_Tensor,           //张量
    };

    enum class FITKPostFieldType {
        Post_None,              //空
        Post_Point,             //点
        Post_Cell,              //单元
    };

    enum class FITKPostGridType {
        Post_None,              //未知类型
        Post_Structured,        //结构化网格
        Post_UnStructured,      //非结构化网格
    };

    enum class FITKPostStreamDirection {
        Post_None,
        Post_Forward,           //向前
        Post_Backward,          //向后
        Post_Both,              //兼具
    };

    enum class FITKPostLightType {
        Post_Positional,        //位置灯光
        Post_Direction,         //方向灯光
    };

    struct FITKPostPointStr
    {
        double _X = 0.0;
        double _Y = 0.0;
        double _Z = 0.0;
    };

    struct FITKPostColorStr
    {
        double _R = 0.0;
        double _G = 0.0;
        double _B = 0.0;
    };
}

#endif
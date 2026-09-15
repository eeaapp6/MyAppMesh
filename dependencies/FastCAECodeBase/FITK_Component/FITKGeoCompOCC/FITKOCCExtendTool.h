/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOCCExtendTool.h
 * @brief  OCC的补充工具类
 * @author YanZhiHui(chanyuantiandao@126.com)
 * @date   2024-10-15
 */
#ifndef FITKOCCEXTENDTOOL_H
#define FITKOCCEXTENDTOOL_H

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

namespace OCC {
    /**
     * @brief  OCC的补充工具类.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-10-15
     */
    class FITKOCCExtendTool {

    public:
        /**
         * @brief  判断形状是否为空.
         * @param  shape 形状
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        static bool IsEmptyShape(const TopoDS_Shape& shape) {
            // TShape* == nullptr
            if (shape.IsNull()) return true;
            // 针对组合性转需要进一步判断
            return shape.NbChildren() == 0;
        }
        /**
         * @brief  获取形状的中心（包围盒的中心）.
         * @param  shape 形状
         * @return 中心点
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        static gp_Pnt center(const TopoDS_Shape& shape) {
            Bnd_Box box;
            BRepBndLib::Add(shape, box);
            Standard_Real xMin, xMax, yMin, yMax, zMin, zMax;
            box.Get(xMin, yMin, zMin, xMax, yMax, zMax);
            return { xMin + (xMax - xMin) / 2, yMin + (yMax - yMin) / 2, zMin + (zMax - zMin) / 2 };
        }
    };
}

#endif // FITKOCCEXTENDTOOL_H

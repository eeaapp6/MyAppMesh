/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef GEOCOMMANDCOMMON_H_
#define GEOCOMMANDCOMMON_H_

#include <list>
#include <vector>
#include <gp_Pln.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <Geom_Plane.hxx>
#include <Standard_Handle.hxx>

#include <QList>

namespace Geometry
{
    class GeometrySet;
}

namespace OCC
{
    /**
     * @brief       嵌套线框数据结构体。(首个对象为最外层边框或独立的不封闭边线)
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-10-13
     */
    struct EncloseWire
    {
    public:
        EncloseWire() {}
        EncloseWire(TopoDS_Wire w) : Wire(w) {}

        TopoDS_Wire Wire;
        QList<EncloseWire> Children;

        bool operator==(const EncloseWire & obj) 
        { 
            Q_UNUSED(obj); 
            return false; 
        }

        bool isNull()
        {
            return Wire.IsNull();
        }
    };

    /**
     * @brief       包围盒比较结果。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-10-13
     */
    enum BoxCompare
    {
        BC_None = -1,
        BC_Same = 0,
        BC_1_in_2,
        BC_2_in_1,
        BC_Intersect,
        BC_NonIntersect,
    };

    class  FITKOCCComandCommon
    {
    private:
        /**
         * @brief       内部结构 - 线框面积结构体。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-17
         */
        struct WireWithArea
        {
        public:
            WireWithArea() {}
            WireWithArea(TopoDS_Wire & w, double a) 
                : Wire(w), Area(a) {}

            TopoDS_Wire Wire;
            double Area = 0.;

            bool operator() (WireWithArea & w1, WireWithArea & w2)
            {
                return w1.Area > w2.Area;
            }
        };

    public:
        FITKOCCComandCommon() = default;
        ~FITKOCCComandCommon() = default;

        /**
         * @brief       平铺组合对象。[静态]
         * @param[in]   shape：输入对象[引用]
         * @param[out]  shapes：平铺子对象列表[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        static void FlattenCompShape(const TopoDS_Shape & shape, QList<TopoDS_Shape> & shapes);

        /**
         * @brief       递归平铺组合对象至非组合对象层级。[静态]
         * @param[in]   shape：组合体或组合对象[引用]
         * @param[out]  shapes：非二维形状列表[引用]
         * @param[out]  shapes2D：二维形状列表[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        static void FlattenCompShape2D(const TopoDS_Shape & shape, QList<TopoDS_Shape> & shapes, QList<TopoDS_Shape> & shapes2D);

        /**
         * @brief       递归平铺组合对象至非组合对象层级。[静态]
         * @param[in]   shape：组合体或组合对象[引用]
         * @param[out]  shapes：非三维形状列表[引用]
         * @param[out]  shapes3D：三维形状列表[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        static void FlattenCompShape3D(const TopoDS_Shape & shape, QList<TopoDS_Shape> & shapes, QList<TopoDS_Shape> & shapes3D);

        /**
         * @brief       递归平铺组合对象至非组合对象层级。[静态]
         * @param[in]   shape：组合体或组合对象[引用]
         * @param[out]  shapes：非二维三维形状列表[引用]
         * @param[out]  shapes2D：二维形状列表[引用]
         * @param[out]  shapes3D：三维形状列表[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-24
         */
        static void FlattenCompShape2D3D(const TopoDS_Shape & shape, QList<TopoDS_Shape> & shapes, 
            QList<TopoDS_Shape> & shapes2D, QList<TopoDS_Shape> & shapes3D);

        /**
         * @brief       递归平铺组合对象至非组合对象层级。[静态]
         * @param[in]   shape：组合体或组合对象[引用]
         * @param[out]  shapes：非独立面、线、点形状列表[引用]
         * @param[out]  shapesBase：独立面、线、点形状列表[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        static void FlattenCompShapeBaseTopo(const TopoDS_Shape & shape, QList<TopoDS_Shape> & shapes, QList<TopoDS_Shape> & shapesBase);

        /**
         * @brief       多维度模型融合，支持一二三维模型之间互相求和与分割。[静态]
         * @param[in]   shape1：第一个形状
         * @param[in]   shape2：第二个形状
         * @param[out]  result：合并后的结果形状
         * @param[in]   noNerg：是否执行非正则布尔[缺省]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-26
         */
        static bool MergeShapes(const TopoDS_Shape & shape1, const TopoDS_Shape & shape2, TopoDS_Shape & result, bool noNerg = false);

        /**
         * @brief       多维度模型融合，支持一二三维模型之间互相求和与分割。[静态]
         * @param[in]   shapes：形状列表
         * @param[out]  result：合并后的结果形状
         * @param[in]   noNerg：是否执行非正则布尔[缺省]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-29
         */
        static bool MergeShapes(QList<TopoDS_Shape>& shapes, TopoDS_Shape& result, bool noNerg = false);

        /**
         * @brief       将给定形状拆分成边并组合成线框。[静态]
         * @param[in]   shape：边、线框或组合体[引用]
         * @param[out]  wires：线框列表[引用]
         * @param[in]   tol：容差[缺省]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-02
         */
        static void MakeWires(const TopoDS_Shape & shape, QList<TopoDS_Wire> & wires, double tol = 1e-6);

        /**
         * @brief       将给定形状拆分成边并组合成线框，根据包围盒尺寸进行分组。[静态]
         * 
         *              P.S.
         *              注意，此方法仅限于所有线框均共平面情况下使用，并且内部不会检测线框间自相交。
         * 
         * @param[in]   shape：边、线框或组合体[引用]
         * @param[out]  wiresList：嵌套线框组列表[引用]
         * @param[in]   pln：线框所在平面
         * @param[in]   tol：容差[缺省]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        static void MakeWires(const TopoDS_Shape & shape, QList<EncloseWire> & wiresList, gp_Pln pln, double tol = 1e-6);

        /**
         * @brief       平铺线框组。[静态]
         * @param[in]   iWiresList：输入线框组[引用]
         * @param[out]  oWiresList：输出线框组[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        static void FlattenWires(QList<EncloseWire> & iWiresList, QList<EncloseWire> & oWiresList);

        /**
         * @brief       将多个嵌套线框组创建为一组面。[静态]
         * @param[in]   wiresList：线框组[引用]
         * @param[out]  faces：结果面列表[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        static void MakeFaces(QList<EncloseWire> & wiresList, QList<TopoDS_Face> & faces);

        /**
         * @brief       将给定形状拆分成边并组合成线框组合对象。[静态]
         * @param[in]   shape：边、线框或组合体[引用]
         * @param[out]  wires：线框组合对象[引用]
         * @param[in]   tol：容差[缺省]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-02
         */
        static void MakeWires(const TopoDS_Shape & shape, TopoDS_Shape & wires, double tol = 1e-6);

        /**
         * @brief       判断盒子是否包含另一个盒子。[静态]
         * @param[in]   box1：盒子1
         * @param[in]   box2：盒子2
         * @param[in]   tol：容差[缺省]
         * @return      包含状态：-1：错误值，0：盒子相等，1：盒子1包含盒子2，2：盒子2包含盒子1，3：盒子相交
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        static BoxCompare CompareBox(const Bnd_Box & box1, const Bnd_Box & box2, double tol = 1e-6);

        /**
         * @brief       容差小数比较。[静态]
         * @param[in]   p1：数值1
         * @param[in]   p2：数值2
         * @param[in]   tol：容差[缺省]
         * @return      是否相等
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        static bool FuzzyCompare(double p1, double p2, double tol = 1e-12);

        static std::vector<TopoDS_Wire> bulidWire(std::list<TopoDS_Edge>& edgeList);
        static TopoDS_Shape makeFace(std::vector<TopoDS_Wire>& wires);
        static bool isInside(const TopoDS_Wire&, const TopoDS_Wire&);
        static TopoDS_Face validateFace(const TopoDS_Face&);
        static TopoDS_Shape removeSplitter(const TopoDS_Shape& shape);
        static bool isEmpty(const TopoDS_Shape &shape);
        //从inputShape中移除component，返回移除之前的副本
        static TopoDS_Shape removeShape(TopoDS_Shape* inputShape, TopoDS_Shape* component);
        static gp_Ax2 getEdgeAxis(Geometry::GeometrySet* set, int edgeindex);
        static gp_Ax2 getFaceAxis(Geometry::GeometrySet* set, int index);

    private:
        static TopoDS_Shape makeFace(std::list<TopoDS_Wire>&);

        /**
         * @brief       内部工具方法 - 将给定线框递归查询包围盒尺寸并追加至子节点。[静态]
         * @param[in]   pew：上层嵌套结构[引用]
         * @param[in]   ew：嵌套结构[引用]
         * @param[in]   wire：给定新线框[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        static bool addToChildren(EncloseWire & pew, EncloseWire & ew, TopoDS_Wire & wire, Handle(Geom_Plane) & plane,
            Standard_Real uFirst, Standard_Real uLast, Standard_Real vFirst, Standard_Real vLast);

    };












    // sort bounding boxes according to diagonal length
    class Wire_Compare : public std::binary_function<const TopoDS_Wire&,
        const TopoDS_Wire&, bool>
    {
    public:
        bool operator() (const TopoDS_Wire& w1, const TopoDS_Wire& w2)
        {
            Bnd_Box box1, box2;
            if (!w1.IsNull())
            {
                BRepBndLib::Add(w1, box1);
                box1.SetGap(0.0);
            }

            if (!w2.IsNull())
            {
                BRepBndLib::Add(w2, box2);
                box2.SetGap(0.0);
            }

            return box1.SquareExtent() < box2.SquareExtent();
        }
    };
}

#endif
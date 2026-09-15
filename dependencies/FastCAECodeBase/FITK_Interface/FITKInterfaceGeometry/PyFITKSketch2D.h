/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PyFITKSketch2D.h
 * @brief  草图数据工具 为python装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2025-06-25
 */
#ifndef _PY_FITKSketch2D_H___
#define _PY_FITKSketch2D_H___

#include <QString>
#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoSketch2D.h"

namespace Interface
{
    /**
     * @brief python草图数据
     * @author wangning (2185896382@qq.com)
     * @date 2025-06-25
     */
    class FITKInterfaceGeometryAPI PyFITKSketch2D : public QObject
    {
        Q_OBJECT
        /**
         * @brief       草图形状类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-06
         */
        enum SketchShape
        {
            SS_NONE = -1,

            // 形状。
            SS_Vertex = 0,  // 点
            SS_Wire,        // 多段线
            SS_Rect,        // 矩形
            SS_Circle,      // 整圆
            SS_Ellipse,     // 椭圆
            SS_ArcOfCircle, // 圆上截取圆弧
            SS_ArcOfTan,    // 两点圆弧
            SS_ArcOfP3,     // 三点圆弧
            SS_Spline,      // 样条曲线 

            // 构造线。
            SS_ConLine,         // 两点直线
            SS_ConHLine,        // 水平线
            SS_ConVLine,        // 垂直线
            SS_ConAngleLine,    // 角度线
            SS_ConCircle,       // 圆

        };

    public:
        /**
        * @brief  草图数据
        * @param[i] UseFor 类型
        * @author wangning (2185896382@qq.com)
        * @date   2025-06-25
        */
        explicit PyFITKSketch2D(int UseFor);
        virtual ~PyFITKSketch2D();

        /**
         * @brief  设置平面
         * @param[i]   pos：面中心点坐标 nor：面法向 up：面向上方向
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        bool setPlane(const QVector<double>& pos_nor_up);
        /**
         * @brief  添加绘制
         * @param[i] typeShape 形状类型 point图形所需坐标
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        bool addDraw(int typeShape, const QVector<double>& point);
        /**
         * @brief  获取草图ID
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        int getSketchID();
        /**
        * @brief  设置当前草图
        * @author wangning (2185896382@qq.com)
        * @date   2025-06-25
        */
        bool setCurrentSketch(const QString& name);
        /**
         * @brief  获取草图指针
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoCommand* getSketch();
        /**
         * @brief  判断草绘是否有效(如果自相交未封闭则无效)
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        bool checkSketch();
        /**
         * @brief  删除绘制
         * @param[i] num：第几次绘制 indexs：索引
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        bool eraserEdge(int num, const QList<int>& indexs);
        /**
        * @brief  删除构造线
        * @param[i] indexs：索引
        * @author wangning (2185896382@qq.com)
        * @date   2025-06-25
        */
        bool eraserConstructions(const QList<int>& indexs);
        /**
         * @brief  检查草绘是否可用（如果已被添加到全局链表，代表已被Part使用，则草绘不可用）
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        bool isUsable();

    public slots:
        /**
         * @brief 删除草图槽函数
         * @author wangning (2185896382@qq.com)
         * @date 2025-01-02
         */
        void sketchToNull(int id);

    private:
        /**
         * @brief  通过索引获取虚拓扑链表
         * @param[i] num草图第几个绘图命令 cmdID几何命令ID index特征索引 type类型
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        QList<Interface::VirtualShape> getShapes(int num, const QList<int>& indexs, Interface::FITKGeoEnum::VTopoShapeType type);

    private:
        /**
         * @brief  草图数据
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        Interface::FITKAbsGeoSketch2D* _sketch2D{};
        /**
         * @brief  多段线的上一个点
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        QVector<double> _tempLastPoint{};
        /**
         * @brief  当前脚本草图ID
         * @author wangning (2185896382@qq.com)
         * @date   2025-06-25
         */
        int _sketch2DId{ -1 };
    };
}

#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    GraphEventOperator.h
 * @brief   三维可视对象数据操作器接口类。
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-12
 *********************************************************************/

#ifndef __GRAPHEVENTOPERATOR_H__
#define __GRAPHEVENTOPERATOR_H__

#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"

#include "OperatorsInterfaceAPI.h"

#include <QColor>
#include <QVariant>

namespace Exchange
{
    class FITKOCCGraphObject3D;
}

namespace EventOper
{
    /**
     * @brief   预览方式。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-04
     */
    enum PreviewType
    {
        // 默认方式。（完整数据）
        PT_Default = 0,

        // 模型标号。
        PT_IndexLabels,

        // 方向。
        PT_Direction,
    };

    /**
     * @brief   三维可视对象数据操作器接口类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-12
     */
    class OperatorsInterfaceAPI GraphEventOperator : public Core::FITKAbstractOperator
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        explicit GraphEventOperator() = default;

        /**
         * @brief   析构函数。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        virtual ~GraphEventOperator() = default;

        // 前处理渲染与窗口功能。
        //@{
        /**
         * @brief   初始化三维窗口元素。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-08
         */
        virtual void initialize3D();

        /**
         * @brief   根据提供的数据对象ID更新对应可视化对象。（没有则创建）[虚函数]
         * @param   dataObjId：数据对象ID
         * @param   forceUpdate：是否强制刷新[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        virtual void updateGraph(int dataObjId, bool forceUpdate = false);

        /**
         * @brief   刷新全部数据可视化对象。（没有则创建，目前仅适用于读取工程。）[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        virtual void updateGraphs();

        /**
         * @brief   根据提供的数据对象ID递归更新对应可视化对象。（没有则不创建，目前仅适用于建模操作器。）[虚函数]
         * @param   dataObjId：数据对象ID
         * @param   refLayer：更新引用数据的层级（-1则递归至根数据）[缺省]
         * @param   recDown：向下递归（被引用方向查询）[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        virtual void updateGraphRecursively(int dataObjId, int refLayer = 2, bool recDown = true);

        /**
         * @brief   根据提供的数据对象ID更新对应可视化对象可见性。[虚函数]
         * @param   dataObjId：数据对象ID
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        virtual void updateGraphVisibility(int dataObjId);

        /**
         * @brief   预览数据。[虚函数]
         * @param   dataObjId：数据对象ID
         * @param   type：预览类型[缺省]
         * @param   color：显示颜色[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-20
         */
        virtual void preview(int dataObjId, PreviewType type = PT_Default, QColor color = QColor());

        /**
         * @brief   手动清除预览可视化对象。[虚函数]
         * @param   dataObjId：数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-03
         */
        virtual void clearPreview(int dataObjId = -1);

        /**
         * @brief   显示几何分组可视化对象。[虚函数]
         * @param   dataObjId：数据ID
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        virtual void showGlobalComponent(int dataObjId);

        /**
         * @brief   清除显示几何分组可视化对象。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        virtual void clearGlobalComponent();

        /**
         * @brief   设置基准元素可见性。[虚函数]
         * @param   visible：是否可见
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-30
         */
        virtual void setDatumsVisibility(bool visible);

        /**
         * @brief   设置草图可视化对象置顶。[虚函数]
         * @param   dataObjId：草图数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-13
         */
        virtual void setSketchTopMost(int dataObjId = -1);

        /**
         * @brief   取消全部草图可视化对象置顶。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-13
         */
        virtual void clearSketchTopMost();

        /**
         * @brief   设置几何局部透明度。[虚函数]
         * @param   cmdId：几何命令ID
         * @param   geomType：几何形状类型枚举
         * @param   geoIndice：几何对象索引列表
         * @param   trans：透明度
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-07-07
         */
        virtual void setShapeTransparency(int cmdId, int geomType, QList<int> geoIndice, double trans);

        /**
         * @brief   局部隐藏几何。[虚函数]
         * @param   cmdId：几何命令ID
         * @param   geomType：几何形状类型枚举
         * @param   geoIndice：几何对象索引列表
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-07-07
         */
        virtual void setShapeHidden(int cmdId, int geomType, QList<int> geoIndice);

        /**
         * @brief   取消设置全部面透明度。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-07-07
         */
        virtual void clearShapeFaceTransparency();
        //@}

        // 高亮功能接口。
        //@{
        /**
         * @brief   高亮可视化对象。[虚函数]
         * @param   dataObjId：数据对象ID
         * @param   info：附加信息[缺省]
         * @param   color：高亮颜色[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-30
         */
        virtual void highlight(int dataObjId, QVariant info = QVariant(), QColor color = QColor());

        /**
         * @brief   取消全部高亮、高级高亮数据的高亮效果。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-30
         */
        virtual void clearHighlight();
        //@}

        // 渲染专用接口。
        //@{
        /**
         * @brief   刷新渲染窗口。[虚函数]
         * @param   fitView：是否重置相机
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-24
         */
        virtual void reRender(bool fitView = false);
        //@}

        // 开启草绘模式。
        //@{
        /**
         * @brief   开启草绘模式
         * @param   isOn：开启或关闭草绘模式
         * @param   virtualTopoID：虚拓扑ID
         * @return  true 成功 false 失败
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
         */
        virtual bool enableSketchMode(bool isOn, int virtualTopoID);

        /**
         * @brief   开启草绘模式。[虚函数]
         * @param   isOn：开启或关闭草绘模式
         * @param   pos：中心
         * @param   nor：法向
         * @param   up：向上方向
         * @return  是否成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-20
         */
        virtual bool enableSketchMode(bool isOn, double* pos, double* nor, double* up);
        //@}

    };
}   // namespace EventOper

#endif // __GRAPHEVENTOPERATOR_H__
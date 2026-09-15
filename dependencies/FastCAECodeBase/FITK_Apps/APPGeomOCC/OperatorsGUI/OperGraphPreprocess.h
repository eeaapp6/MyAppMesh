/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperGraphPreprocess.h
 * @brief   前处理可视化事件操作器。
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-12
 *********************************************************************/

#ifndef _OPERGRAPHPREPROCESS_H__
#define _OPERGRAPHPREPROCESS_H__

#include "OperGraphEvent3D.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUIOper
{
    /**
     * @brief   前处理可视化事件处理。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-12
     */
    class OperGraphPreprocess : public OperGraphEvent3D
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        explicit OperGraphPreprocess() = default;

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        ~OperGraphPreprocess() = default;

        // 前处理渲染与窗口功能。
        //@{
        /**
         * @brief   初始化三维窗口元素。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-08
         */
        void initialize3D() override;

        /**
         * @brief   根据提供的数据对象ID更新对应可视化对象。（没有则创建）[重写]
         * @param   dataObjId：数据对象ID
         * @param   forceUpdate：是否强制刷新[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void updateGraph(int dataObjId, bool forceUpdate = false) override;

        /**
         * @brief   刷新全部数据可视化对象。（没有则创建，目前仅适用于读取工程。）[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        void updateGraphs() override;

        /**
         * @brief   根据提供的数据对象ID递归更新对应可视化对象。（没有则不创建，目前仅适用于建模操作器。）[重写]
         * @param   dataObjId：数据对象ID
         * @param   refLayer：更新引用数据的层级（-1则递归至根数据）[缺省]
         * @param   recDown：向下递归（被引用方向查询）[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        void updateGraphRecursively(int dataObjId, int refLayer = 2, bool recDown = true) override;

        /**
         * @brief   根据提供的数据对象ID更新对应可视化对象可见性。[重写]
         * @param   dataObjId：数据对象ID
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        void updateGraphVisibility(int dataObjId) override;

        /**
         * @brief   预览数据。[重写]
         * @param   dataObjId：数据对象ID
         * @param   type：预览类型[缺省]
         * @param   color：显示颜色[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-20
         */
        void preview(int dataObjId, EventOper::PreviewType type = EventOper::PT_Default, QColor color = QColor()) override;

        /**
         * @brief   手动清除预览可视化对象。[重写]
         * @param   dataObjId：数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-03
         */
        void clearPreview(int dataObjId = -1) override;

        /**
         * @brief   显示几何分组可视化对象。[重写]
         * @param   dataObjId：数据ID
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        void showGlobalComponent(int dataObjId) override;

        /**
         * @brief   清除显示几何分组可视化对象。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        void clearGlobalComponent() override;

        /**
         * @brief   设置基准元素可见性。[重写]
         * @param   visible：是否可见
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-30
         */
        void setDatumsVisibility(bool visible) override;

        /**
         * @brief   设置草图可视化对象置顶。[重写]
         * @param   dataObjId：草图数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-13
         */
        void setSketchTopMost(int dataObjId = -1) override;

        /**
         * @brief   取消全部草图可视化对象置顶。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-13
         */
        void clearSketchTopMost() override;

        /**
         * @brief   设置几何局部透明度。[重写]
         * @param   cmdId：几何命令ID
         * @param   geomType：几何形状类型枚举
         * @param   geoIndice：几何对象索引列表
         * @param   trans：透明度
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-07-07
         */
        void setShapeTransparency(int cmdId, int geomType, QList<int> geoIndice, double trans) override;

        /**
         * @brief   局部隐藏几何。[重写]
         * @param   cmdId：几何命令ID
         * @param   geomType：几何形状类型枚举
         * @param   geoIndice：几何对象索引列表
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-07-07
         */
        void setShapeHidden(int cmdId, int geomType, QList<int> geoIndice) override;

        /**
         * @brief   取消设置全部面透明度。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-07-07
         */
        void clearShapeFaceTransparency() override;
        //@}

        // 高亮功能接口。
        //@{
        /**
         * @brief   高亮整个可视化对象。[重写]
         * @param   dataObjId：数据对象ID
         * @param   info：附加信息[缺省]
         * @param   color：高亮颜色[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-05
         */
        void highlight(int dataObjId, QVariant info = QVariant(), QColor color = QColor()) override;

        /**
         * @brief   取消全部高亮、高级高亮数据的高亮效果。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-05
         */
        void clearHighlight() override;
        //@}

        // 渲染专用接口。
        //@{
        /**
         * @brief   刷新渲染窗口。[重写]
         * @param   fitView：是否重置相机
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-24
         */
        void reRender(bool fitView = false) override;
        //@}

        // 功能中转调用接口。
        //@{
        /**
         * @brief   开启草绘模式
         * @param   isOn：开启或关闭草绘模式
         * @param   virtualTopoID：虚拓扑ID
         * @return  true 成功 false 失败
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
         */
        virtual bool enableSketchMode(bool isOn, int virtualTopoID) override;

        /**
         * @brief   开启草绘模式。[重写]
         * @param   isOn：开启或关闭草绘模式
         * @param   pos：中心
         * @param   nor：法向
         * @param   up：向上方向
         * @return  是否成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-20
         */
        bool enableSketchMode(bool isOn, double* pos, double* nor, double* up) override;

    };

    Register2FITKOperatorRepo(GraphPreprocess, OperGraphPreprocess);

}  // namespace GUIOper

#endif

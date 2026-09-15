/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperGraphTool.h
 * @brief   前处理可视化工具操作器。
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-18
 *********************************************************************/

#ifndef _OPERGRAPHTOOL_H__
#define _OPERGRAPHTOOL_H__

#include "OperatorsInterface/GraphToolOperator.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

 // 前置声明
namespace Render
{
    class FITKGraph3DWindowOCC;
}

namespace Exchange
{
    class FITKOCCGraphObject3D;
}

namespace GUIOper
{
    /**
     * @brief   前处理可视化工具操作器。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-18
     */
    class OperGraphTool : public EventOper::GraphToolOperator
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        explicit OperGraphTool() = default;

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        ~OperGraphTool() = default;

        /**
         * @brief   测量。[重写]
         * @param   items：数据对象ID列表
         * @param   color：显示颜色[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        void measure(QList<int> items, EventOper::MeasureToolType type, QColor color = QColor()) override;

        /**
         * @brief   手动清除测量可视化对象。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        void clearMeasure() override;

        /**
         * @brief   初始化拾取工具（创建并添加至窗口）。[重写]
         * @param   calculateCenter：是否拾取形状中心位置
         * @param   type：拾取工具类型
         * @param   color：显示颜色[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        void initPickTool(bool calculateCenter, EventOper::PickToolType type, QColor color = QColor()) override;

        /**
         * @brief   手动清除拾取工具可视化对象。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        void clearPickTool() override;

        /**
         * @brief   获取拾取工具可视化对象。[重写]
         * @param   type：工具类型
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        Exchange::FITKOCCGraphObject3D* getPickToolObject(EventOper::PickToolType type) override;

        /**
         * @brief   获取拾取工具可视化对象。[重写]
         * @param   type：工具类型
         * @param   visible：是否显示
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        void setPickToolObjectVisible(EventOper::PickToolType type, bool visible) override;

        /**
         * @brief   设置拾取方向工具锚定虚拓扑对象ID。[重写]
         * @param   mainId：（如：命令ID、基准元素ID）
         * @param   subId：次级数据ID（如：虚拓扑ID）
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        void setPickDirectionAnchorTopoId(int mainId, int subId) override;

        /**
         * @brief   设置拾取方向工具用户自定义方向。[重写]
         * @param   dir：方向
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        void setPickDirectionUserDirection(QList<double> dir) override;

        /**
         * @brief   获取拾取工具当前方向。[重写]
         * @param   dir：方向[引用]
         * @return  拾取工具当前是否有存储的方向数据
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        bool getPickedDirection(QList<double> & dir) override;

        /**
         * @brief   获取拾取形状坐标工具拾取到的投影坐标。[重写]
         * @param   pos：坐标[引用]
         * @return  拾取工具当前是否有存储的坐标数据
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        bool getPickedProjPosition(QList<double> & pos) override;

        /**
         * @brief   设置拾取形状坐标工具投影目标对象ID。[重写]
         * @param   targetCmdId：（如：命令ID、基准元素ID）
         * @param   targetTopoId：次级数据ID（如：虚拓扑ID）
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-18
         */
        void setPickProjPosTargetId(int targetCmdId, int targetTopoId) override;

    private:
        /**
         * @brief   获取三维可视化窗口。
         * @return  可视化窗口
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        Render::FITKGraph3DWindowOCC* getGraphWidget();

        /**
         * @brief   添加可视化对象至三维窗口。
         * @param   obj：可视化对象
         * @param   graphWidget：三维窗口
         * @param   fitView：是否重置视角[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        void addGraphObjectToWidget(Exchange::FITKOCCGraphObject3D* obj, Render::FITKGraph3DWindowOCC* graphWidget, bool fitView = false);

    };

    Register2FITKOperatorRepo(GraphTool, OperGraphTool);

}  // namespace GUIOper

#endif // _OPERGRAPHTOOL_H__

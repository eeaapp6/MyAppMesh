/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    GraphToolOperator.h
 * @brief   三维可视化工具操作器接口类。
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-18
 *********************************************************************/

#ifndef __GRAPHTOOLOPERATOR_H__
#define __GRAPHTOOLOPERATOR_H__

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
    * @brief   测量工具类型。
    * @author  ChengHaotian (yeguangbaozi@foxmail.com)
    * @date    2024-09-18
    */
    enum MeasureToolType
    {
        // 位置。
        MTT_Position = 0,

        // 距离。
        MTT_Distance,

        // 夹角。
        MTT_Angle,

        // 面积。
        MTT_Area,

        // 体积。
        MTT_Volume

    };

    /**
     * @brief   拾取工具类型。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-18
     */
    enum PickToolType
    {
        // 边/面投影坐标。
        PTT_Proj_Pos = 0,

        // 方向。
        PTT_Direction

    };

    /**
     * @brief   三维可视化工具操作器接口类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-18
     */
    class OperatorsInterfaceAPI GraphToolOperator : public Core::FITKAbstractOperator
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        explicit GraphToolOperator() = default;

        /**
         * @brief   析构函数。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual ~GraphToolOperator() = default;      

        /**
         * @brief   测量。[虚函数]
         * @param   items：数据对象ID列表
         * @param   type：测量工具类型
         * @param   color：显示颜色[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual void measure(QList<int> items, MeasureToolType type, QColor color = QColor());

        /**
         * @brief   手动清除测量可视化对象。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual void clearMeasure();

        /**
         * @brief   初始化拾取工具（创建并添加至窗口）。[虚函数]
         * @param   calculateCenter：是否拾取形状中心位置
         * @param   type：拾取工具类型
         * @param   color：显示颜色[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual void initPickTool(bool calculateCenter, PickToolType type, QColor color = QColor());

        /**
         * @brief   手动清除拾取工具可视化对象。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual void clearPickTool();

        /**
         * @brief   获取拾取工具可视化对象。[虚函数]
         * @param   type：工具类型
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual Exchange::FITKOCCGraphObject3D* getPickToolObject(PickToolType type);

        /**
         * @brief   获取拾取工具可视化对象。[虚函数]
         * @param   type：工具类型
         * @param   visible：是否显示
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual void setPickToolObjectVisible(PickToolType type, bool visible);

        /**
         * @brief   设置拾取方向工具锚定虚拓扑对象ID。[虚函数]
         * @param   mainId：（如：命令ID、基准元素ID）
         * @param   subId：次级数据ID（如：虚拓扑ID）
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual void setPickDirectionAnchorTopoId(int mainId, int subId);

        /**
         * @brief   设置拾取方向工具用户自定义方向。[虚函数]
         * @param   dir：方向
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual void setPickDirectionUserDirection(QList<double> dir);

        /**
         * @brief   获取拾取工具当前方向。[虚函数]
         * @param   dir：方向[引用]
         * @return  拾取工具当前是否有存储的方向数据
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual bool getPickedDirection(QList<double> & dir);

        /**
         * @brief   获取拾取形状坐标工具拾取到的投影坐标。[虚函数]
         * @param   pos：坐标[引用]
         * @return  拾取工具当前是否有存储的坐标数据
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual bool getPickedProjPosition(QList<double> & pos);

        /**
         * @brief   设置拾取形状坐标工具投影目标对象ID。[虚函数]
         * @param   targetCmdId：（如：命令ID、基准元素ID）
         * @param   targetTopoId：次级数据ID（如：虚拓扑ID）
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        virtual void setPickProjPosTargetId(int targetCmdId, int targetTopoId);

    };
}   // namespace EventOper

#endif // __GRAPHTOOLOPERATOR_H__
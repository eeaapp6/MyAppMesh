/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    GraphMarkerProvider.h
 * @brief   三维工具符号可视化数据管理。
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-10
 *********************************************************************/

#ifndef __GRAPHMARKERPROVIDER_H__
#define __GRAPHMARKERPROVIDER_H__

#include "GraphProviderBase.h"

#include "GraphDataProviderAPI.h"

#include <QHash>

// 前置声明

namespace GraphData
{
    /**
     * @brief   测量工具类型。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-10
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
     * @date    2024-09-13
     */
    enum PickToolType
    {
        // 枚举起始。
        PTT_Begin = 0,

        // 边投影坐标。
        PTT_Proj_Pos = PTT_Begin,

        // 方向。
        PTT_Direction,

        // 枚举结束。
        PTT_End

    };

    /**
     * @brief   三维工具符号可视化数据管理。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-10
     */
    class GRAPHDATAPROVIDERAPI GraphMarkerProvider : public GraphProviderBase
    {
        Q_OBJECT

        // 友元，防止外部手动创建或析构。
        friend class GraphProviderManager;

    public:
        /**
         * @brief   获取当前算例已实例化的可视化对象。
         * @return  可视化对象列表。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        QList<Exchange::FITKOCCGraphObject3D*> getCurrentGraphObjs() override;

        /**
         * @brief   获取类名。[重写]
         * @return  类名
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        QString getClassName() override;

        /**
         * @brief   获取测量工具可视化对象。
         * @param   items：被测量对象
         * @param   type：工具类型
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        Exchange::FITKOCCGraphObject3D* getMeasureToolGraphObject(QList<int> items, MeasureToolType type);

        /**
         * @brief   获取拾取工具。
         * @param   type：工具类型
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-13
         */
        Exchange::FITKOCCGraphObject3D* getPickToolGraphObject(PickToolType type);

        /**
         * @brief   获取全部拾取工具。（没有则创建）
         * @return  可视化对象列表
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-08
         */
        QList<Exchange::FITKOCCGraphObject3D*> getAllPickToolGraphObjects();

        /**
         * @brief   清除测量工具可视化对象。
         * @param   dataObjId：数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        void clearMeasureObject();

        /**
         * @brief   清除拾取工具可视化对象。
         * @param   dataObjId：数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-13
         */
        void clearPickToolObject();     

    private:
        /**
         * @brief   构造函数。
         * @param   graphWidget：可视化窗口
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        GraphMarkerProvider(Render::FITKGraph3DWindowOCC* graphWidget);

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        ~GraphMarkerProvider();

    private:
        /**
         * @brief   测量工具可视化对象字典。（键为程序预设测量工具类型）
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        QHash<MeasureToolType, Exchange::FITKOCCGraphObject3D*> m_measureObjHash;

        /**
         * @brief   拾取工具可视化对象字典。（键为程序预设拾取工具类型）
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-13
         */
        QHash<PickToolType, Exchange::FITKOCCGraphObject3D*> m_pickToolObjHash;

    };
}   // namespace GraphData

#endif // __GRAPHMARKERPROVIDER_H__

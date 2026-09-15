/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphMarkProvider.h
 * @brief  三维可视化数据管理器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-04-17
 *********************************************************************/
#ifndef __GRAPHMARKPROVIDER_H__
#define __GRAPHMARKPROVIDER_H__

#include "GraphProviderBase.h"

#include "GraphDataProviderAPI.h"

#include <QHash>

namespace AbaqusData {
    class FITKDataCase;
}

namespace GraphData
{
    /**
     * @brief  三维可视化数据管理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-02-13
     */
    class GraphDataProviderAPI GraphMarkProvider : public GraphProviderBase
    {
        Q_OBJECT

            // 友元，防止外部手动创建或析构。
            friend class GraphProviderManager;

    public:
        /**
         * @brief   获取当前算例已实例化的可视化对象。
         * @return  可视化对象列表。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        QList<Exchange::GraphVTKObject3D*> getCurrentGraphObjs() override;

        /**
         * @brief   获取类名。[重写]
         * @return  类名
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        QString getClassName() override;
    private:
        /**
         * @brief    构造函数
         * @param[i] graphWidget                    渲染界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        GraphMarkProvider(Comp::FITKGraph3DWindowVTK* graphWidget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-11
         */
        ~GraphMarkProvider();
    private:
    };
}   // namespace GraphData

#endif // __GRAPHMARKPROVIDER_H__

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    GraphModelProvider.h
 * @brief   三维模型可视化数据管理。
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-12
 *********************************************************************/

#ifndef __GRAPHMODELPROVIDER_H__
#define __GRAPHMODELPROVIDER_H__

#include "GraphProviderBase.h"

#include "GraphDataProviderAPI.h"

#include "FITK_Component/FITKRenderWindowOCC/TypeDefOCCWindow.h"

#include <QHash>

namespace GraphData
{
    /**
     * @brief   三维可视化数据分类管理类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-12
     */
    class GRAPHDATAPROVIDERAPI GraphModelProvider : public GraphProviderBase
    {
        Q_OBJECT

        // 友元，防止外部手动创建或析构。
        friend class GraphProviderManager;

    public:
        /**
         * @brief   获取当前算例已实例化的可视化对象。
         * @return  可视化对象列表。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        QList<Exchange::FITKOCCGraphObject3D*> getCurrentGraphObjs() override;

        /**
         * @brief   获取类名。[重写]
         * @return  类名
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        QString getClassName() override;

        /**
         * @brief   根据数据ID获取模型（几何）可视化对象。（没有则创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        Exchange::FITKOCCGraphObject3D* getModelGraphObject(int dataObjId);

        /**
         * @brief   根据数据ID获取模型（几何）预览可视化对象。（没有则创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-28
         */
        Exchange::FITKOCCGraphObject3D* getModelPreviewGraphObject(int dataObjId);

        /**
         * @brief   获取全部模型（几何）可视化对象。
         * @param   createGraph：可视化对象不存在是否需要创建[缺省]
         * @return  可视化对象列表
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-30
         */
        QList<Exchange::FITKOCCGraphObject3D*> getAllModelGraphObjects(bool createGraph = false);

        /**
         * @brief   根据数据ID获取2D草图可视化对象。（没有则创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        Exchange::FITKOCCGraphObject3D* getSketch2DGraphObject(int dataObjId);

        /**
         * @brief   获取全部2D草图可视化对象。（没有则不创建）
         * @param   createGraph：可视化对象不存在是否需要创建[缺省]
         * @return  可视化对象列表
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        QList<Exchange::FITKOCCGraphObject3D*> getAllSkecth2DGraphObjects(bool createGraph = false);

        /**
         * @brief   获取全部建模可视化对象。（没有则不创建）
         * @param   createGraph：可视化对象不存在是否需要创建[缺省]
         * @return  可视化对象列表
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-20
         */
        QList<Exchange::FITKOCCGraphObject3D*> getAllCommandGraphObjects(bool createGraph = false);

        /**
         * @brief   根据数据ID获取基准元素可视化对象。（没有则创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-30
         */
        Exchange::FITKOCCGraphObject3D* getDatumGraphObject(int dataObjId);
        
        /**
         * @brief   根据数据ID获取基准元素预览可视化对象。（没有则创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        Exchange::FITKOCCGraphObject3D* getDatumPreviewGraphObject(int dataObjId);        

        /**
         * @brief   获取全部基准元素可视化对象。
         * @param   createGraph：可视化对象不存在是否需要创建[缺省]
         * @return  可视化对象列表
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-30
         */
        QList<Exchange::FITKOCCGraphObject3D*> getAllDatumGraphObjects(bool createGraph = false);

        /**
         * @brief   根据数据ID获取全局几何分组可视化对象。（没有则创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        Exchange::FITKOCCGraphObject3D* getGlobalComponentGraphObject(int dataObjId);

        /**
         * @brief   获取全部几何分组可视化对象。
         * @param   createGraph：可视化对象不存在是否需要创建[缺省]
         * @return  可视化对象列表
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        QList<Exchange::FITKOCCGraphObject3D*> getGlobalComponentGraphObjects(bool createGraph = false);

        /**
         * @brief   根据虚拓扑数据ID获取虚拓扑形状预览可视化对象。（没有则创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-06
         */
        Exchange::FITKOCCGraphObject3D* getTopoShapePreviewGraphObject(int dataObjId);

        /**
         * @brief   根据虚拓扑数据ID获取索引标签预览可视化对象。（没有则创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-03
         */
        Exchange::FITKOCCGraphObject3D* getTopoLabelPreviewGraphObject(int dataObjId);

        /**
         * @brief   根据虚拓扑数据ID获取形状方向预览可视化对象。（没有则创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        Exchange::FITKOCCGraphObject3D* getTopoDirectionPreviewGraphObject(int dataObjId);
        
        /**
         * @brief   根据数据ID获取对应可视化对象。（没有则不创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        Exchange::FITKOCCGraphObject3D* getCurrentGraphObjByDataId(int dataObjId);

        /**
         * @brief   根据数据ID更新可视化对象，没有则跳出。
         * @param   dataObjId：数据ID
         * @param   info：附加信息[缺省]
         * @return  是否存在该数据ID所对应可视化对象。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        bool updateObjById(int dataObjId, QVariant info = QVariant());

        /**
         * @brief   开启给定模型可视化对象拾取状态。（ID为-1则开启所有对象拾取状态）
         * @param   mode：拾取模式
         * @param   dataObjectId：开启拾取状态的数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        void setModelSelectMode(CommonOCC::SelectMode mode, int dataObjectId = -1);

        /**
         * @brief   开启给定草图可视化对象拾取状态。（ID为-1则开启所有对象拾取状态）
         * @param   mode：拾取模式
         * @param   dataObjectId：开启拾取状态的数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        void setSketchSelectMode(CommonOCC::SelectMode mode, int dataObjectId = -1);

        /**
         * @brief   开启给定基准元素可视化对象拾取状态。（ID为-1则开启所有对象拾取状态）
         * @param   mode：拾取模式
         * @param   dataObjectId：开启拾取状态的数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        void setDatumSelectMode(CommonOCC::SelectMode mode, int dataObjectId = -1);

        /**
         * @brief   开启给定草图可视化对象拾取内部线状态。（ID为-1则开启所有对象拾取状态）
         * @param   dataObjectId：开启拾取状态的数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-14
         */
        void setSketchSelectEdgeMode(int dataObjectId = -1);

        /**
         * @brief   根据给定数据ID清除预览对象。
         * @param   dataObjId：数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-03
         */
        void clearPreviewObject(int dataObjId = -1);

    private:
        /**
         * @brief   构造函数。
         * @param   graphWidget：可视化窗口
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        GraphModelProvider(Render::FITKGraph3DWindowOCC* graphWidget);

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        ~GraphModelProvider();

    private:
        /**
         * @brief   模型（几何）可视化对象字典。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        QHash<int, Exchange::FITKOCCGraphObject3D*> m_modelObjHash;

        /**
         * @brief   2D草图可视化对象字典。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        QHash<int, Exchange::FITKOCCGraphObject3D*> m_sketchObjHash;

        /**
         * @brief   模型（几何）及其他预览可视化对象字典。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-28
         */
        QHash<int, Exchange::FITKOCCGraphObject3D*> m_previewObjHash;

        /**
         * @brief   基准元素可视化对象字典。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-30
         */
        QHash<int, Exchange::FITKOCCGraphObject3D*> m_datumObjHash;

        /**
         * @brief   全局几何分组可视化对象字典。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        QHash<int, Exchange::FITKOCCGraphObject3D*> m_componentObjHash;

    };
}   // namespace GraphData

#endif // __GRAPHMODELPROVIDER_H__

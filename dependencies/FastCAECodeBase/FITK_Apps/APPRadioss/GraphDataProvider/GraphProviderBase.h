/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    GraphProviderBase.h
 * @brief   三维可视化数据管理基类。
 *
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-04-17
 *********************************************************************/

#ifndef __GRAPHPROVIDERBASE_H__
#define __GRAPHPROVIDERBASE_H__

#include <QObject>

#include "GraphDataProviderAPI.h"

#include <QHash>

 // 前置声明
namespace Core
{
    class FITKAbstractDataObject;
}
namespace Interface
{
    class FITKAbsGeoCommand;
}

namespace Comp
{
    class FITKGraph3DWindowVTK;
}

namespace Exchange
{
    class GraphVTKObject3D;
}

namespace Radioss
{
    class FITKAbstractRWall;
}

namespace GraphData
{
    /**
     * @brief   三维可视化数据分类管理类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-12
     */
    class GraphDataProviderAPI GraphProviderBase : public QObject
    {
        Q_OBJECT

            // 友元，防止外部手动创建或析构。
            friend class GraphProviderManager;

    public:
        /**
         * @brief   获取当前算例已实例化的可视化对象。[纯虚函数]
         * @return  可视化对象列表。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        virtual QList<Exchange::GraphVTKObject3D*> getCurrentGraphObjs() = 0;

        /**
         * @brief   获取当前算例已实例化的当前可见可视化对象。
         * @return  可视化对象列表。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        QList<Exchange::GraphVTKObject3D*> getCurrentVisibleGraphObjs();

        /**
         * @brief   获取类名。[虚函数]
         * @return  类名
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        virtual QString getClassName();

        /**
         * @brief   根据数据ID更新可视化对象，没有则跳出。[虚函数]
         * @param   dataId：数据ID
         * @param   info：附加信息[缺省]
         * @return  是否存在该数据ID所对应可视化对象。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        virtual bool updateObjById(int dataId, QVariant info = QVariant());

        /**
         * @brief   更新全部当前可见可视化对象显示状态。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void updateVisibility();

        /**
         * @brief   清除全部管理中的可视化对象高亮与高级高亮效果。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-30
         */
        void clearHighlight();

    protected:
        /**
         * @brief   构造函数。
         * @param   graphWidget：可视化窗口
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        GraphProviderBase(Comp::FITKGraph3DWindowVTK* graphWidget);

        /**
         * @brief   析构函数。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        virtual ~GraphProviderBase();

        /**
         * @brief   移除数据管理字典中的所有可视化对象数据并析构。
         * @param   hash：数据字典
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void deleteObjsHash(QHash<int, Exchange::GraphVTKObject3D*>& hash);

        /**
         * @brief   移除数据管理字典中的所有可视化对象数据并析构。
         * @param   hash：数据字典
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void deleteObjsHash(QHash<int, QHash<int, Exchange::GraphVTKObject3D*>>& hash);
        /**
         * @brief    通过数据对象、适配器关键字与需要保存到的字典生成或获取可视化对象
         * @param[i] adaptorKeyName                         适配器关键字
         * @param[i] objDict                                可视化对象字典[引用]
         * @param[i] dataObj                                数据对象
         * @return   Exchange::GraphVTKObject3D*            渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-28
         */
        Exchange::GraphVTKObject3D* getGraphObject(QString adaptorKeyName, QHash<int, Exchange::GraphVTKObject3D*>& objDict, Core::FITKAbstractDataObject* dataObj);
        /**
         * @brief    获取几何渲染对象
         * @param[i] keyName               渲染适配器key
         * @param[i] objDict               渲染对象管理链表
         * @param[i] geoCom                几何数据对象
         * @return   GraphObjectBase*      渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-31
         */
        Exchange::GraphVTKObject3D* getGeoGraphObject(QString keyName, QHash<int, Exchange::GraphVTKObject3D*>& objDict, Interface::FITKAbsGeoCommand* geoCom);
        /**
         * @brief    通过数据对象、适配器关键字与需要保存到的字典生成或获取集合或表面可视化对象
         * @param[i] key                                    适配器关键字
         * @param[i] objDict                                可视化对象字典[引用]
         * @param[i] dataObj                                数据对象
         * @param[i] modeId                                 集合或表面所属模型id
         * @return   Exchange::GraphVTKObject3D*            渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-28
         */
        Exchange::GraphVTKObject3D* getSetOrSurfGraphObject(QString key, QHash<int, QHash<int, Exchange::GraphVTKObject3D*>>& objDict, Core::FITKAbstractDataObject* dataObj, int modeId);
        /**
         * @brief    通过集合对象，调用可视化适配创建集合可视化数据
         * @param[i] adaptorKeyName                                           适配器关键字
         * @param[i] dataObj                                                  数据对象
         * @param[i] parentId                                                 Instance、Part、Assembly数据ID
         * @return   Exchange::GraphVTKObject3D *                             可视化对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-05
         */
        Exchange::GraphVTKObject3D* getGraphObjectBySet(QString adaptorKeyName, Core::FITKAbstractDataObject* dataObj, int parentId);
    protected:
        /**
         * @brief   管理器所管理的可视化窗口。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        Comp::FITKGraph3DWindowVTK* m_graphWidget{ nullptr };

        /**
         * @brief   临时预览可视化对象数据字典。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        QHash<int, Exchange::GraphVTKObject3D*> m_previewObjHash = {};

    };
}   // namespace GraphData

#endif // __GRAPHPROVIDERBASE_H__

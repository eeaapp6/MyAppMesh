/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    GraphProviderManager.h
 * @brief   三维数据管理类。
 *
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-04-17
 *********************************************************************/

#ifndef __GRAPHPROVIDERMANAGER_H__
#define __GRAPHPROVIDERMANAGER_H__

#include <QObject>

#include "GraphDataProviderAPI.h"

#include <QMutex>
#include <QMutexLocker>
#include <QHash>

#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"

 // 前置声明
namespace Comp
{
    class FITKGraph3DWindowVTK;
}

namespace GraphData
{
    class GraphProviderBase;
    class GraphModelProvider;
    class GraphMarkProvider;
    class GraphPostProvider;
}

namespace GraphData
{
    /**
     * @brief   管理所有三维数据分装类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-03-27
     */
    class GraphDataProviderAPI GraphProviderManager : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief   获取单例实例。[静态]
         * @return  实例
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        static GraphProviderManager* getInstance();

        /**
         * @brief   手动销毁。[静态]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        static void Delete();

        /**
         * @brief   获取三维模型可视化对象分装类，没有则初始化。
         * @param   graphWidget：可视化窗口
         * @return  三维模型数据分装类指针
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        GraphModelProvider* getModelProvider(Comp::FITKGraph3DWindowVTK* graphWidget);

        /**
         * @brief   获取符号可视化对象分装类，没有则初始化。
         * @param   graphWidget：可视化窗口
         * @return  符号可视化对象分装类指针
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        GraphMarkProvider* getMarkProvider(Comp::FITKGraph3DWindowVTK* graphWidget);
        /**
         * @brief    获取后处理可视化对象分装类，没有则初始化
         * @param[i] graphWidget                         可视化窗口
         * @param[i] isInit                              是否创建
         * @return   GraphPostOperator*                  后处理可视化对象分装类
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-13
         */
        GraphPostProvider* getPostProvider(Comp::FITKGraph3DWindowVTK* graphWidget,bool isInit = true);

        /**
         * @brief   在所有算例数据管理尝试寻找该数据ID对应的可视化对象并刷新，如果不存在则跳出。
         * @param   dataId：需要更新的数据ID
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void updateObjectById(int dataId);
        /**
         * @brief    根据当前的拾取信息设置演员的拾取状态
         * @param[i] pickObjType             拾取对象类型
         * @param[i] pickMethod              拾取对象方法
         * @param[i] dataObjId               生效数据ID（-1则对所有数据生效）[缺省]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setActorStateByPickInfo(int pickObjType, int pickMethod, int dataObjId = -1);
    private:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        GraphProviderManager();

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        ~GraphProviderManager();

        /**
         * @brief   析构数据管理器。
         * @param   provider：数据管理器[引用]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        template<class T>
        void deleteProvider(T*& provider)
        {
            if (provider)
            {
                delete provider;
            }

            provider = nullptr;
        }

        /**
         * @brief   批量析构数据管理器。
         * @param   providers：数据管理器列表[引用]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        template<class T>
        void deleteProviders(QList<T*>& providers)
        {
            // 清除数据管理指针。
            for (T* obj : providers)
            {
                if (obj)
                {
                    delete obj;
                }
            }

            providers.clear();
        }

    private:
        /**
         * @brief   静态实例。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        static GraphProviderManager* s_instance;

        /**
         * @brief   静态锁。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        static QMutex m_mutex;
        /**
         * @brief  三维窗口中模型可视化对象分装类
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-02-13
         */
        GraphModelProvider* m_modelProvider{ nullptr };
        /**
         * @brief  三维窗口中符号可视化对象分装类
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-02-13
         */
        GraphMarkProvider* m_markProvider{ nullptr };
        /**
         * @brief  三维窗口中后处理可视化对象分装类
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-02-13
         */
        QHash<int, GraphPostProvider*> m_postProvider = {};

    };
}   // namespace GraphData

#endif // __GRAPHPROVIDERMANAGER_H__

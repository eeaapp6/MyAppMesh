/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphPostProvider.h
 * @brief  三维后处理可视化数据管理
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-09-10
 *********************************************************************/

#ifndef _GRAPHPOSTPROVIDER_H__
#define _GRAPHPOSTPROVIDER_H__

#include "GraphDataProviderAPI.h"
#include "GraphProviderBase.h"

namespace GraphData
{
    /**
     * @brief  三维后处理可视化数据管理
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-09-10
     */
    class GraphDataProviderAPI GraphPostProvider : public GraphProviderBase
    {
        //友元，防止外部手动创建或析构
        friend class GraphProviderManager;
        Q_OBJECT;
    public:
        /**
         * @brief    获取当前算例已实例化的可视化对象
         * @return   QList<Exchange::GraphVTKObject3D*>
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-10
         */
        QList<Exchange::GraphVTKObject3D*> getCurrentGraphObjs() override;
        /**
         * @brief    通过id获取渲染对象
         * @param[i] dataObjId 数据对象
         * @param[i] isInit    是否初始化
         * @return   Exchange::GraphVTKObject3D *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-10
         */
        Exchange::GraphVTKObject3D* getModelGraphObject(int dataObjId, bool isInit = true);
    protected:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        GraphPostProvider(Comp::FITKGraph3DWindowVTK* graphWidget);
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        virtual ~GraphPostProvider();
    protected:
        /**
         * @brief  后处理
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        QHash<int, Exchange::GraphVTKObject3D*> _postObjHash = {};
    };
}
#endif // !_GRAPHPOSTPROVIDER_H__
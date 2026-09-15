/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKViewAdaptorPost.h
 * @brief  后处理模型可视化适配器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-09-10
 *********************************************************************/

#ifndef _GRAPHVTKVIEWADAPTORPOST_H__
#define _GRAPHVTKVIEWADAPTORPOST_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKViewAdaptorBase.h"

namespace Exchange
{
    /**
     * @brief  后处理模型可视化适配器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-09-10
     */
    class GraphDataVTKAdaptorAPI GraphVTKViewAdaptorPost : public GraphVTKViewAdaptorBase
    {
        FITKCLASS(Exchange, GraphVTKViewAdaptorPost);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        explicit GraphVTKViewAdaptorPost() = default;
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        ~GraphVTKViewAdaptorPost() = default;
        /**
         * @brief    创建可视化数据
         * @return   bool
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-10
         */
        bool update() override;
    };
    Register2FITKViewAdaptorFactory(PostModel, Interface::FITKAbstractStructuralPostVTK, GraphVTKViewAdaptorPost);
}
#endif // !_GRAPHVTKVIEWADAPTORPOST_H__

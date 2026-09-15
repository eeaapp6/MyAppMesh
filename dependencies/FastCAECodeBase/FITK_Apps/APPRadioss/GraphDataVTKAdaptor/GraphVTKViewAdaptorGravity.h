/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKViewAdaptorGravity.h
 * @brief  重力载荷渲染适配器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-28
 *********************************************************************/
#ifndef _GRAPHVTKVIEWADAPTORGRAVITY_H__
#define _GRAPHVTKVIEWADAPTORGRAVITY_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKViewAdaptorBase.h"

namespace Exchange
{
    /**
     * @brief  重力载荷渲染适配器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-08-28
     */
    class GraphDataVTKAdaptorAPI GraphVTKViewAdaptorGravity : public GraphVTKViewAdaptorBase
    {
        FITKCLASS(Exchange, GraphVTKViewAdaptorGravity);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-28
         */
        explicit GraphVTKViewAdaptorGravity() = default;
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-28
         */
        ~GraphVTKViewAdaptorGravity() = default;
        /**
         * @brief  更新
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-28
         */
        bool update() override;
    };
    Register2FITKViewAdaptorFactory(RadiossGravity, Radioss::FITKGravity, GraphVTKViewAdaptorGravity);
}
#endif // !_GRAPHVTKVIEWADAPTORGRAVITY_H__

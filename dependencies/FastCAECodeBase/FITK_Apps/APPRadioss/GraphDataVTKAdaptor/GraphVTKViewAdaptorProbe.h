/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKViewAdaptorProbe.h
 * @brief  探针渲染适配器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-12-04
 *********************************************************************/
#ifndef _GraphVTKViewAdaptorProbe_H__
#define _GraphVTKViewAdaptorProbe_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKViewAdaptorBase.h"

namespace Radioss {
    class FITKProbeAcceleration;
    class FITKProbeSection;
}

namespace Exchange
{
    /**
     * @brief  探针渲染适配器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-12-04
     */
    class GraphDataVTKAdaptorAPI GraphVTKViewAdaptorProbe : public GraphVTKViewAdaptorBase
    {
        FITKCLASS(Exchange, GraphVTKViewAdaptorProbe);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-18
         */
        explicit GraphVTKViewAdaptorProbe() = default;
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-18
         */
        ~GraphVTKViewAdaptorProbe() = default;
        /**
         * @brief  更新
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-18
         */
        bool update() override;
    };
    Register2FITKViewAdaptorFactory(RadiossProbeSection, Radioss::FITKProbeSection, GraphVTKViewAdaptorProbe);
    Register2FITKViewAdaptorFactory(RadiossProbeAccel, Radioss::FITKProbeAcceleration, GraphVTKViewAdaptorProbe);
}

#endif // !_GraphVTKViewAdaptorProbe_H__

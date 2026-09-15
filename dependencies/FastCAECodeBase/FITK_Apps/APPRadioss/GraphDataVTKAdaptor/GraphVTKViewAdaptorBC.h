/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKViewAdaptorBC.h
 * @brief  边界条件可视化适配器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-10-27
 *********************************************************************/

#ifndef _GraphVTKViewAdaptorBC_H__
#define _GraphVTKViewAdaptorBC_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKViewAdaptorBase.h"

namespace Radioss
{
    class FITKRadiossBCS;
    class FITKRadiossBCSCyclic;
}

namespace Exchange
{
    /**
     * @brief  边界条件可视化适配器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-10-27
     */
    class GraphDataVTKAdaptorAPI GraphVTKViewAdaptorBC : public GraphVTKViewAdaptorBase
    {
        FITKCLASS(Exchange, GraphVTKViewAdaptorBC);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-10-27
         */
        explicit GraphVTKViewAdaptorBC() = default;
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-10-27
         */
        ~GraphVTKViewAdaptorBC() = default;
        /**
         * @brief  更新
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-10-27
         */
        bool update() override;
    };
    Register2FITKViewAdaptorFactory(RadiossBCS, Radioss::FITKRadiossBCS, GraphVTKViewAdaptorBC);
    Register2FITKViewAdaptorFactory(RadiossBCSCyclic, Radioss::FITKRadiossBCSCyclic, GraphVTKViewAdaptorBC);
    Register2FITKViewAdaptorFactory(RadiossBCSLagMul, Radioss::FITKRadiossBCSLagMul, GraphVTKViewAdaptorBC);
}
#endif // !_GRAPHVTKVIEWADPATORFIELD_H__

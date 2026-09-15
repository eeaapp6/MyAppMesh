/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKViewAdaptorPartMesh.h
 * @brief  part网格可视化适配器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-07-18
 *********************************************************************/
#ifndef _GRAPHVTKVIEWADAPTORPARTMESH_H__
#define _GRAPHVTKVIEWADAPTORPARTMESH_H__

#include "GraphVTKViewAdaptorBase.h"
#include "GraphDataVTKAdaptorAPI.h"

namespace Radioss {
    class FITKRadiossPart;
}
namespace Exchange {

    /**
     * @brief  part网格可视化适配器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-07-18
     */
    class GraphDataVTKAdaptorAPI GraphVTKViewAdaptorPartMesh : public GraphVTKViewAdaptorBase
    {

        FITKCLASS(Exchange, GraphVTKViewAdaptorPartMesh);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-18
         */
        explicit GraphVTKViewAdaptorPartMesh() = default;
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-18
         */
        ~GraphVTKViewAdaptorPartMesh() = default;
        /**
         * @brief  更新
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-18
         */
        bool update() override;
    };
    Register2FITKViewAdaptorFactory(RadiossPart, Radioss::FITKRadiossPart, GraphVTKViewAdaptorPartMesh);
}

#endif // !GRAPHVTKVIEWADAPTORPARTMESH_H__

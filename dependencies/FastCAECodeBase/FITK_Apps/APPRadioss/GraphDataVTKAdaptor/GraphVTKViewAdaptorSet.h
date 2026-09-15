/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKViewAdaptorSet.h
 * @brief  集合适配器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-21
 *********************************************************************/
#ifndef __GraphVTKViewAdaptorSet_H__
#define __GraphVTKViewAdaptorSet_H__

#include "GraphVTKViewAdaptorBase.h"

#include "GraphDataVTKAdaptorAPI.h"

namespace Exchange
{
    /**
     * @brief  集合适配器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-08-21
     */
    class GraphDataVTKAdaptorAPI GraphVTKViewAdaptorSet : public GraphVTKViewAdaptorBase
    {
        FITKCLASS(Exchange, GraphVTKViewAdaptorSet);
    public:
        /**
         * @brief    构造函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-21
         */
        explicit GraphVTKViewAdaptorSet() = default;
        /**
         * @brief    析构函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-21
         */
        virtual ~GraphVTKViewAdaptorSet() = default;
        /**
         * @brief    执行适配
         * @return   bool                    是否成功
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-21
         */
        bool update() override;

    };
    Register2FITKViewAdaptorFactory(SetNode, Radioss::FITKRadiossNodeGroup, GraphVTKViewAdaptorSet);
    Register2FITKViewAdaptorFactory(SetEle, Radioss::FITKRadiossElementGroup, GraphVTKViewAdaptorSet);
}

#endif

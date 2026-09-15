/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelMergeEdges.h
 * @brief       OCC合并边功能实现。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-02-09
 *********************************************************************/

#ifndef  __FITKOCCMODELMERGEEDGES_H__
#define  __FITKOCCMODELMERGEEDGES_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoMergeEdges.h"

namespace OCC
{
    /**
     * @brief       OCC合并边命令。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-02-09
     */
    class FITKGeoCompOCCAPI FITKOCCModelMergeEdges :
        public Interface::FITKAbsGeoMergeEdges, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-09
         */
        FITKOCCModelMergeEdges();
        
        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-09
         */
        virtual ~FITKOCCModelMergeEdges() = default;

        /**
         * @brief       执行命令。[虚函数][重写]
         * @return      是否执行成功。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-09
         */
        virtual bool update() override;

    };
}

#endif // !__FITKOCCMODELMERGEEDGES_H__

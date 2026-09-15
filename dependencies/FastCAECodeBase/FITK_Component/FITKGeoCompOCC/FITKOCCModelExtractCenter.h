/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKOCCModelExtractCenter.h
 * @brief   OCC Extract Center Command Class
 * @date    2025-03-27
 * @author  libaojun
 *********************************************************************/

#ifndef __FITKOCCMODELEXTRACTCENTER_H__
#define __FITKOCCMODELEXTRACTCENTER_H__

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExtractCenter.h"
#include "FITKAbstractOCCModel.h"

namespace OCC
{
    /**
    * @brief OCC Extract Center Command Class
    * @date 2025-03-27
    * @author libaojun
    */
    class FITKGeoCompOCCAPI FITKOCCModelExtractCenter : public Interface::FITKAbsGeoModelExtractCenter, public OCCShapeAgent
    {
    public:
        /**
         * @brief Construct a new FITKOCCModelExtractCenter object
         * @date 2025-03-27
         * @author libaojun
         */
        explicit FITKOCCModelExtractCenter();
        virtual ~FITKOCCModelExtractCenter() = default;

        /**
         * @brief 更新函数，重写基类虚函数
         * @return 成功返回 true，失败返回 false
         * @date 2025-03-27
         * @author libaojun
         */
        bool update() override;

    private:
        /**
         * @brief 处理球面情况
         * @return 成功返回 true，失败返回 false
         * @date 2025-03-27
         * @author libaojun
         */
        bool handleSphere();

        /**
         * @brief 处理圆弧情况
         * @return 成功返回 true，失败返回 false
         * @date 2025-03-27
         * @author libaojun
         */
        bool handleArc();
    };
}

#endif
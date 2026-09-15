/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITKOCCSPLITTER_H
#define FITKOCCSPLITTER_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSplitter.h"

namespace OCC {
    /**
         * @brief  曲线分割抽象类.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
    class FITKGeoCompOCCAPI FITKOCCCurveSplitter :
        public Interface::FITKAbsGeoCurveSplitter, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCCurveSplitter();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCCurveSplitter() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  面分割抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKGeoCompOCCAPI FITKOCCSurfaceSplitter :
        public Interface::FITKAbsGeoSurfaceSplitter, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCSurfaceSplitter();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCSurfaceSplitter() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  体分割抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKGeoCompOCCAPI FITKOCCSolidSplitter :
        public Interface::FITKAbsGeoSolidSplitter, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCSolidSplitter();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCSolidSplitter() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

}
#endif // FITKOCCSPLITTER_H

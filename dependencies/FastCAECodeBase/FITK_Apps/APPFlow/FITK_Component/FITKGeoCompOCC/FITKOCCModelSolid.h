/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef  FITKOCCMODELSOLID_H
#define  FITKOCCMODELSOLID_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperSolid.h"

namespace OCC
{
    /**
     * @brief  封闭曲面形成的体.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCModelClosedSurfaceSolid :
        public Interface::FITKAbsGeoModelClosedSurfaceSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelClosedSurfaceSolid();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelClosedSurfaceSolid() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
    /**
     * @brief  拉伸曲面OCC类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCModelExtrudeSolid :
        public Interface::FITKAbsGeoModelExtrudeSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelExtrudeSolid();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelExtrudeSolid() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;

    };

    /**
     * @brief  旋转曲面OCC类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCModelRevolSolid :
        public Interface::FITKAbsGeoModelRevolSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelRevolSolid();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelRevolSolid() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;

    };

    /**
     * @brief  扫略体OCC类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCModelSweepSolid :
        public Interface::FITKAbsGeoModelSweepSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelSweepSolid();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelSweepSolid() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;

    };

    /**
     * @brief  多截面扫略抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCModelMultiSectionSolid :
        public Interface::FITKAbsGeoModelMultiSectionSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelMultiSectionSolid();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelMultiSectionSolid() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
}

#endif // !FITKOCCMODELSOLID_H

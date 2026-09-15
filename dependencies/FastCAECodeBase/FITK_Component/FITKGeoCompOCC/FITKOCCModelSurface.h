/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef  FITKOCCMODELSURFACE_H
#define  FITKOCCMODELSURFACE_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCoverEdges.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelShellFromSolid.h"

namespace OCC
{
    /**
     * @brief       实体抽壳形成壳面几何命令OCC实现类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-07-06
     */
    class FITKGeoCompOCCAPI FITKOCCModelShellFromSolid :
        public Interface::FITKAbsGeoModelShellFromSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        FITKOCCModelShellFromSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        virtual ~FITKOCCModelShellFromSolid() = default;

        /**
         * @brief       更新模型。
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        virtual bool update() override;
    };

    /**
     * @brief  封闭曲线形成的面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCModelClosedWireSurface :
        public Interface::FITKAbsGeoModelClosedWireSurface, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelClosedWireSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelClosedWireSurface() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };


    /**
     * @brief       覆盖边创建面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-29
     */
    class FITKGeoCompOCCAPI FITKOCCModelCoverEdges : public Interface::FITKAbsGeoCoverEdges, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        FITKOCCModelCoverEdges();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        virtual ~FITKOCCModelCoverEdges() = default;

        /**
         * @brief       刷新命令。[重写]
         * @return      执行结果
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        bool update() override;

    };

    /**
     * @brief  偏移曲面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCModelOffsetSurface :
        public Interface::FITKAbsGeoModelOffsetSurface, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelOffsetSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelOffsetSurface() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;

    };

    /**
     * @brief  拉伸曲面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCModelExtrudeSurface :
        public Interface::FITKAbsGeoModelExtrudeSurface, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelExtrudeSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelExtrudeSurface() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;

    };

    /**
     * @brief  旋转曲面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCModelRevolSurface :
        public Interface::FITKAbsGeoModelRevolSurface, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelRevolSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelRevolSurface() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;

    };

    /**
     * @brief  扫略曲面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCModelSweepSurface :
        public Interface::FITKAbsGeoModelSweepSurface, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelSweepSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelSweepSurface() = default;
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
    class FITKGeoCompOCCAPI FITKOCCModelMultiSectionSurface :
        public Interface::FITKAbsGeoModelMultiSectionSurface, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelMultiSectionSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelMultiSectionSurface() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  桥接曲面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelBridgeSurface :
        public Interface::FITKAbsGeoModelBridgeSurface, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelBridgeSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelBridgeSurface() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  体的面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelSolidSurface :
        public Interface::FITKAbsGeoModelSolidSurface, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKOCCModelSolidSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKOCCModelSolidSurface() = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

}

#endif // !FITKOCCMODELSURFACE_H

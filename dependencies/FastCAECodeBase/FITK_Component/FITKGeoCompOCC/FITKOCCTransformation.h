/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITKOCCTRANSFORMATION_H
#define FITKOCCTRANSFORMATION_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoTransformation.h"

namespace OCC {

    /**
     * @brief  平移.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-10-15
     */
    class FITKGeoCompOCCAPI FITKOCCModelTransform :
        public Interface::FITKAbsGeoModelTransform, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelTransform();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelTransform() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  通过两点平移.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-10-15
     */
    class FITKGeoCompOCCAPI FITKOCCModelTransformByTwoPoints :
        public Interface::FITKAbsGeoModelTransformByTwoPoints, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelTransformByTwoPoints();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelTransformByTwoPoints() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  通过方向和距离.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-10-15
     */
    class FITKGeoCompOCCAPI FITKOCCModelTransformByDirAndDis :
        public Interface::FITKAbsGeoModelTransformByDirAndDis, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelTransformByDirAndDis();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelTransformByDirAndDis() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  旋转.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-10-15
     */
    class FITKGeoCompOCCAPI FITKOCCModelRotate :
        public Interface::FITKAbsGeoModelRotate, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelRotate();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelRotate() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  缩放.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-10-15
     */
    class FITKGeoCompOCCAPI FITKOCCModelScale :
        public Interface::FITKAbsGeoModelScale, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelScale();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelScale() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  镜像.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-10-15
     */
    class FITKGeoCompOCCAPI FITKOCCModelMirror :
        public Interface::FITKAbsGeoModelMirror, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelMirror();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelMirror() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  矩阵阵列.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-10-15
     */
    class FITKGeoCompOCCAPI FITKOCCModelRectangularPattern :
        public Interface::FITKAbsGeoModelRectangularPattern, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelRectangularPattern();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelRectangularPattern() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  环形阵列.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-10-15
     */
    class FITKGeoCompOCCAPI FITKOCCModelCircularPattern :
        public Interface::FITKAbsGeoModelCircularPattern, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelCircularPattern();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelCircularPattern() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };
}

#endif // FITKOCCTRANSFORMATION_H

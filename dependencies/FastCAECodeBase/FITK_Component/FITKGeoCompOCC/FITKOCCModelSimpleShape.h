/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/


#ifndef  __FITK_OCC_MODEL_SIMPLESHAPE_H__
#define  __FITK_OCC_MODEL_SIMPLESHAPE_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelBox.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCylinder.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCone.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelFrustum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSphere.h"

namespace OCC
{
    /**
     * @brief 长方体
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-05
     */
    class FITKGeoCompOCCAPI FITKOCCModelBox :public Interface::FITKAbsGeoModelBox,
         public OCCShapeAgent
    {
    //    FITKCLASS(OCC, FITKAbstractOCCModel);
    public: 
        /**
         * @brief Construct a new FITKOCCModelBox object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        explicit FITKOCCModelBox();
        /**
         * @brief Destroy the FITKOCCModelBox object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual ~FITKOCCModelBox() = default;
    
        /**
         * @brief 更新模型
         * @return true 更新成功
         * @return false 更新失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual bool update() override; 

    };

    class FITKGeoCompOCCAPI FITKOCCModelCylinder : public Interface::FITKAbsGeoModelCylinder,
        public OCCShapeAgent
    {
 //       FITKCLASS(OCC, FITKAbstractOCCModel);
    public:
        /**
         * @brief Construct a new FITKOCCModelBox object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        explicit FITKOCCModelCylinder();
        /**
         * @brief Destroy the FITKOCCModelBox object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual ~FITKOCCModelCylinder() = default;

        /**
         * @brief 更新模型
         * @return true 更新成功
         * @return false 更新失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual bool update() override;
    };

    /**
     * @brief       圆锥创建命令OCC实现。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-08
     */
    class FITKGeoCompOCCAPI FITKOCCModelCone : public Interface::FITKAbsGeoModelCone,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        explicit FITKOCCModelCone();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        virtual ~FITKOCCModelCone() = default;

        /**
         * @brief       更新模型。
         * @return      true: 更新成功
         * @return      false: 更新失败
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        virtual bool update() override;

    };

    /**
     * @brief       棱台创建命令OCC实现。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-08
     */
    class FITKGeoCompOCCAPI FITKOCCModelFrustum : public Interface::FITKAbsGeoModelFrustum,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        explicit FITKOCCModelFrustum();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        virtual ~FITKOCCModelFrustum() = default;

        /**
         * @brief       更新模型。
         * @return      true: 更新成功
         * @return      false: 更新失败
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-08
         */
        virtual bool update() override;

    };

    class FITKGeoCompOCCAPI FITKOCCModelSphere : public Interface::FITKAbsGeoModelSphere,
        public OCCShapeAgent
    {
    //        FITKCLASS(OCC, FITKAbstractOCCModel);
    public:
        /**
         * @brief Construct a new FITKOCCModelBox object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        explicit FITKOCCModelSphere();
        /**
         * @brief Destroy the FITKOCCModelBox object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual ~FITKOCCModelSphere() = default;

        /**
         * @brief 更新模型
         * @return true 更新成功
         * @return false 更新失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual bool update() override;
    };
}

#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelMergePart.h
 * @brief       OCC模型合并命令实现类。
 * 
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-13
 *********************************************************************/

#ifndef  __FITKOCCMODELMERGEPART_H__
#define  __FITKOCCMODELMERGEPART_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelMergePart.h"

namespace OCC
{
    /**
     * @brief       OCC模型合并命令实现类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelMergePart : public Interface::FITKAbsGeoModelMergePart,
        public OCCShapeAgent
    {
        FITKCLASS(OCC, FITKOCCModelMergePart);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        explicit FITKOCCModelMergePart();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKOCCModelMergePart() = default;

        /**
         * @brief       更新模型。[重写]
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual bool update() override;

        /**
         * @brief       执行布尔求交运算并更新模型。
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        bool updateBoolCommon();

        /**
         * @brief       执行布尔求差运算并更新模型。
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        bool updateBoolCut();

        /**
         * @brief       执行布尔求和运算并更新模型。
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        bool updateBoolAdd();

    };
}

#endif

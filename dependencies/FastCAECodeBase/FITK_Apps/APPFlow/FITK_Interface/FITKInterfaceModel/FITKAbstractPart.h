/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbstractPart.h
 * @brief       抽象部件模型。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-09-26
 *********************************************************************/

#ifndef __FITKABSTRACTPART_H__
#define __FITKABSTRACTPART_H__

#include "FITKAbstractModel.h"
#include "FITKInterfaceModelAPI.h"

// 前置声明。

namespace Interface
{
    /**
     * @brief       抽象部件模型。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-26
     */
    class FITKInterfaceModelAPI FITKAbstractPart : public FITKAbstractModel
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        explicit FITKAbstractPart() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        virtual ~FITKAbstractPart() = default;

        /**
         * @brief       获取抽象类型网格。[纯虚函数]
         * @return      网格数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        virtual Core::FITKAbstractNDataObject* getAbstractMesh() = 0;

        /**
         * @brief       获取指定类型网格。[模板]
         * @return      网格数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        template <class T>
        T* getMeshAs()
        {
            return dynamic_cast<T*>(getAbstractMesh());
        }

        /**
         * @brief       获取抽象类型几何。[纯虚函数]
         * @return      几何数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        virtual Core::FITKAbstractNDataObject* getAbstractGeom() = 0;

        /**
         * @brief       获取指定类型几何。[模板]
         * @return      几何数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        template <class T>
        T* getGeomAs()
        {
            return dynamic_cast<T*>(getAbstractGeom());
        }

    };
}

#endif //!__FITKABSTRACTPART_H__

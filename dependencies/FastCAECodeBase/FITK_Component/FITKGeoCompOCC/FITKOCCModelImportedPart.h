/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelImportedPart.h
 * @brief       导入的几何部件形状。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-27
 *********************************************************************/

#ifndef __FITKOCCMODELIMPORTEDPART_H__
#define __FITKOCCMODELIMPORTEDPART_H__

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoImportedPart.h"
#include "FITKAbstractOCCModel.h"
#include "FITKGeoCompOCCAPI.h"

// 前置声明
class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       导入的几何部件形状。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelImportedPart : public Interface::FITKAbsGeoImportedPart, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        explicit FITKOCCModelImportedPart();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        virtual ~FITKOCCModelImportedPart() = default;

        /**
         * @brief       更新命令。[重写]
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool update() override;

        /**
         * @brief       保存几何形状。
         * @param[in]   shape：几何形状[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        void setShape(TopoDS_Shape & shape);

        /**
         * @brief       获取数据对象的拷贝。[重写]
         * @return      备份数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-10
         */
        Interface::FITKAbsGeoImportedPart* getCopy() override;

    };
}

#endif //!__FITKOCCMODELIMPORTEDPART_H__

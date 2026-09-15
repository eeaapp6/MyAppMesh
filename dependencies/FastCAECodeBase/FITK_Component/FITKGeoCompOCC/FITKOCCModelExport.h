/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelExport.h
 * @brief       OCC模型数据写出命令。（全部模型合并）

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-27
 *********************************************************************/

#ifndef _FITKOCCMODEL_EXPORT_H___
#define _FITKOCCMODEL_EXPORT_H___

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExport.h"

// 前置声明
class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC模型数据写出命令。（全部模型合并）
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelExport : public Interface::FITKAbsGeoModelExport
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        explicit FITKOCCModelExport() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        virtual ~FITKOCCModelExport() = default;

        /**
         * @brief       执行命令。（文件写出）[虚函数][重写]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        virtual bool update() override;

    private:
        /**
         * @brief       形状保存至文件。
         * @param[in]   fileName：文件名称
         * @param[in]   shape：OCC形状数据
         * @return      是否写出成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        bool writeFile(QString fileName, TopoDS_Shape & shape);

    };
}


#endif
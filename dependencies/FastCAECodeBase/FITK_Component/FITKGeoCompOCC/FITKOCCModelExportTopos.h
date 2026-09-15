/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelExportTopos.h
 * @brief       OCC模型数据写出命令。（指定拓扑合并）

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-09-23
 *********************************************************************/

#ifndef __FITKOCCMODELEXPORTTOPOS_H__
#define __FITKOCCMODELEXPORTTOPOS_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExportTopos.h"

// 前置声明
class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC模型数据写出命令。（指定拓扑合并）
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-23
     */
    class FITKGeoCompOCCAPI FITKOCCModelExportTopos : public Interface::FITKAbsGeoModelExportTopos
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        explicit FITKOCCModelExportTopos() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        virtual ~FITKOCCModelExportTopos() = default;

        /**
         * @brief       执行命令。（文件写出）[虚函数][重写]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        virtual bool update() override;

    private:
        /**
         * @brief       形状保存至文件。
         * @param[in]   fileName：文件名称
         * @param[in]   shape：OCC形状数据
         * @return      是否写出成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-23
         */
        bool writeFile(QString fileName, TopoDS_Shape & shape);

    };
}

#endif // !__FITKOCCMODELEXPORTTOPOS_H__
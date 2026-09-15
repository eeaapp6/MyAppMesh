/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKAbsGeoModelOperLine.h
 * @brief   抽象线操作类
 *
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-08-19
 *********************************************************************/

#ifndef  FITKOCCMODELOPERLINE_H
#define  FITKOCCMODELOPERLINE_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperLine.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoRepairSmallEdges.h"

namespace OCC
{
    class FITKGeoCompOCCAPI FITKOCCModelOperLine : public Interface::FITKAbsGeoModelOperLine, public OCCShapeAgent
    {
    public:
        FITKOCCModelOperLine();
        virtual ~FITKOCCModelOperLine() override = default;

        /**
         * @brief 更新模型
         * @return true
         * @return false
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        virtual bool update() override;

    private:

        /**
         * @brief 合并
         * @param   shapes：操作形状列表
         * @return true
         * @return false
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        bool updateMerge(QList<TopoDS_Shape> shapes);

        /**
         * @brief 按中点分割
         * @param   shapes：操作形状列表
         * @return true
         * @return false
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        bool updateSplitByMiddlePosition(QList<TopoDS_Shape> shapes);

        /**
         * @brief 按角度分割
         * @param   shapes：操作形状列表
         * @return true
         * @return false
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        bool updateSplitByAngle(QList<TopoDS_Shape> shapes);
    };

    class FITKGeoCompOCCAPI FITKOCCModelMiddleOperLine : public Interface::FITKAbsGeoModelMiddleOperLine, public OCCShapeAgent
    {
    public:
        FITKOCCModelMiddleOperLine();
        virtual ~FITKOCCModelMiddleOperLine() override = default;

        /**
         * @brief 更新模型
         * @return true
         * @return false
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        virtual bool update() override;

    private:
        /**
         * @brief 按中点分割
         * @param   shapes：操作形状列表
         * @return true
         * @return false
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        bool updateSplitByMiddlePosition(QList<TopoDS_Shape> shapes);
    };

    /**
     * @brief       OCC修复小边类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-08
     */
    class FITKGeoCompOCCAPI FITKOCCModelRepairSmallEdges : public Interface::FITKAbsGeoRepairSmallEdges, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-08
         */
        FITKOCCModelRepairSmallEdges();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-08
         */
        ~FITKOCCModelRepairSmallEdges() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-08
         */
        bool update() override;

    };
}

#endif // !FITKABSGEOMODELOPERLINE_H
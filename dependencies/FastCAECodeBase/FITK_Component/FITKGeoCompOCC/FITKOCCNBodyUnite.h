/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCNBodyUnite.h
 * @brief       多实体布尔求和OCC实现。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-07-08
 *********************************************************************/

#ifndef __FITKOCCNBODYUNITE_H__
#define __FITKOCCNBODYUNITE_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperNBodyUnite.h"

// 前置声明。
class TopoDS_Shape;
class TopoDS_Solid;
class TopoDS_CompSolid;

namespace OCC
{
    /**
     * @brief       多实体布尔求和OCC实现。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-08
     */
    class FITKGeoCompOCCAPI FITKOCCNBodyUnite : public Interface::FITKAbsGeoOperNBodyUnite, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        explicit FITKOCCNBodyUnite();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        virtual ~FITKOCCNBodyUnite() = default;

        /**
         * @brief       更新命令。
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        virtual bool update() override;

    private:
        /**
         * @brief       工具方法 - 从复合实体中移除一组实体。
         * @param[in]   compsolid：复合体对象
         * @param[in]   solids：移除的实体列表
         * @return      结果形状
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        TopoDS_Shape removeSolidFromCompsolid(const TopoDS_CompSolid & compsolid, QList<const TopoDS_Solid*> solids);

        /**
         * @brief       工具方法 - 从组合对象中移除一组实体。
         * @param[in]   comp：组合对象
         * @param[in]   solids：移除的实体列表
         * @return      结果形状
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        TopoDS_Shape removeSolidFromComp(const TopoDS_Shape & comp, QList<const TopoDS_Solid*> solids);

    };
}

#endif // !__FITKOCCNBODYUNITE_H__

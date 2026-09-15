/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelPart.h
 * @brief       OCC几何部件。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-27
 *********************************************************************/

#ifndef  __FITKOCCMODELPART_H__
#define  __FITKOCCMODELPART_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"

#include "FITKGeoCompOCCAPI.h"

// 前置声明
class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC几何部件。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelPart : public Interface::FITKAbsGeoPart, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        explicit FITKOCCModelPart();
        
        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        virtual ~FITKOCCModelPart() = default;

        /**
         * @brief       获取部件当前形状。
         * @return      部件形状
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        TopoDS_Shape* getPartShape();

        /**
         * @brief       获取部件当前形状的备份。
         * @return      部件形状
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        TopoDS_Shape getCopiedPartShape();

        /**
         * @brief       更新部件形状。
         * @param[in]   shape：子命令形状
         * @param[in]   unite：是否与原模型求和[缺省]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        void updatePartShape(const TopoDS_Shape & shape, bool unite = false);

        /**
         * @brief       几何形状回退：同步部件几何形状。
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-28
         */
        bool backtrackingShape() override;

        /**
         * @brief       更新命令。
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        virtual bool update() override;

        /**
         * @brief       重写添加数据对象功能。（添加命令并更新部件几何）[重写]
         * @param[in]   obj：数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        void appendDataObj(Core::FITKAbstractDataObject* obj) override;

    protected:
        /**
         * @brief       重组组合对象。
         * @param[in]   iShape：原始形状
         * @param[in]   oShape：输出形状[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        void reUnite(const TopoDS_Shape & iShape, TopoDS_Shape & oShape);

    };
}

#endif // !__FITKOCCMODELPART_H__

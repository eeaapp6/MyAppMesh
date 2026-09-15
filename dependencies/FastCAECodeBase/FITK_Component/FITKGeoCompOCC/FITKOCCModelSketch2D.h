/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelSketch2D.h
 * @brief       OCC2D草图数据类

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-09-05
 *********************************************************************/

#ifndef __FITKOCCMODELSKETCH_H__
#define __FITKOCCMODELSKETCH_H__
 
#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSketch2D.h"

#include <QHash>

// 前置声明
class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC2D草图数据类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-09-05
     */
    class FITKGeoCompOCCAPI FITKOCCModelSketch2D :public Interface::FITKAbsGeoSketch2D,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        explicit FITKOCCModelSketch2D();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        virtual ~FITKOCCModelSketch2D() = default;

        /**
         * @brief       更新数据。[虚函数][重写]
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-05
         */
        bool update() override;

        /**
         * @brief       判断草图是否闭合。[重写]
         * @return      草图是否闭合
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-01
         */
        bool isClosed() override;

        /**
         * @brief       判断草图是否自相交。[重写]
         * @return      草图是否自相交
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        bool isSelfIntersection() override;

        /**
         * @brief       判断草图是否连贯。[重写]
         * @return      是否连贯
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-16
         */
        bool isConnected() override;

        /**
         * @brief       检查草图。[重写]
         * @param[out]  isClosed：草图是否闭合[引用]
         * @param[out]  isSelfIntersection：草图是否自相交[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        void checkState(bool& isClosed, bool& isSelfIntersection) override;

        /**
         * @brief       获取草图起始点与起始点方向。[重写]
         * @param[out]  point：位置
         * @param[out]  dir：方向
         * @return      是否获取成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-04
         */
        bool getFirstPointAndDir(double* point, double* dir) override;

        /**
         * @brief       获取草图结束点与起始点方向。[重写]
         * @param[out]  point：位置
         * @param[out]  dir：方向
         * @return      是否获取成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-04
         */
        bool getLastPointAndDir(double* point, double* dir) override;

        /**
         * @brief       重写添加数据对象功能。（添加命令并更新草图几何）[重写]
         * @param[in]   obj：数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-16
         */
        void appendDataObj(Core::FITKAbstractDataObject* obj) override;

        /**
         * @brief       处理命令，合并草图模型，获取形状。
         * @param[out]  oEdges：草图模型形状
         * @return      是否存在有效形状
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-16
         */
        bool getShape(QList<TopoDS_Shape> & oEdges);

        /**
         * @brief       处理命令，合并草图模型，获取形状。
         * @param[out]  oShape：草图模型形状
         * @return      是否存在有效形状
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-16
         */
        bool getShape(TopoDS_Shape & oShape);

    protected:
        /**
         * @brief       子命令形状状态字典。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-04
         */
        QHash<int, int> m_cmdIdShapeHash;

    };
}

#endif // __FITKOCCMODELSKETCH_H__

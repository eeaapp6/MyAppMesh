/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelOperSolid.h
 * @brief       OCC体特征操作数据类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-26
 *********************************************************************/

#ifndef  FITKOCCMODELOPERSOLID_H
#define  FITKOCCMODELOPERSOLID_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperSolid.h"

class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC倒角类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-20
     */
    class FITKGeoCompOCCAPI FITKOCCModelChamferSolid : public Interface::FITKAbsGeoModelChamferSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        FITKOCCModelChamferSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        ~FITKOCCModelChamferSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        bool update() override;

    };

    /**
     * @brief       OCC倒圆类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-20
     */
    class FITKGeoCompOCCAPI FITKOCCModelFilletSolid : public Interface::FITKAbsGeoModelFilletSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        FITKOCCModelFilletSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        ~FITKOCCModelFilletSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        bool update() override;

    };

    /**
     * @brief       OCC移除特征工具类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-27
     */
    class FITKOCCDefeatureTool
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        FITKOCCDefeatureTool() {}

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        ~FITKOCCDefeatureTool() {}

        /**
         * @brief       去除特征工具方法。[静态]
         * @param[in]   cmdId：模型数据ID
         * @param[in]   faceIndice：被操作的面数据虚拓扑索引列表
         * @param[out]  outputShape：输出形状[引用]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        static bool Defeature(int cmdId, QList<int> faceIndice, TopoDS_Shape & outputShape);
    };

    /**
     * @brief       OCC移除特征类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-29
     */
    class FITKGeoCompOCCAPI FITKOCCModelDefeatureSolid : public Interface::FITKAbsGeoModelDefeatureSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        FITKOCCModelDefeatureSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        ~FITKOCCModelDefeatureSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        bool update() override;

    };

    /**
     * @brief       OCC移除倒角类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelRemoveChamferSolid : public Interface::FITKAbsGeoModelRemoveChamferSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        FITKOCCModelRemoveChamferSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        ~FITKOCCModelRemoveChamferSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        bool update() override;

    };

    /**
     * @brief       OCC移除倒圆类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelRemoveFilletSolid : public Interface::FITKAbsGeoModelRemoveFilletSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        FITKOCCModelRemoveFilletSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        ~FITKOCCModelRemoveFilletSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        bool update() override;

    };

    /**
     * @brief       OCC填补孔洞类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-26
     */
    class FITKGeoCompOCCAPI FITKOCCModelFillHoleSolid : public Interface::FITKAbsGeoModelFillHoleSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        FITKOCCModelFillHoleSolid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        ~FITKOCCModelFillHoleSolid() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        bool update() override;

    };
}

#endif // !FITKOCCMODELOPERSOLID_H

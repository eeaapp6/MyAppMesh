/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKOCCModelOperFace.h
 * @brief   抽象线操作类
 *
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-08-19
 *********************************************************************/

#ifndef  FITKOCCMODELOPERFACE_H
#define  FITKOCCMODELOPERFACE_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperFace.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoRepairSmallFaces.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoReplaceFaces.h"

namespace OCC
{
    /**
     * @brief  抽象面操作类
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-31
     */
    class FITKGeoCompOCCAPI FITKOCCModelOperFace : public Interface::FITKAbsGeoModelOperFace, public OCCShapeAgent
    {
    public:
        FITKOCCModelOperFace();
        virtual ~FITKOCCModelOperFace() override = default;

        /**
         * @brief   更新模型
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-19
         */
        virtual bool update() override;

    private:

        /**
         * @brief   填补孔洞
         * @param   shapes：操作形状列表
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
         */
        bool updateFillHoles(QList<TopoDS_Shape> shapes);

        /**
         * @brief   删除悬浮边
         * @param   shapes：操作形状列表
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        bool updateDeleteFloatingEdge(QList<TopoDS_Shape> shapes);

        /**
         * @brief   将点映射到面
         * @param   shapes：操作形状列表
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
         */
        bool updateImprintMapPointToFace(QList<TopoDS_Shape> shapes);

        /**
         * @brief   将线映射到面
         * @param   shapes：操作形状列表
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
         */
        bool updateImprintMapLineToFace(QList<TopoDS_Shape> shapes);

        /**
         * @brief   将圆映射到面
         * @param   shapes：操作形状列表
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
         */
        bool updateImprintMapCircleToFace(QList<TopoDS_Shape> shapes);
    };

    /**
     * @brief  抽象面操作类 填补孔洞
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-31
     */
    class FITKGeoCompOCCAPI FITKOCCModelOperFaceFillHoles : public Interface::FITKAbsGeoModelOperFaceFillHoles, public OCCShapeAgent
    {
    public:
        FITKOCCModelOperFaceFillHoles();
        virtual ~FITKOCCModelOperFaceFillHoles() override = default;
        /**
             * @brief   更新模型
             * @return  true
             * @return  false
             * @author  fulipeng (fulipengqd@yeah.net)
             * @date    2024-08-19
             */
        virtual bool update() override;

    private:

        /**
         * @brief   填补孔洞
         * @param   shapes：操作形状列表
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
         */
        bool updateFillHoles(QList<TopoDS_Shape> shapes);

    };

    /**
     * @brief  抽象面操作类 填补缝隙
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-31
     */
    class FITKGeoCompOCCAPI FITKOCCModelOperFaceFillGaps : public Interface::FITKAbsGeoModelOperFaceFillGaps, public OCCShapeAgent
    {
    public:
        FITKOCCModelOperFaceFillGaps();
        virtual ~FITKOCCModelOperFaceFillGaps() override = default;
        /**
             * @brief   更新模型
             * @return  true
             * @return  false
             * @author  fulipeng (fulipengqd@yeah.net)
             * @date    2024-08-19
             */
        virtual bool update() override;

    private:

        /**
         * @brief   填补缝隙
         * @param   shapes：操作形状列表
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
         */
        bool updateFillGaps(QList<TopoDS_Shape> shapes);

        /**
         * @brief   检查并强制曲线闭合
         * @param   shapes：操作形状列表
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
         */
        TopoDS_Shape CheckAndForceCurveClosure(QList<TopoDS_Shape> shapes, bool& isSuccessful);
    };

    /**
     * @brief  抽象面操作类 删除悬浮边
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-31
     */
    class FITKGeoCompOCCAPI FITKOCCModelOperFaceDeleteFloatingEdge : public Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge, public OCCShapeAgent
    {
    public:
        FITKOCCModelOperFaceDeleteFloatingEdge();
        virtual ~FITKOCCModelOperFaceDeleteFloatingEdge() override = default;
        /**
             * @brief   更新模型
             * @return  true
             * @return  false
             * @author  fulipeng (fulipengqd@yeah.net)
             * @date    2024-08-19
             */
        virtual bool update() override;

    private:

        /**
         * @brief   删除悬浮边
         * @param   shapes：操作形状列表
         * @return  true
         * @return  false
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-31
         */
        bool updateDeleteFloatingEdge(QList<TopoDS_Shape> shapes);
    };

    /**
     * @brief       OCC移除面类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-28
     */
    class FITKGeoCompOCCAPI FITKOCCModelRemoveFace : public Interface::FITKAbsGeoModelOperFaceRemoveFace, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-28
         */
        FITKOCCModelRemoveFace();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-28
         */
        ~FITKOCCModelRemoveFace() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-28
         */
        bool update() override;

    };

    /**
     * @brief       OCC修复小面类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-09
     */
    class FITKGeoCompOCCAPI FITKOCCModelRepairSmallFaces : public Interface::FITKAbsGeoRepairSmallFaces, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-09
         */
        FITKOCCModelRepairSmallFaces();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-09
         */
        ~FITKOCCModelRepairSmallFaces() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-09
         */
        bool update() override;

    };

    /**
     * @brief       OCC延伸面类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-25
     */
    class FITKGeoCompOCCAPI FITKOCCModelExtendFace : public Interface::FITKAbsGeoModelOperFaceExtendFace, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-25
         */
        FITKOCCModelExtendFace();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-25
         */
        ~FITKOCCModelExtendFace() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-25
         */
        bool update() override;

    };

    /**
     * @brief       OCC替换面类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-25
     */
    class FITKGeoCompOCCAPI FITKOCCModelReplaceFaces : public Interface::FITKAbsGeoReplaceFaces, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-25
         */
        FITKOCCModelReplaceFaces();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-25
         */
        ~FITKOCCModelReplaceFaces() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-25
         */
        bool update() override;

    };
}

#endif // !FITKABSGEOMODELOPERLINE_H
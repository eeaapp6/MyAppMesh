/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKOCCModelOperCompound.h
 * @brief   OCC复合对象特征操作数据类。

 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-09-07
 *********************************************************************/

#ifndef  FITKOCCMODELOPERCOMPOUND_H
#define  FITKOCCMODELOPERCOMPOUND_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperCompound.h"

class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief   OCC复合对象特征操作数据类。
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-09-07
     */
    class FITKGeoCompOCCAPI FITKOCCModelOperCompound : public Interface::FITKAbsGeoModelOperCompound, public OCCShapeAgent
    {
    public:
        /**
         * @brief   构造函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        FITKOCCModelOperCompound();

        /**
         * @brief   析构函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        ~FITKOCCModelOperCompound() = default;

    };

    /**
     * @brief   OCC复合对象添加操作数据类。
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-09-07
     */
    class FITKGeoCompOCCAPI FITKOCCModelOperAddCompound : public FITKOCCModelOperCompound
    {
    public:
        /**
         * @brief   构造函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        FITKOCCModelOperAddCompound();

        /**
         * @brief   析构函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        ~FITKOCCModelOperAddCompound() = default;

        /**
         * @brief   更新数据。[重写]
         * @return  是否成功
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        bool update() override;

    };

    /**
     * @brief   OCC复合对象移除操作数据类。
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-09-07
     */
    class FITKGeoCompOCCAPI FITKOCCModelOperRemoveCompound : public FITKOCCModelOperCompound
    {
    public:
        /**
         * @brief   构造函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        FITKOCCModelOperRemoveCompound();

        /**
         * @brief   析构函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        ~FITKOCCModelOperRemoveCompound() = default;

        /**
         * @brief   更新数据。[重写]
         * @return  是否成功
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        bool update() override;
    };
}

#endif // !FITKOCCMODELOPERSOLID_H

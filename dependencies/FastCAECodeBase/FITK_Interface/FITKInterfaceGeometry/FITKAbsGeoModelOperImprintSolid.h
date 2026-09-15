/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKAbsGeoModelOperImprintSolid.h
 * @brief   抽象压印体操作类
 *
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-09-05
 *********************************************************************/

#ifndef  FITKABSGEOMODELOPERIMPRINTSOLID_H
#define  FITKABSGEOMODELOPERIMPRINTSOLID_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"


namespace Interface
{
    class FITKAbsGeoShapeAgent;

    /**
     * @brief  抽象压印体操作类
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-09-05
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOperImprintSolid : public FITKAbsGeoCommand
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbsGeoModelOperImprintSolid);
    public:

        /**
         * @brief  压印体操作类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-09-05
         */
        enum GeoSolidOperType
        {
            GBTNone,
            GBTImprintSolid,             ///压印体
        };
        Q_ENUM(GeoSolidOperType)

    public:
        FITKAbsGeoModelOperImprintSolid() = default;
        virtual ~FITKAbsGeoModelOperImprintSolid() override = default;

        /**
         * @brief   获取几何命令类型
         * @param   geoCommandID：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-04
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief   添加操作对象
         * @param   geoCommandShape：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-05
         */
        void add(VirtualShape geoCommandShape);

        /**
         * @brief   添加操作对象
         * @param   geoCommandShape：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-05
         */
        void set(VirtualShape geoCommandShape);
        /**
         * @brief   添加多个操作对象
         * @param   geoCommandShapes：操作对象链表
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-05
         */
        void add(QList<VirtualShape> geoCommandShapes);

        /**
         * @brief   添加多个操作对象
         * @param   geoCommandShapes：操作对象链表
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-05
         */
        void set(QList<VirtualShape> geoCommandShapes);

        /**
         * @brief   移除操作对象
         * @param   geoCommand：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-05
         */
        void remove(int index);

        /**
         * @brief   移除所有操作对象
         * @param   geoCommand：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-26
         */
        void clear();

        /**
         * @brief   设置类型
         * @param   t：操作类型
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-26
         */
        void setType(GeoSolidOperType t);

        /**
         * @brief   设置类型
         * @return  返回操作类型
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-26
         */
        FITKAbsGeoModelOperImprintSolid::GeoSolidOperType getType();

        /**
         * @brief   获取虚拓扑图形
         * @param   geoCommand：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-19
         */
        QList<VirtualShape> getVShapes();
        /**
         * @brief  获取操作对象列表
         * @return 操作对象列表
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-09-05
         */
        QList<int> getModelOperFacesID();

        /**
         * @brief  设置公差
         * @return 操作对象列表
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-23
         */
        void setTolerance(double tolerance);

        /**
         * @brief  获取公差
         * @return 操作对象列表
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-23
         */
        double getTolerance();

    protected:
        /**
         * @brief  面操作类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-09-05
         */
        GeoSolidOperType _faceOperType{ GBTNone };

        /**
         * @brief  操作对象列表
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-09-05
         */
        QList<VirtualShape> m_tempVShapes{};

        /**
         * @brief  公差
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-23
         */
        double _tolerance = 1e-2;
    };
}

#endif // !FITKAbsGeoModelOperImprintSolid_H

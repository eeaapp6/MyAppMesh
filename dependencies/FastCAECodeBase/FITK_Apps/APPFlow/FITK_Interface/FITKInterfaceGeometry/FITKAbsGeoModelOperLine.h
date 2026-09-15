/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
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

#ifndef  FITKABSGEOMODELOPERLINE_H
#define  FITKABSGEOMODELOPERLINE_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"


namespace Interface
{
    class FITKAbsGeoShapeAgent;

    /**
     * @brief  抽象线操作类
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-19
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOperLine : public FITKAbsGeoCommand
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbsGeoModelOperLine);
    public:

        /**
         * @brief  线操作类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        enum GeoLineOperType
        {
            GBTNone,
            GBTMerge,
            GBTSplitByMiddlePosition,
            GBTSplitByAngle,
        };
        Q_ENUM(GeoLineOperType)

    public:
        FITKAbsGeoModelOperLine() = default;
        virtual ~FITKAbsGeoModelOperLine() = default;

        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief   添加操作对象
         * @param   geoCommandID：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-19
         */
        void add(VirtualShape geoCommandID);

        /**
         * @brief   添加多个操作对象
         * @param   geoCommandIDs：操作对象链表
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-19
         */
        void add(QList<VirtualShape> geoCommandIDs);

        /**
         * @brief   设置操作对象
         * @param   geoCommandIDs：操作对象链表
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-10-15
         */
        void set(QList<VirtualShape> geoCommandIDs);

        /**
         * @brief   移除操作对象
         * @param   index：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-19
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
        void setType(GeoLineOperType t);

        /**
         * @brief   设置类型
         * @return  返回操作类型
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-26
         */
        FITKAbsGeoModelOperLine::GeoLineOperType getType();

        /**
         * @brief   获取虚拓扑图形
         * @param   geoCommand：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-19
         */
        QList<VirtualShape> getVShapes();

        /**
         * @brief  获取操作对象ID列表
         * @return 操作对象列表
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        QList<int> getModelOperLinesID();

        /**
         * @brief  设置角度
         * @param   angle：角度
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        void setAngle(double angle);

        /**
        * @brief  获取角度
        * @return 角度
        * @author fulipeng (fulipengqd@yeah.net)
        * @date   2024-08-19
        */
        double getAngle();


    protected:
        /**
         * @brief  线操作类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        GeoLineOperType _lineOperType{ GBTNone };
        /**
         * @brief  操作对象ID列表
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        QList<VirtualShape> m_tempVShapes{};
        /**
         * @brief  角度 根据角度分割时使用
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-19
         */
        double _angle = 0;

    };


    /**
      * @brief  抽象类 合并线数据结构
      * @author fulipeng (fulipengqd@yeah.net)
      * @date   2024-08-19
      */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelMergeOperLine : public FITKAbsGeoModelOperLine
    {
        FITKCLASS(Interface, FITKAbsGeoModelMergeOperLine);
    public:
        FITKAbsGeoModelMergeOperLine() = default;
        virtual ~FITKAbsGeoModelMergeOperLine() = default;

        /**
         * @brief   获取几何命令类型
         * @param   geoCommandID：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-04
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    };

    /**
     * @brief  抽象类 线中点分割操作数据结构
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-19
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelMiddleOperLine : public FITKAbsGeoModelOperLine
    {
        FITKCLASS(Interface, FITKAbsGeoModelMiddleOperLine);
    public:
        FITKAbsGeoModelMiddleOperLine() = default;
        virtual ~FITKAbsGeoModelMiddleOperLine() = default;

        /**
         * @brief   获取几何命令类型
         * @param   geoCommandID：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-04
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    };

    /**
     * @brief  抽象类 线角度分割操作数据结构
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-19
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelAngleOperLine : public FITKAbsGeoModelOperLine
    {
        FITKCLASS(Interface, FITKAbsGeoModelAngleOperLine);
    public:
        FITKAbsGeoModelAngleOperLine() = default;
        virtual ~FITKAbsGeoModelAngleOperLine() = default;

        /**
         * @brief   获取几何命令类型
         * @param   geoCommandID：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-04
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    };
}

#endif // !FITKABSGEOMODELOPERLINE_H
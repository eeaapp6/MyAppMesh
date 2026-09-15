/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKAbsGeoModelOperFace.h
 * @brief   抽象面操作类
 *
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-08-21
 *********************************************************************/

#ifndef  FITKABSGEOMODELOPERFACE_H
#define  FITKABSGEOMODELOPERFACE_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"


namespace Interface
{
    class FITKAbsGeoShapeAgent;

    /**
     * @brief  抽象面操作类
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-21
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOperFace : public FITKAbsGeoCommand
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbsGeoModelOperFace);
    public:

        /**
         * @brief  面操作类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-21
         */
        enum GeoFaceOperType
        {
            GBTNone,
            GBTFillHoles,                ///填补孔洞
            GBTFillGaps,                 ///填补缝隙
            GBTDeleteFloatingEdge,       ///删除悬浮边
            GBTImprintMapPointToFace,    ///点映射到面
            GBTImprintMapLineToFace,     ///线映射到面
            GBTImprintMapCircleToFace,   ///圆映射到面
        };
        Q_ENUM(GeoFaceOperType)

    public:
        FITKAbsGeoModelOperFace() = default;
        virtual ~FITKAbsGeoModelOperFace() override = default;

        /**
         * @brief   添加操作对象
         * @param   geoCommandShape：操作对象结构体
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-21
         */
        void add(VirtualShape geoCommandShape);

        /**
         * @brief   添加操作对象
         * @param   geoCommandShape：操作对象结构体
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-21
         */
        void set(VirtualShape geoCommandShape);

        /**
         * @brief   添加多个操作对象
         * @param   geoCommandShapes：操作对象结构体链表
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-21
         */
        void add(QList<VirtualShape> geoCommandShapes);

        /**
         * @brief   添加多个操作对象
         * @param   geoCommandShapes：操作对象结构体链表
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-21
         */
        void set(QList<VirtualShape> geoCommandShapes);

        /**
         * @brief   移除操作对象
         * @param   index：索引
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-21
         */
        void remove(int index);

        /**
         * @brief   移除所有操作对象
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
        void setType(GeoFaceOperType t);

        /**
         * @brief   获取类型
         * @return  返回操作类型
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-26
         */
        FITKAbsGeoModelOperFace::GeoFaceOperType getType();

        /**
         * @brief   获取虚拓扑图形
         * @return   geoCommand：操作对象
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-19
         */
        QList<VirtualShape> getVShapes();
        /**
         * @brief   获取操作对象列表
         * @return  操作对象列表
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-21
         */
        QList<int> getModelOperFacesID();

        /**
         * @brief   设置公差
         * @param   tolerance 公差
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
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
         * @brief   通过形状类型、命令ID、虚拓扑ID 获取形状
         * @param   shapeType       形状类型
         * @param   m_solidCmdId    命令ID
         * @param   virtualTopoId   虚拓扑ID
         * @return  返回虚拓扑形状
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-26
         */
        Interface::FITKAbsVirtualTopo* getRefModelVirtualTopo(Interface::FITKGeoEnum::VTopoShapeType shapeType, int m_solidCmdId, int virtualTopoId);

    protected:
        /**
         * @brief  面操作类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-21
         */
        GeoFaceOperType _faceOperType{ GBTNone };

        /**
         * @brief  操作对象列表
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-21
         */
        QList<VirtualShape> m_tempVShapes{};

        /**
         * @brief  公差
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-23
         */
        double _tolerance = 1e-2;
    };

    /**
     * @brief  抽象面操作类 移除面
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2025-03-24
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOperFaceRemoveFace : public FITKAbsGeoModelOperFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelOperFaceRemoveFace);
    public:
        FITKAbsGeoModelOperFaceRemoveFace() = default;
        ~FITKAbsGeoModelOperFaceRemoveFace() = default;

        /**
         * @brief  获取几何命令类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-31
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    };

    /**
     * @brief  抽象面操作类 延伸面
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2025-03-26
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOperFaceExtendFace : public FITKAbsGeoModelOperFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelOperFaceExtendFace);
        Q_OBJECT
    public:
        /**
         * @brief  Specify界面选择类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2025-04-01
         */
        enum SpecifyModel
        {
            SpecifyEdge,
            SpecifyFace,
        };
        Q_ENUM(SpecifyModel);

        /**
         * @brief  界面选择类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2025-04-01
         */
        enum MethodModel
        {
            MethodDistance,
            MethodFace,
        };
        Q_ENUM(MethodModel);

    public:
        FITKAbsGeoModelOperFaceExtendFace() = default;
        ~FITKAbsGeoModelOperFaceExtendFace() = default;

        /**
         * @brief  获取几何命令类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-31
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief   设置线参数
         * @param   edges:线参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-31
         */
        void setEdges(QList<VirtualShape> edges);
        /**
         * @brief   获取线
         * @return  线参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-31
         */
        QList<VirtualShape> getEdges();

        /**
         * @brief   设置选择的面
         * @param   face 面参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-31
         */
        void setFaces(QList<VirtualShape> face);

        /**
         * @brief   获取选择的面
         * @return  面参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-31
         */
        QList<VirtualShape> getFaces();

        /**
         * @brief   设置标记面
         * @param   tagFaces 标记面
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-31
         */
        void setTagFaces(QList<VirtualShape> tagFaces);
        /**
         * @brief   获取标记面
         * @return  标记面
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-31
         */
        QList<VirtualShape> getTagFaces();

        /**
         * @brief   设置距离
         * @param   dis 距离
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-31
         */
        void setDistance(double dis);
        /**
         * @brief   获取距离
         * @return  距离
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-31
         */
        double getDistance();

        /**
         * @brief   设置是否修剪
         * @param   isTrim 是否开启修剪
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-26
         */
        void setTrim(bool isTrim);

        /**
        * @brief   是否修剪
        * @return  是否开启修剪
        * @author  fulipeng (fulipengqd@yeah.net)
        * @date    2025-03-26
        */
        bool getTrim();

        /**
         * @brief   设置延伸面选择方法
         * @param   sm
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-01
         */
        void setSpecifyMode(SpecifyModel sm);

        /**
         * @brief   设置延伸面方法
         * @param   mm
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-01
         */
        void setMethodModel(MethodModel mm);

        /**
         * @brief       获取延伸面选择方法。
         * @return      伸面选择方法
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        SpecifyModel getSpecifyMode();

        /**
         * @brief       获取延伸面方法。
         * @return      延伸面方法
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-12
         */
        MethodModel getMethodModel();

    protected:
        /**
         * @brief   延伸长度
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-26
         */
        double _distance = 0;

        /**
         * @brief   Trim to extended underlying target surfaces
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-03-31
         */
        bool _trim = false;

        /**
         * @brief  线操作对象列表
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2025-04-01
         */
        QList<VirtualShape> _tempEdgeShapes{};

        /**
         * @brief  操作对象列表
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2025-04-01
         */
        QList<VirtualShape> _tempTagFaceShapes{};

        /**
         * @brief   key specify value Method 用于区分界面选项
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-01
         */
        QPair<SpecifyModel, MethodModel> _selectedMode{ SpecifyEdge,MethodDistance };

    };

    /**
     * @brief  抽象面操作类 修复面
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2025-03-26
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOperFaceRepairFaces : public FITKAbsGeoModelOperFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelOperFaceRepairFaces);
    public:
        FITKAbsGeoModelOperFaceRepairFaces() = default;
        ~FITKAbsGeoModelOperFaceRepairFaces() = default;

        /**
         * @brief  获取几何命令类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-31
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    };

    /**
     * @brief  抽象面操作类 填补缝隙
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-21
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOperFaceFillGaps : public FITKAbsGeoModelOperFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelOperFaceFillGaps);
    public:
        FITKAbsGeoModelOperFaceFillGaps() = default;
        ~FITKAbsGeoModelOperFaceFillGaps() = default;

        /**
         * @brief  获取几何命令类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-31
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    protected:

    };

    /**
     * @brief  抽象面操作类 填补孔洞
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-21
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOperFaceFillHoles : public FITKAbsGeoModelOperFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelOperFaceFillHoles);
    public:
        FITKAbsGeoModelOperFaceFillHoles() = default;
        ~FITKAbsGeoModelOperFaceFillHoles() = default;

        /**
         * @brief  获取几何命令类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-31
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    protected:

    };

    /**
     * @brief  抽象面操作类 删除悬浮边
     * @author fulipeng (fulipengqd@yeah.net)
     * @date   2024-08-21
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOperFaceDeleteFloatingEdge : public FITKAbsGeoModelOperFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelOperFaceDeleteFloatingEdge);
    public:
        FITKAbsGeoModelOperFaceDeleteFloatingEdge() = default;
        ~FITKAbsGeoModelOperFaceDeleteFloatingEdge() = default;

        /**
         * @brief  获取几何命令类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-31
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief  设置主面
         * @param  vface：主面
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-31
         */
        void setMFace(VirtualShape vface);

        /**
         * @brief  获取主面
         * @return 主面
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-31
         */
        VirtualShape getMFace();

    protected:

        /**
         * @brief  操作主面
         * @author fulipeng (fulipengqd@yeah.net)
         * @date   2024-08-31
         */
        VirtualShape m_face{};
    };


    /**
     * @brief       编辑圆柱面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-25
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelModifyCylinder : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelModifyCylinder);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        FITKAbsGeoModelModifyCylinder() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        virtual ~FITKAbsGeoModelModifyCylinder() = default;

        /**
         * @brief       获取几何命令类型。
         * @return      几何命令
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置圆柱面。
         * @param[in]   face：圆柱面拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        void setCylinderFace(VirtualShape face);

        /**
         * @brief       获取设置的圆柱面。
         * @return      圆柱面拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        VirtualShape getCylinderFace();

        /**
         * @brief       设置半径。
         * @param[in]   radius：半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        void setRadius(double radius);

        /**
         * @brief       获取设置的半径。
         * @return      半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        double getRadius();

    protected:
        /**
         * @brief       圆柱面拓扑信息。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        VirtualShape m_face{};

        /**
         * @brief       半径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        double m_radius = 1.;

    };


    /**
     * @brief       平移面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-26
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelTransformFace : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelTransformFace);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        FITKAbsGeoModelTransformFace() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        virtual ~FITKAbsGeoModelTransformFace() = default;

        /**
         * @brief       获取几何命令类型。
         * @return      几何命令
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置被平移面。
         * @param[in]   faces：被平移面拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        void setFaces(QList<VirtualShape> faces);

        /**
         * @brief       添加被平移面。
         * @param[in]   face：被平移面拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        void addFace(VirtualShape face);

        /**
         * @brief       获取设置的面。
         * @return      被平移面拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        QList<VirtualShape> getFaces();

        /**
         * @brief       设置平移方向。
         * @param[in]   dx：平移方向x分量
         * @param[in]   dy：平移方向y分量
         * @param[in]   dz：平移方向z分量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        void setDirection(double dx, double dy, double dz);

        /**
         * @brief       获取平移方向。
         * @param[out]  dx：平移方向x分量[引用]
         * @param[out]  dy：平移方向y分量[引用]
         * @param[out]  dz：平移方向z分量[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        void getDirection(double & dx, double & dy, double & dz);

        /**
         * @brief       设置平移距离。
         * @param[in]   dist：距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        void setDistance(double dist);

        /**
         * @brief       获取设置的平移距离。
         * @return      距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        double getDistance();

    protected:
        /**
         * @brief       被平移面拓扑信息。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        QList<VirtualShape> m_faces;

        /**
         * @brief       平移距离。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        double m_distance = 0.;

        /**
         * @brief       平移方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        double m_direction[3]{ 0., 0., 1. };

    };

    /**
     * @brief       旋转面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-10-09
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelRotateFace : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelRotateFace);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        FITKAbsGeoModelRotateFace() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        virtual ~FITKAbsGeoModelRotateFace() = default;

        /**
         * @brief       获取几何命令类型。
         * @return      几何命令
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置被旋转面。
         * @param[in]   faces：被旋转面拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        void setFaces(QList<VirtualShape> faces);

        /**
         * @brief       添加被旋转面。
         * @param[in]   face：被旋转面拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        void addFace(VirtualShape face);

        /**
         * @brief       获取设置的面。
         * @return      被旋转面拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        QList<VirtualShape> getFaces();

        /**
         * @brief       设置旋转轴上一点坐标。
         * @param[in]   px：旋转轴上一点坐标x分量
         * @param[in]   py：旋转轴上一点坐标y分量
         * @param[in]   pz：旋转轴上一点坐标z分量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        void setAxisPosition(double px, double py, double pz);

        /**
         * @brief       获取旋转轴上一点坐标。
         * @param[out]  px：旋转轴上一点坐标x分量[引用]
         * @param[out]  py：旋转轴上一点坐标y分量[引用]
         * @param[out]  pz：旋转轴上一点坐标z分量[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        void getAxisPosition(double & px, double & py, double & pz);

        /**
         * @brief       设置旋转轴方向。
         * @param[in]   dx：旋转轴方向x分量
         * @param[in]   dy：旋转轴方向y分量
         * @param[in]   dz：旋转轴方向z分量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        void setAxisDirection(double dx, double dy, double dz);

        /**
         * @brief       获取旋转轴方向。
         * @param[out]  dx：旋转轴方向x分量[引用]
         * @param[out]  dy：旋转轴方向y分量[引用]
         * @param[out]  dz：旋转轴方向z分量[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        void getAxisDirection(double & dx, double & dy, double & dz);

        /**
         * @brief       设置旋转弧度。
         * @param[in]   angle：旋转弧度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        void setAngleRadian(double angle);

        /**
         * @brief       获取设置的旋转弧度。
         * @return      旋转弧度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        double getAngleRadian();

    protected:
        /**
         * @brief       被旋转面拓扑信息。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        QList<VirtualShape> m_faces;

        /**
         * @brief       旋转弧度。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        double m_angle = 0.;

        /**
         * @brief       旋转轴上一点位置。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        double m_axisPos[3]{ 0., 0., 0. };

        /**
         * @brief       旋转轴方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-09
         */
        double m_axisDir[3]{ 0., 0., 1. };

    };

    /**
     * @brief       编辑倒圆面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-26
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelModifyBlend : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelModifyBlend);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        FITKAbsGeoModelModifyBlend() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        virtual ~FITKAbsGeoModelModifyBlend() = default;

        /**
         * @brief       获取几何命令类型。
         * @return      几何命令
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置倒圆面。
         * @param[in]   face：倒圆面拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        void setFace(VirtualShape face);

        /**
         * @brief       获取设置的面。
         * @return      倒圆面拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        VirtualShape getFace();

        /**
         * @brief       设置倒圆半径。
         * @param[in]   r：倒圆半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        void setRadii(double r);

        /**
         * @brief       获取设置的倒圆半径。
         * @return      倒圆半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-26
         */
        double getRadii();

    protected:
        /**
         * @brief       倒圆面拓扑信息。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        VirtualShape m_face;

        /**
         * @brief       倒圆半径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        double m_radii = 0.;

    };
}

#endif // !FITKAbsGeoModelOperFace_H
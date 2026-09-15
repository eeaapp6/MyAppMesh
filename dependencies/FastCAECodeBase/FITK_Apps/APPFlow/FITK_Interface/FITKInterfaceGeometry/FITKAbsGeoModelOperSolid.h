/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelOperSolid.h
 * @brief       体特征处理数据抽象类。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-26
 *********************************************************************/

#ifndef  FITKABSGEOMODELOPERSOLID_H
#define  FITKABSGEOMODELOPERSOLID_H

#include "FITKAbsGeoModelSolid.h"

#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       倒角抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-20
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelChamferSolid : public FITKAbsGeoModelSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelChamferSolid);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        FITKAbsGeoModelChamferSolid() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        virtual ~FITKAbsGeoModelChamferSolid() = default;

        /**
         * @brief       获取几何命令类型。[重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置被倒角模型的数据ID。
         * @param[in]   cmdId：模型数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        void setInputCmdId(int cmdId);

        /**
         * @brief       获取被倒角模型的数据ID。
         * @return      模型数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        int getInputCmdId();

        /**
         * @brief       设置被倒角的边虚拓扑数据索引列表。
         * @param[in]   edgeIndice：边虚拓扑ID列表[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        void setEdgeTopos(QList<VirtualShape> edgeTopos);

        /**
         * @brief       获取被倒角的边虚拓扑数据索引列表。
         * @return      边虚拓扑ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        QList<VirtualShape> & getEdgeTopos();

        /**
         * @brief       设置倒角距离。
         * @param[in]   dist：倒角距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        void setDistance(double dist);

        /**
         * @brief       获取倒角距离。
         * @return      倒角距离
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        double getDistance();

    protected:
        /**
         * @brief       进行倒角操作的数据ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        int m_solidCmdId = -1;

        /**
         * @brief       进行倒角操作的虚拓扑边索引列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        QList<VirtualShape> m_edgeVirtualTopos;

        /**
         * @brief       倒角距离。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        double m_dist = 0.;
    };

    /**
     * @brief       倒圆抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-20
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelFilletSolid : public FITKAbsGeoModelSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelFilletSolid);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        FITKAbsGeoModelFilletSolid() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        virtual ~FITKAbsGeoModelFilletSolid() = default;

        /**
         * @brief       获取几何命令类型。[重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置被倒圆模型的数据ID。
         * @param[in]   cmdId：模型数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        void setInputCmdId(int cmdId);

        /**
         * @brief       获取被倒圆模型的数据ID。
         * @return      模型数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        int getInputCmdId();

        /**
         * @brief       设置被倒圆的边虚拓扑数据ID列表。
         * @param[in]   edgeIds：边虚拓扑ID列表[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        void setEdgeTopos(QList<VirtualShape> edgeTopos);

        /**
         * @brief       获取被倒圆的边虚拓扑数据ID列表。
         * @return      边虚拓扑ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        QList<VirtualShape> & getEdgeTopos();

        /**
         * @brief       设置倒圆半径。
         * @param[in]   radius：倒圆半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        void setRadius(double radius);

        /**
         * @brief       获取倒圆半径。
         * @return      倒圆半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        double getRadius();

    protected:
        /**
         * @brief       进行倒角操作的数据ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        int m_solidCmdId = -1;

        /**
         * @brief       进行倒圆操作的虚拓扑边索引列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        QList<VirtualShape> m_edgeVirtualTopos;

        /**
         * @brief       倒圆半径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        double m_radius = 0.;
    };

    /**
     * @brief       特征移除抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-26
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelDefeatureSolid : public FITKAbsGeoModelSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelDefeatureSolid);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        FITKAbsGeoModelDefeatureSolid() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        virtual ~FITKAbsGeoModelDefeatureSolid() = default;

        /**
         * @brief       获取几何命令类型。[虚函数][重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        virtual FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置被移除特征模型的数据ID。
         * @param[in]   cmdId：模型数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        void setInputCmdId(int cmdId);

        /**
         * @brief       获取被移除特征模型的数据ID。
         * @return      模型数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        int getInputCmdId();

        /**
         * @brief       设置被移除特征面虚拓扑数据ID列表。
         * @param[in]   faceIds：面虚拓扑ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        void setFaceTopos(QList<VirtualShape> faceTopos);

        /**
         * @brief       获取被移除特征面虚拓扑数据ID列表。
         * @return      面虚拓扑ID列表[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        QList<VirtualShape> & getFaceTopos();

    protected:
        /**
         * @brief       进行被移除特征操作的体模型数据ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        int m_solidCmdId = -1;

        /**
         * @brief       进行倒角操作的虚拓扑面索引列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        QList<VirtualShape> m_faceVirtualTopos;

    };

    /**
     * @brief       删除倒角抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-26
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelRemoveChamferSolid : public FITKAbsGeoModelDefeatureSolid
    {
        // FITKCLASS(Interface, FITKAbsGeoModelRemoveChamferSolid);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        FITKAbsGeoModelRemoveChamferSolid() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        virtual ~FITKAbsGeoModelRemoveChamferSolid() = default;

        /**
         * @brief       获取几何命令类型。[重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    };

    /**
     * @brief       删除倒圆抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-26
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelRemoveFilletSolid : public FITKAbsGeoModelDefeatureSolid
    {
        // FITKCLASS(Interface, FITKAbsGeoModelRemoveFilletSolid);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        FITKAbsGeoModelRemoveFilletSolid() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        virtual ~FITKAbsGeoModelRemoveFilletSolid() = default;

        /**
         * @brief       获取几何命令类型。[重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    };

    /**
     * @brief       填补孔洞抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-26
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelFillHoleSolid : public FITKAbsGeoModelDefeatureSolid
    {
        // FITKCLASS(Interface, FITKAbsGeoModelFillHoleSolid);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        FITKAbsGeoModelFillHoleSolid() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        virtual ~FITKAbsGeoModelFillHoleSolid() = default;

        /**
         * @brief       获取几何命令类型。[重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-26
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    };

    /**
     * @brief   圆孔洞抽象类。
     * @author  fulipeng (flipengqd@yeah.net)
     * @date    2025-01-02
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCircularHoleSolid : public FITKAbsGeoModelSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelCircularHoleSolid);
    public:
        /**
         * @brief   构造函数。
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-02
         */
        FITKAbsGeoModelCircularHoleSolid() = default;

        /**
         * @brief   析构函数。[虚函数]
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-02
         */
        virtual ~FITKAbsGeoModelCircularHoleSolid() = default;

        /**
         * @brief   获取几何命令类型。[重写]
         * @return  命令类型
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-02
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief   设置孔洞点.
         * @param   x x坐标值
         * @param   y y坐标值
         * @param   z z坐标值
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-02
         */
        void setHolePoint(double x, double y, double z);
        void getHolePoint(double& x, double& y, double& z);
        /**
         * @brief   设置孔洞点.
         * @param   point* xyz的坐标值
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-02
         */
        void setHolePoint(double* point);
        void getHolePoint(double*& point);

        /**
         * @brief   设置翻转,相对于面的朝向.
         * @param   bool 是否翻转.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date     2025-01-02
         */
        void setFlip(bool isFilp);

        /**
         * @brief   获取翻转,相对于面的朝向.
         * @return  bool 是否翻转.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-02
         */
        bool getFlip();

        /**
         * @brief   设置孔的直径
         * @param   diameter 直径.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-02
         */
        void setDiameter(double diameter);

        /**
         * @brief   获取孔的直径
         * @return  diameter 直径.
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-02
         */
        double getDiameter();

        void setOperFace(VirtualShape faceShape);
        VirtualShape getOperFace();

        void setOperEdge1(VirtualShape edgeShape);
        VirtualShape getOperEdge1();
        void setDistanceLocateHole1(double distance);
        double getDistanceLocateHole1();

        void setOperEdge2(VirtualShape edgeShape);
        VirtualShape getOperEdge2();
        void setDistanceLocateHole2(double distance);
        double getDistanceLocateHole2();

    protected:

        /**
         * @brief  孔的位置，通过计算获得.
         * @author fulipeng (flipengqd@yeah.net)
         * @date   2025-01-02
         */
        double _holePoint[3];

        /**
         * @brief  是否翻转孔的方向
         * @author fulipeng (flipengqd@yeah.net)
         * @date   2025-01-02
         */
        bool _flip = false;

        /**
         * @brief  孔的直径.
         * @author fulipeng (flipengqd@yeah.net)
         * @date   2025-01-02
         */
        double _diameter = 0.6;

        /**
         * @brief  操作的几何面.
         * @author fulipeng (flipengqd@yeah.net)
         * @date   2025-01-02
         */
        VirtualShape _geoOperFace;

        /**
         * @brief  第一条边.
         * @author fulipeng (flipengqd@yeah.net)
         * @date   2025-01-02
         */
        VirtualShape _geoEdge01;

        /**
         * @brief  孔距离线1的距离.
         * @author fulipeng (flipengqd@yeah.net)
         * @date   2025-01-02
         */
        double _distanceLocateHole01 = 2;

        /**
         * @brief  第二条边.
         * @author fulipeng (flipengqd@yeah.net)
         * @date   2025-01-02
         */
        VirtualShape _geoEdge02;

        /**
         * @brief  孔距离线2的距离.
         * @author fulipeng (flipengqd@yeah.net)
         * @date   2025-01-02
         */
        double _distanceLocateHole02 = 2;
    };
}

#endif // !FITKABSGEOMODELOPERSOLID_H

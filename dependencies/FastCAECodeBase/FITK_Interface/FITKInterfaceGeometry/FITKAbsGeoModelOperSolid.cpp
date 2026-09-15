/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelOperSolid.h" 

namespace Interface
{
    // 倒角。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelChamferSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTChamferSolid;
    }

    void FITKAbsGeoModelChamferSolid::setInputCmdId(int cmdId)
    {
        // 被倒角模型数据ID。
        m_solidCmdId = cmdId;
    }

    int FITKAbsGeoModelChamferSolid::getInputCmdId()
    {
        return m_solidCmdId;
    }

    void FITKAbsGeoModelChamferSolid::setEdgeTopos(QList<VirtualShape> edgeTopos)
    {
        // 被倒角边虚拓扑ID列表。
        m_edgeVirtualTopos = edgeTopos;
    }

    QList<VirtualShape> & FITKAbsGeoModelChamferSolid::getEdgeTopos()
    {
        return m_edgeVirtualTopos;
    }

    void FITKAbsGeoModelChamferSolid::setDistance(double dist)
    {
        // 倒角距离。
        m_dist = dist;
    }

    double FITKAbsGeoModelChamferSolid::getDistance()
    {
        return m_dist;
    }
    //@}

    // 倒圆。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelFilletSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTFilletSolid;
    }

    void FITKAbsGeoModelFilletSolid::setInputCmdId(int cmdId)
    {
        // 被倒圆模型数据ID。
        m_solidCmdId = cmdId;
    }

    int FITKAbsGeoModelFilletSolid::getInputCmdId()
    {
        return m_solidCmdId;
    }

    void FITKAbsGeoModelFilletSolid::setEdgeTopos(QList<VirtualShape> edgeTopos)
    {
        // 被倒角边虚拓扑ID列表。
        m_edgeVirtualTopos = edgeTopos;
    }

    QList<VirtualShape> & FITKAbsGeoModelFilletSolid::getEdgeTopos()
    {
        return m_edgeVirtualTopos;
    }

    void FITKAbsGeoModelFilletSolid::setRadius(double radius)
    {
        // 倒圆半径。
        m_radius = radius;
    }

    double FITKAbsGeoModelFilletSolid::getRadius()
    {
        return m_radius;
    }
    //@}

    // 特征移除抽象类。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelDefeatureSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTDefeature;
    }

    void FITKAbsGeoModelDefeatureSolid::setInputCmdId(int cmdId)
    {
        // 被移除特征模型数据ID。
        m_solidCmdId = cmdId;
    }

    int FITKAbsGeoModelDefeatureSolid::getInputCmdId()
    {
        return m_solidCmdId;
    }

    void FITKAbsGeoModelDefeatureSolid::setFaceTopos(QList<VirtualShape> faceTopos)
    {
        // 被移除特征面虚拓扑ID。
        m_faceVirtualTopos = faceTopos;
    }

    QList<VirtualShape> & FITKAbsGeoModelDefeatureSolid::getFaceTopos()
    {
        return m_faceVirtualTopos;
    }
    //@}

    // 移除倒角。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRemoveChamferSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRemoveChamfer;
    }
    //@}

    // 移除倒圆。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRemoveFilletSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRemoveFillet;
    }
    //@}

    // 填补孔洞。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelFillHoleSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTFillHole;
    }
    //@}

    // 添加圆孔。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCircularHoleSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCircularHole;
    }
    //@}

    void FITKAbsGeoModelCircularHoleSolid::setHolePoint(double x, double y, double z)
    {
        _holePoint[0] = x;
        _holePoint[1] = y;
        _holePoint[2] = z;
    }

    void FITKAbsGeoModelCircularHoleSolid::getHolePoint(double& x, double& y, double& z)
    {
        x = _holePoint[0];
        y = _holePoint[1];
        z = _holePoint[2];
    }

    void FITKAbsGeoModelCircularHoleSolid::setHolePoint(double* point)
    {
        _holePoint[0] = point[0];
        _holePoint[1] = point[1];
        _holePoint[2] = point[2];
    }

    void FITKAbsGeoModelCircularHoleSolid::getHolePoint(double*& point)
    {
        point[0] = _holePoint[0];
        point[1] = _holePoint[1];
        point[2] = _holePoint[2];
    }

    void FITKAbsGeoModelCircularHoleSolid::setFlip(bool isFilp)
    {
        _flip = isFilp;
    }

    bool FITKAbsGeoModelCircularHoleSolid::getFlip()
    {
        return _flip;
    }

    void FITKAbsGeoModelCircularHoleSolid::setDiameter(double diameter)
    {
        _diameter = diameter;
    }

    double FITKAbsGeoModelCircularHoleSolid::getDiameter()
    {
        return _diameter;
    }

    void FITKAbsGeoModelCircularHoleSolid::setOperFace(VirtualShape faceShape)
    {
        _geoOperFace = faceShape;
    }

    VirtualShape FITKAbsGeoModelCircularHoleSolid::getOperFace()
    {
        return _geoOperFace;
    }

    void FITKAbsGeoModelCircularHoleSolid::setOperEdge1(VirtualShape edgeShape)
    {
        _geoEdge01 = edgeShape;
    }

    VirtualShape FITKAbsGeoModelCircularHoleSolid::getOperEdge1()
    {
        return _geoEdge01;
    }

    void FITKAbsGeoModelCircularHoleSolid::setDistanceLocateHole1(double distance)
    {
        _distanceLocateHole01 = distance;
    }

    double FITKAbsGeoModelCircularHoleSolid::getDistanceLocateHole1()
    {
        return _distanceLocateHole01;
    }

    void FITKAbsGeoModelCircularHoleSolid::setOperEdge2(VirtualShape edgeShape)
    {
        _geoEdge02 = edgeShape;
    }

    VirtualShape FITKAbsGeoModelCircularHoleSolid::getOperEdge2()
    {
        return _geoEdge02;
    }
    void FITKAbsGeoModelCircularHoleSolid::setDistanceLocateHole2(double distance)
    {
        _distanceLocateHole02 = distance;
    }
    double FITKAbsGeoModelCircularHoleSolid::getDistanceLocateHole2()
    {
        return _distanceLocateHole02;
    }
}

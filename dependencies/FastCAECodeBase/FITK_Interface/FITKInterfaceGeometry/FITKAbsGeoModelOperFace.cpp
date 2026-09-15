/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelOperFace.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
namespace Interface
{
    void FITKAbsGeoModelOperFace::add(VirtualShape geoCommandShape)
    {
        m_tempVShapes.append(geoCommandShape);
    }

    void FITKAbsGeoModelOperFace::add(QList<VirtualShape> geoCommandShapes)
    {
        m_tempVShapes.append(geoCommandShapes);
    }

    void FITKAbsGeoModelOperFace::set(VirtualShape geoCommandShape)
    {
        m_tempVShapes.clear();
        m_tempVShapes.append(geoCommandShape);
    }

    void FITKAbsGeoModelOperFace::set(QList<VirtualShape> geoCommandShapes)
    {
        m_tempVShapes = geoCommandShapes;
    }


    void FITKAbsGeoModelOperFace::remove(int index)
    {
        m_tempVShapes.removeAt(index);
    }

    void FITKAbsGeoModelOperFace::clear()
    {
        m_tempVShapes.clear();
    }

    void FITKAbsGeoModelOperFace::setType(GeoFaceOperType t)
    {
        _faceOperType = t;
    }

    FITKAbsGeoModelOperFace::GeoFaceOperType FITKAbsGeoModelOperFace::getType()
    {
        return _faceOperType;
    }

    QList<VirtualShape> FITKAbsGeoModelOperFace::getVShapes()
    {
        return m_tempVShapes;
    }

    QList<int> FITKAbsGeoModelOperFace::getModelOperFacesID()
    {
        QList<int> _geoCommands;

        for (int i = 0; i < m_tempVShapes.size(); i++)
        {
            _geoCommands.append(m_tempVShapes[i].CmdId);
        }
        return _geoCommands;
    }

    void FITKAbsGeoModelOperFace::setTolerance(double tolerance)
    {
        _tolerance = tolerance;
    }

    double FITKAbsGeoModelOperFace::getTolerance()
    {
        return _tolerance;
    }


    Interface::FITKAbsVirtualTopo* FITKAbsGeoModelOperFace::getRefModelVirtualTopo(Interface::FITKGeoEnum::VTopoShapeType shapeType, int m_solidCmdId, int virtualTopoId)
    {
        //数据获取
        Interface::FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(m_solidCmdId);
        if (comm == nullptr) return nullptr;
        auto sVirtualTopoManager = comm->getShapeVirtualTopoManager(shapeType);
        if (!sVirtualTopoManager) return nullptr;
        Interface::FITKAbsVirtualTopo* faceVirtualShape = sVirtualTopoManager->getDataByIndex(virtualTopoId);
        return faceVirtualShape;
    }

    //移除面
    //{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelOperFaceRemoveFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRemoveFace;
    }
    //}

    //延申面
    //{

    void FITKAbsGeoModelOperFaceExtendFace::setEdges(QList<VirtualShape> edges)
    {
        _tempEdgeShapes = edges;
    }

    QList<VirtualShape> FITKAbsGeoModelOperFaceExtendFace::getEdges()
    {
        return _tempEdgeShapes;
    }

    void FITKAbsGeoModelOperFaceExtendFace::setFaces(QList<VirtualShape> face)
    {
        m_tempVShapes = face;
    }

    QList<VirtualShape> FITKAbsGeoModelOperFaceExtendFace::getFaces()
    {
        return m_tempVShapes;
    }

    void FITKAbsGeoModelOperFaceExtendFace::setTagFaces(QList<VirtualShape> faces)
    {
        _tempTagFaceShapes = faces;
    }

    QList<VirtualShape> FITKAbsGeoModelOperFaceExtendFace::getTagFaces()
    {
        return _tempTagFaceShapes;
    }

    void FITKAbsGeoModelOperFaceExtendFace::setDistance(double dis)
    {
        _distance = dis;
    }

    double FITKAbsGeoModelOperFaceExtendFace::getDistance()
    {
        return _distance;
    }

    void FITKAbsGeoModelOperFaceExtendFace::setTrim(bool isTrim)
    {
        _trim = isTrim;
    }

    bool FITKAbsGeoModelOperFaceExtendFace::getTrim()
    {
        return _trim;
    }

    void FITKAbsGeoModelOperFaceExtendFace::setSpecifyMode(SpecifyModel sm)
    {
        _selectedMode.first = sm;
    }

    void FITKAbsGeoModelOperFaceExtendFace::setMethodModel(MethodModel mm)
    {
        _selectedMode.second = mm;
    }

    FITKAbsGeoModelOperFaceExtendFace::SpecifyModel FITKAbsGeoModelOperFaceExtendFace::getSpecifyMode()
    {
        return _selectedMode.first;
    }

    FITKAbsGeoModelOperFaceExtendFace::MethodModel FITKAbsGeoModelOperFaceExtendFace::getMethodModel()
    {
        return _selectedMode.second;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelOperFaceExtendFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTExtendFace;
    }
    //}

    //修复面
    //{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelOperFaceRepairFaces::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRepairFace;
    }
    //}

    //填补缝隙
    //{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelOperFaceFillGaps::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTFillGapsFace;
    }
    //}

    //填补孔洞
    //{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelOperFaceFillHoles::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTFillHolesFace;
    }
    //}

    //删除悬浮边
    //{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelOperFaceDeleteFloatingEdge::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTDeleteFloatingEdge;
    }

    void FITKAbsGeoModelOperFaceDeleteFloatingEdge::setMFace(VirtualShape vface)
    {
        m_face = vface;
    }
    VirtualShape FITKAbsGeoModelOperFaceDeleteFloatingEdge::getMFace()
    {
        return m_face;
    }
    //}

    // 编辑圆柱面。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelModifyCylinder::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTModifyCylinder;
    }

    void FITKAbsGeoModelModifyCylinder::setCylinderFace(VirtualShape face)
    {
        m_face = face;
    }

    VirtualShape FITKAbsGeoModelModifyCylinder::getCylinderFace()
    {
        return m_face;
    }

    void FITKAbsGeoModelModifyCylinder::setRadius(double radius)
    {
        m_radius = radius;
    }

    double FITKAbsGeoModelModifyCylinder::getRadius()
    {
        return m_radius;
    }
    //@}

    // 平移面。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelTransformFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTTransformFace;
    }

    void FITKAbsGeoModelTransformFace::setFaces(QList<VirtualShape> faces)
    {
        // 替换设置的面组。
        m_faces = faces;
    }

    void FITKAbsGeoModelTransformFace::addFace(VirtualShape face)
    {
        // 追加不重复的面。
        for (const VirtualShape & shape : m_faces)
        {
            if (shape.CmdId == face.CmdId && 
                (shape.VirtualTopoIndex == shape.VirtualTopoIndex ||
                shape.VirtualTopoId == shape.VirtualTopoId))
            {
                return;
            }
        }

        m_faces.push_back(face);
    }

    QList<VirtualShape> FITKAbsGeoModelTransformFace::getFaces()
    {
        return m_faces;
    }

    void FITKAbsGeoModelTransformFace::setDirection(double dx, double dy, double dz)
    {
        // 保存平移方向。
        m_direction[0] = dx;
        m_direction[1] = dy;
        m_direction[2] = dz;
    }

    void FITKAbsGeoModelTransformFace::getDirection(double & dx, double & dy, double & dz)
    {
        // 获取平移方向。
        dx = m_direction[0];
        dy = m_direction[1];
        dz = m_direction[2];
    }

    void FITKAbsGeoModelTransformFace::setDistance(double dist)
    {
        // 保存平移距离。
        m_distance = dist;
    }

    double FITKAbsGeoModelTransformFace::getDistance()
    {
        return m_distance;
    }
    //@}

    // 旋转面。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRotateFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRotateFace;
    }

    void FITKAbsGeoModelRotateFace::setFaces(QList<VirtualShape> faces)
    {
        // 替换设置的面组。
        m_faces = faces;
    }

    void FITKAbsGeoModelRotateFace::addFace(VirtualShape face)
    {
        // 追加不重复的面。
        for (const VirtualShape & shape : m_faces)
        {
            if (shape.CmdId == face.CmdId &&
                (shape.VirtualTopoIndex == shape.VirtualTopoIndex ||
                    shape.VirtualTopoId == shape.VirtualTopoId))
            {
                return;
            }
        }

        m_faces.push_back(face);
    }

    QList<VirtualShape> FITKAbsGeoModelRotateFace::getFaces()
    {
        return m_faces;
    }

    void FITKAbsGeoModelRotateFace::setAxisPosition(double px, double py, double pz)
    {
        // 保存旋转轴上坐标。
        m_axisPos[0] = px;
        m_axisPos[1] = py;
        m_axisPos[2] = pz;
    }

    void FITKAbsGeoModelRotateFace::getAxisPosition(double & px, double & py, double & pz)
    {
        // 获取旋转轴上坐标。
        px = m_axisPos[0];
        py = m_axisPos[1];
        pz = m_axisPos[2];
    }

    void FITKAbsGeoModelRotateFace::setAxisDirection(double dx, double dy, double dz)
    {
        // 保存旋转轴方向。
        m_axisDir[0] = dx;
        m_axisDir[1] = dy;
        m_axisDir[2] = dz;
    }

    void FITKAbsGeoModelRotateFace::getAxisDirection(double & dx, double & dy, double & dz)
    {
        // 获取旋转轴方向。
        dx = m_axisDir[0];
        dy = m_axisDir[1];
        dz = m_axisDir[2];
    }

    void FITKAbsGeoModelRotateFace::setAngleRadian(double angle)
    {
        // 保存旋转弧度。
        m_angle = angle;
    }

    double FITKAbsGeoModelRotateFace::getAngleRadian()
    {
        return m_angle;
    }
    //@}

    // 编辑倒圆面。
    // @{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelModifyBlend::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTModifyBlend;
    }

    void FITKAbsGeoModelModifyBlend::setFace(VirtualShape face)
    {
        // 设置倒圆面。
        m_face = face;
    }

    VirtualShape FITKAbsGeoModelModifyBlend::getFace()
    {
        return m_face;
    }

    void FITKAbsGeoModelModifyBlend::setRadii(double r)
    {
        m_radii = r;
    }

    double FITKAbsGeoModelModifyBlend::getRadii()
    {
        return m_radii;
    }
    //@}
}
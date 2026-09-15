/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGeometryMeshVS.h"

#include "FITKGeometryMeshEntity.h"

namespace Interface
{
    FITKGeometryMeshVS::~FITKGeometryMeshVS()
    {
        //清理全部
        this->clear();
    }

    void FITKGeometryMeshVS::insertPoint(int id, FITKGeoMeshVSPt* pt)
    {
        //错误判断
        if (pt == nullptr || id < 0) return;
        //已经存在
        if (_points.contains(id))
            delete _points[id];
        _points.insert(id, pt);
    }

    void FITKGeometryMeshVS::insertEdge(int id, FITKGeoMeshVSEdgeEntity* edge)
    {
        //错误判断
        if (edge == nullptr || id < 0) return;
        if (_edges.contains(id))
            delete _edges[id];
        _edges.insert(id, edge);
    }

    void FITKGeometryMeshVS::insertFace(int id, FITKGeoMeshVSFaceEntity* face)
    {
        //错误判断
        if (face == nullptr || id < 0) return;
        if (_faces.contains(id))
            delete _faces[id];
        _faces.insert(id, face);
    }
 
    const QHash<int, FITKGeoMeshVSPt*> & FITKGeometryMeshVS::getPointItems() const
    {
        return _points;
    }

    const QHash<int, FITKGeoMeshVSEdgeEntity*> & FITKGeometryMeshVS::getEdgeItems() const
    {
        return _edges;
    }

    const QHash<int, FITKGeoMeshVSFaceEntity*> & FITKGeometryMeshVS::getFaceItems() const
    {
        return _faces;
    }

    void FITKGeometryMeshVS::clear()
    {
        //清空点对象
        QList<FITKGeoMeshVSPt*>  pts = _points.values();
        for (FITKGeoMeshVSPt* pt : pts)
            delete pt;
        _points.clear();
        //清空边对象
        QList<FITKGeoMeshVSEdgeEntity*>  edges = _edges.values();
        for (FITKGeoMeshVSEdgeEntity* edge : edges)
            delete edge;
        _edges.clear();
        //清空面对象
        QList<FITKGeoMeshVSFaceEntity*>  faces = _faces.values();
        for (FITKGeoMeshVSFaceEntity* face : faces)
            delete face;
        _faces.clear();
    }

    bool FITKGeometryMeshVS::isValid()
    {
        //判断是否为空
        return _points.size() + _edges.size() + _faces.size() > 0;
    }

}


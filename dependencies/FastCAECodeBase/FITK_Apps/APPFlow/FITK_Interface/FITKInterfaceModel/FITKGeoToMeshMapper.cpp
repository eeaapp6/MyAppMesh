/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGeoToMeshMapper.h"

#include <QMutexLocker>

namespace Interface
{
    QMutex FITKGeoToMeshMapper::_mutex;

    FITKGeoToMeshMapper::FITKGeoToMeshMapper()
    {

    }

    FITKGeoToMeshMapper::~FITKGeoToMeshMapper()
    {

    }

    void FITKGeoToMeshMapper::clear()
    {
        _meshVerOfGeoNode.clear();
        _meshEleOfGeoEdge.clear();
        _meshEleOfGeoFace.clear();
        _meshEleOfGeoCoplane.clear();
        _meshEleOfGeoSolid.clear();
    }

    void FITKGeoToMeshMapper::copy(FITKGeoToMeshMapper * copyObj)
    {
        if (copyObj == nullptr) {
            return;
        }

        _meshVerOfGeoNode = copyObj->_meshVerOfGeoNode;
        _meshEleOfGeoEdge = copyObj->_meshEleOfGeoEdge;
        _meshEleOfGeoWireEdge = copyObj->_meshEleOfGeoWireEdge;
        _meshEleOfGeoFace = copyObj->_meshEleOfGeoFace;
        _meshEleOfGeoCoplane = copyObj->_meshEleOfGeoCoplane;
        _meshEleOfGeoSolid = copyObj->_meshEleOfGeoSolid;
    }

    void FITKGeoToMeshMapper::insertMeshVertexOfGeoNode(int geoNodeID, int meshVertexID)
    {
        QMutexLocker locker(&_mutex);

        _meshVerOfGeoNode.insert(geoNodeID, meshVertexID);
    }

    void FITKGeoToMeshMapper::removeMeshVertexOfGeoNode(int geoNodeID)
    {
        QMutexLocker locker(&_mutex);

        _meshVerOfGeoNode.remove(geoNodeID);
    }

    const QHash<int, int>& FITKGeoToMeshMapper::getMeshVertexOfGeoNode()
    {
        return _meshVerOfGeoNode;
    }

    int FITKGeoToMeshMapper::getMeshVertexOfGeoNode(int geoNodeID)
    {
        return _meshVerOfGeoNode.value(geoNodeID);
    }

    void FITKGeoToMeshMapper::insertMeshEleOfGeoFace(int faceID, int eleID, int faceIndex)
    {
        QMutexLocker locker(&_mutex);

        _meshEleOfGeoFace[faceID].insert(eleID, faceIndex);
    }

    void FITKGeoToMeshMapper::removeMeshEleOfGeoFace(int faceID)
    {
        QMutexLocker locker(&_mutex);

        _meshEleOfGeoFace.remove(faceID);
    }

    const QHash<int, QHash<int, int>>& FITKGeoToMeshMapper::getMeshEleOfGeoFace()
    {
        return _meshEleOfGeoFace;
    }

    void FITKGeoToMeshMapper::insertMeshEleOfGeoCoplane(FITKGeoToMeshMapStrSolidFace solidAndFace, int eleID, int faceIndex)
    {
        QMutexLocker locker(&_mutex);

        _meshEleOfGeoCoplane[solidAndFace].insert(eleID, faceIndex);
    }

    const QHash<FITKGeoToMeshMapStrSolidFace, QHash<int, int>>& FITKGeoToMeshMapper::getMeshEleOfGeoCoplane()
    {
        return _meshEleOfGeoCoplane;
    }

    void FITKGeoToMeshMapper::insertMeshEleOfGeoEdge(int edgeID, int eleID, int edgeIndex)
    {
        QMutexLocker locker(&_mutex);

        _meshEleOfGeoEdge[edgeID].insert(eleID, edgeIndex);
    }

    void FITKGeoToMeshMapper::removeMeshEleOfGeoEdge(int edgeID)
    {
        QMutexLocker locker(&_mutex);

        _meshEleOfGeoEdge.remove(edgeID);
    }

    const QHash<int, QHash<int, int>>& FITKGeoToMeshMapper::getMeshEleOfGeoEdge()
    {
        return _meshEleOfGeoEdge;
    }

    void FITKGeoToMeshMapper::insertMeshEleOfGeoWireEdge(int wireEdgeID, int eleID)
    {
        QMutexLocker locker(&_mutex);

        _meshEleOfGeoWireEdge[wireEdgeID].append(eleID);
    }

    const QHash<int, QList<int>>& FITKGeoToMeshMapper::getMeshEleOfGeoWireEdge()
    {
        return _meshEleOfGeoWireEdge;
    }

    void FITKGeoToMeshMapper::insertMapOfGeoSolid(int solidID, int meshEleID)
    {
        QMutexLocker locker(&_mutex);

        _meshEleOfGeoSolid[solidID].append(meshEleID);
    }

    void FITKGeoToMeshMapper::removeMapOfGeoSolid(int solidID)
    {
        QMutexLocker locker(&_mutex);

        _meshEleOfGeoSolid.remove(solidID);
    }

    const QHash<int, QList<int>>& FITKGeoToMeshMapper::getMeshEleOfGeoSolid()
    {
        return _meshEleOfGeoSolid;
    }
}

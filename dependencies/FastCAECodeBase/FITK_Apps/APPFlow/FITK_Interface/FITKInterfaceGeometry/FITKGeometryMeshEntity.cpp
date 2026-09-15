/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGeometryMeshEntity.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"


namespace Interface
{
    FITKGeoMeshVSPt::FITKGeoMeshVSPt(double* coor)
    {
        if (coor == nullptr)  return;
        //设置坐标
        for (int i = 0; i < 3; ++i)
            _coor[i] = coor[i];
    }

    FITKGeoMeshVSPt::FITKGeoMeshVSPt(const float& x, const float& y, const float& z)
    {
        //设置坐标
        this->setXYZ(x, y, z);
    }

    void FITKGeoMeshVSPt::setXYZ(const float& x, const float& y, const float& z)
    {
        //复制数据
        _coor[0] = x;
        _coor[1] = y;
        _coor[2] = z;
    }

    float FITKGeoMeshVSPt::x()
    {
        //返回x
        return _coor[0];
    }

    float FITKGeoMeshVSPt::y()
    {
        //返回y
        return _coor[1];
    }

    float FITKGeoMeshVSPt::z()
    {
        //返回z
        return _coor[2];
    }

    FITKGeoMeshVSTri::FITKGeoMeshVSTri(const int p1, const int p2, const int p3)
    {
        _p[0] = p1; _p[1] = p2; _p[2] = p3;
    }




    int FITKGeoMeshVSTri::getNode(const int index)
    {
        //返回点
        if (index < 0 || index >2) return -1;
        return _p[index];
    }


    const int* FITKGeoMeshVSTri::getNode() const
    {
        return _p;
    }

    FITKGeoMeshVSFaceEntity::~FITKGeoMeshVSFaceEntity()
    {
        //清空内存
        this->clear();
    }

    void FITKGeoMeshVSFaceEntity::clear()
    {
        const int n = _triList.count();
        for (int i = 0; i < n; ++i)
        {
            //释放内存
            FITKGeoMeshVSTri* t = _triList.at(i);
            if (t) delete t;
        }
        //清空
        _triList.clear();
    }

    void FITKGeoMeshVSFaceEntity::appendMeshTri(FITKGeoMeshVSTri* tri)
    {
        //添加三角面
        if (tri != nullptr) _triList.append(tri);
    }

    int FITKGeoMeshVSFaceEntity::getTriangleCount()
    {
        return _triList.size();
    }


    const QList<FITKGeoMeshVSTri*>& FITKGeoMeshVSFaceEntity::getTriangles() const
    {
        //返回三角面列表
        return _triList;
    }

    FITKGeoMeshVSEdgeEntity::~FITKGeoMeshVSEdgeEntity()
    {
        this->clear();
    }

    void FITKGeoMeshVSEdgeEntity::clear()
    {
        const int n = _ptList.count();
        for (int i = 0; i < n; ++i)
        {
            //释放内存
            FITKGeoMeshVSPt* p = _ptList.at(i);
            if (p) delete p;
        }
        _ptList.clear();
    }

    void FITKGeoMeshVSEdgeEntity::appendPoint(FITKGeoMeshVSPt* pt)
    {
        //添加点
        if (pt) _ptList.append(pt);
    }

    int FITKGeoMeshVSEdgeEntity::getPointCount()
    {
        return _ptList.count();
    }

    const QList<FITKGeoMeshVSPt*>& FITKGeoMeshVSEdgeEntity::getPoints() const
    {
        //返回点列表
        return _ptList;
    }

}


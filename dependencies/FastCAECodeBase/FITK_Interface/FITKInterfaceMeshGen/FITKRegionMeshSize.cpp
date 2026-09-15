/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRegionMeshSize.h"

namespace Interface
{

    FITKAbstractRegionMeshSize::RegionType Interface::FITKAbstractRegionMeshSize::getRegionType()
    {
        //错误值
        return RegionNone;
    }

    FITKAbstractRegionMeshSize::~FITKAbstractRegionMeshSize()
    {
    }

    void FITKAbstractRegionMeshSize::setDivision(int dim, int divNum)
    {
        //错误判断
        if (dim < 0 || dim >= 3) return;
        _division[dim] = divNum;
    }

    void FITKAbstractRegionMeshSize::setGrading(int dim, double grad)
    {
        //错误判断
        if (dim < 0 || dim >= 3) return;
        _grading[dim] = grad;
    }

    int FITKAbstractRegionMeshSize::getDivision(int dim)
    {
        //错误判断
        if (dim < 0 || dim >= 3) return -1;
        return _division[dim];
    }

    double FITKAbstractRegionMeshSize::getGrading(int dim)
    {
        //错误判断
        if (dim < 0 || dim >= 3) return -1;
        return _grading[dim];
    }

    void FITKAbstractRegionMeshSize::insertBoundary(int faceID, BoundaryType bt)
    {
        _boundarys.insert(faceID, bt);
    }

    Interface::FITKAbstractRegionMeshSize::BoundaryType FITKAbstractRegionMeshSize::getBoundary(int faceID)
    {
        return _boundarys[faceID];
    }

    void FITKAbstractRegionMeshSize::insertFaceName(int faceID, QString bn)
    {
        _facesName.insert(faceID, bn);
    }

    QString FITKAbstractRegionMeshSize::getFaceName(int faceID)
    {
        return _facesName[faceID];
    }

    double FITKAbstractRegionMeshSize::getInternalSize()
    {
        return getValueT<double>("InternalSize");
    }
    void FITKAbstractRegionMeshSize::setInternalSize(double vIn)
    {
        setValue("InternalSize", vIn);
    }
    double FITKAbstractRegionMeshSize::getExternalSize()
    {
        return getValueT<double>("ExternalSize");
    }
    void FITKAbstractRegionMeshSize::setExternalSize(double vOut)
    {
        setValue("ExternalSize", vOut);
    }
    double FITKAbstractRegionMeshSize::getThickness()
    {
        return getValueT<double>("Thickness");
    }
    void FITKAbstractRegionMeshSize::setThickness(double thickness)
    {
        setValue("Thickness", thickness);
    }

    QList<FITKAbstractRegionMeshSize*> FITKRegionMeshSizeManager::getRigonByType(FITKAbstractRegionMeshSize::RegionType rt)
    {
        QList<FITKAbstractRegionMeshSize*> rs;
        //遍历查找
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractRegionMeshSize* r = this->getDataByIndex(i);
            if(r == nullptr) continue;
            //类型判断
            if (rt == r->getRegionType())
                rs.append(r);
        }
        return rs;
    }

}


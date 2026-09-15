/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGlobalMeshGenerateAlgorithmInfo.h"

namespace Interface
{
    FITKGlobalMeshGenerateAlgorithmInfo::FITKGlobalMeshGenerateAlgorithmInfo()
    {
        this->setValue("Dimension", 3);
        this->setValue("2DAlgorithm", FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::Delaunay2D);
        this->setValue("3DAlgorithm", FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D::Delaunay3D);
        this->setValue("2DReAlgorithm", FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D::Blossom);
        this->setValue("ReTriangularMesh", false);
        this->setValue("SubdivisionAlgorithm", FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::None);
        this->setValue("SmoothStep", 1.0);
        this->setValue("ElementOrder", 1);
        this->setValue("UseIncElem", false);
        this->setValue("GeometryStitch", true);
        this->setValue("GeoStitchTol", 0.001);
        this->setValue("GeometryCoherence", false);
    }

    void FITKGlobalMeshGenerateAlgorithmInfo::setMeshGenerateDimension(int dim)
    {
        //设置2DAlgorithm
        this->setValue("Dimension", dim);
    }
    int FITKGlobalMeshGenerateAlgorithmInfo::getMeshGenerateDimension()
    {
        //获取2DAlgorithm
        return FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D(this->getValueT<int>("Dimension"));
    }

    void FITKGlobalMeshGenerateAlgorithmInfo::set2DAlgorithm(FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D algorithm2D)
    {
        //设置2DAlgorithm
        this->setValue("2DAlgorithm", algorithm2D);
    }
    FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D FITKGlobalMeshGenerateAlgorithmInfo::get2DAlgorithm()
    {
        //获取2DAlgorithm
        return FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D(this->getValueT<int>("2DAlgorithm"));
    }
    void FITKGlobalMeshGenerateAlgorithmInfo::set3DAlgorithm(FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D algorithm3D)
    {
        //设置3DAlgorithm
        this->setValue("3DAlgorithm", algorithm3D);
    }
    FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D FITKGlobalMeshGenerateAlgorithmInfo::get3DAlgorithm()
    {
        //获取3DAlgorithm
        return FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D(this->getValueT<int>("3DAlgorithm"));
    }
    void FITKGlobalMeshGenerateAlgorithmInfo::set2DRecombinationAlgorithm(FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D reAlgorithm2D)
    {
        //设置2d重组算法
        this->setValue("2DReAlgorithm", reAlgorithm2D);
    }
    FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D FITKGlobalMeshGenerateAlgorithmInfo::get2DRecombinationAlgorithm()
    {
        //获取2d重组算法
        return FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D(this->getValueT<int>("2DReAlgorithm"));
    }
    void FITKGlobalMeshGenerateAlgorithmInfo::setRecombineAllTriMeshes(bool reTriMesh)
    {
        //设置重新组合所有三角形网格
        this->setValue("ReTriangularMesh", reTriMesh);
    }
    bool FITKGlobalMeshGenerateAlgorithmInfo::getRecombineAllTriMeshes()
    {
        //是否重新组合所有三角形网格
        return this->getValueT<bool>("ReTriangularMesh");
    }
    void FITKGlobalMeshGenerateAlgorithmInfo::setSubdivisionAlgorithm(FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm subdivisionAlgorithm)
    {
        //设置细分算法
        this->setValue("SubdivisionAlgorithm", subdivisionAlgorithm);
    }
    FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm FITKGlobalMeshGenerateAlgorithmInfo::getSubdivisionAlgorithm()
    {
        //获取细分算法
        return FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm(this->getValueT<int>("SubdivisionAlgorithm"));
    }
    void FITKGlobalMeshGenerateAlgorithmInfo::setSmoothStep(int smoothStep)
    {
        //设置Smoothing Steps
        this->setValue("SmoothStep", smoothStep);
    }
    int FITKGlobalMeshGenerateAlgorithmInfo::getSmoothStep()
    {
        //获取Smoothing Steps
        return this->getValueT<int>("SmoothStep");
    }
    void FITKGlobalMeshGenerateAlgorithmInfo::setElementOrder(int eleOrader)
    {
        //设置单元阶数
        this->setValue("ElementOrder", eleOrader);
    }
    int FITKGlobalMeshGenerateAlgorithmInfo::getElementOrder()
    {
        //获取单元阶数
        return this->getValueT<int>("ElementOrder");
    }
    void FITKGlobalMeshGenerateAlgorithmInfo::setUseIncElem(bool useIncElem)
    {
        //设置不完整的单元
        this->setValue("UseIncElem", useIncElem);
    }
    bool FITKGlobalMeshGenerateAlgorithmInfo::getUseIncElem()
    {
        //是否设置不完整的单元
        return this->getValueT<bool>("UseIncElem");
    }
    void FITKGlobalMeshGenerateAlgorithmInfo::setGeometryStitch(bool isStitch)
    {
        //设置几何缝合
        this->setValue("GeometryStitch", isStitch);
    }
    bool FITKGlobalMeshGenerateAlgorithmInfo::getGeometryStitch()
    {
        //是否设置几何缝合
        return this->getValueT<bool>("GeometryStitch");
    }
    void FITKGlobalMeshGenerateAlgorithmInfo::setGeometryStitchTolerance(double tol)
    {
        //设置几何缝合容差
        this->setValue("GeoStitchTol", tol);
    }
    double FITKGlobalMeshGenerateAlgorithmInfo::getGeometryStitchTolerance()
    {
        //获取几何缝合容差
        return this->getValueT<double>("GeoStitchTol");
    }
    void FITKGlobalMeshGenerateAlgorithmInfo::setGeometryCoherence(bool isCoherence)
    {
        //设置几何连贯
        this->setValue("GeometryCoherence", isCoherence);
    }
    bool FITKGlobalMeshGenerateAlgorithmInfo::getGeometryCoherence()
    {
        //是否设置几何连贯
        return this->getValueT<bool>("GeometryCoherence");
    }
}



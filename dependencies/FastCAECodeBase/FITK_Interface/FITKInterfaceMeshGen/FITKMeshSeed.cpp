/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshSeed.h"

#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"

namespace Interface
{
    FITKMeshSeed::FITKMeshSeed(int virTopoID) :
        FITKAbstractNDataObject(), _virTopoID(virTopoID)
    {

    }

    FITKMeshSeed::~FITKMeshSeed()
    {

    }

    FITKMeshSeed::FITKSeedType FITKMeshSeed::getSeedType()
    {
        return _seedType;
    }

    int FITKMeshSeed::getVirTopoId()
    {
        return _virTopoID;
    }

    QList<Core::FITKPoint> FITKMeshSeed::getSeedPoints()
    {
        QList<Core::FITKPoint> points;
        for (const FITKMeshSeedInfo& info : _seedPoints) {
            points.append(info._point);
        }
        return points;
    }

    QList<FITKMeshSeedInfo> FITKMeshSeed::getSeedPointInfos()
    {
        return _seedPoints;
    }

    void FITKMeshSeed::setSeedPointInfos(QList<FITKMeshSeedInfo> points)
    {
        _seedPoints = points;
    }

    void FITKMeshSeed::setSeedType(FITKMeshSeed::FITKSeedType type)
    {
        _seedType = type;
    }

    FITKMeshSeed::FITKLocalSeedConstraint FITKMeshSeed::getConstraintType()
    {
        return _conType;
    }

    void FITKMeshSeed::setConstraintType(FITKMeshSeed::FITKLocalSeedConstraint type)
    {
        _conType = type;
    }

    FITKMeshSeed::FITKLocalSeedMethod FITKMeshSeed::getMethod()
    {
        return _method;
    }

    void FITKMeshSeed::setMethod(FITKMeshSeed::FITKLocalSeedMethod method)
    {
        _method = method;
    }

    FITKMeshSeed::FITKLocalSeedBias FITKMeshSeed::getBias()
    {
        return _bias;
    }

    void FITKMeshSeed::setBias(FITKMeshSeed::FITKLocalSeedBias bias)
    {
        _bias = bias;
    }

    void FITKMeshSeed::setEleNumber(int num)
    {
        _eleNum = num;
    }

    int FITKMeshSeed::getEleNumber()
    {
        return _eleNum;
    }

    void FITKMeshSeed::setBiasRatio(double biasRatio)
    {
        _biasRatio = biasRatio;
    }

    double FITKMeshSeed::getBiasRatio()
    {
        return _biasRatio;
    }

    void FITKMeshSeed::setEleSize(double size)
    {
        _eleSize = size;
    }

    double FITKMeshSeed::getEleSize()
    {
        return _eleSize;
    }

    void FITKMeshSeed::setEleMinSize(double size)
    {
        _eleMinSize = size;
    }

    double FITKMeshSeed::getEleMinSize()
    {
        return _eleMinSize;
    }

    void FITKMeshSeed::setEleMaxSize(double size)
    {
        _eleMaxSize = size;
    }

    double FITKMeshSeed::getEleMaxSize()
    {
        return _eleMaxSize;
    }

    void FITKMeshSeed::calculateEleSize(double & minSize, double & maxSize)
    {
        if (_seedPoints.size() <= 1) {
            return;
        }

        for (int i = 0; i < _seedPoints.size() - 1; i++) {
            const FITKMeshSeedInfo& firSeedInfo = _seedPoints[i];
            const FITKMeshSeedInfo& secSeedInfo = _seedPoints[i + 1];

            double distance = Core::Distance(firSeedInfo._point, secSeedInfo._point);

            if (distance < minSize) {
                minSize = distance;
            }

            if (distance > maxSize) {
                maxSize = distance;
            }
        }
    }
}

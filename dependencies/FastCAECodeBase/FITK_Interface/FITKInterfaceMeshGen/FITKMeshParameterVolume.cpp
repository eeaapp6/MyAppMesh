/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshParameterVolume.h"

namespace Interface
{
    FITKMeshParamType FITKMeshParameterVolume::getMeshParamType()
    {
        return FITKMeshParamType::FITK_Mesh_Volume;
    }

    void FITKMeshParameterVolume::parameterCopy(FITKMeshParameterVolume& parameter)
    {
        _eleType = parameter._eleType;
        _eleGenType = parameter._eleGenType;
        _globalMeshSize = parameter._globalMeshSize;
        _maxMeshSize = parameter._maxMeshSize;
        _minMeshSize = parameter._minMeshSize;
        _maxNumberThreads = parameter._maxNumberThreads;
        _modelID = parameter._modelID;
    }
    void FITKMeshParameterVolume::setGlobalMeshSize(double gSize)
    {
        _globalMeshSize = gSize;
    }

    double FITKMeshParameterVolume::getGlobalMeshSize()
    {
        return _globalMeshSize;
    }

    void FITKMeshParameterVolume::setMaxMeshSize(double size)
    {
        _maxMeshSize = size;
    }

    double FITKMeshParameterVolume::getMaxMeshSize()
    {
        return _maxMeshSize;
    }

    void FITKMeshParameterVolume::setMinMeshSize(double size)
    {
        _minMeshSize = size;
    }

    double FITKMeshParameterVolume::getMinMeshSize()
    {
        return _minMeshSize;
    }

    void FITKMeshParameterVolume::setMaxNumberOfThreads(int numberThreads)
    {
        if (numberThreads > 0)  _maxNumberThreads = numberThreads;
    }

    int FITKMeshParameterVolume::getMaxNumberOfThreads()
    {
        return _maxNumberThreads;
    }

    bool FITKMeshParameterVolume::setElementType(ElementType eleType)
    {
        _eleType = eleType;
        return true;
    }

    FITKMeshParameterVolume::ElementType FITKMeshParameterVolume::getElementType()
    {
        return _eleType;
    }

    void FITKMeshParameterVolume::setElementGenType(ElementGenType type)
    {
        _eleGenType = type;
    }

    FITKMeshParameterVolume::ElementGenType FITKMeshParameterVolume::getElementGenType()
    {
        return _eleGenType;
    }

    FITKVMeshSweepParameter FITKMeshParameterVolume::getSweepPara()
    {
        return _sweepPara;
    }

    void FITKMeshParameterVolume::setSweepPara(FITKVMeshSweepParameter para)
    {
        _sweepPara = para;
    }

    bool FITKMeshParameterVolume::getIsCalculateOfSweep()
    {
        if (_sweepPara._sweepPathID < 1) {
            return true;
        }

        if (_sweepPara._sweepTargetFaceID < 1) {
            return true;
        }

        for (int souId : _sweepPara._sweepSourceFaceIDs) {
            if (souId < 1) {
                return true;
            }
        }
        return false;
    }
}



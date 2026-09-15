/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshParameterSurface.h"

namespace Interface
{
    FITKMeshParamType FITKMeshParameterSurface::getMeshParamType()
    {
        return FITKMeshParamType::FITK_Mesh_Surface;
    }
    void FITKMeshParameterSurface::parameterCopy(FITKMeshParameterSurface& parameter)
    {
        _eleType = parameter._eleType;
        _techniqueType = parameter._techniqueType;
        _globalMeshSize = parameter._globalMeshSize;
        _maxMeshSize = parameter._maxMeshSize;
        _minMeshSize = parameter._minMeshSize;
        _elementOrder = parameter._elementOrder;
        _maxNumberThreads = parameter._maxNumberThreads;
        _modelID = parameter._modelID;
    }

    void FITKMeshParameterSurface::initControlDefault()
    {
        _eleType = ElementType::Triangle;
        _globalMeshSize = 0;
        _maxMeshSize = 0;
        _minMeshSize = 0;
    }

    bool FITKMeshParameterSurface::setAnisotropicMeshing(bool toggle)
    {
        return false;
    }

    bool FITKMeshParameterSurface::getAnisotropicMeshing()
    {
        return false;
    }

    bool FITKMeshParameterSurface::setAnisotropicRatio(double ratio)
    {
        return false;
    }

    double FITKMeshParameterSurface::getAnisotropicRatio()
    {
        return 0.0;
    }

    void FITKMeshParameterSurface::setClosedGeometry(bool toggle)
    {
    }

    bool FITKMeshParameterSurface::getClosedGeometry()
    {
        return false;
    }

    void FITKMeshParameterSurface::setElementOrder(int order)
    {
        _elementOrder = order;
    }

    int FITKMeshParameterSurface::getElementOrder()
    {
        return _elementOrder;
    }

    void FITKMeshParameterSurface::setGlobalMeshSize(double gSize)
    {
        _globalMeshSize = gSize;
    }

    double FITKMeshParameterSurface::getGlobalMeshSize()
    {
        return _globalMeshSize;
    }

    void FITKMeshParameterSurface::setMaxMeshSize(double size)
    {
        _maxMeshSize = size;
    }

    double FITKMeshParameterSurface::getMaxMeshSize()
    {
        return _maxMeshSize;
    }

    void FITKMeshParameterSurface::setMinMeshSize(double size)
    {
        _minMeshSize = size;
    }

    double FITKMeshParameterSurface::getMinMeshSize()
    {
        return _minMeshSize;
    }

    bool FITKMeshParameterSurface::setElementType(ElementType eleType)
    {
        _eleType = eleType;
        QMetaEnum et = QMetaEnum::fromType<ElementType>();
        QString str = et.valueToKey((int)eleType);
        if (str.isEmpty()) return false;
        return true;
    }

    QString FITKMeshParameterSurface::getElementTypeForString()
    {
        QMetaEnum et = QMetaEnum::fromType<ElementType>();
        QString str = et.valueToKey((int)_eleType);
        return str;
    }

    FITKMeshParameterSurface::ElementType FITKMeshParameterSurface::getElementType()
    {
        return _eleType;
    }

    void FITKMeshParameterSurface::setTechnique(TechniqueType tType)
    {
        _techniqueType = tType;
    }

    FITKMeshParameterSurface::TechniqueType FITKMeshParameterSurface::getTechnique()
    {
        return _techniqueType;
    }

    void FITKMeshParameterSurface::setMaxNumberOfThreads(int numberThreads)
    {
        if (numberThreads > 0)  _maxNumberThreads = numberThreads;
    }

    int FITKMeshParameterSurface::getMaxNumberOfThreads()
    {
        return _maxNumberThreads;
    }
}



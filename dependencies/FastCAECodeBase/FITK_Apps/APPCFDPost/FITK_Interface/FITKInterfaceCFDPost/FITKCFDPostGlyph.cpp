/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostGlyph.h"
#include "FITKAlgGlyph.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"

#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostGlyph::FITKCFDPostGlyph(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _glyphAlg = FITKAlgGlyph::New();
        _glyphAlg->SetInputConnection(parentData->getOutputPort());

        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_glyphAlg->GetOutputPort());
        _mappers.append(mapper);
    }

    FITKCFDPostGlyph::~FITKCFDPostGlyph()
    {
        if (_glyphAlg) {
            _glyphAlg->Delete();
            _glyphAlg = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostGlyph::getPostDataType()
    {
        return FITKPostDataType::Post_Glyph;
    }

    vtkDataSet* FITKCFDPostGlyph::getOutput()
    {
        if (_glyphAlg == nullptr)return nullptr;
        return _glyphAlg->GetOutput();
    }

    vtkAlgorithmOutput* FITKCFDPostGlyph::getOutputPort()
    {
        if (_glyphAlg == nullptr)return nullptr;
        return _glyphAlg->GetOutputPort();
    }

    void FITKCFDPostGlyph::update()
    {
        if (_glyphAlg == nullptr)return;
        _glyphAlg->Modified();

        _glyphAlg->setVectorName(_vectorName);
        _glyphAlg->setTipResolution(_tipResolution);
        _glyphAlg->setTipRadius(_tipRadius);
        _glyphAlg->setTipLength(_tipLength);
        _glyphAlg->setShaftResolution(_shaftResolution);
        _glyphAlg->setShaftRadius(_shaftRadius);
        _glyphAlg->setScaleFactor(_scaleFactor);
        _glyphAlg->setPointDataUsed(_pointDataUsed);
        _glyphAlg->setPointNum(_pointNum);
        _glyphAlg->setMaxNumber(_maxNumber);
        _glyphAlg->setScaleMode(_scaleMode);
        _glyphAlg->Update();
        FITKAbstractCFDPostData::update();
    }
    QString FITKCFDPostGlyph::getVectorName()
    {
        return _vectorName;
    }

    void FITKCFDPostGlyph::setVectorName(QString vectorName)
    {
        _vectorName = vectorName;
    }
    int FITKCFDPostGlyph::getTipResolution()
    {
        return _tipResolution;
    }
    void FITKCFDPostGlyph::setTipResolution(int tipResolution)
    {
        _tipResolution = tipResolution;
    }

    double FITKCFDPostGlyph::getTipRadius()
    {
        return _tipRadius;
    }

    void FITKCFDPostGlyph::setTipRadius(double tipRadius)
    {
        _tipRadius = tipRadius;
    }
    double FITKCFDPostGlyph::getTipLength()
    {
        return _tipLength;
    }

    void FITKCFDPostGlyph::setTipLength(double tipLength)
    {
        _tipLength = tipLength;
    }
    int FITKCFDPostGlyph::getShaftResolution()
    {
        return _shaftResolution;
    }
    void FITKCFDPostGlyph::setShaftResolution(int shaftResolution)
    {
        _shaftResolution = shaftResolution;
    }
    double FITKCFDPostGlyph::getShaftRadius()
    {
        return _shaftRadius;
    }

    void FITKCFDPostGlyph::setShaftRadius(double shaftRadius)
    {
        _shaftRadius = shaftRadius;
    }
    double FITKCFDPostGlyph::getScaleFactor()
    {
        return _scaleFactor;
    }
    void FITKCFDPostGlyph::setScaleFactor(double scaleFactor)
    {
        _scaleFactor = scaleFactor;
    }
    bool FITKCFDPostGlyph::getPointDataUsed()
    {
        return _pointDataUsed;
    }
    void FITKCFDPostGlyph::setPointDataUsed(bool pointDataUsed)
    {
        _pointDataUsed = pointDataUsed;
    }
    int FITKCFDPostGlyph::getPointNum()
    {
        return _pointNum;
    }
    void FITKCFDPostGlyph::setPointNum(int pointNum)
    {
        _pointNum = pointNum;
    }
    int FITKCFDPostGlyph::getMaxNumber()
    {
        return _maxNumber;
    }
    void FITKCFDPostGlyph::setMaxNumber(int maxNumber)
    {
        _maxNumber = maxNumber;
    }
    int FITKCFDPostGlyph::getScaleMode()
    {
        return _scaleMode;
    }
    void FITKCFDPostGlyph::setScaleMode(int scaleMode)
    {
        _scaleMode = scaleMode;
    }
}

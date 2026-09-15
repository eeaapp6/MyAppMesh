/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostSliceCylinder.h"
#include "FITKCFDPostData.h"
#include "FITKCFDPost3DManager.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkCylinder.h>
#include <vtkCutter.h>
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostSliceCylinder::FITKCFDPostSliceCylinder(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _cylinder = vtkCylinder::New();

        _sliceDataSet = vtkCutter::New();
        _sliceDataSet->SetInputConnection(parentData->getOutputPort());
        _sliceDataSet->SetCutFunction(_cylinder);

        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_sliceDataSet->GetOutputPort());
        _mappers.append(mapper);

        //创建特征
        initFeatureEdge();
    }

    FITKCFDPostSliceCylinder::~FITKCFDPostSliceCylinder()
    {
        if (_cylinder) {
            _cylinder->Delete();
            _cylinder = nullptr;
        }

        if (_sliceDataSet) {
            _sliceDataSet->Delete();
            _sliceDataSet = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostSliceCylinder::getPostDataType()
    {
        return FITKPostDataType::Post_SliceCylinder;
    }

    vtkDataSet* FITKCFDPostSliceCylinder::getOutput()
    {
        if (_sliceDataSet == nullptr)return nullptr;
        return _sliceDataSet->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostSliceCylinder::getOutputPort()
    {
        if (_sliceDataSet == nullptr)return nullptr;
        return _sliceDataSet->GetOutputPort();
    }

    void FITKCFDPostSliceCylinder::setValue(double* center, double* axis, double radius)
    {
        if (_cylinder == nullptr)return;
        _cylinder->SetCenter(center);
        _cylinder->SetAxis(axis);
        _cylinder->SetRadius(radius);
    }

    void FITKCFDPostSliceCylinder::getValue(double* center, double* axis, double& radius)
    {
        if (_cylinder == nullptr)return;
        _cylinder->GetCenter(center);
        _cylinder->GetAxis(axis);
        radius = _cylinder->GetRadius();
    }
}
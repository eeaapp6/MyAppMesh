/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostSliceSphere.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkCutter.h>
#include <vtkDataSetMapper.h>
#include <vtkSphere.h>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostSliceSphere::FITKCFDPostSliceSphere(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _sphere = vtkSphere::New();

        _sliceDataSet = vtkCutter::New();
        _sliceDataSet->SetInputConnection(parentData->getOutputPort());
        _sliceDataSet->SetCutFunction(_sphere);

        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_sliceDataSet->GetOutputPort());
        _mappers.append(mapper);

        //创建特征
        initFeatureEdge();
    }

    FITKCFDPostSliceSphere::~FITKCFDPostSliceSphere()
    {
        if (_sphere) {
            _sphere->Delete();
            _sphere = nullptr;
        }

        if (_sliceDataSet) {
            _sliceDataSet->Delete();
            _sliceDataSet = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostSliceSphere::getPostDataType()
    {
        return Interface::FITKPostDataType::Post_SliceSphere;
    }

    vtkDataSet* FITKCFDPostSliceSphere::getOutput()
    {
        if (_sliceDataSet == nullptr)return nullptr;
        return _sliceDataSet->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostSliceSphere::getOutputPort()
    {
        if (_sliceDataSet == nullptr)return nullptr;
        return _sliceDataSet->GetOutputPort();
    }

    void FITKCFDPostSliceSphere::setValue(double* origin, double radius)
    {
        if (_sphere == nullptr)return;
        _sphere->SetCenter(origin);
        _sphere->SetRadius(radius);
    }

    void FITKCFDPostSliceSphere::getValue(double* origin, double& radius)
    {
        if (_sphere == nullptr)return;
        _sphere->GetCenter(origin);
        radius = _sphere->GetRadius();
    }
}
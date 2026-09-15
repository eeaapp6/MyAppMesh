/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostClipSphere.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkClipDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkSphere.h>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostClipSphere::FITKCFDPostClipSphere(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _sphere = vtkSphere::New();

        _clipDataSet = vtkClipDataSet::New();
        _clipDataSet->SetInputConnection(parentData->getOutputPort());
        _clipDataSet->SetClipFunction(_sphere);
        _clipDataSet->InsideOutOn();

        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_clipDataSet->GetOutputPort());
        _mappers.append(mapper);

        //创建特征
        initFeatureEdge();
    }

    FITKCFDPostClipSphere::~FITKCFDPostClipSphere()
    {
        if (_sphere) {
            _sphere->Delete();
            _sphere = nullptr;
        }

        if (_clipDataSet) {
            _clipDataSet->Delete();
            _clipDataSet = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostClipSphere::getPostDataType()
    {
        return Interface::FITKPostDataType::Post_ClipSphere;
    }

    vtkDataSet* FITKCFDPostClipSphere::getOutput()
    {
        if (_clipDataSet == nullptr)return nullptr;
        return _clipDataSet->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostClipSphere::getOutputPort()
    {
        if (_clipDataSet == nullptr)return nullptr;
        return _clipDataSet->GetOutputPort();
    }

    void FITKCFDPostClipSphere::setValue(double* origin, double radius)
    {
        if (_sphere == nullptr)return;
        _sphere->SetCenter(origin);
        _sphere->SetRadius(radius);
    }

    void FITKCFDPostClipSphere::getValue(double* origin, double& radius)
    {
        if (_sphere == nullptr)return;
        _sphere->GetCenter(origin);
        radius = _sphere->GetRadius();
    }
}
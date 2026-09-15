/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostClipCylinder.h"
#include "FITKCFDPostData.h"
#include "FITKCFDPost3DManager.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkCylinder.h>
#include <vtkClipDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostClipCylinder::FITKCFDPostClipCylinder(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _cylinder = vtkCylinder::New();

        _clipDataSet = vtkClipDataSet::New();
        _clipDataSet->SetInputConnection(parentData->getOutputPort());
        _clipDataSet->SetClipFunction(_cylinder);
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

    FITKCFDPostClipCylinder::~FITKCFDPostClipCylinder()
    {
        if (_cylinder) {
            _cylinder->Delete();
            _cylinder = nullptr;
        }

        if (_clipDataSet) {
            _clipDataSet->Delete();
            _clipDataSet = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostClipCylinder::getPostDataType()
    {
        return FITKPostDataType::Post_ClipCylinder;
    }

    vtkDataSet* FITKCFDPostClipCylinder::getOutput()
    {
        if (_clipDataSet == nullptr)return nullptr;
        return _clipDataSet->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostClipCylinder::getOutputPort()
    {
        if (_clipDataSet == nullptr)return nullptr;
        return _clipDataSet->GetOutputPort();
    }

    void FITKCFDPostClipCylinder::setValue(double * center, double * axis, double radius)
    {
        if (_cylinder == nullptr)return;
        _cylinder->SetCenter(center);
        _cylinder->SetAxis(axis);
        _cylinder->SetRadius(radius);
    }

    void FITKCFDPostClipCylinder::getValue(double * center, double * axis, double & radius)
    {
        if (_cylinder == nullptr)return;
        _cylinder->GetCenter(center);
        _cylinder->GetAxis(axis);
        radius = _cylinder->GetRadius();
    }
}
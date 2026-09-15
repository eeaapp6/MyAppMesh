/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostClipPlane.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkClipDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkPlane.h>
#include <vtkPlaneWidget.h>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostClipPlane::FITKCFDPostClipPlane(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _plane = vtkPlane::New();

        _clipDataSet = vtkClipDataSet::New();
        _clipDataSet->SetInputConnection(parentData->getOutputPort());
        _clipDataSet->SetClipFunction(_plane);
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

    FITKCFDPostClipPlane::~FITKCFDPostClipPlane()
    {
        if (_plane) {
            _plane->Delete();
            _plane = nullptr;
        }

        if (_clipDataSet) {
            _clipDataSet->Delete();
            _clipDataSet = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostClipPlane::getPostDataType()
    {
        return Interface::FITKPostDataType::Post_ClipPlane;
    }

    vtkDataSet* FITKCFDPostClipPlane::getOutput()
    {
        if (_clipDataSet == nullptr)return nullptr;
        return _clipDataSet->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostClipPlane::getOutputPort()
    {
        if (_clipDataSet == nullptr)return nullptr;
        return _clipDataSet->GetOutputPort();
    }

    void FITKCFDPostClipPlane::setValue(double * origin, double * normal)
    {
        if (_plane == nullptr)return;
        _plane->SetOrigin(origin);
        _plane->SetNormal(normal);
    }

    void FITKCFDPostClipPlane::getValue(double * origin, double * normal)
    {
        if (_plane == nullptr)return;
        _plane->GetOrigin(origin);
        _plane->GetNormal(normal);
    }
}
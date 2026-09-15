/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostSlicePlane.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkDataSetMapper.h>
#include <vtkPlane.h>
#include <vtkPlaneWidget.h>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCutter.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostSlicePlane::FITKCFDPostSlicePlane(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _plane = vtkPlane::New();

        _slice = vtkCutter::New();
        _slice->SetInputConnection(parentData->getOutputPort());
        _slice->SetCutFunction(_plane);

        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_slice->GetOutputPort());
        _mappers.append(mapper);

        //创建特征
        initFeatureEdge();
    }

    FITKCFDPostSlicePlane::~FITKCFDPostSlicePlane()
    {
        if (_plane) {
            _plane->Delete();
            _plane = nullptr;
        }

        if (_slice) {
            _slice->Delete();
            _slice = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostSlicePlane::getPostDataType()
    {
        return Interface::FITKPostDataType::Post_SlicePlane;
    }

    vtkDataSet* FITKCFDPostSlicePlane::getOutput()
    {
        if (_slice == nullptr)return nullptr;
        return _slice->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostSlicePlane::getOutputPort()
    {
        if (_slice == nullptr)return nullptr;
        return _slice->GetOutputPort();
    }

    void FITKCFDPostSlicePlane::setValue(double * origin, double * normal)
    {
        if (_plane == nullptr)return;
        _plane->SetOrigin(origin);
        _plane->SetNormal(normal);
    }

    void FITKCFDPostSlicePlane::getValue(double * origin, double * normal)
    {
        if (_plane == nullptr)return;
        _plane->GetOrigin(origin);
        _plane->GetNormal(normal);
    }
}
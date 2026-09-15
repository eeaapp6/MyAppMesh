/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostClipBox.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkClipDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkBox.h>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostClipBox::FITKCFDPostClipBox(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _box = vtkBox::New();

        _clipDataSet = vtkClipDataSet::New();
        _clipDataSet->SetInputConnection(parentData->getOutputPort());
        _clipDataSet->SetClipFunction(_box);
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

    FITKCFDPostClipBox::~FITKCFDPostClipBox()
    {
        if (_box) {
            _box->Delete();
            _box = nullptr;
        }

        if (_clipDataSet) {
            _clipDataSet->Delete();
            _clipDataSet = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostClipBox::getPostDataType()
    {
        return FITKPostDataType::Post_ClipBox;
    }

    vtkDataSet * FITKCFDPostClipBox::getOutput()
    {
        if (_clipDataSet == nullptr)return nullptr;
        return _clipDataSet->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostClipBox::getOutputPort()
    {
        if (_clipDataSet == nullptr)return nullptr;
        return _clipDataSet->GetOutputPort();
    }

    void FITKCFDPostClipBox::setValue(double* boundary)
    {
        if (_box == nullptr)return;
        _box->SetBounds(boundary);
    }

    void FITKCFDPostClipBox::getValue(double* boundary)
    {
        if (_box == nullptr)return;
        _box->GetBounds(boundary);
    }
}

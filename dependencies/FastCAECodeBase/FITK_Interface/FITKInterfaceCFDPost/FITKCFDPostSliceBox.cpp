/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostSliceBox.h"

#include "FITKCFDPostData.h"
#include "FITKCFDPost3DManager.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkBox.h>
#include <vtkCutter.h>
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostSliceBox::FITKCFDPostSliceBox(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _box = vtkBox::New();

        _sliceDataSet = vtkCutter::New();
        _sliceDataSet->SetInputConnection(parentData->getOutputPort());
        _sliceDataSet->SetCutFunction(_box);

        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_sliceDataSet->GetOutputPort());
        _mappers.append(mapper);

        //创建特征
        initFeatureEdge();
    }

    FITKCFDPostSliceBox::~FITKCFDPostSliceBox()
    {
        if (_box) {
            _box->Delete();
            _box = nullptr;
        }

        if (_sliceDataSet) {
            _sliceDataSet->Delete();
            _sliceDataSet = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostSliceBox::getPostDataType()
    {
        return FITKPostDataType::Post_SliceBox;
    }

    vtkDataSet * FITKCFDPostSliceBox::getOutput()
    {
        if (_sliceDataSet == nullptr)return nullptr;
        return _sliceDataSet->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostSliceBox::getOutputPort()
    {
        if (_sliceDataSet == nullptr)return nullptr;
        return _sliceDataSet->GetOutputPort();
    }

    void FITKCFDPostSliceBox::setValue(double * bounds)
    {
        if (_box == nullptr)return;
        _box->SetBounds(bounds);
    }

    void FITKCFDPostSliceBox::getValue(double * bounds)
    {
        if (_box == nullptr)return;
        _box->GetBounds(bounds);
    }
}


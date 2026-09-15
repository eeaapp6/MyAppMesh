/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostThreshold.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkThreshold.h>
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostThreshold::FITKCFDPostThreshold(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _threshold = vtkThreshold::New();
        _threshold->SetInputConnection(parentData->getOutputPort());

        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_threshold->GetOutputPort());
        _mappers.append(mapper);

        //创建特征
        initFeatureEdge();
    }

    FITKCFDPostThreshold::~FITKCFDPostThreshold()
    {
        if (_threshold) {
            _threshold->Delete();
            _threshold = nullptr;
        }
    }
    FITKPostDataType FITKCFDPostThreshold::getPostDataType()
    {
        return FITKPostDataType::Post_Threshold;
    }

    vtkDataSet * FITKCFDPostThreshold::getOutput()
    {
        if (_threshold == nullptr)return nullptr;
        return _threshold->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostThreshold::getOutputPort()
    {
        if (_threshold == nullptr)return nullptr;
        return _threshold->GetOutputPort();
    }

    void FITKCFDPostThreshold::setValue(double lower, double upper, Interface::FITKPostFieldType fieldType, QString fieldName)
    {
        if (_threshold == nullptr)return;

#if VTK_MAJOR_VERSION > 9 || (VTK_MAJOR_VERSION == 9 && VTK_MINOR_VERSION >= 1)
        _threshold->SetLowerThreshold(lower);
        _threshold->SetUpperThreshold(upper);
#else
        _threshold->ThresholdBetween(lower, upper);
#endif

        _fieldName = fieldName;
        QByteArray bName = _fieldName.toLocal8Bit();
        char* cName = bName.data();

        _fieldType = fieldType;
        switch (_fieldType){
        case Interface::FITKPostFieldType::Post_Point: {
            _threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, cName);
            break;
        }
        case Interface::FITKPostFieldType::Post_Cell: {
            _threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, cName);
            break;
        }
        }
    }

    void FITKCFDPostThreshold::getValue(double& lower, double& upper, Interface::FITKPostFieldType& fieldType, QString& fieldName)
    {
        if (_threshold == nullptr)return;
        lower = _threshold->GetLowerThreshold();
        upper = _threshold->GetUpperThreshold();
        fieldType = _fieldType;
        fieldName = _fieldName;
    }
}
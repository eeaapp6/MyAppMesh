/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostIsosurface.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkContourFilter.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostIsosurface::FITKCFDPostIsosurface(int parentID)
    {
        _CFDPostParentID = parentID;

        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        vtkDataSet* dataSet = parentData->getOutput();
        int pointNum = dataSet->GetNumberOfPoints();
        int cellNum = dataSet->GetNumberOfCells();
        vtkAlgorithmOutput* dataSetOutput = parentData->getOutputPort();

        _contourFilter = vtkContourFilter::New();
        _contourFilter->SetInputConnection(parentData->getOutputPort());
        //设置标量计算
        _contourFilter->SetComputeScalars(true);
        //设置法线计算
        _contourFilter->SetComputeNormals(true);
        //设置梯度计算
        _contourFilter->SetComputeGradients(true);
        
        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_contourFilter->GetOutputPort());
        _mappers.append(mapper);

        //创建特征
        initFeatureEdge();
    }

    FITKCFDPostIsosurface::~FITKCFDPostIsosurface()
    {
        if (_contourFilter) {
            _contourFilter->Delete();
            _contourFilter = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostIsosurface::getPostDataType()
    {
        return FITKPostDataType::Post_Isosurface;
    }

    void FITKCFDPostIsosurface::setFiledName(const QString filedName)
    {
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;
        if (parentData->getOutput() == nullptr && parentData->getOutput()->GetPointData() == nullptr)return;
        QByteArray bName = filedName.toLocal8Bit();
        char* cName = bName.data();
        parentData->getOutput()->GetPointData()->SetActiveScalars(cName);
        _filedName = filedName;
    }

    QString FITKCFDPostIsosurface::getFiledName()
    {
        return _filedName;
    }

    void FITKCFDPostIsosurface::setFiledType(const FITKPostFieldType type)
    {
        _filedType = type;
    }

    FITKPostFieldType FITKCFDPostIsosurface::getFiledType()
    {
        return _filedType;
    }

    void FITKCFDPostIsosurface::setValue(const QList<double> value)
    {
        if (_contourFilter == nullptr)return;
        //设置切面数量
        _contourFilter->SetNumberOfContours(value.size());
        _valueSize = value.size();
        for (int i = 0; i < _valueSize; i++) {
            double v = value[i];
            //设置数据
            _contourFilter->SetValue(i, v);
        }
    }

    QList<double> FITKCFDPostIsosurface::getValue()
    {
        QList<double> values = {};
        if (_contourFilter == nullptr)return values;
        for (int i = 0; i < _valueSize; i++) {
            double v = _contourFilter->GetValue(i);
            values.append(v);
        }
        return values;
    }

    vtkDataSet * FITKCFDPostIsosurface::getOutput()
    {
        if (_contourFilter == nullptr)return nullptr;
        return _contourFilter->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostIsosurface::getOutputPort()
    {
        if (_contourFilter == nullptr)return nullptr;
        return _contourFilter->GetOutputPort();
    }
}


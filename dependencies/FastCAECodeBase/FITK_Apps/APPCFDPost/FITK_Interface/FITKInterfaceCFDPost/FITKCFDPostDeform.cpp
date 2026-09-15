/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostDeform.h"
#include "FITKCFDPostData.h"
#include "FITKCFDPost3DManager.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKDeformFilter.h"

#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostDeform::FITKCFDPostDeform(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _deformFilter = FITKDeformFilter::New();
        _deformFilter->SetInputConnection(parentData->getOutputPort());

        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_deformFilter->GetOutputPort());
        _mappers.append(mapper);

        //创建特征
        initFeatureEdge();
    }

    FITKCFDPostDeform::~FITKCFDPostDeform()
    {
        if (_deformFilter) {
            _deformFilter->Delete();
            _deformFilter = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostDeform::getPostDataType()
    {
        return FITKPostDataType::Post_Deform;
    }

    vtkDataSet* FITKCFDPostDeform::getOutput()
    {
        if (_deformFilter == nullptr)return nullptr;
        return _deformFilter->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostDeform::getOutputPort()
    {
        if (_deformFilter == nullptr)return nullptr;
        return _deformFilter->GetOutputPort();
    }

    void FITKCFDPostDeform::setDeformFactor(const double fac)
    {
        _deformFactor = fac;
        if (_deformFilter)
        {
            double cd = _deformFilter->GetScaleFactor();
            double d = _unDeformFactor + _deformFactor;
            //变形一致，不进行更新操作
            if (fabs(d - cd) < 1e-9) return;
            _deformFilter->SetScaleFactor(d);
        }
    }

    double FITKCFDPostDeform::getDeformFactor()
    {
        return _deformFactor;
    }

    void FITKCFDPostDeform::setUnDeformFactor(const double fac)
    {
        _unDeformFactor = fac;
    }

    double FITKCFDPostDeform::getUnDeformFactor()
    {
        return _unDeformFactor;
    }

    void FITKCFDPostDeform::setDeformVariable(const QString & va)
    {
        if (_deformFilter == nullptr) return;
        //变量相同不进行操作
        QString cv = this->getDeformVariable();
        if (cv == va) return;
        QByteArray ba = va.toLocal8Bit();
        _deformFilter->SetDeformArrayName(ba.data());
    }

    QString FITKCFDPostDeform::getDeformVariable()
    {
        return QString::fromLocal8Bit(_deformFilter->GetDeformArrayName());
    }
}
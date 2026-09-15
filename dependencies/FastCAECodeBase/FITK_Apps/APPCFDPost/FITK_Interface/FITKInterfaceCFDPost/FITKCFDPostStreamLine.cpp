/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostStreamLine.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkLineSource.h>
#include <vtkStreamTracer.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>

namespace Interface
{
    FITKCFDPostStreamLine::FITKCFDPostStreamLine(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _lineSource = vtkLineSource::New();

        //初始化过滤器
        _streamFilter = vtkStreamTracer::New();
        _streamFilter->SetInputConnection(parentData->getOutputPort());
        _streamFilter->SetSourceConnection(_lineSource->GetOutputPort());

        _streamFilter->SetIntegratorTypeToRungeKutta45();
        _streamFilter->SetIntegrationStepUnit(2);//设置流线积分步长单位
        _streamFilter->SetInitialIntegrationStep(0.2);//设置流线积分初始步长
        _streamFilter->SetMinimumIntegrationStep(0.01);//设置流线积分最小步长
        _streamFilter->SetMaximumIntegrationStep(0.5);//设置流线积分最大步长
        _streamFilter->SetMaximumNumberOfSteps(1000);//设置流线积分最大步数
        _streamFilter->SetTerminalSpeed(1e-12);//设置流线积分终止速度
        _streamFilter->SetMaximumError(1e-6);
        _streamFilter->SetComputeVorticity(true);

        //初始化mapper
        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_streamFilter->GetOutputPort());

        _mappers.append(mapper);
    }

    FITKCFDPostStreamLine::~FITKCFDPostStreamLine()
    {
        if (_lineSource) {
            _lineSource->Delete();
            _lineSource = nullptr;
        }

        if (_streamFilter) {
            _streamFilter->Delete();
            _streamFilter = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostStreamLine::getPostDataType()
    {
        return FITKPostDataType::Post_StreamLine;
    }

    vtkDataSet* FITKCFDPostStreamLine::getOutput()
    {
        if (_streamFilter == nullptr)return nullptr;
        return _streamFilter->GetOutput();
    }

    vtkAlgorithmOutput * FITKCFDPostStreamLine::getOutputPort()
    {
        if (_streamFilter == nullptr)return nullptr;
        return _streamFilter->GetOutputPort();
    }

    void FITKCFDPostStreamLine::setVector(FITKPostFieldType vectorType, QString vectorName)
    {
        if (_streamFilter == nullptr)return;
        _vectorType = vectorType;
        _vectorName = vectorName;
        QByteArray vectorB = vectorName.toLocal8Bit();
        char* v = vectorB.data();
        switch (_vectorType){
        case Interface::FITKPostFieldType::Post_Point:
            _streamFilter->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, v);
            break;
        case Interface::FITKPostFieldType::Post_Cell:
            _streamFilter->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, v);
            break;
        }
    }

    void FITKCFDPostStreamLine::getVector(FITKPostFieldType& vectorType, QString& vectorName)
    {
        vectorType = _vectorType;
        vectorName = _vectorName;
    }

    void FITKCFDPostStreamLine::setDirection(FITKPostStreamDirection dir)
    {
        if (_streamFilter == nullptr)return;
        _directionType = dir;
        switch (_directionType){
        case Interface::FITKPostStreamDirection::Post_Forward:
            _streamFilter->SetIntegrationDirectionToBackward();
            break;
        case Interface::FITKPostStreamDirection::Post_Backward:
            _streamFilter->SetIntegrationDirectionToForward();
            break;
        case Interface::FITKPostStreamDirection::Post_Both:
            _streamFilter->SetIntegrationDirectionToBoth();
            break;
        }
    }

    FITKPostStreamDirection FITKCFDPostStreamLine::getDirection()
    {
        return _directionType;
    }

    void FITKCFDPostStreamLine::setLength(double length)
    {
        if (_streamFilter) {
            //设置流线长度
            _streamFilter->SetMaximumPropagation(length);
        }
    }

    double FITKCFDPostStreamLine::getLength()
    {
        if (_streamFilter) {
            //设置流线长度
            return _streamFilter->GetMaximumPropagation();
        }
        return 0.0;
    }

    void FITKCFDPostStreamLine::setLine(double* startPoint, double* endPoint, int resolution)
    {
        if (_lineSource == nullptr)return;
        _lineSource->SetPoint1(startPoint);
        _lineSource->SetPoint2(endPoint);
        _lineSource->SetResolution(resolution);
    }

    void FITKCFDPostStreamLine::getLine(double* startPoint, double* endPoint, int& resolution)
    {
        if (_lineSource == nullptr)return;
        _lineSource->GetPoint1(startPoint);
        _lineSource->GetPoint2(endPoint);
        resolution = _lineSource->GetResolution();
    }
}

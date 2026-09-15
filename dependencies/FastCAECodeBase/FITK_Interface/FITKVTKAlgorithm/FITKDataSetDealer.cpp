/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKDataSetDealer.h"

#include "vtkInformation.h"
#include "vtkDataSet.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkDemandDrivenPipeline.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include "vtkIntArray.h"
#include "vtkIdTypeArray.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"

#include "vtkDataSetWriter.h"

FITKDataSetDealer* FITKDataSetDealer::New()
{
    FITKDataSetDealer* filter = new FITKDataSetDealer;
    filter->InitializeObjectBase();
    return filter;
}

void FITKDataSetDealer::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

int FITKDataSetDealer::RequestData(vtkInformation* vtkNotUsed(request), vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    // Add the array to data set.
    vtkDataSet* inputData = vtkDataSet::GetData(inputVector[0]);
    vtkDataSet* outputData = vtkDataSet::GetData(outputVector);
    if (inputData == nullptr || outputData == nullptr)
    {
        vtkErrorMacro("Input or output data object is invalid!");
        return 0;
    }

    // Copy data.
    outputData->CopyStructure(inputData);
    outputData->GetPointData()->PassData(inputData->GetPointData());
    outputData->GetCellData()->PassData(inputData->GetCellData());

    // Deal with the array names.
    initArrayNames(outputData);

    // Add the cell id array.
    // addCellIdArray(outputData->GetCellData(), outputData->GetNumberOfCells());

    return 1;
}

vtkDataSet* FITKDataSetDealer::GetOutput(int idx)
{
    return vtkDataSet::SafeDownCast(this->GetOutputDataObject(idx));
}

vtkDataSet* FITKDataSetDealer::GetOutput()
{
    return vtkDataSet::SafeDownCast(this->GetOutputDataObject(0));
}

int FITKDataSetDealer::FillInputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
    return 1;
}

int FITKDataSetDealer::FillOutputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkDataSet");
    return 1;
}

int FITKDataSetDealer::ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
    {
        return this->RequestInformation(request, inputVector, outputVector);
    }
    if (request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
    {
        return this->RequestUpdateExtent(request, inputVector, outputVector);
    }
    if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
    {
        return this->RequestData(request, inputVector, outputVector);
    }
    return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}

void FITKDataSetDealer::initArrayNames(vtkDataObject* obj)
{
    // Down cast to the data set.
    vtkDataSet* dataSet = vtkDataSet::SafeDownCast(obj);
    if (!dataSet)
    {
        return;
    }

    // Deal with the point arrays.
    vtkPointData* pointData = dataSet->GetPointData();
    if (pointData)
    {
        int nPtArray = pointData->GetNumberOfArrays();
        for (int i = 0; i < nPtArray; i++)
        {
            vtkDataArray* dArray = pointData->GetArray(i);
            initArrayName(dArray);
        }
    }

    // Deal with the cell arrays.
    vtkCellData* cellData = dataSet->GetCellData();
    if (cellData)
    {
        int nCellArray = cellData->GetNumberOfArrays();
        for (int i = 0; i < nCellArray; i++)
        {
            vtkDataArray* dArray = cellData->GetArray(i);
            initArrayName(dArray);
        }
    }  
}

void FITKDataSetDealer::initArrayName(vtkDataArray* dArray)
{
    if (!dArray)
    {
        return;
    }

    // Deals the array names.
    int nComp = dArray->GetNumberOfComponents();
    if (nComp == 1)
    {
        QString cName = dArray->GetComponentName(0);
        if (cName.isEmpty())
        {
            cName = dArray->GetName();
        }

        dArray->SetComponentName(0, cName.toUtf8().data());
    }
    else if (nComp == 3)
    {
        QString cName1 = dArray->GetComponentName(0);
        if (cName1.isEmpty())
        {
            cName1 = "X";
        }

        QString cName2 = dArray->GetComponentName(1);
        if (cName2.isEmpty())
        {
            cName2 = "Y";
        }

        QString cName3 = dArray->GetComponentName(2);
        if (cName3.isEmpty())
        {
            cName3 = "Z";
        }

        dArray->SetComponentName(0, cName1.toUtf8().data());
        dArray->SetComponentName(1, cName2.toUtf8().data());
        dArray->SetComponentName(2, cName3.toUtf8().data());
    }
    else
    {
        for (int i = 0; i < nComp; i++)
        {
            QString cName = dArray->GetComponentName(i);
            if (cName.isEmpty())
            {
                dArray->SetComponentName(i, QString("Component_%1").arg(i + 1).toUtf8().data());
            }
        }
    }
}

void FITKDataSetDealer::addCellIdArray(vtkCellData* cellData, int nCell)
{
    vtkSmartPointer<vtkIdTypeArray> cellIdArray = vtkSmartPointer<vtkIdTypeArray>::New();
    cellIdArray->SetName("_ELEMENT_ID_");
    cellIdArray->SetNumberOfComponents(1);

    for (int i = 0; i < nCell; i++)
    {
        cellIdArray->InsertNextValue(i);
    }

    cellData->AddArray(cellIdArray);
}
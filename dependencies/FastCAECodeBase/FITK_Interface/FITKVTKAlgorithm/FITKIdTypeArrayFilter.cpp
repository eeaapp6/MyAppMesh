/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKIdTypeArrayFilter.h"

// VTK
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkIdTypeArray.h>

#include <string>

// VTK macro.
vtkStandardNewMacro(FITKIdTypeArrayFilter);

FITKIdTypeArrayFilter::FITKIdTypeArrayFilter()
{
    // Initialize the id array name.
    this->SetIdArrayName("FITKIntArray");
}

int FITKIdTypeArrayFilter::RequestData(vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    // get the info objects
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    // get the input and output
    vtkDataSet* input = vtkDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkDataSet* output = vtkDataSet::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkIdType numPts, numCells, id;
    vtkIdTypeArray* ptIdArray;
    vtkIdTypeArray* cellIdArray;
    vtkPointData *inPD = input->GetPointData(), *outPD = output->GetPointData();
    vtkCellData *inCD = input->GetCellData(), *outCD = output->GetCellData();

    // Initialize
    //
    vtkDebugMacro(<< "Generating array!");

    // First, copy the input to the output as a starting point
    output->CopyStructure(input);

    numPts = input->GetNumberOfPoints();
    numCells = input->GetNumberOfCells();

    // Loop over points.
    //
    if (numPts > 0)
    {
        ptIdArray = vtkIdTypeArray::New();
        ptIdArray->SetNumberOfValues(numPts);

        for (id = 0; id < numPts; id++)
        {
            ptIdArray->SetValue(id, this->IntValue);
        }

        ptIdArray->SetName(this->IdArrayName);
        outPD->AddArray(ptIdArray);
        outPD->CopyFieldOff(this->IdArrayName);
        ptIdArray->Delete();
    }

    // Loop over cells.
    //
    if (numCells > 0)
    {
        cellIdArray = vtkIdTypeArray::New();
        cellIdArray->SetNumberOfValues(numCells);

        for (id = 0; id < numCells; id++)
        {
            cellIdArray->SetValue(id, this->IntValue);
        }

        cellIdArray->SetName(this->IdArrayName);
        outCD->AddArray(cellIdArray);
        outCD->CopyFieldOff(this->IdArrayName);
        cellIdArray->Delete();
    }

    outPD->PassData(inPD);
    outCD->PassData(inCD);

    return 1;
}

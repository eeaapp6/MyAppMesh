/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCoordinateAlgorithm.h"

#include "vtkCell.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkIdList.h"
#include "vtkIdTypeArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkTransform.h"
#include "vtkTrivialProducer.h"
#include "vtkUniformGrid.h"
#include "vtkUnsignedCharArray.h"
#include "vtkStringArray.h"

vtkStandardNewMacro(FITKCoordinateAlgorithm);

//----------------------------------------------------------------------------
// Construct object with scaling on, scaling mode is by scalar value,
// scale factor = 1.0.
FITKCoordinateAlgorithm::FITKCoordinateAlgorithm()
{
    this->SetNumberOfInputPorts(2);
    this->OutputPointsPrecision = vtkAlgorithm::DEFAULT_PRECISION;
    // this->SetLabelArrayName("Labels");

    // by default process active point scalars
    this->SetInputArrayToProcess(
        0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, vtkDataSetAttributes::SCALARS);
}

//----------------------------------------------------------------------------
int FITKCoordinateAlgorithm::RequestData(vtkInformation* vtkNotUsed(request), vtkInformationVector** inputVector,
    vtkInformationVector* outputVector)
{
    // get the info objects
    vtkDataSet* input = vtkDataSet::GetData(inputVector[0], 0);
    vtkPolyData* output = vtkPolyData::GetData(outputVector, 0);

    return this->Execute(input, inputVector[1], output) ? 1 : 0;
}

//----------------------------------------------------------------------------
bool FITKCoordinateAlgorithm::Execute(vtkDataSet* input, vtkInformationVector* sourceVector, vtkPolyData* output)
{
    vtkDataArray* inSScalars = this->GetInputArrayToProcess(0, input);
    // vtkDataArray* inVectors = this->GetInputArrayToProcess(1, input);
    return this->Execute(input, sourceVector, output, inSScalars, nullptr);
}

//----------------------------------------------------------------------------
bool FITKCoordinateAlgorithm::Execute(vtkDataSet* input, vtkInformationVector* vtkNotUsed(sourceVector), vtkPolyData* output,
    vtkDataArray* inSScalars, vtkDataArray* vtkNotUsed(inVectors))
{
    assert(input && output);
    if (input == nullptr || output == nullptr)
    {
        // nothing to do.
        return true;
    }

    vtkPointData* pd;
    vtkIdType numPts, numSourcePts, numSourceCells, inPtId, i;
    vtkPoints* sourcePts = nullptr;
    vtkPoints* newPts;
    vtkDataArray* newScalars = nullptr;
    double x[3], s = 0.0;
    vtkTransform* trans = vtkTransform::New();
    vtkSmartPointer<vtkIdList> pointIdList = vtkSmartPointer<vtkIdList>::New();
    vtkIdList* cellPts;
    int npts;
    vtkIdList* pts;
    vtkIdType ptIncr, cellIncr, cellId;
    double scalex, scaley, scalez;
    vtkPointData* outputPD = output->GetPointData();
    vtkSmartPointer<vtkIdList> srcPointIdList = vtkSmartPointer<vtkIdList>::New();
    vtkSmartPointer<vtkIdList> dstPointIdList = vtkSmartPointer<vtkIdList>::New();

    vtkDebugMacro(<< "Generating glyphs");

    pts = vtkIdList::New();
    pts->Allocate(VTK_CELL_SIZE);

    pd = input->GetPointData();

    numPts = input->GetNumberOfPoints();
    if (numPts < 1)
    {
        vtkDebugMacro(<< "No points to glyph!");
        pts->Delete();
        trans->Delete();
        return 1;
    }

    // Allocate storage for output PolyData
    //
    outputPD->CopyVectorsOff();
    outputPD->CopyNormalsOff();
    outputPD->CopyTCoordsOff();

    vtkSmartPointer<vtkPolyData> source = vtkSmartPointer<vtkPolyData>::New();

#if VTK_MAJOR_VERSION >= 9
    source->AllocateExact(0, 0, 3, 6, 0, 0, 0, 0);
#endif

    vtkSmartPointer<vtkPoints> spoints = vtkSmartPointer<vtkPoints>::New();
    spoints->Allocate(4);
    spoints->InsertNextPoint(0, 0, 0);
    spoints->InsertNextPoint(this->DirectionX);
    spoints->InsertNextPoint(this->DirectionY);
    spoints->InsertNextPoint(this->DirectionZ);
    vtkIdType defaultPointIdsX[2];
    defaultPointIdsX[0] = 0;
    defaultPointIdsX[1] = 1;
    vtkIdType defaultPointIdsY[2];
    defaultPointIdsY[0] = 0;
    defaultPointIdsY[1] = 2;
    vtkIdType defaultPointIdsZ[2];
    defaultPointIdsZ[0] = 0;
    defaultPointIdsZ[1] = 3;
    source->SetPoints(spoints);
    source->InsertNextCell(VTK_LINE, 2, defaultPointIdsX);
    source->InsertNextCell(VTK_LINE, 2, defaultPointIdsY);
    source->InsertNextCell(VTK_LINE, 2, defaultPointIdsZ);

    sourcePts = source->GetPoints();
    numSourcePts = sourcePts->GetNumberOfPoints();
    numSourceCells = source->GetNumberOfCells();

    // Prepare to copy output.
    pd = input->GetPointData();
    outputPD->CopyAllocate(pd, numPts * numSourcePts);

    srcPointIdList->SetNumberOfIds(numSourcePts);
    dstPointIdList->SetNumberOfIds(numSourcePts);

    newPts = vtkPoints::New();

    // Set the desired precision for the points in the output.
    if (this->OutputPointsPrecision == vtkAlgorithm::DEFAULT_PRECISION)
    {
        newPts->SetDataType(VTK_FLOAT);
    }
    else if (this->OutputPointsPrecision == vtkAlgorithm::SINGLE_PRECISION)
    {
        newPts->SetDataType(VTK_FLOAT);
    }
    else if (this->OutputPointsPrecision == vtkAlgorithm::DOUBLE_PRECISION)
    {
        newPts->SetDataType(VTK_DOUBLE);
    }

    newPts->Allocate(numPts * numSourcePts);

    // Setting up for calls to PolyData::InsertNextCell()
#if VTK_MAJOR_VERSION < 9
    output->Allocate(3 * numPts * numSourceCells, numPts * numSourceCells);
#else
    output->AllocateEstimate(numPts * numSourceCells, 3);
#endif

    // Traverse all Input points, transforming Source points and copying
    // point attributes.
    //
    ptIncr = 0;
    cellIncr = 0;

    vtkStringArray* labelArray = vtkStringArray::New();
    labelArray->SetName(this->LabelArrayName);

    for (inPtId = 0; inPtId < numPts; inPtId++)
    {
        scalex = scaley = scalez = 1.0;
        if (!(inPtId % 10000))
        {
            this->UpdateProgress(static_cast<double>(inPtId) / numPts);
            if (this->GetAbortExecute())
            {
                break;
            }
        }

        // Get the scalar and vector data
        if (inSScalars)
        {
            s = inSScalars->GetComponent(inPtId, 0);
            scalex = scaley = scalez = s;
        }

        // Now begin copying/transforming glyph
        trans->Identity();

        // Copy all topology (transformation independent)
        for (cellId = 0; cellId < numSourceCells; cellId++)
        {
            source->GetCellPoints(cellId, pointIdList);
            cellPts = pointIdList;
            npts = cellPts->GetNumberOfIds();
            for (pts->Reset(), i = 0; i < npts; i++)
            {
                pts->InsertId(i, cellPts->GetId(i) + ptIncr);
            }
            output->InsertNextCell(source->GetCellType(cellId), pts);
        }

        // translate Source to Input point
        input->GetPoint(inPtId, x);
        trans->Translate(x[0], x[1], x[2]);

        // scale the points
        trans->Scale(scalex, scaley, scalez);

        // multiply points and normals by resulting matrix
        trans->TransformPoints(sourcePts, newPts);

        // Copy point data from source (if possible)
        if (pd)
        {
            for (i = 0; i < numSourcePts; ++i)
            {
                srcPointIdList->SetId(i, inPtId);
                dstPointIdList->SetId(i, ptIncr + i);
            }
            outputPD->CopyData(pd, srcPointIdList, dstPointIdList);
        }

        ptIncr += numSourcePts;
        cellIncr += numSourceCells;

        labelArray->InsertNextValue("");
        labelArray->InsertNextValue(this->XLabel);
        labelArray->InsertNextValue(this->YLabel);
        labelArray->InsertNextValue(this->ZLabel);
    }

    // Update ourselves and release memory
    //
    output->SetPoints(newPts);
    newPts->Delete();

    if (newScalars)
    {
        int idx = outputPD->AddArray(newScalars);
        outputPD->SetActiveAttribute(idx, vtkDataSetAttributes::SCALARS);
        newScalars->Delete();
    }

    output->GetPointData()->AddArray(labelArray);

    labelArray->Delete();
    output->Squeeze();
    trans->Delete();
    pts->Delete();

    return true;
}

int FITKCoordinateAlgorithm::RequestUpdateExtent(vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    // get the info objects
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation* sourceInfo = inputVector[1]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    if (sourceInfo)
    {
        sourceInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER(), 0);
        sourceInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES(), 1);
        sourceInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(), 0);
    }
    inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER(),
        outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER()));
    inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES(),
        outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES()));
    inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(),
        outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS()));
    inInfo->Set(vtkStreamingDemandDrivenPipeline::EXACT_EXTENT(), 1);

    return 1;
}

//----------------------------------------------------------------------------
int FITKCoordinateAlgorithm::FillInputPortInformation(int port, vtkInformation* info)
{
    if (port == 0)
    {
        info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
        return 1;
    }
    else if (port == 1)
    {
        info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
        info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
        info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
        return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------
void FITKCoordinateAlgorithm::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Output Points Precision: " << this->OutputPointsPrecision << "\n";

    os << endl;
}
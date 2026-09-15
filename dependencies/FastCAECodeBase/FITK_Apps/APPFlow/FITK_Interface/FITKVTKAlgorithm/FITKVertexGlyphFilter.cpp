/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKVertexGlyphFilter.h"

#include "vtkCellArray.h"
#include "vtkGraph.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkUnstructuredGrid.h"
//#include "vtkDemandDrivenPipeline.h"
//#include "vtkStreamingDemandDrivenPipeline.h"

#include "vtkSmartPointer.h"
#define VTK_CREATE(type, name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

vtkStandardNewMacro(FITKVertexGlyphFilter);

//-----------------------------------------------------------------------------

FITKVertexGlyphFilter::FITKVertexGlyphFilter() = default;

FITKVertexGlyphFilter::~FITKVertexGlyphFilter() = default;

void FITKVertexGlyphFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

//-----------------------------------------------------------------------------

int FITKVertexGlyphFilter::FillInputPortInformation(int, vtkInformation* info)
{
    info->Remove(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE());
    info->Append(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkGraph");
    info->Append(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPointSet");
    return 1;
}

//int FITKVertexGlyphFilter::FillOutputPortInformation(int, vtkInformation* info)
//{
//    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkUnstructuredGrid");
//    return 1;
//}

//int FITKVertexGlyphFilter::ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
//{
//    if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
//    {
//        return this->RequestInformation(request, inputVector, outputVector);
//    }
//    if (request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
//    {
//        return this->RequestUpdateExtent(request, inputVector, outputVector);
//    }
//    if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
//    {
//        return this->RequestData(request, inputVector, outputVector);
//    }
//    return this->Superclass::ProcessRequest(request, inputVector, outputVector);
//}

//-----------------------------------------------------------------------------

int FITKVertexGlyphFilter::RequestData(vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    // Get the info objects.
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    // Get the input and output.
    vtkPointSet* psInput = vtkPointSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkGraph* graphInput = vtkGraph::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkUnstructuredGrid* output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkPoints* points = nullptr;
    if (psInput)
    {
        points = psInput->GetPoints();
    }
    else if (graphInput)
    {
        points = graphInput->GetPoints();
    }

    // If no points, then nothing to do.
    if (points == nullptr)
    {
        return 1;
    }

    output->Reset();
    output->Allocate();
    output->SetPoints(points);

    //vtkPoints* pts = vtkPoints::New();
    //pts->DeepCopy(points);
    //output->SetPoints(pts);
    //pts->Delete();

    vtkIdType numPoints = points->GetNumberOfPoints();

    if (psInput)
    {
        output->GetPointData()->PassData(psInput->GetPointData());
    }
    else if(graphInput)
    {
        output->GetPointData()->PassData(graphInput->GetVertexData());
    }

    for (vtkIdType i = 0; i < numPoints; i++)
    {
        vtkIdType cell[1]{ i };
        output->InsertNextCell(VTKCellType::VTK_VERTEX, 1, cell);
    }

    output->Squeeze();

    return 1;
}

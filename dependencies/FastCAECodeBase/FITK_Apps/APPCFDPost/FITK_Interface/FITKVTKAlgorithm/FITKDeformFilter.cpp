/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKDeformFilter.h"

// VTK
#include <vtkDemandDrivenPipeline.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkCellData.h>
#include <vtkArrayDispatch.h>

// For VTK 7
//@{
#include <vtkSmartPointer.h>
#include <vtkCell.h>
//@}

//----------------------------------------------------------------------------
namespace
{
    // Used by the WarpVectorDispatch1Vector worker, defined below:
    template <typename VectorArrayT>
    struct WarpVectorDispatch2Points
    {
        FITKDeformFilter* Self;
        VectorArrayT* Vectors;

        WarpVectorDispatch2Points(FITKDeformFilter* self, VectorArrayT* vectors)
            : Self(self)
            , Vectors(vectors)
        {
        }

        template <typename InPointArrayT, typename OutPointArrayT>
        void operator()(InPointArrayT* inPtArray, OutPointArrayT* outPtArray)
        {
            typedef typename OutPointArrayT::ValueType PointValueT;
            const vtkIdType numTuples = inPtArray->GetNumberOfTuples();
            const double scaleFactor = this->Self->GetScaleFactor();

            assert(this->Vectors->GetNumberOfComponents() == 3);
            assert(inPtArray->GetNumberOfComponents() == 3);
            assert(outPtArray->GetNumberOfComponents() == 3);

            for (vtkIdType t = 0; t < numTuples; ++t)
            {
                if (!(t & 0xfff))
                {
                    this->Self->UpdateProgress(t / static_cast<double>(numTuples));
                    if (this->Self->GetAbortExecute())
                    {
                        return;
                    }
                }

                for (int c = 0; c < 3; ++c)
                {
                    PointValueT val =
                        inPtArray->GetTypedComponent(t, c) + scaleFactor * this->Vectors->GetTypedComponent(t, c);
                    outPtArray->SetTypedComponent(t, c, val);
                }
            }
        }
    };

    // Dispatch just the vector array first, we can cut out some generated code
    // since the point arrays will have the same type.
    struct WarpVectorDispatch1Vector
    {
        FITKDeformFilter* Self;
        vtkDataArray* InPoints;
        vtkDataArray* OutPoints;

        WarpVectorDispatch1Vector(FITKDeformFilter* self, vtkDataArray* inPoints, vtkDataArray* outPoints)
            : Self(self)
            , InPoints(inPoints)
            , OutPoints(outPoints)
        {
        }

        template <typename VectorArrayT>
        void operator()(VectorArrayT* vectors)
        {
            WarpVectorDispatch2Points<VectorArrayT> worker(this->Self, vectors);
            if (!vtkArrayDispatch::Dispatch2SameValueType::Execute(this->InPoints, this->OutPoints, worker))
            {
                vtkGenericWarningMacro("Error dispatching point arrays.");
            }
        }
    };
} // end anon namespace

FITKDeformFilter* FITKDeformFilter::New()
{
    FITKDeformFilter* filter = new FITKDeformFilter;
    filter->InitializeObjectBase();
    return filter;
}

void FITKDeformFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

int FITKDeformFilter::RequestData(vtkInformation* vtkNotUsed(request), vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    // Add the array to data set.
    vtkPointSet* inputData = vtkPointSet::GetData(inputVector[0]);
    vtkPointSet* outputData = vtkPointSet::GetData(outputVector);
    if (inputData == nullptr)
    {
        vtkErrorMacro("Input data object is invalid!");
        return 0;
    }

    // Get the point data of the input data.
    auto pointData = inputData->GetPointData();
    if (pointData == nullptr)
    {
        vtkErrorMacro("The point data of data set is invalid!");
        return 0;
    }

    // Check the array name and component indice of U, V, W.
    bool hasValidArray = true;
    vtkDataArray* deformArray = pointData->GetArray(this->DeformArrayName);
    if (deformArray == nullptr)
    {
        hasValidArray = false;
    }

    // Check component indice.
    if (this->UComponentIndex < 0 || this->VComponentIndex < 0 || this->WComponentIndex < 0)
    {
        hasValidArray = false;
    }

    // Check component indice with array.
    if (hasValidArray)
    {
        int nComp = deformArray->GetNumberOfComponents();
        if (nComp <= this->UComponentIndex || nComp <= this->VComponentIndex || nComp <= this->WComponentIndex)
        {
            hasValidArray = false;
        }
    }

    // Reset the scale factor value to zero if the factor is below zero.
    //if (ScaleFactor < 0)
    //{
    //    ScaleFactor = 0.;
    //}

    // Copy structure.
    outputData->CopyStructure(inputData);

    if (inputData == nullptr || inputData->GetPoints() == nullptr)
    {
        return 1;
    }

    vtkPoints* points;
    vtkIdType numPts;

    numPts = inputData->GetPoints()->GetNumberOfPoints();

    if (this->Deform && hasValidArray)
    {
        // SETUP AND ALLOCATE THE OUTPUT
        numPts = inputData->GetNumberOfPoints();
        points = inputData->GetPoints()->NewInstance();
        points->SetDataType(inputData->GetPoints()->GetDataType());
        points->Allocate(numPts);
        points->SetNumberOfPoints(numPts);
        outputData->SetPoints(points);
        points->Delete();

        // call templated function.
        // We use two dispatches since we need to dispatch 3 arrays and two share a
        // value type. Implementing a second type-restricted dispatch reduces
        // the amount of generated templated code.
        WarpVectorDispatch1Vector worker(
            this, inputData->GetPoints()->GetData(), outputData->GetPoints()->GetData());
        if (!vtkArrayDispatch::Dispatch::Execute(deformArray, worker))
        {
            vtkWarningMacro("Dispatch failed for vector array.");
        }
    }

    // now pass the data.
    outputData->GetPointData()->CopyNormalsOff(); // distorted geometry
    outputData->GetPointData()->PassData(inputData->GetPointData());
    outputData->GetCellData()->PassData(inputData->GetCellData());

    return 1;
}

FITKDeformFilter::FITKDeformFilter()
{
    // Initialize the algorithm parameters.
    this->ScaleFactor = 1;
    this->DeformArrayName = NULL;
    this->Deform = 1;

    this->UComponentIndex = 0;
    this->VComponentIndex = 1;
    this->WComponentIndex = 2;
}

FITKDeformFilter::~FITKDeformFilter()
{
    // Nothing to do here.
}
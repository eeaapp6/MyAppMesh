/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMaxMinValueFilter.h"

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
#include <vtkStringArray.h>
#include <vtkDoubleArray.h>

// For VTK 7
//@{
#include <vtkCellType.h>
#include <vtkCell.h>
//@}

// Qt
#include <QString>

FITKMaxMinValueFilter* FITKMaxMinValueFilter::New()
{
    FITKMaxMinValueFilter* filter = new FITKMaxMinValueFilter;
    filter->InitializeObjectBase();
    return filter;
}

void FITKMaxMinValueFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

int FITKMaxMinValueFilter::ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
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

int FITKMaxMinValueFilter::RequestData(vtkInformation* vtkNotUsed(request), vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    // Check the input data.
    vtkDataSet* inputData = vtkDataSet::GetData(inputVector[0]);
    vtkUnstructuredGrid* outputData = vtkUnstructuredGrid::GetData(outputVector);
    if (inputData == nullptr)
    {
        vtkErrorMacro("Input data object is invalid!");
        return 0;
    }

    // Check component indice.
    if (this->ComponentIndex < 0 && this->ComponentIndex != -1)
    {
        return 0;
    }

    // Get the point data and cell data of the input data.
    vtkPointData* pointData = inputData->GetPointData();
    vtkCellData* cellData = inputData->GetCellData();

    // Check the array.
    vtkDataArray* array{ nullptr };
    if (this->ArrayType == 1)
    {
        if (pointData)
        {
            array = pointData->GetArray(this->ArrayName);
        }
    }
    else if (this->ArrayType == 2)
    {
        if (cellData)
        {
            array = cellData->GetArray(this->ArrayName);
        }
    }

    // Whether the array is valid.
    if (!array)
    {
        return 0;
    }

    int nComps = array->GetNumberOfComponents();

    // Check component index with array.
    if (nComps <= this->ComponentIndex)
    {
        return 0;
    }

    // Copy a new data for output.
    vtkSmartPointer<vtkUnstructuredGrid> tempData = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkStringArray> stringArray = vtkSmartPointer<vtkStringArray>::New();
    vtkSmartPointer<vtkDoubleArray> scalarArray = vtkSmartPointer<vtkDoubleArray>::New();

    stringArray->SetName(this->LabelArrayName);
    tempData->SetPoints(points);

    // Add the array to point data.
    tempData->GetPointData()->AddArray(stringArray);

    // Get the number of input points.
    int nPts = inputData->GetNumberOfPoints();
    int nCells = inputData->GetNumberOfCells();

    double* range = array->GetRange(this->ComponentIndex);

    double r1 = range[0];
    double r2 = range[1];

    double value = 0;
    QString text;
    if (this->PeakDataType == 1)
    {
        value = r2;
        text = QString("MAX: %1").arg(r2, 0, 'e', 3);
    }
    else if (this->PeakDataType == 2)
    {
        value = r1;
        text = QString("MIN: %1").arg(r1, 0, 'e', 3);
    }

    // Get the center of the mesh.
    double* meshCenter = inputData->GetCenter();

    // The length is 0.125 * mag;
    double* bounds = inputData->GetBounds();
    double scale = sqrt(pow(bounds[1] - bounds[0], 2) + pow(bounds[3] - bounds[2], 2) + pow(bounds[5] - bounds[4], 2)) / 8.;

    if (!qFuzzyCompare(r1, r2))
    {
        // Max value.
        if (this->ArrayType == 1)
        {
            for (int i = 0; i < nPts; i++)
            {
                double val = 0.;
                if (this->ComponentIndex == -1)
                {
                    double* tp = array->GetTuple(i);

                    double magSqr = 0.;
                    for (int j = 0; j < nComps; j++)
                    {
                        magSqr += tp[j] * tp[j];
                    }

                    double mag = sqrt(magSqr);
                    val = mag;
                }
                else
                {
                    double compVal = array->GetComponent(i, this->ComponentIndex);
                    val = compVal;
                }

                if (qFuzzyCompare(val, value))
                {
                    double* pt = inputData->GetPoint(i);
                    points->InsertNextPoint(pt);
                    stringArray->InsertNextValue("");
                    scalarArray->InsertNextValue(0.);

                    double direc[3]{ 0., 0., 0. };
                    getLineDirection(meshCenter, pt, direc);

                    double ptOut[3]{ 0., 0., 0. };
                    for (int j = 0; j < 3; j++)
                    {
                        ptOut[j] = pt[j] + direc[j] * scale;
                    }

                    points->InsertNextPoint(ptOut);
                    stringArray->InsertNextValue(text.toUtf8().data());
                    scalarArray->InsertNextValue(1.);

                    if (this->GetFirst)
                    {
                        break;
                    }
                }
            }
        }
        // Min value.
        else if (this->ArrayType == 2)
        {
            for (int i = 0; i < nCells; i++)
            {
                double val = 0.;
                if (this->ComponentIndex == -1)
                {
                    double* tp = array->GetTuple(i);

                    double magSqr = 0.;
                    for (int j = 0; j < nComps; j++)
                    {
                        magSqr += tp[j] * tp[j];
                    }

                    double mag = sqrt(magSqr);
                    val = mag;
                }
                else
                {
                    double compVal = array->GetComponent(i, this->ComponentIndex);
                    val = compVal;
                }

                if (qFuzzyCompare(val, value))
                {
                    double center[3]{ 0., 0., 0. };
                    getCellCenter(inputData->GetCell(i), center);
                    points->InsertNextPoint(center);
                    stringArray->InsertNextValue("");
                    scalarArray->InsertNextValue(0.);

                    double direc[3]{ 0., 0., 0. };
                    getLineDirection(meshCenter, center, direc);

                    double ptOut[3]{ 0., 0., 0. };
                    for (int j = 0; j < 3; j++)
                    {
                        ptOut[j] = center[j] + direc[j] * scale;
                    }

                    points->InsertNextPoint(ptOut);
                    stringArray->InsertNextValue(text.toUtf8().data());
                    scalarArray->InsertNextValue(1.);

                    if (this->GetFirst)
                    {
                        break;
                    }
                }
            }
        }
    }

    // Create line cells.
    for (int i = 0; i < points->GetNumberOfPoints() / 2; i++)
    {
        vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
        cell->InsertNextId(i * 2);
        cell->InsertNextId(i * 2 + 1);
        tempData->InsertNextCell(VTKCellType::VTK_LINE, cell);
    }

    // Copy structures and array.
    outputData->CopyStructure(tempData);
    outputData->GetPointData()->PassData(tempData->GetPointData());
    outputData->GetPointData()->SetScalars(scalarArray);

    return 1;
}

FITKMaxMinValueFilter::FITKMaxMinValueFilter()
{
    // Initialize the algorithm parameters.
    this->GetFirst = true;
    this->ArrayName = NULL;

    this->ComponentIndex = 0;
}

FITKMaxMinValueFilter::~FITKMaxMinValueFilter()
{
    // Nothing to do here.
}

int FITKMaxMinValueFilter::FillInputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
    return 1;
}

int FITKMaxMinValueFilter::FillOutputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkUnstructuredGrid");
    return 1;
}

void FITKMaxMinValueFilter::getCellCenter(vtkCell* cell, double* center)
{
    // Check the input.
    if (!cell || !center)
    {
        return;
    }

    // Get the cell center of the cell.
    double max[3]{ 0., 0., 0. };
    int nPt = cell->GetNumberOfPoints();
    if (nPt == 0)
    {
        return;
    }

    for (int i = 0; i < nPt; i++)
    {
        double* pt = cell->GetPoints()->GetPoint(i);
        for (int j = 0; j < 3; j++)
        {
            max[j] += pt[j];
        }
    }

    for (int i = 0; i < 3; i++)
    {
        center[i] = max[i] / nPt;
    }
}

void FITKMaxMinValueFilter::getLineDirection(double* pt1, double* pt2, double* direc)
{
    // Get the direction of the line.
    // The direction is from pt1 to pt2.
    double length = sqrt(pow(pt2[0] - pt1[0], 2) + pow(pt2[1] - pt1[1], 2) + pow(pt2[2] - pt1[2], 2));
    if (length == 0)
    {
        direc[0] = 0.;
        direc[1] = 0.;
        direc[2] = 1.;
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        direc[i] = (pt2[i] - pt1[i]) / length;
    }
}

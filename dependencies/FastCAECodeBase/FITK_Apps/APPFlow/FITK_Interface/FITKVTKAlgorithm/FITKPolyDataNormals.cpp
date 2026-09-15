/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPolyDataNormals.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkIdList.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolygon.h"
#include "vtkTriangleStrip.h"

vtkStandardNewMacro(FITKPolyDataNormals);

FITKPolyDataNormals::FITKPolyDataNormals()
{
    this->ComputePointNormals = 0;
    this->ComputeCellNormals = 1;
    this->OutputPointsPrecision = vtkAlgorithm::DEFAULT_PRECISION;
    this->OldMesh = nullptr;
    this->NewMesh = nullptr;
    this->PolyNormals = nullptr;
}

#define VTK_CELL_NOT_VISITED 0
#define VTK_CELL_VISITED 1

// Generate normals for polygon meshes
int FITKPolyDataNormals::RequestData(vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    // get the info objects
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    // get the input and output
    vtkPolyData* input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkIdType npts = 0;
#if VTK_MAJOR_VERSION < 9
    vtkIdType* pts = nullptr; 
#else
    const vtkIdType* pts = nullptr;
#endif

    vtkIdType numNewPts;
    double flipDirection = 1.0;
    vtkIdType numVerts, numLines, numPolys, numStrips;
    vtkIdType cellId;
    vtkIdType numPts;
    vtkPoints* inPts;
    vtkCellArray *inPolys, *inStrips, *polys;
    vtkFloatArray* newNormals;
    vtkPointData *pd, *outPD;
    vtkDataSetAttributes* outCD = output->GetCellData();
    double n[3];
    vtkCellArray* newPolys;

    vtkDebugMacro(<< "Generating surface normals");

    numVerts = input->GetNumberOfVerts();
    numLines = input->GetNumberOfLines();
    numPolys = input->GetNumberOfPolys();
    numStrips = input->GetNumberOfStrips();
    if ((numPts = input->GetNumberOfPoints()) < 1)
    {
        vtkDebugMacro(<< "No data to generate normals for!");
        return 1;
    }

    // If there is nothing to do, pass the data through
    if ((this->ComputePointNormals == 0 && this->ComputeCellNormals == 0) ||
        (numPolys < 1 && numStrips < 1))
    { // don't do anything! pass data through
        output->CopyStructure(input);
        output->GetPointData()->PassData(input->GetPointData());
        output->GetCellData()->PassData(input->GetCellData());
        return 1;
    }

    if (numStrips < 1)
    {
        output->GetCellData()->PassData(input->GetCellData());
    }

    // Load data into cell structure.  We need two copies: one is a
    // non-writable mesh used to perform topological queries.  The other
    // is used to write into and modify the connectivity of the mesh.
    //
    inPts = input->GetPoints();
    inPolys = input->GetPolys();
    inStrips = input->GetStrips();

    this->OldMesh = vtkPolyData::New();
    this->OldMesh->SetPoints(inPts);
    if (numStrips > 0) // have to decompose strips into triangles
    {
        vtkDataSetAttributes* inCD = input->GetCellData();
        // When we have triangle strips, make sure to create and copy
        // the cell data appropriately. Since strips are broken into
        // triangles, cell data cannot be passed as it is and needs to
        // be copied tuple by tuple.
        outCD->CopyAllocate(inCD);
        if (numPolys > 0)
        {
            polys = vtkCellArray::New();
            polys->DeepCopy(inPolys);
            vtkSmartPointer<vtkIdList> ids = vtkSmartPointer<vtkIdList>::New();
            ids->SetNumberOfIds(numPolys);
            for (vtkIdType i = 0; i < numPolys; i++)
            {
                ids->SetId(i, i);
            }
            outCD->CopyData(inCD, ids, ids);
        }
        else
        {
            polys = vtkCellArray::New();

#if VTK_MAJOR_VERSION < 9
            polys->Allocate(polys->EstimateSize(numStrips, 5));
#else
            polys->AllocateEstimate(numStrips, 5);
#endif          
        }
        vtkIdType inCellIdx = numPolys;
        vtkIdType outCellIdx = numPolys;
        for (inStrips->InitTraversal(); inStrips->GetNextCell(npts, pts); inCellIdx++)
        {
            vtkTriangleStrip::DecomposeStrip(npts, pts, polys);
            // Copy the cell data for the strip to each triangle.
            for (vtkIdType i = 0; i < npts - 2; i++)
            {
                outCD->CopyData(inCD, inCellIdx, outCellIdx++);
            }
        }
        this->OldMesh->SetPolys(polys);
        polys->Delete();
        numPolys = polys->GetNumberOfCells(); // added some new triangles
    }
    else
    {
        this->OldMesh->SetPolys(inPolys);
        polys = inPolys;
    }
    this->OldMesh->BuildLinks();
    this->UpdateProgress(0.10);

    pd = input->GetPointData();
    outPD = output->GetPointData();

    this->NewMesh = vtkPolyData::New();
    this->NewMesh->SetPoints(inPts);
    // create a copy because we're modifying it
    newPolys = vtkCellArray::New();
    newPolys->DeepCopy(polys);
    this->NewMesh->SetPolys(newPolys);
    this->NewMesh->BuildCells(); // builds connectivity

    this->UpdateProgress(0.333);

    //  Initial pass to compute polygon normals without effects of neighbors
    //
    this->PolyNormals = vtkFloatArray::New();
    this->PolyNormals->SetNumberOfComponents(3);
    this->PolyNormals->SetName("Normals");
    this->PolyNormals->SetNumberOfTuples(numVerts + numLines + numPolys);

    vtkIdType offsetCells = numVerts + numLines;
    n[0] = 1.0;
    n[1] = 0.0;
    n[2] = 0.0;
    for (cellId = 0; cellId < offsetCells; cellId++)
    {
        // add a default value for vertices and lines
        // normals do not have meaningful values, we set them to X
        this->PolyNormals->SetTuple(cellId, n);
    }

    // Modified by CHT
    //@{
    for (cellId = 0; cellId < this->OldMesh->GetNumberOfCells(); cellId++)
    {
        if ((cellId % 1000) == 0)
        {
            this->UpdateProgress(0.333 + 0.333 * (double)cellId / (double)numPolys);
            if (this->GetAbortExecute())
            {
                break;
            }
        }

        vtkPolygon::ComputeNormal(this->OldMesh->GetCell(cellId)->GetPoints(), n);
        this->PolyNormals->SetTuple(offsetCells + cellId, n);
    }
    //@}

    //for (cellId = 0, newPolys->InitTraversal(); newPolys->GetNextCell(npts, pts); cellId++)
    //{
    //    if ((cellId % 1000) == 0)
    //    {
    //        this->UpdateProgress(0.333 + 0.333 * (double)cellId / (double)numPolys);
    //        if (this->GetAbortExecute())
    //        {
    //            break;
    //        }
    //    }

    //    vtkPolygon::ComputeNormal(inPts, npts, pts, n);
    //    this->PolyNormals->SetTuple(offsetCells + cellId, n);
    //}

    numNewPts = numPts;
    outPD->CopyNormalsOff();
    outPD->PassData(pd);

    this->UpdateProgress(0.80);

    newNormals = vtkFloatArray::New();
    newNormals->SetNumberOfComponents(3);
    newNormals->SetNumberOfTuples(numNewPts);
    newNormals->SetName("Normals");
    float* fNormals = newNormals->WritePointer(0, 3 * numNewPts);
    std::fill_n(fNormals, 3 * numNewPts, 0);

    float* fPolyNormals = this->PolyNormals->WritePointer(3 * offsetCells, 3 * numPolys);

    if (this->ComputePointNormals)
    {
        for (cellId = 0, newPolys->InitTraversal(); newPolys->GetNextCell(npts, pts); ++cellId)
        {
            for (vtkIdType i = 0; i < npts; ++i)
            {
                fNormals[3 * pts[i]] += fPolyNormals[3 * cellId];
                fNormals[3 * pts[i] + 1] += fPolyNormals[3 * cellId + 1];
                fNormals[3 * pts[i] + 2] += fPolyNormals[3 * cellId + 2];
            }
        }

        for (vtkIdType i = 0; i < numNewPts; ++i)
        {
            const double length =
                sqrt(fNormals[3 * i] * fNormals[3 * i] + fNormals[3 * i + 1] * fNormals[3 * i + 1] +
                    fNormals[3 * i + 2] * fNormals[3 * i + 2]) *
                flipDirection;
            if (length != 0.0)
            {
                fNormals[3 * i] /= length;
                fNormals[3 * i + 1] /= length;
                fNormals[3 * i + 2] /= length;
            }
        }
    }

    output->SetPoints(inPts);

    if (this->ComputeCellNormals)
    {
        outCD->SetNormals(this->PolyNormals);
    }
    this->PolyNormals->Delete();

    if (this->ComputePointNormals)
    {
        outPD->SetNormals(newNormals);
    }
    newNormals->Delete();

    output->SetPolys(newPolys);
    newPolys->Delete();

    // copy the original vertices and lines to the output
    output->SetVerts(input->GetVerts());
    output->SetLines(input->GetLines());

    this->OldMesh->Delete();
    this->NewMesh->Delete();

    return 1;
}

void FITKPolyDataNormals::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Compute Point Normals: " << (this->ComputePointNormals ? "On\n" : "Off\n");
    os << indent << "Compute Cell Normals: " << (this->ComputeCellNormals ? "On\n" : "Off\n");
    os << indent << "Precision of the output points: " << this->OutputPointsPrecision << "\n";
}

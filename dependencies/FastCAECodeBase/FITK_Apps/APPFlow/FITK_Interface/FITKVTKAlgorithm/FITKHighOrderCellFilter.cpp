/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKHighOrderCellFilter.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkIdTypeArray.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkUnsignedCharArray.h"

#include "vtkCell.h"
#include "vtkSmartPointer.h"

#define VTK_CREATE_CUSTOM(type, name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#include <algorithm>
#include <unordered_map>
#include <vector>

static const unsigned char NO_EDGE_FLAG_CUSTOM = static_cast<unsigned char>(-1);

//=============================================================================
namespace FITKHighOrderCellFilterNamespace
{
    // Description:
    // Simple class used internally to define an edge based on the endpoints.  The
    // endpoints are canonically identified by the lower and higher values.
    class EdgeEndpointsCus
    {
    public:
        EdgeEndpointsCus()
            : MinEndPoint(-1)
            , MaxEndPoint(-1)
        {
        }
        EdgeEndpointsCus(vtkIdType endpointA, vtkIdType endpointB)
            : MinEndPoint((endpointA < endpointB) ? endpointA : endpointB)
            , MaxEndPoint((endpointA < endpointB) ? endpointB : endpointA)
        {
        }
        EdgeEndpointsCus(const EdgeEndpointsCus& other) = default;
        ~EdgeEndpointsCus() = default;
        const vtkIdType MinEndPoint;
        const vtkIdType MaxEndPoint;
        inline bool operator==(const EdgeEndpointsCus& other) const
        {
            return ((this->MinEndPoint == other.MinEndPoint) && (this->MaxEndPoint == other.MaxEndPoint));
        }

    private:
        void operator=(const EdgeEndpointsCus&);
    };
    struct EdgeEndpointsHashCus
    {
    public:
        size_t operator()(const EdgeEndpointsCus& edge) const
        {
            return static_cast<size_t>(edge.MinEndPoint + edge.MaxEndPoint);
        }
    };

    // Description:
    // Holds the information necessary for the facet this edge came from.
    class EdgeInformationCus
    {
    public:
        vtkIdType OriginalCellId;
        vtkIdType OriginalFaceId;
        vtkIdType StartPointId;
    };

    // Description:
    // A map from edge endpoints to the information about that edge.
    typedef std::unordered_map<EdgeEndpointsCus, EdgeInformationCus, EdgeEndpointsHashCus> EdgeMapType;

    void RecordEdgeFlag(vtkPolyData* output, EdgeInformationCus& edgeInfo,
        vtkUnsignedCharArray* edgeFlagArray, unsigned char flag, vtkIdType* duplicatePointMap, 
        int npts)
    {
        vtkIdType pt = edgeInfo.StartPointId;

        // Added by CHT
        //@{
        // The line cell must be shown.
        if (npts == 2)
        {
            edgeFlagArray->SetValue(pt, 1);
            return;
        }
        //@}

        if (edgeFlagArray->GetValue(pt) == flag)
        {
            // Edge flag already set correctly.  Nothing to do.
            return;
        }

        if (edgeFlagArray->GetValue(pt) == NO_EDGE_FLAG_CUSTOM)
        {
            // Nothing has set the edge flag yet.  Just set it and return.
            edgeFlagArray->SetValue(pt, flag);
            return;
        }

        // If we are here then some other cell has already put a flag on this
        // point different than ours.  We have to adjust our cell topology to
        // use a duplicate point.
        if (duplicatePointMap[pt] == -1)
        {
            // No duplicate made.  We need to make one.
            vtkPoints* points = output->GetPoints();
            double coords[3];
            points->GetPoint(pt, coords);
            vtkIdType newPt = points->InsertNextPoint(coords);
            duplicatePointMap[pt] = newPt;
            // Copying attributes from yourself seems weird, but is valid.
            vtkPointData* pd = output->GetPointData();
            pd->CopyData(pd, pt, newPt);
            edgeFlagArray->InsertValue(newPt, flag);
        }

#if VTK_MAJOR_VERSION >= 9
        output->ReplaceCellPoint(edgeInfo.OriginalCellId, edgeInfo.StartPointId, duplicatePointMap[pt]);
#else
        // ReplaceCellPoint in vtkPolyData will crash in vtk 7, use ReplaceCell instead.
        vtkCell* cell = output->GetCell(edgeInfo.OriginalCellId);
        int nCellPoint = cell->GetNumberOfPoints();
        vtkIdType* cellNew = new long long[nCellPoint];
        for (int i = 0; i < nCellPoint; i++)
        {
            if (cell->GetPointId(i) == edgeInfo.StartPointId)
            {
                cellNew[i] = duplicatePointMap[pt];
            }
            else
            {
                cellNew[i] = cell->GetPointId(i);
            }
        }

        output->ReplaceCell(edgeInfo.OriginalCellId, nCellPoint, cellNew);
#endif

        edgeInfo.StartPointId = duplicatePointMap[pt];
    }
}
using namespace FITKHighOrderCellFilterNamespace;

//=============================================================================
vtkStandardNewMacro(FITKHighOrderCellFilter);

//-----------------------------------------------------------------------------
FITKHighOrderCellFilter::FITKHighOrderCellFilter()
{
    // OrginalCellIdsName = "vtkOriginalCellIds";
}

FITKHighOrderCellFilter::~FITKHighOrderCellFilter() = default;

void FITKHighOrderCellFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

//-----------------------------------------------------------------------------
int FITKHighOrderCellFilter::RequestData(vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    vtkPolyData* input = vtkPolyData::GetData(inputVector[0]);
    vtkPolyData* output = vtkPolyData::GetData(outputVector);

    vtkIdType npts;

#if VTK_MAJOR_VERSION >= 9
    const vtkIdType* pts;
#else
    vtkIdType* pts;
#endif

    std::vector<vtkIdType> originalPts;

    //if (!input->GetCellData()->HasArray(OrginalCellIdsName))
    //{
    //    // Did not find the field used to identify the face each linear patch comes
    //    // from.  The original data was probably linear so using all the edges
    //    // should be OK (and is at least the best we can do), so just pass the data.
    //    output->ShallowCopy(input);
    //    return 1;
    //}

    //vtkIdTypeArray* faceIds =
    //    vtkIdTypeArray::SafeDownCast(input->GetCellData()->GetAbstractArray(OrginalCellIdsName));
    //if (!faceIds)
    //{
    //    vtkErrorMacro(<< OrginalCellIdsName << " array is not of expected type.");
    //    return 0;
    //}

    // Shallow copy the cell data.  All the cells get copied to output.
    output->GetCellData()->PassData(input->GetCellData());

    // Deep copy the point information and be ready to add points.
    VTK_CREATE_CUSTOM(vtkPoints, points);
    points->DeepCopy(input->GetPoints());
    output->SetPoints(points);
    vtkPointData* inputPD = input->GetPointData();
    vtkPointData* outputPD = output->GetPointData();
    outputPD->CopyAllocate(inputPD);
    vtkIdType numOriginalPoints = points->GetNumberOfPoints();
    for (vtkIdType i = 0; i < numOriginalPoints; i++)
    {
        outputPD->CopyData(inputPD, i, i);
    }

    // Create an edge flag array.
    VTK_CREATE_CUSTOM(vtkUnsignedCharArray, edgeflags);
    edgeflags->SetName("vtkEdgeFlags");
    outputPD->AddArray(edgeflags);
    outputPD->SetActiveAttribute("vtkEdgeFlags", vtkDataSetAttributes::EDGEFLAG);
    edgeflags->SetNumberOfComponents(1);
    edgeflags->SetNumberOfTuples(numOriginalPoints);
    std::fill(edgeflags->GetPointer(0), edgeflags->GetPointer(numOriginalPoints), NO_EDGE_FLAG_CUSTOM);

    // Some (probably many) points will have to be duplicated because different
    // cells will need different edge flags.  This array maps the original
    // point id to the duplicate id.
    std::vector<vtkIdType> duplicatePointMap(numOriginalPoints);
    std::fill(duplicatePointMap.begin(), duplicatePointMap.end(), -1);

#if VTK_MAJOR_VERSION >= 9
    // Shallow copy the verts.  Set the edge flags to true.
    vtkCellArray* inputVerts = input->GetVerts();
    output->SetVerts(inputVerts);
    for (inputVerts->InitTraversal(); inputVerts->GetNextCell(npts, pts);)
    {
        for (vtkIdType i = 0; i < npts; i++)
        {
            edgeflags->SetValue(pts[i], 1);
        }
    }

    // Shallow copy the lines. Set the edge flags to true.
    vtkCellArray* inputLines = input->GetLines();
    output->SetLines(inputLines);
    for (inputLines->InitTraversal(); inputLines->GetNextCell(npts, pts);)
    {
        // No need to set edge flag for last index.
        for (vtkIdType i = 0; i < npts - 1; i++)
        {
            edgeflags->SetValue(pts[i], 1);
        }
    }

    // Shallow copy the triangle strips.  Set the edge flags to true.
    vtkCellArray* inputStrips = input->GetStrips();
    output->SetStrips(inputStrips);
    for (inputStrips->InitTraversal(); inputStrips->GetNextCell(npts, pts);)
    {
        for (vtkIdType i = 0; i < npts; i++)
        {
            edgeflags->SetValue(pts[i], 1);
        }
    }

    // Deep copy the polygons because we will be changing some indices when we
    // duplicate points.
    VTK_CREATE_CUSTOM(vtkCellArray, outputPolys);
    outputPolys->DeepCopy(input->GetPolys());
    output->SetPolys(outputPolys);

    // Iterate over all the input facets and see which edge interfaces belonged to
    // different faces.  We do that by recording the original face id in a map.
    // When we find a pair of edges, we turn on the appropriate edge flag if they
    // came from different faces, or turn it off if they came from the same face.
    EdgeMapType edgeMap;
    vtkIdType inputCellId = inputVerts->GetNumberOfCells() + inputLines->GetNumberOfCells();
    for (outputPolys->InitTraversal(); outputPolys->GetNextCell(npts, pts); inputCellId++)
    {
        if ((inputCellId % 1000) == 0)
        {
            this->UpdateProgress(static_cast<double>(inputCellId) / input->GetNumberOfCells());
            if (this->GetAbortExecute())
                return 0;
        }
        // Record the original points of the polygon.  As we iterate over edges,
        // we may change the indices, but we always compare edges by the original
        // indices.
        originalPts.resize(npts);
        std::copy(pts, pts + npts, originalPts.begin());

        vtkIdType originalFace = inputCellId;
        // vtkIdType originalFace = faceIds->GetValue(inputCellId);
        for (vtkIdType i = 0; i < npts; i++)
        {
            EdgeEndpointsCus edge(originalPts[i], originalPts[(i + 1) % npts]);
            EdgeInformationCus edgeInfo;
            edgeInfo.OriginalCellId = inputCellId;
            edgeInfo.OriginalFaceId = originalFace;
            edgeInfo.StartPointId = pts[i];

            EdgeMapType::iterator edgeMatch = edgeMap.find(edge);
            if (edgeMatch == edgeMap.end())
            {
                // Not encountered yet.  Add to the map.
                edgeMap.insert(std::make_pair(edge, edgeInfo));
            }
            else
            {
                // The edge flag is true if the edge connects two different faces.
                unsigned char eflag =
                    static_cast<unsigned char>(edgeMatch->second.OriginalFaceId != originalFace);
                RecordEdgeFlag(output, edgeMatch->second, edgeflags, eflag, &duplicatePointMap.at(0), npts);
                RecordEdgeFlag(output, edgeInfo, edgeflags, eflag, &duplicatePointMap.at(0), npts);
                // Remove the edge from the map since we already found the pair.
                edgeMap.erase(edgeMatch);
            }
        } // For each edge
    }   // For each cell.

    // Everything left in the edge map has no match.  It must necessarily be
    // on the outside of a face.
    for (EdgeMapType::iterator iter = edgeMap.begin(); iter != edgeMap.end(); iter++)
    {
        RecordEdgeFlag(output, iter->second, edgeflags, 1, &duplicatePointMap.at(0), npts);
    }

#else 
    // Shallow copy the verts.  Set the edge flags to true.
    vtkCellArray* inputVerts = input->GetVerts();
    output->SetVerts(inputVerts);
    if (inputVerts->GetNumberOfCells() != 0)
    {
        for (inputVerts->InitTraversal(); inputVerts->GetNextCell(npts, pts);)
        {
            for (vtkIdType i = 0; i < npts; i++)
            {
                edgeflags->SetValue(pts[i], 1);
            }
        }
    }

    // Shallow copy the lines. Set the edge flags to true.
    vtkCellArray* inputLines = input->GetLines();
    output->SetLines(inputLines);
    if (inputLines->GetNumberOfCells() != 0)
    {
        for (inputLines->InitTraversal(); inputLines->GetNextCell(npts, pts);)
        {
            // No need to set edge flag for last index.
            for (vtkIdType i = 0; i < npts - 1; i++)
            {
                edgeflags->SetValue(pts[i], 1);
            }
        }
    }

    // Shallow copy the triangle strips.  Set the edge flags to true.
    vtkCellArray* inputStrips = input->GetStrips();
    output->SetStrips(inputStrips);
    if (inputStrips->GetNumberOfCells() != 0)
    {
        for (inputStrips->InitTraversal(); inputStrips->GetNextCell(npts, pts);)
        {
            for (vtkIdType i = 0; i < npts; i++)
            {
                edgeflags->SetValue(pts[i], 1);
            }
        }
    }

    // Deep copy the polygons because we will be changing some indices when we
    // duplicate points.
    VTK_CREATE_CUSTOM(vtkCellArray, outputPolys);
    outputPolys->DeepCopy(input->GetPolys());
    output->SetPolys(outputPolys);

    // Iterate over all the input facets and see which edge interfaces belonged to
    // different faces.  We do that by recording the original face id in a map.
    // When we find a pair of edges, we turn on the appropriate edge flag if they
    // came from different faces, or turn it off if they came from the same face.
    EdgeMapType edgeMap;
    vtkIdType inputCellId = inputVerts->GetNumberOfCells() + inputLines->GetNumberOfCells();

    if (outputPolys->GetNumberOfCells() != 0)
    {
        outputPolys->InitTraversal();
        for (int j = 0; j < outputPolys->GetNumberOfCells(); j++, inputCellId++)
            //for (outputPolys->InitTraversal(); outputPolys->GetNextCell(npts, pts); inputCellId++)
        {
            outputPolys->GetNextCell(npts, pts);

            if ((inputCellId % 1000) == 0)
            {
                this->UpdateProgress(static_cast<double>(inputCellId) / input->GetNumberOfCells());
                if (this->GetAbortExecute())
                    return 0;
            }
            // Record the original points of the polygon.  As we iterate over edges,
            // we may change the indices, but we always compare edges by the original
            // indices.

            originalPts.resize(npts);
            std::copy(pts, pts + npts, originalPts.begin());

            vtkIdType originalFace = inputCellId;
            // vtkIdType originalFace = faceIds->GetValue(inputCellId);
            for (vtkIdType i = 0; i < npts; i++)
            {
                EdgeEndpointsCus edge(originalPts[i], originalPts[(i + 1) % npts]);
                EdgeInformationCus edgeInfo;
                edgeInfo.OriginalCellId = inputCellId;
                edgeInfo.OriginalFaceId = originalFace;
                edgeInfo.StartPointId = pts[i];

                EdgeMapType::iterator edgeMatch = edgeMap.find(edge);
                if (edgeMatch == edgeMap.end())
                {
                    // Not encountered yet.  Add to the map.
                    edgeMap.insert(std::make_pair(edge, edgeInfo));
                }
                else
                {
                    // The edge flag is true if the edge connects two different faces.
                    unsigned char eflag =
                        static_cast<unsigned char>(edgeMatch->second.OriginalFaceId != originalFace);
                    RecordEdgeFlag(output, edgeMatch->second, edgeflags, eflag, &duplicatePointMap.at(0), npts);
                    RecordEdgeFlag(output, edgeInfo, edgeflags, eflag, &duplicatePointMap.at(0), npts);
                    // Remove the edge from the map since we already found the pair.
                    edgeMap.erase(edgeMatch);
                }
            } // For each edge
        }   // For each cell.
    }

    // Everything left in the edge map has no match.  It must necessarily be
    // on the outside of a face.
    for (EdgeMapType::iterator iter = edgeMap.begin(); iter != edgeMap.end(); iter++)
    {
        RecordEdgeFlag(output, iter->second, edgeflags, 1, &duplicatePointMap.at(0), npts);
    }
#endif

    // If any points are unmarked, set some edge flag on them (although they
    // are probably not referenced by any cell).
    for (vtkIdType i = 0; i < numOriginalPoints; i++)
    {
        if (edgeflags->GetValue(i) == NO_EDGE_FLAG_CUSTOM)
            edgeflags->SetValue(i, 1);
    }

    return 1;
}

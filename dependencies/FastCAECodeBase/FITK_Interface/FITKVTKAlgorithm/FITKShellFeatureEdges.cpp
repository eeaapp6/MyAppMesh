/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKShellFeatureEdges.h"

// #include "Common/DebugLogger.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkIncrementalPointLocator.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkMergePoints.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolygon.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkTriangleStrip.h"
#include "vtkUnsignedCharArray.h"

#include "vtkIdTypeArray.h"
#include "vtkMergePoints.h"

#include "vtkVersionMacros.h"

vtkStandardNewMacro(FITKShellFeatureEdges);

// Construct object with feature angle = 30; all types of edges, except
// manifold edges, are extracted and colored.
FITKShellFeatureEdges::FITKShellFeatureEdges()
{
    this->FeatureAngle = 5.0;
    this->KeepOnly2D = false;
    this->BoundaryEdges = 0;
    this->FeatureEdges = 1;
    this->NonManifoldEdges = 0;
    this->ManifoldEdges = 0;
    this->Coloring = 0;
    this->Locator = nullptr;
    this->OutputPointsPrecision = vtkAlgorithm::DEFAULT_PRECISION;
    this->PassThroughIds = 0;
}

FITKShellFeatureEdges::~FITKShellFeatureEdges()
{
    if (this->Locator)
    {
        this->Locator->Delete();
        this->Locator = nullptr;
    }
}

// Generate feature edges for mesh
int FITKShellFeatureEdges::RequestData(
    vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector,
    vtkInformationVector* outputVector)
{
    // Clear cache data.
    _surEdgeIDMap.clear();
    _edgePtSurfPtMap.clear();
    _surfPtEdgePtMap.clear();
    _ptLines.clear();
    _nullVec.clear();
    _edgeSurCell.clear();

    // get the info objects
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    // get the input and output
    vtkPolyData* input = vtkPolyData::SafeDownCast(
        inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkPolyData* output = vtkPolyData::SafeDownCast(
        outInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkPoints* inPts;
    vtkPoints* newPts;
    vtkFloatArray* newScalars = nullptr;
    vtkIdTypeArray* newIDArr = nullptr;
    vtkCellArray* newLines;
    vtkPolyData* Mesh;
    int             i;
    vtkIdType       j, numNei, cellId;
    vtkIdType       numBEdges, numNonManifoldEdges, numFedges, numManifoldEdges;
    double          scalar, n[3], x1[3], x2[3];
    double          cosAngle = 0;
    vtkIdType       lineIds[2];
    vtkIdType       npts = 0;

    // Added by CHT
#if VTK_MAJOR_VERSION >=9
    const vtkIdType* pts = nullptr;
#else
    vtkIdType* pts = nullptr;
#endif

    vtkCellArray* inPolys, * inStrips, * newPolys;
    vtkFloatArray* polyNormals = nullptr;
    vtkIdType       numPts, numCells, numPolys, numStrips, nei;
    vtkIdList* neighbors;
    vtkIdType       p1, p2, newId;
    vtkPointData* pd = input->GetPointData(), * outPD = output->GetPointData();
    vtkCellData* cd = input->GetCellData(), * outCD = output->GetCellData();

    unsigned char* ghosts = nullptr;
    vtkDebugMacro(<< "Executing feature edges");

    vtkDataArray* temp = nullptr;
    if (cd)
    {
        temp = cd->GetArray(vtkDataSetAttributes::GhostArrayName());
    }
    if ((!temp) || (temp->GetDataType() != VTK_UNSIGNED_CHAR) || (temp->GetNumberOfComponents() != 1))
    {
        vtkDebugMacro("No appropriate ghost levels field available.");
    }
    else
    {
        ghosts = static_cast<vtkUnsignedCharArray*>(temp)->GetPointer(0);
    }

    //  Check input
    //
    inPts = input->GetPoints();
    numCells = input->GetNumberOfCells();
    numPolys = input->GetNumberOfPolys();
    numStrips = input->GetNumberOfStrips();
    if ((numPts = input->GetNumberOfPoints()) < 1 || !inPts ||
        (numPolys < 1 && numStrips < 1))
    {
        vtkDebugMacro(<< "No input data!");
        return 1;
    }

    if (!this->BoundaryEdges && !this->NonManifoldEdges &&
        !this->FeatureEdges && !this->ManifoldEdges)
    {
        vtkDebugMacro(<< "All edge types turned off!");
        return 1;
    }

    // Build cell structure.  Might have to triangulate the strips.
    Mesh = vtkPolyData::New();
    Mesh->SetPoints(inPts);
    inPolys = input->GetPolys();
    // 原有的节点id存储在此处
    //vtkIdTypeArray* inPolyData = inPolys->GetData( );
    //vtkNotUsed(inPolyData);
    if (numStrips > 0)
    {
        newPolys = vtkCellArray::New();
        if (numPolys > 0)
        {
            newPolys->DeepCopy(inPolys);
        }
        else
        {
            newPolys->Allocate(newPolys->EstimateSize(numStrips, 5));
        }

        inStrips = input->GetStrips();
        for (inStrips->InitTraversal(); inStrips->GetNextCell(npts, pts); )
        {
            vtkTriangleStrip::DecomposeStrip(npts, pts, newPolys);
        }

        Mesh->SetPolys(newPolys);
        newPolys->Delete();
    }
    else
    {
        newPolys = inPolys;
        Mesh->SetPolys(newPolys);
    }
    Mesh->BuildLinks();

    // Allocate storage for lines/points (arbitrary allocation sizes)
    //
    newPts = vtkPoints::New();

    // Set the desired precision for the points in the output.
    if (this->OutputPointsPrecision == vtkAlgorithm::DEFAULT_PRECISION)
    {
        newPts->SetDataType(inPts->GetDataType());
    }
    else if (this->OutputPointsPrecision == vtkAlgorithm::SINGLE_PRECISION)
    {
        newPts->SetDataType(VTK_FLOAT);
    }
    else if (this->OutputPointsPrecision == vtkAlgorithm::DOUBLE_PRECISION)
    {
        newPts->SetDataType(VTK_DOUBLE);
    }

    newPts->Allocate(numPts / 10, numPts);
    newLines = vtkCellArray::New();
    newLines->Allocate(numPts / 10);

    if (this->Coloring)
    {
        newScalars = vtkFloatArray::New();
        newScalars->SetName("Edge Types");
        newScalars->Allocate(numCells / 10, numCells);
    }
    vtkCellData* cellData = input->GetCellData();
    vtkPointData* pointData = input->GetPointData();

    //input与grid的单元号对应关系
    vtkDataArray* cellDataArr = cellData->GetArray("vtkOriginalCellIds");
    //input与grid的节点号对应关系
    vtkDataArray* pointDataArr = pointData->GetArray("vtkOriginalPointIds");
    // The dimension of the original cells.
    vtkIntArray* cellDimArray = vtkIntArray::SafeDownCast(cellData->GetArray("vtkOriginalCellDims"));

    if (this->PassThroughIds)
    {
        // 若需要输出原始点号，放开此语句
        // 若input中无所需数据，则不输出原id
        if (cellDataArr == nullptr || pointDataArr == nullptr)
        {
            // DebugWarn( "ShellFeatureEdges::PassThroughIdsOff,because of no oriId data" );
            PassThroughIdsOff();
        }
        else
        {
            newIDArr = vtkIdTypeArray::New();
            newIDArr->SetName("OriIDArray");
            newIDArr->SetNumberOfComponents(4);
        }
    }
    outPD->CopyAllocate(pd, numPts);
    outCD->CopyAllocate(cd, numCells);

    // Get our locator for merging points
    //
    if (this->Locator == nullptr)
    {
        this->CreateDefaultLocator();
    }
    this->Locator->InitPointInsertion(newPts, input->GetBounds());

    // Loop over all polygons generating boundary, non-manifold,
    // and feature edges
    //
    if (this->FeatureEdges)
    {
        polyNormals = vtkFloatArray::New();
        polyNormals->SetNumberOfComponents(3);
        polyNormals->Allocate(3 * newPolys->GetNumberOfCells());

        for (cellId = 0, newPolys->InitTraversal(); newPolys->GetNextCell(npts, pts);
            cellId++)
        {
            vtkPolygon::ComputeNormal(inPts, npts, pts, n);
            polyNormals->InsertTuple(cellId, n);
        }

        cosAngle = cos(vtkMath::RadiansFromDegrees(this->FeatureAngle));
    }

    neighbors = vtkIdList::New();
    neighbors->Allocate(VTK_CELL_SIZE);

    int       abort = 0;
    vtkIdType progressInterval = numCells / 20 + 1;

    numBEdges = numNonManifoldEdges = numFedges = numManifoldEdges = 0;

    vtkSmartPointer<vtkIdList> neiForBd = vtkSmartPointer<vtkIdList>::New();
    vtkSmartPointer<vtkIdList> pointCells1 = vtkSmartPointer<vtkIdList>::New();

    int lineId = 0;
    for (cellId = 0, newPolys->InitTraversal();
        newPolys->GetNextCell(npts, pts) && !abort; cellId++)
    {
        if (!(cellId % progressInterval))  //manage progress / early abort
        {
            this->UpdateProgress(static_cast<double>(cellId) / numCells);
            abort = this->GetAbortExecute();
        }

        // Skip the 3D cells.
        if (cellDimArray && this->KeepOnly2D)
        {
            if (cellDimArray->GetValue(cellId) == 3)
            {
                continue;
            }
        }

        for (i = 0; i < npts; i++)
        {
            p1 = pts[i];
            p2 = pts[(i + 1) % npts];

            // Added by ChengHaotian Remove duplicated edge.
            //@{
            //int numCellCurrent = newLines->GetNumberOfCells();
            //for (int j = 0; j < numCellCurrent; j++)
            //{
            //    vtkSmartPointer<vtkIdList> pointPair = vtkSmartPointer<vtkIdList>::New();
            //    newLines->GetCellAtId(j, pointPair);

            //    // We just need to check if the p2-p1 edge exist, because the each edge will only be added once.
            //    if (/*(pointPair->GetId(0) == p1 && pointPair->GetId(1) == p2)
            //        || */(pointPair->GetId(1) == p1 && pointPair->GetId(0) == p2))
            //    {
            //        continue;
            //    }
            //}
            //@}

            Mesh->GetCellEdgeNeighbors(cellId, p1, p2, neighbors);
            numNei = neighbors->GetNumberOfIds();
            int numNei2 = numNei;
            //无临近单元的单元为边界单元
            if (this->BoundaryEdges && numNei < 1)
            {
                if (ghosts &&
                    ghosts[cellId] & vtkDataSetAttributes::DUPLICATECELL)
                {
                    continue;
                }
                else
                {
                    numBEdges++;
                    scalar = 0.0;
                }
            }
            // 有1个以上相邻单元，是非流形面
            else if (this->NonManifoldEdges && numNei > 1)
            {
                // check to make sure that this edge hasn't been created before
                for (j = 0; j < numNei; j++)
                {
                    if (neighbors->GetId(j) < cellId)
                    {
                        break;
                    }
                }
                if (j >= numNei)
                {
                    if (ghosts &&
                        ghosts[cellId] & vtkDataSetAttributes::DUPLICATECELL)
                    {
                        continue;
                    }
                    else
                    {
                        numNonManifoldEdges++;
                        scalar = 0.222222;
                    }
                }
                else
                {
                    continue;
                }
            }
            // numNei == 1可能是特征边，判断nei>cellId是为了防止重复
            else if (this->FeatureEdges &&
                numNei == 1 && (nei = neighbors->GetId(0)) > cellId)
            {

                // _edgeSurCell[newId].push_back(cellId);

                Mesh->GetCellEdgeNeighbors(cellId, p1, p2, neighbors);

                // neighbors->Reset();
                neiForBd->Reset();
                neiForBd->SetNumberOfIds(0);
                pointCells1->Reset();
                pointCells1->SetNumberOfIds(0);
                Mesh->GetPointCells(p1, pointCells1);

                int neiNumTemp = 0;
                for (int index1 = 0; index1 < pointCells1->GetNumberOfIds(); index1++)
                {
                    int cellIdNei1 = pointCells1->GetId(index1);
                    vtkCell* ptCell1 = Mesh->GetCell(cellIdNei1);
                    vtkIdList* cPtIds1 = ptCell1->GetPointIds();
                    if (cPtIds1->IsId(p2) != -1)
                    {
                        neiForBd->InsertNextId(cellIdNei1);
                        neiNumTemp++;
                    }
                }

                // Added by ChengHaotian
                //@{
                if (neiNumTemp == 2)
                {
                    // Get the angle between two cells.
                    int id1 = neiForBd->GetId(0);
                    int id2 = neiForBd->GetId(1);

                    double normal1[3]{ 0., 0., 0. };
                    double normal2[3]{ 0., 0., 0. };

                    vtkCell* neiCell1 = Mesh->GetCell(id1);
                    vtkPolygon::ComputeNormal(neiCell1->GetPoints(), normal1);

                    vtkCell* neiCell2 = Mesh->GetCell(id2);
                    vtkPolygon::ComputeNormal(neiCell2->GetPoints(), normal2);

                    double rad = vtkMath::AngleBetweenVectors(normal1, normal2);
                    double angle = vtkMath::DegreesFromRadians(rad);

                    double factor = 30.;
                    if (angle > 180 - factor || angle < factor)
                    {
                        continue;
                    }
                }
                else if (neiNumTemp == 1)
                {

                }
                else
                {
                    continue;
                }
                //@}

                double neiTuple[3];
                double cellTuple[3];
                polyNormals->GetTuple(nei, neiTuple);
                polyNormals->GetTuple(cellId, cellTuple);
                double cos1 = vtkMath::Dot(neiTuple, cellTuple);
                // 角度大于等于特征角度，则为特征边
                if (cos1 <= cosAngle)
                {
                    if (ghosts &&
                        ghosts[cellId] & vtkDataSetAttributes::DUPLICATECELL)
                    {
                        continue;
                    }
                    else
                    {
                        numFedges++;
                        scalar = 0.444444;
                    }
                }
                else
                {
                    continue;
                }
            }
            else if (this->ManifoldEdges &&
                numNei == 1 && neighbors->GetId(0) > cellId)
            {
                if (ghosts &&
                    ghosts[cellId] & vtkDataSetAttributes::DUPLICATECELL)
                {
                    continue;
                }
                else
                {
                    numManifoldEdges++;
                    scalar = 0.666667;
                }
            }
            else
            {
                continue;
            }

            // Add edge to output
            Mesh->GetPoint(p1, x1);
            Mesh->GetPoint(p2, x2);

            if (this->Locator->InsertUniquePoint(x1, lineIds[0]))
            {
                outPD->CopyData(pd, p1, lineIds[0]);
            }

            if (this->Locator->InsertUniquePoint(x2, lineIds[1]))
            {
                outPD->CopyData(pd, p2, lineIds[1]);
            }

            /// 添加特征网格与表面网格间节点映射
            //int ptNum = input->GetNumberOfPoints( );
            //Q_UNUSED(ptNum);
            if (_edgePtSurfPtMap.contains(lineIds[0]))
            {
                int oldValue = _edgePtSurfPtMap.value(lineIds[0]);
                if (p1 != oldValue)
                {
                    //int a = 0;
                    //Q_UNUSED(a);
                }
            }
            else
            {
                _edgePtSurfPtMap.insert(lineIds[0], p1);
                _edgePtSurfPtMap.insert(lineIds[1], p2);

                _surfPtEdgePtMap.insert(p1, lineIds[0]);
                _surfPtEdgePtMap.insert(p2, lineIds[1]);
            }

            newId = newLines->InsertNextCell(2, lineIds);
            _edgeSurCell[newId];
            _edgeSurCell[newId].push_back(cellId);

            _surEdgeIDMap[cellId];
            _surEdgeIDMap[cellId].push_back(newId);
            for (int i = 0; i < numNei2; ++i)
            {
                vtkIdType neiID = neighbors->GetId(i);
                _edgeSurCell[newId].push_back(neiID);
                _surEdgeIDMap[neiID].push_back(newId);

            }

            // m_edgeSurfaceHash.insert(newId, QPair<int, int>(cellId, ));

            outCD->CopyData(cd, cellId, newId);
            if (this->Coloring)
            {
                newScalars->InsertTuple(newId, &scalar);
            }

            _ptLines[p1];
            _ptLines[p2];
            _ptLines[p1].push_back(newId);
            _ptLines[p2].push_back(newId);

            if (this->PassThroughIds)
            {
                double ids[4] = { 0 };
                ids[0] = cellId;
                ids[1] = p1;
                ids[2] = p2;

                // 若需要获取原点号时放开
//                ids[ 0 ]           = cellDataArr->GetComponent( cellId, 0 );
//                ids[ 1 ]           = pointDataArr->GetComponent( p1, 0 );
//                ids[ 2 ]           = pointDataArr->GetComponent( p2, 0 );
                ids[3] = pointDataArr->GetComponent(lineId, 0);
                newIDArr->InsertTuple(newId, ids);
            }
        }
    }

    vtkDebugMacro(<< "Created " << numBEdges << " boundary edges, "
        << numNonManifoldEdges << " non-manifold edges, "
        << numFedges << " feature edges, "
        << numManifoldEdges << " manifold edges");

    //  Update ourselves.
    //
    if (this->FeatureEdges)
    {
        polyNormals->Delete();
    }

    Mesh->Delete();

    output->SetPoints(newPts);
    newPts->Delete();
    neighbors->Delete();

    output->SetLines(newLines);
    newLines->Delete();
    this->Locator->Initialize();  //release any extra memory
    if (this->Coloring)
    {
        int idx = outCD->AddArray(newScalars);
        outCD->SetActiveAttribute(idx, vtkDataSetAttributes::SCALARS);
        newScalars->Delete();
    }
    if (PassThroughIds)
    {
        newIDArr->SetName("OriIds");
        outCD->AddArray(newIDArr);
        newIDArr->Delete();
    }
    return 1;
}

void FITKShellFeatureEdges::CreateDefaultLocator()
{
    if (this->Locator == nullptr)
    {
        this->Locator = vtkMergePoints::New();
    }
}

vtkMTimeType FITKShellFeatureEdges::GetMTime()
{
    vtkMTimeType mTime = this->Superclass::GetMTime();
    vtkMTimeType time;

    if (this->Locator != nullptr)
    {
        time = this->Locator->GetMTime();
        mTime = (time > mTime ? time : mTime);
    }
    return mTime;
}

const std::vector< int > FITKShellFeatureEdges::getLinesByPt(int ptID) const
{
    if (_ptLines.find(ptID) != _ptLines.end())
    {
        return _ptLines.at(ptID);
    }
    else
    {
        return _nullVec;
    }

}
const std::vector< int > FITKShellFeatureEdges::getEdgeCells(int iEdgeCellID, bool* b) const
{
    if (_edgeSurCell.find(iEdgeCellID) == _edgeSurCell.end())
    {
        if (b != nullptr) *b = false;
        std::vector< int > rtV;
        return rtV;
    }
    else
    {
        if (b != nullptr) *b = true;
        return _edgeSurCell.at(iEdgeCellID);
    }
}

const QVector< int > FITKShellFeatureEdges::getSurCellEdge(int iCell, bool* b) const
{
    if (_surEdgeIDMap.find(iCell) != _surEdgeIDMap.end())
    {
        if (b != nullptr) *b = true;
        return _surEdgeIDMap.value(iCell);
    }
    QVector< int > rt;
    if (b != nullptr) *b = false;
    return rt;
}

int FITKShellFeatureEdges::getSurfacePointIDByEdgePointID(int edgePointID)
{
    return _edgePtSurfPtMap.value(edgePointID, -1);
}

int FITKShellFeatureEdges::getEdgePointIDBySurfacePointID(int surfPointID)
{
    return _surfPtEdgePtMap.value(surfPointID, -1);
}

//int FITKShellFeatureEdges::getEdgeIndex(int edgeCellId, int& surfaceCellId)
//{
//    edgeCellId = -1;
//    surfaceCellId = -1;
//
//    if (!m_edgeSurfaceHash.contains(edgeCellId))
//    {
//        return -1;
//    }
//
//    QPair<int, int> & pair = m_edgeSurfaceHash[edgeCellId];
//    surfaceCellId = pair.first;
//    return pair.second;
//}

int FITKShellFeatureEdges::RequestUpdateExtent(
    vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector,
    vtkInformationVector* outputVector)
{
    // get the info objects
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    int numPieces, ghostLevel;

    numPieces =
        outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES());
    ghostLevel =
        outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS());

    if (numPieces > 1)
    {
        inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(),
            ghostLevel + 1);
    }

    return 1;
}

void FITKShellFeatureEdges::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Feature Angle: " << this->FeatureAngle << "\n";
    os << indent << "Boundary Edges: " << (this->BoundaryEdges ? "On\n" : "Off\n");
    os << indent << "Feature Edges: " << (this->FeatureEdges ? "On\n" : "Off\n");
    os << indent << "Non-Manifold Edges: " << (this->NonManifoldEdges ? "On\n" : "Off\n");
    os << indent << "Manifold Edges: " << (this->ManifoldEdges ? "On\n" : "Off\n");
    os << indent << "Coloring: " << (this->Coloring ? "On\n" : "Off\n");

    if (this->Locator)
    {
        os << indent << "Locator: " << this->Locator << "\n";
    }
    else
    {
        os << indent << "Locator: (none)\n";
    }

    os << indent << "Output Points Precision: " << this->OutputPointsPrecision << "\n";
}

/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectModelBase.h"

// VTK
#include <vtkObject.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include <vtkCellType.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPolygon.h>

// Graph 
#include "FITKFluidVTKCommons.h"
#include "FITKFluidVTKShapeColorMap.h"
#include "FITKFluidVTKGraphObjectSelect.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshVS.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshEntity.h"

namespace Exchange
{
    FITKFluidVTKGraphObjectModelBase::FITKFluidVTKGraphObjectModelBase(Core::FITKAbstractDataObject* dataObj)
        : FITKFluidVTKGraphObject3D(dataObj)
    {
        // Initialize the polygon offset.
        m_polygonOffset = FITKFluidVTKCommons::s_polygonOffsetGeom;

        // Save the data type.
        m_shapeInfo.Type = FITKFluidVTKCommons::ModelShape;

        // Set the layer need to be rendered.
        setRenderLayer(0, 2);
    }

    bool FITKFluidVTKGraphObjectModelBase::test(QVariant& details)
    {
        if (!details.isValid())
        {
            return false;
        }

        return true;
    }

    FITKFluidVTKGraphObjectModelBase::~FITKFluidVTKGraphObjectModelBase()
    {
        // Delete the model data.
        deleteVtkObj(m_gridVertex);
        deleteVtkObj(m_gridEdge);
        deleteVtkObj(m_gridFace);

        // Delete face normals.
        deleteVtkObj(m_normalFacePoint);

        // Clear the maps.
        m_solidFaceIdsHash.clear();
        m_faceSolidIdMap.clear();
        m_cellIdVertexIdMap.clear();
        m_cellIdEdgeIdMap.clear();
        m_cellIdFaceIdMap.clear();
        m_vertexIdCellIdMap.clear();
        m_edgeIdCellIdMap.clear();

        // Delete color maps.
        deleteObj(m_colorMapVertex);
        deleteObj(m_colorMapEdge);
        deleteObj(m_colorMapFace);

        // Set the pointer to NULL.
        m_triData = nullptr;
        m_topoMgr = nullptr;
    }

    void FITKFluidVTKGraphObjectModelBase::init()
    {
        // Create model grid data.
        //@{
        m_gridVertex = vtkUnstructuredGrid::New();
        m_gridVertex->SetPoints(vtkSmartPointer<vtkPoints>::New());
        m_gridEdge = vtkUnstructuredGrid::New();
        m_gridEdge->SetPoints(vtkSmartPointer<vtkPoints>::New());
        m_gridFace = vtkPolyData::New();
        m_gridFace->SetPoints(vtkSmartPointer<vtkPoints>::New());
        m_gridFace->SetPolys(vtkSmartPointer<vtkCellArray>::New());

        // Add the points normals for face grid.
        m_normalFacePoint = vtkDoubleArray::New();
        m_normalFacePoint->SetNumberOfComponents(3);
        m_gridFace->GetPointData()->SetNormals(m_normalFacePoint);
        //@}

        // Create the highlight selector.
        m_highlightSelector = new FITKFluidVTKGraphObjectSelect;
        m_addinGraphObjList.push_back(m_highlightSelector);
    }
    
    bool FITKFluidVTKGraphObjectModelBase::meshVSToVtkDataSet()
    {
        // Check the input data.
        if (!m_triData || !m_gridVertex || !m_gridEdge || !m_gridFace)
        {
            return false;
        }         
        
        // Initialize the vertex grid.
        createVertexGrid();

        // Initialize the edge grid.
        createEdgeGrid();

        // Initialize the face grid.
        createFaceGrid();

        // Initialize the solid face ID map.
        initializeSolidFaceMap();

        return true;
    }

    void FITKFluidVTKGraphObjectModelBase::createVertexGrid()
    {
        // Check the data set and its points.
        if (!m_gridVertex || !m_triData)
        {
            return;
        }

        vtkPoints* points = m_gridVertex->GetPoints();
        if (!points)
        {
            return;
        }

        // Get all points.
        const QHash<int, Interface::FITKGeoMeshVSPt*> pointsHash = m_triData->getPointItems();
        QHashIterator<int, Interface::FITKGeoMeshVSPt*> iter(pointsHash);

        int ptIndex = -1;

        while (iter.hasNext())
        {
            iter.next();

            int meshPtId = iter.key();
            Interface::FITKGeoMeshVSPt* pt = iter.value();

            // Add point.
            ptIndex = points->InsertNextPoint(pt->x(), pt->y(), pt->z());

            // Add vertex cell.
            vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
            cell->InsertNextId(ptIndex);
            m_gridVertex->InsertNextCell(VTKCellType::VTK_VERTEX, cell);

            // Save the vertex id.
            m_cellIdVertexIdMap.push_back(meshPtId);
            m_vertexIdCellIdMap.insert(meshPtId, ptIndex);
        }
    }

    void FITKFluidVTKGraphObjectModelBase::createEdgeGrid()
    {
        // Check the data set and its points.
        if (!m_gridEdge || !m_triData)
        {
            return;
        }

        vtkPoints* points = m_gridEdge->GetPoints();
        if (!points)
        {
            return;
        }

        // Get all edges.
        const QHash<int, Interface::FITKGeoMeshVSEdgeEntity*> edgesHash = m_triData->getEdgeItems();
        QHashIterator<int, Interface::FITKGeoMeshVSEdgeEntity*> iter(edgesHash);

        int ptIndex = -1, cellIndex = -1;

        while (iter.hasNext())
        {
            iter.next();

            int meshEdgeId = iter.key();
            Interface::FITKGeoMeshVSEdgeEntity* edge = iter.value();

            // Get edge points.
            const QList<Interface::FITKGeoMeshVSPt*> & pts = edge->getPoints();

            // Get the node data and create poly line.
            vtkSmartPointer<vtkIdList> polyLineCell = vtkSmartPointer<vtkIdList>::New();

            for (Interface::FITKGeoMeshVSPt* pt : pts)
            {
                // Add point.
                ptIndex = points->InsertNextPoint(pt->x(), pt->y(), pt->z());
                polyLineCell->InsertNextId(ptIndex);
            }
      
            // Add cell.
            cellIndex = m_gridEdge->InsertNextCell(VTKCellType::VTK_POLY_LINE, polyLineCell);

            // Save the edge id.
            m_cellIdEdgeIdMap.push_back(meshEdgeId);
            m_edgeIdCellIdMap.insert(meshEdgeId, cellIndex);
        }
    }

    void FITKFluidVTKGraphObjectModelBase::createFaceGrid()
    {
        // Check the data set and its points.
        if (!m_gridFace || !m_triData)
        {
            return;
        }

        vtkPoints* points = m_gridFace->GetPoints();
        if (!points)
        {
            return;
        }

        // Get all faces.
        const QHash<int, Interface::FITKGeoMeshVSFaceEntity*> facesHash = m_triData->getFaceItems();
        QHashIterator<int, Interface::FITKGeoMeshVSFaceEntity*> iter(facesHash);

        int cellIndex = -1;

        // For computing points nromals.
        //@{
        vtkDoubleArray* tempCellNormals = vtkDoubleArray::New();
        tempCellNormals->SetNumberOfComponents(3);
        vtkPoints* tempPts = vtkPoints::New();
        tempPts->SetNumberOfPoints(3);
        double tempNor3[3]{ 0., 0., 0. };
        //@}

        int ptIndexStart = 0;

        while (iter.hasNext())
        {
            iter.next();

            int meshFaceId = iter.key();
            Interface::FITKGeoMeshVSFaceEntity* face = iter.value();

            // Get all points of the face.
            const QList<Interface::FITKGeoMeshVSPt*> & facePoints = face->getPoints();
            for (Interface::FITKGeoMeshVSPt* facePoint : facePoints)
            {
                points->InsertNextPoint(facePoint->x(), facePoint->y(), facePoint->z());
            }

            // Get face triangles.
            const QList<Interface::FITKGeoMeshVSTri*> & tris = face->getTriangles();

            for (Interface::FITKGeoMeshVSTri* tri : tris)
            {
                // Get points indice pf the triangle.
                const int* nodes = tri->getNode();

                // Get points index in VTK grid of vertice
                int ptIndex1 = ptIndexStart + nodes[0];
                int ptIndex2 = ptIndexStart + nodes[1];
                int ptIndex3 = ptIndexStart + nodes[2];

                // Add temp points for calculating normals.
                tempPts->SetPoint(0, points->GetPoint(ptIndex1));
                tempPts->SetPoint(1, points->GetPoint(ptIndex2));
                tempPts->SetPoint(2, points->GetPoint(ptIndex3));

                // Add cell.
                vtkSmartPointer<vtkIdList> triCell = vtkSmartPointer<vtkIdList>::New();
                triCell->InsertNextId(ptIndex1);
                triCell->InsertNextId(ptIndex2);
                triCell->InsertNextId(ptIndex3);
                cellIndex = m_gridFace->InsertNextCell(VTKCellType::VTK_TRIANGLE, triCell);

                // Add cell normals.
                vtkPolygon::ComputeNormal(tempPts, tempNor3);
                tempCellNormals->InsertNextTuple3(tempNor3[0], tempNor3[1], tempNor3[2]);

                // Save the face id.
                m_cellIdFaceIdMap.push_back(meshFaceId);
                m_faceCellIdsHash[meshFaceId].push_back(cellIndex);
            }

            // Update the point start index.
            ptIndexStart += facePoints.count();
        }

        // Calculate points normals for the face grid.
        m_normalFacePoint->SetNumberOfTuples(m_gridFace->GetNumberOfPoints());
        m_normalFacePoint->FillComponent(0, 0);
        m_normalFacePoint->FillComponent(1, 0);
        m_normalFacePoint->FillComponent(2, 0);

        int cellId = 0;
        vtkIdType npts = 0;
#if VTK_MAJOR_VERSION < 9
        vtkIdType* pts = nullptr;
#else
        const vtkIdType* pts = nullptr;
#endif

        // Add two double array.
        auto addDouble3 = [&](double* v1, double* v2, double* result)
        {
            for (int i = 0; i < 3; i++)
            {
                result[i] = v1[i] + v2[i];
            }
        };

        double result[3]{ 0., 0., 0. };
        for (cellId = 0, m_gridFace->GetPolys()->InitTraversal(); m_gridFace->GetPolys()->GetNextCell(npts, pts); ++cellId)
        {
            double* norCellCurrent = tempCellNormals->GetTuple3(cellId);

            for (vtkIdType i = 0; i < npts; ++i)
            {
                double* norPtCurrent = m_normalFacePoint->GetTuple3(pts[i]);

                // Add normals.
                addDouble3(norPtCurrent, norCellCurrent, result);
                m_normalFacePoint->SetTuple3(pts[i], result[0], result[1], result[2]);
            }
        }

        double flipDirection = 1.0;
        for (vtkIdType i = 0; i < m_gridFace->GetNumberOfPoints(); ++i)
        {
            double* ptNormal = m_normalFacePoint->GetTuple3(i);
            const double length = sqrt(pow(ptNormal[0], 2) + pow(ptNormal[1], 2) + pow(ptNormal[2], 2)) * flipDirection;
            if (!qFuzzyCompare(length, 0.))
            {
                ptNormal[0] /= length;
                ptNormal[1] /= length;
                ptNormal[2] /= length;
            }

            m_normalFacePoint->SetTuple3(i, ptNormal[0], ptNormal[1], ptNormal[2]);
        }

        // Delete temp data.
        tempCellNormals->Delete();
        tempPts->Delete();

        // Initialize the face-solid hash array length.
        // m_faceSolidIdMap.resize(iCell + 1);
    }

    void FITKFluidVTKGraphObjectModelBase::initializeSolidFaceMap()
    {
        // Check the model topo-mamager.
        if (!m_topoMgr)
        {
            return;
        }

        // Get the solid manager.
        Interface::FITKShapeVirtualTopoManager* tpSolidMgr = m_topoMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        if (!tpSolidMgr)
        {
            return;
        }

        // Get all solid of the model.
        int nSolids = tpSolidMgr->getDataCount();
        for (int i = 0; i < nSolids; i++)
        {
            Interface::FITKAbsVirtualTopo* vSolid = tpSolidMgr->getDataByIndex(i);
            int solidId = vSolid->getDataObjectID();

            // Get all sub-shapes of the solid.( faces )
            int nSubShape = vSolid->getSubTopoCount();
            for (int j = 0; j < nSubShape; j++)
            {
                Interface::FITKAbsVirtualTopo* vFace = vSolid->getSubTopo(j);
                int faceId = vFace->getDataObjectID();

                // Save the ids.
                m_solidFaceIdsHash[solidId].push_back(faceId);

                // Insert the face-solid id.
                m_faceSolidIdMap[faceId] = solidId;
            }
        }
    }

    void FITKFluidVTKGraphObjectModelBase::setVisible(bool visibility)
    {
        // Show or hide face.
        if (m_fActorFace)
        {
            m_fActorFace->SetVisibility(m_viewModes[FITKFluidVTKCommons::SMVM_Shade] && visibility);
        }

        // Show or hide edge.
        if (m_fActorEdge)
        {
            m_fActorEdge->SetVisibility(m_viewModes[FITKFluidVTKCommons::SMVM_Wireframe] && visibility);
        }

        // Show or hide vertex.
        if (m_fActorVertex)
        {
            m_fActorVertex->SetVisibility(m_viewModes[FITKFluidVTKCommons::SMVM_Vertex] && visibility);
        }

        // The highlightor.
        if (m_highlightSelector)
        {
            m_highlightSelector->setVisible(visibility && getDataVisibility() && (m_isHighlighting || m_isAdvHighlighting));
        }
    }

    void FITKFluidVTKGraphObjectModelBase::setViewMode(FITKFluidVTKCommons::ShapeMeshViewMode type, bool visible)
    {
        // Get the shape mesh actor by the given type.
        FITKGraphActor* fActor{ nullptr };

        switch (type)
        {
        case FITKFluidVTKCommons::SMVM_Shade:
            fActor = m_fActorFace;
            break;
        case FITKFluidVTKCommons::SMVM_Wireframe:
            fActor = m_fActorEdge;
            break;
        case FITKFluidVTKCommons::SMVM_Vertex:
            fActor = m_fActorVertex;
            break;
        default:
            return;
        }

        if (!fActor)
        {
            return;
        }

        // Set the visibility for the actor.
        fActor->SetVisibility(visible);

        // Save flag.
        FITKFluidVTKGraphObject3D::setViewMode(type, visible);
    }

    void FITKFluidVTKGraphObjectModelBase::setTransparent(bool isOn)
    {
        // Get the opacity.
        double opa = isOn ? 1. - FITKFluidVTKCommons::s_transparency : 1.;

        if (m_fActorFace)
        {
            m_fActorFace->GetProperty()->SetOpacity(opa);
        }

        if (m_fActorEdge)
        {
            m_fActorEdge->GetProperty()->SetOpacity(opa);
        }

        if (m_fActorVertex)
        {
            m_fActorVertex->GetProperty()->SetOpacity(opa);
        }
    }

    void FITKFluidVTKGraphObjectModelBase::setColor(QColor color, FITKFluidVTKCommons::ShapeType type, int index)
    {
        switch (type)
        {
        case FITKFluidVTKCommons::ModelVertex:
        {
            // Get the vertex cell ( vertex  cell ) id by the given vertex id.
            int cellId = -1;
            if (!m_vertexIdCellIdMap.contains(index) || !m_colorMapVertex)
            {
                return;
            }

            cellId = m_vertexIdCellIdMap[index];
            m_colorMapVertex->setCellsColor(c_defaultArrName, { cellId }, color);
            break;
        }
        case FITKFluidVTKCommons::ModelEdge:
        {
            // Get the edge cell ( poly-line cell ) id by the given edge id.
            int cellId = -1;
            if (!m_edgeIdCellIdMap.contains(index) || !m_colorMapEdge)
            {
                return;
            }

            cellId = m_edgeIdCellIdMap[index];
            m_colorMapEdge->setCellsColor(c_defaultArrName, { cellId }, color);
            break;
        }
        case FITKFluidVTKCommons::ModelFace:
        {
            // Get the face cell ( triangle cell ) ids by the given face id.
            if (!m_faceCellIdsHash.contains(index) || !m_colorMapFace)
            {
                return;
            }

            const QVector<int> & cellIds = m_faceCellIdsHash[index];
            m_colorMapFace->setCellsColor(c_defaultArrName, cellIds.toList(), color);
            break;
        }
        case FITKFluidVTKCommons::ModelSolid:
        {
            // Get the solid face ids by the given solid id.
            if (!m_solidFaceIdsHash.contains(index) || !m_colorMapFace)
            {
                return;
            }

            const QVector<int> & faceIds = m_solidFaceIdsHash[index];

            // Get the face cell ids.
            QVector<int> cellIds;
            for (const int & faceId : faceIds)
            {
                cellIds << m_faceCellIdsHash[faceId];
            }

            m_colorMapFace->setCellsColor(c_defaultArrName, cellIds.toList(), color);
            break;
        }
        default:
            return;
        }
    }

    void FITKFluidVTKGraphObjectModelBase::highlight(FITKFluidVTKCommons::ShapeType type, QColor color)
    {
        // Show the actor and initialize the data.
        if (!m_highlightSelector)
        {
            return;
        }

        // Default highlight all faces.
        if (type == FITKFluidVTKCommons::ShapeType::ShapeTypeNone)
        {
            type = FITKFluidVTKCommons::ShapeType::ModelFace;
        }

        vtkDataSet* meshDataSet = getMesh(type);
        if (!meshDataSet)
        {
            return;
        }

        QVector<int> indice;
        for (int i = 0; i < meshDataSet->GetNumberOfCells(); i++)
        {
            indice.push_back(i);
        }

        // Check the color.
        if (!color.isValid())
        {
            color = Qt::red;
        }

        m_highlightSelector->setColor(color);
        m_highlightSelector->setSelectData(meshDataSet, indice, type);
        m_highlightSelector->setVisible(getDataVisibility());

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::highlight();
    }

    void FITKFluidVTKGraphObjectModelBase::disHighlight()
    {
        // Hide the actor only.
        if (m_highlightSelector)
        {
            m_highlightSelector->setVisible(false);
        }

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::disHighlight();
    }

    void FITKFluidVTKGraphObjectModelBase::advanceHighlight(FITKFluidVTKCommons::ShapeType type, QVector<int> indice, QColor color)
    {
        // Show the actor and initialize the data.
        if (!m_highlightSelector)
        {
            return;
        }

        if (indice.isEmpty())
        {
            return;
        }

        vtkDataSet* meshDataSet = getMesh(type);
        if (!meshDataSet)
        {
            return;
        }

        // Get the model shape type.
        FITKFluidVTKCommons::ShapeAbsEnum shapeEnum;
        switch (type)
        {
        case FITKFluidVTKCommons::ModelVertex:
            shapeEnum = FITKFluidVTKCommons::ShapeAbsEnum::STA_VERTEX;
            break;
        case FITKFluidVTKCommons::ModelEdge:
            shapeEnum = FITKFluidVTKCommons::ShapeAbsEnum::STA_EDGE;
            break;
        case FITKFluidVTKCommons::ModelFace:
            shapeEnum = FITKFluidVTKCommons::ShapeAbsEnum::STA_FACE;
            break;
        case FITKFluidVTKCommons::ModelSolid:
            shapeEnum = FITKFluidVTKCommons::ShapeAbsEnum::STA_SOLID;
            break;
        case FITKFluidVTKCommons::ShapeTypeNone:
        case FITKFluidVTKCommons::MeshNode:
        case FITKFluidVTKCommons::MeshElement:
        default:
            return;
        }

        // Get the cell ids in VTK.
        int nCells = meshDataSet->GetNumberOfCells();
        QVector<int> cellFlags;
        cellFlags.resize(nCells);
        cellFlags.fill(0);

        // Get VTK ids by Shape ids.
        for (const int & index : indice)
        {
            QVector<int> vtkIds = getVTKCellIdsByShapeId(index, shapeEnum);

            for (const int & vtkId : vtkIds)
            {
                cellFlags[vtkId] = 1;
            }
        }

        QVector<int> vtkIndice;
        for (int i = 0; i < cellFlags.count(); i++)
        {
            if (cellFlags[i])
            {
                vtkIndice.push_back(i);
            }
        }

        // Check the color.
        if (!color.isValid())
        {
            color = Qt::red;
        }

        m_highlightSelector->setColor(color);
        m_highlightSelector->setSelectData(meshDataSet, vtkIndice, type);
        m_highlightSelector->setVisible(getDataVisibility());

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::advanceHighlight(type, indice, color);
    }

    void FITKFluidVTKGraphObjectModelBase::disAdvanceHighlight()
    {
        // Clear data and hide the actor.
        if (m_highlightSelector)
        {
            m_highlightSelector->clearData();
            m_highlightSelector->setVisible(false);
        }

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::disAdvanceHighlight();
    }

    int FITKFluidVTKGraphObjectModelBase::getNumberOf(FITKFluidVTKCommons::ShapeType type)
    {
        if (!m_triData)
        {
            return 0;
        }

        switch (type)
        {
        case FITKFluidVTKCommons::ModelVertex:
            return m_triData->getPointItems().count();
        case FITKFluidVTKCommons::ModelEdge:
            return m_triData->getEdgeItems().count();
        case FITKFluidVTKCommons::ModelFace:
            return m_triData->getFaceItems().count();
        case FITKFluidVTKCommons::ModelSolid:
            return 0;//m_triData->
        case FITKFluidVTKCommons::ShapeTypeNone:
        case FITKFluidVTKCommons::MeshNode:
        case FITKFluidVTKCommons::MeshElement:      
        default:
            return 0;
        }
    }

    void FITKFluidVTKGraphObjectModelBase::setPickMode(FITKFluidVTKCommons::ShapePickMode mode)
    {
        // Lambda for set actor pickable.
        auto setActorPickable = [=](FITKGraphActor* fActor, bool isOn)
        {
            if (fActor)
            {
                fActor->SetPickable(isOn);
            }
        };

        switch (mode)
        {
        case FITKFluidVTKCommons::PickNone:
            setActorPickable(m_fActorVertex, false);
            setActorPickable(m_fActorEdge, false);
            setActorPickable(m_fActorFace, false);
            break;
        case FITKFluidVTKCommons::PickVertex:
            setActorPickable(m_fActorVertex, true);
            setActorPickable(m_fActorEdge, false);
            setActorPickable(m_fActorFace, false);
            break;
        case FITKFluidVTKCommons::PickEdge:
            setActorPickable(m_fActorVertex, false);
            setActorPickable(m_fActorEdge, true);
            setActorPickable(m_fActorFace, false);
            break;
        case FITKFluidVTKCommons::PickFace:
        case FITKFluidVTKCommons::PickSolid:
            setActorPickable(m_fActorVertex, false);
            setActorPickable(m_fActorEdge, false);
            setActorPickable(m_fActorFace, true);
            break;
        case FITKFluidVTKCommons::PickAll:
            setActorPickable(m_fActorVertex, true);
            setActorPickable(m_fActorEdge, true);
            setActorPickable(m_fActorFace, true);
            break;
        default:
            return;
        }

        // Save the pick mode.
        FITKFluidVTKGraphObject3D::setPickMode(mode);
    }

    int FITKFluidVTKGraphObjectModelBase::getShapeIdByVTKCellId(int vtkCellId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType)
    {
        int id = -1;
        if (vtkCellId < 0)
        {
            return id;
        }

        switch (topAbsShapeType)
        {
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_SOLID:
        {
            // Try to get the face.
            if (m_cellIdFaceIdMap.count() <= vtkCellId)
            {
                break;
            }

            // Try to get the solid id.
            const int & faceId = m_cellIdFaceIdMap[vtkCellId];

            if (m_faceSolidIdMap.count() <= faceId)
            {
                break;
            }

            id = m_faceSolidIdMap[faceId];

            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_FACE:
        {
            // Try to get the face.
            if (m_cellIdFaceIdMap.count() <= vtkCellId)
            {
                break;
            }

            // Get the face id.
            id = m_cellIdFaceIdMap[vtkCellId];

            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_EDGE:
        {
            // Try to get the edge.
            if (m_cellIdEdgeIdMap.count() <= vtkCellId)
            {
                break;
            }

            // Get the edge id.
            id = m_cellIdEdgeIdMap[vtkCellId];

            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_VERTEX:
        {
            // Try to get the vertex.
            if (m_cellIdVertexIdMap.count() <= vtkCellId)
            {
                break;
            }

            // Get the vertex id.
            id = m_cellIdVertexIdMap[vtkCellId];

            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_COMPOUND:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_COMPSOLID:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_SHELL:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_WIRE:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_SHAPE:
        default:
            break;
        }

        return id;
    }

    const QVector<int> FITKFluidVTKGraphObjectModelBase::getVTKCellIdsByShapeId(int shapeId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType)
    {
        QVector<int> cellIds;

        switch (topAbsShapeType)
        {
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_SOLID:
        {
            // Try to get the solid's faces.
            if (!m_solidFaceIdsHash.contains(shapeId))
            {
                break;
            }

            // Try to get the faces' cells.
            const QVector<int> & faceIds = m_solidFaceIdsHash[shapeId];
            for (const int & fId : faceIds)
            {
                cellIds << getVTKCellIdsByShapeId(fId, FITKFluidVTKCommons::ShapeAbsEnum::STA_FACE);
            }

            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_FACE:
        {
            if (!m_faceCellIdsHash.contains(shapeId))
            {
                break;
            }

            // Get the face's cell ids.
            cellIds = m_faceCellIdsHash[shapeId];
            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_EDGE:
        {
            if (!m_edgeIdCellIdMap.contains(shapeId))
            {
                break;
            }

            // Get the edge's cell id.( Poly line cell )
            cellIds.push_back(m_edgeIdCellIdMap[shapeId]);
            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_VERTEX:
        {
            if (!m_vertexIdCellIdMap.contains(shapeId))
            {
                break;
            }

            // Get the vertex's cell id.( Vertex cell )
            cellIds.push_back(m_vertexIdCellIdMap[shapeId]);
            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_COMPOUND:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_COMPSOLID:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_SHELL:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_WIRE:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_SHAPE:
        default:
            break;
        }

        return cellIds;
    }

    const QVector<int> FITKFluidVTKGraphObjectModelBase::getVTKCellIdsByVTKCellId(int cellId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType)
    {
        QVector<int> cellIds;

        switch (topAbsShapeType)
        {
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_SOLID:
        {
            if (cellId >= m_cellIdFaceIdMap.count())
            {
                break;
            }

            // Get the cell's solid id, and the solid's faces.
            int faceId = m_cellIdFaceIdMap[cellId];
            int solidId = m_faceSolidIdMap[faceId];

            // Try to get the faces' cells.
            const QVector<int> & faceIds = m_solidFaceIdsHash[solidId];
            for (const int & fId : faceIds)
            {
                cellIds << getVTKCellIdsByShapeId(fId, FITKFluidVTKCommons::ShapeAbsEnum::STA_FACE);
            }

            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_FACE:
        {
            if (cellId >= m_cellIdFaceIdMap.count())
            {
                break;
            }

            // Get the face id.
            int faceId = m_cellIdFaceIdMap[cellId];
            cellIds = m_faceCellIdsHash[faceId];
            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_EDGE:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_VERTEX:
        {
            // The edge and vertex cell id is the given cell id.
            cellIds.push_back(cellId);
            break;
        }
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_COMPOUND:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_COMPSOLID:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_SHELL:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_WIRE:
        case FITKFluidVTKCommons::ShapeAbsEnum::STA_SHAPE:
        default:
            break;
        }

        return cellIds;
    }

    vtkDataSet* FITKFluidVTKGraphObjectModelBase::getMesh(FITKFluidVTKCommons::ShapeType type)
    {
        // Get the mesh data for extractor.
        switch (type)
        {
        case FITKFluidVTKCommons::ModelVertex:
            return m_gridVertex;
        case FITKFluidVTKCommons::ModelEdge:
            return m_gridEdge;
        case FITKFluidVTKCommons::ModelFace:
        case FITKFluidVTKCommons::ModelSolid:
            return m_gridFace;
        default:
            break;
        }

        return nullptr;
    }

    bool FITKFluidVTKGraphObjectModelBase::generateDataSet(Interface::FITKAbsGeoShapeAgent* shapeAgent)
    {
        if (!shapeAgent)
        {
            return false;
        }

        // Trianglate.
        // shapeAgent->triangulation();

        // Get the trianglation of the shape.
        Interface::FITKGeometryMeshVS* triData = shapeAgent->getMeshVS();
        if (!triData)
        {
            return false;
        }

        // Save the trianglation data and the topo-manager.
        m_triData = triData;
        m_topoMgr = shapeAgent->getVirtualTopoManager();

        // Save the hash code of the shape.
        QVariant newState;
        shapeAgent->createShapeState(newState);
        m_shapeState = newState;

        bool flag = meshVSToVtkDataSet();
        return flag;
    }

    void FITKFluidVTKGraphObjectModelBase::generateGraph()
    {
        if (!m_gridVertex || !m_gridEdge || !m_gridFace)
        {
            return;
        }

        // Create vertex actor.
        m_fActorVertex = FITKGraphActor::New();
        m_fActorVertex->setGraphObject(this);
        m_fActorVertex->setActorType(ActorType::VertexActor);
        m_fActorVertex->SetPickable(false);
        m_fActorVertex->SetVisibility(true);
        m_fActorVertex->GetProperty()->SetPointSize(5);
        m_fActorVertex->GetProperty()->SetRepresentation(2);
        m_fActorVertex->setInputDataObject(m_gridVertex);
        m_fActorVertex->setRelativeCoincidentTopologyPolygonOffsetParameters(m_polygonOffset);
        addActor(m_fActorVertex);

        // Create edge actor.
        m_fActorEdge = FITKGraphActor::New();
        m_fActorEdge->setGraphObject(this);
        m_fActorEdge->setActorType(ActorType::EdgeActor);
        m_fActorEdge->SetPickable(false);
        m_fActorEdge->SetVisibility(true);
        m_fActorEdge->GetProperty()->SetLineWidth(2);
        m_fActorEdge->GetProperty()->SetRepresentation(1);
        m_fActorEdge->setInputDataObject(m_gridEdge);
        m_fActorEdge->setRelativeCoincidentTopologyPolygonOffsetParameters(m_polygonOffset);
        addActor(m_fActorEdge);

        // Create face actor.
        m_fActorFace = FITKGraphActor::New();
        m_fActorFace->setGraphObject(this);
        m_fActorFace->setActorType(ActorType::SurfaceActor);
        m_fActorFace->SetPickable(false);
        m_fActorFace->SetVisibility(true);
        m_fActorFace->GetProperty()->SetRepresentation(2);
        m_fActorFace->GetProperty()->SetEdgeVisibility(false);
        m_fActorFace->GetProperty()->SetLineWidth(0.);
        m_fActorFace->setInputDataObject(m_gridFace);
        m_fActorFace->setRelativeCoincidentTopologyPolygonOffsetParameters(m_polygonOffset);
        addActor(m_fActorFace);

        // Set the colors.
        //@{
        m_colorMapVertex = new FITKFluidVTKShapeColorMap;
        m_colorMapEdge = new FITKFluidVTKShapeColorMap;
        m_colorMapFace = new FITKFluidVTKShapeColorMap;

        m_colorMapVertex->setInputData(m_gridVertex);
        m_colorMapEdge->setInputData(m_gridEdge);
        m_colorMapFace->setInputData(m_gridFace);

        m_colorMapVertex->setColor(c_defaultArrName, FITKFluidVTKCommons::s_geoVertexColor);
        m_colorMapEdge->setColor(c_defaultArrName, FITKFluidVTKCommons::s_geoEdgeColor);
        m_colorMapFace->setColor(c_defaultArrName, FITKFluidVTKCommons::s_geoFaceColor);
        //@}

        // Enable color arrays.
        //@{
        m_fActorVertex->setScalarVisibility(true);
        m_fActorEdge->setScalarVisibility(true);
        m_fActorFace->setScalarVisibility(true);

        m_fActorVertex->setScalarModeToUseCellFieldData();
        m_fActorEdge->setScalarModeToUseCellFieldData();
        m_fActorFace->setScalarModeToUseCellFieldData();

        m_colorMapVertex->update();
        m_colorMapEdge->update();
        m_colorMapFace->update();

        m_fActorVertex->selectScalarArray(c_defaultArrName);
        m_fActorEdge->selectScalarArray(c_defaultArrName);
        m_fActorFace->selectScalarArray(c_defaultArrName);
        //@}
    }

    void FITKFluidVTKGraphObjectModelBase::clearData()
    {
        // Reset all data set.
        resetVtkObj(m_gridVertex);
        resetVtkObj(m_gridEdge);
        resetVtkObj(m_gridFace);

        resetVtkObj(m_gridVertex->GetPoints());
        resetVtkObj(m_gridEdge->GetPoints());
        resetVtkObj(m_gridFace->GetPoints());

        // Clear normals.
        resetVtkObj(m_normalFacePoint);

        // Clear the maps.
        m_solidFaceIdsHash.clear();
        m_faceSolidIdMap.clear();
        m_cellIdVertexIdMap.clear();
        m_cellIdEdgeIdMap.clear();
        m_cellIdFaceIdMap.clear();
        m_vertexIdCellIdMap.clear();
        m_edgeIdCellIdMap.clear();

        // Clear color maps.
        m_colorMapVertex->clear();
        m_colorMapEdge->clear();
        m_colorMapFace->clear();

        // Clear the selector.
        if (m_highlightSelector)
        {
            m_highlightSelector->clearData();
        }
    }
}   // namespace Exchange
/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectRegionMesh.h"

// VTK
#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCubeSource.h>
// #include <vtkCylinderSource.h>
#include <vtkLineSource.h>
#include <vtkTubeFilter.h>
#include <vtkSphereSource.h>
#include <vtkDataSetMapper.h>

// Graph and filter
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITKFluidVTKCommons.h"
#include "FITKFluidVTKGraphObjectSelect.h"

// Data
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeBox.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeCylinder.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeSphere.h"

namespace Exchange
{
    FITKFluidVTKGraphObjectRegionMesh::FITKFluidVTKGraphObjectRegionMesh(Interface::FITKAbstractRegionMeshSize* meshData)
        : FITKFluidVTKGraphObject3D(meshData)
    {
        if (!meshData)
        {
            return;
        }

        // Initialize.
        init();

        // Generate the region mesh's geometry.
        bool flag = generateDataSet();
        if (!flag)
        {
            return;
        }

        // Generate the mesh actor.
        generateGraph();

        // Set the layer need to be rendered.
        setRenderLayer(1, 2);

        // Save the data type.
        m_shapeInfo.Type = FITKFluidVTKCommons::MeshShape;
    }

    FITKFluidVTKGraphObjectRegionMesh::~FITKFluidVTKGraphObjectRegionMesh()
    {
        // Delete pointers.
        deleteVtkObj(m_ugrid);
    }

    void FITKFluidVTKGraphObjectRegionMesh::init()
    {
        // Create the grid data.
        m_ugrid = vtkUnstructuredGrid::New();
        m_ugrid->SetPoints(vtkSmartPointer<vtkPoints>::New());

        // Create the highlight selector.
        m_highlightSelector = new FITKFluidVTKGraphObjectSelect;
        m_highlightSelector->setTransparent(true);
        m_addinGraphObjList.push_back(m_highlightSelector);
    }

    void FITKFluidVTKGraphObjectRegionMesh::setVisible(bool visibility)
    {
        if (m_fActor)
        {
            m_fActor->SetVisibility(visibility);
        }
    }

    void FITKFluidVTKGraphObjectRegionMesh::setColor(QColor color)
    {
        // Set the actor color.
        if (m_fActor && color.isValid())
        {
            double color3[3]{ 0., 0., 0. };
            FITKFluidVTKCommons::QColorToDouble3(color, color3);
            m_fActor->GetProperty()->SetColor(color3);
        }
    }

    void FITKFluidVTKGraphObjectRegionMesh::update(bool forceUpdate)
    {
        // This graph object must be update forcly. 
        Q_UNUSED(forceUpdate);

        // Clear data first.
        clearData();

        // Regenerate the mesh geometry.
        generateDataSet();

        // Update visibility.
        updateVisibility();
    }

    void FITKFluidVTKGraphObjectRegionMesh::advanceHighlight(FITKFluidVTKCommons::ShapeType type, QVector<int> indice, QColor color)
    {
        if (indice.count() != 1)
        {
            return;
        }

        Interface::FITKAbstractRegionMeshSize* meshData = dynamic_cast<Interface::FITKAbstractRegionMeshSize*>(_dataObj);
        if (!meshData || !m_ugrid)
        {
            return;
        }

        // Get the region shape type.
        Interface::FITKAbstractRegionMeshSize::RegionType rType = meshData->getRegionType();

        // Check the face index.
        int faceIndex = indice[0];
        int nCells = m_ugrid->GetNumberOfCells();

        QVector<int> vtkIndice;

        switch (rType)
        {
        case Interface::FITKAbstractRegionMeshSize::RegionBox:
        {
            // 0: X-, 1: X+, 2: Y-, 3: Y+, 4: Z-, 5: Z+
            if (faceIndex > 5)
            {
                return;
            }

            // The face index of the region is the same as vtkCubeSource's face index.
            vtkIndice.push_back(faceIndex);

            break;
        }
        case Interface::FITKAbstractRegionMeshSize::RegionCylinder:
        {
            // 0: First disk, 1: Second disk, 2: Cylinder
            if (faceIndex > 2)
            {
                return;
            }

            // The indice of vtk cylinder:
            // 0 -> n: Cylinder, n + 1: First disk, n + 2: Second disk.
            if (faceIndex == 0)
            {
                // Bottom.
                vtkIndice.push_back(nCells - 2);
            }
            else if (faceIndex == 1)
            {
                // Top.
                vtkIndice.push_back(nCells - 1);
            }
            else if (faceIndex == 2)
            {
                // Wall.
                for (int i = 0; i < nCells - 2; i++)
                {
                    vtkIndice.push_back(i);
                }
            }
            
            break;
        }
        case Interface::FITKAbstractRegionMeshSize::RegionSphere:
        {
            // 0: Full sphere
            if (faceIndex > 1)
            {
                return;
            }

            // The full sphere indice.
            for (int i = 0; i < nCells ; i++)
            {
                vtkIndice.push_back(i);
            }

            break;
        }
        case Interface::FITKAbstractRegionMeshSize::RegionNone:
        case Interface::FITKAbstractRegionMeshSize::RigonFromFile:
        case Interface::FITKAbstractRegionMeshSize::RegionUserDef1:
        case Interface::FITKAbstractRegionMeshSize::RegionUserDef2:
        case Interface::FITKAbstractRegionMeshSize::RegionUserDef3:
        case Interface::FITKAbstractRegionMeshSize::RegionUserDef4:
        case Interface::FITKAbstractRegionMeshSize::RegionUserDef5:
        default:
            return;
        }

        m_highlightSelector->setColor(color);
        m_highlightSelector->setSelectData(m_ugrid, vtkIndice, FITKFluidVTKCommons::ShapeType::Others);
        m_highlightSelector->setVisible(getDataVisibility());

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::advanceHighlight(type, indice, color);
    }

    void FITKFluidVTKGraphObjectRegionMesh::disAdvanceHighlight()
    {
        if (m_highlightSelector)
        {
            m_highlightSelector->setVisible(false);
        }

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::disAdvanceHighlight();
    }

    void FITKFluidVTKGraphObjectRegionMesh::clearData()
    {
        // Reset the mesh geometry grid data.
        if (m_ugrid)
        {
            resetVtkObj(m_ugrid);
            resetVtkObj(m_ugrid->GetPoints());
        }
    }

    bool FITKFluidVTKGraphObjectRegionMesh::generateDataSet()
    {
        Interface::FITKAbstractRegionMeshSize* meshData = dynamic_cast<Interface::FITKAbstractRegionMeshSize*>(_dataObj);
        if (!meshData || !m_ugrid)
        {
            return false;
        }

        // Get the region shape type.
        Interface::FITKAbstractRegionMeshSize::RegionType rType = meshData->getRegionType();

        switch (rType)
        {
        case Interface::FITKAbstractRegionMeshSize::RegionBox:
        {
            Interface::FITKRegionMeshSizeBox* boxMS = dynamic_cast<Interface::FITKRegionMeshSizeBox*>(meshData);
            if (!boxMS)
            {
                return false;
            }

            // Get box param.
            double pointMin[3]{ 0., 0., 0. };
            double length[3]{ 0., 0., 0. };
            boxMS->getPoint1(pointMin);
            boxMS->getLength(length);

            // Get the center of the box.
            double center[3]{ 0., 0., 0. };
            for (int i = 0; i < 3; i++)
            {
                center[i] = pointMin[i] + length[i] / 2.;
            }

            // Create the box.
            vtkSmartPointer<vtkCubeSource> box = vtkSmartPointer<vtkCubeSource>::New();
            box->SetCenter(center);
            box->SetXLength(length[0]);
            box->SetYLength(length[1]);
            box->SetZLength(length[2]);
            box->Update();

            // Copy data.
            m_ugrid->DeepCopy(box->GetOutput());

            break;
        }
        case Interface::FITKAbstractRegionMeshSize::RegionCylinder:
        {
            Interface::FITKRegionMeshSizeCylinder* cylinderMS = dynamic_cast<Interface::FITKRegionMeshSizeCylinder*>(meshData);
            if (!cylinderMS)
            {
                return false;
            }

            // Get cylinder param.
            double pointMin[3]{ 0., 0., 0. };
            double dir[3]{ 0., 0., 0. };
            cylinderMS->getLocation(pointMin);
            cylinderMS->getDirection(dir);
            double radius = cylinderMS->getRadius();
            double length = cylinderMS->getLength();

            // Get the second point of the cylinder( point max ).
            double pointMax[3]{ 0., 0., 0. };
            for (int i = 0; i < 3; i++)
            {
                pointMax[i] = pointMin[i] + dir[i] * length;
            }

            // Create the line tube( cylinder ).
            vtkSmartPointer<vtkLineSource> lineAxes = vtkSmartPointer<vtkLineSource>::New();
            lineAxes->SetPoint1(pointMin);
            lineAxes->SetPoint2(pointMax);

            vtkSmartPointer<vtkTubeFilter> tubeCylinder = vtkSmartPointer<vtkTubeFilter>::New();
            tubeCylinder->SetInputConnection(lineAxes->GetOutputPort());
            tubeCylinder->SetRadius(radius);
            tubeCylinder->SetCapping(true);
            tubeCylinder->SetNumberOfSides(360);
            tubeCylinder->Update();

            // Copy data.
            m_ugrid->DeepCopy(tubeCylinder->GetOutput());

            break;
        }
        case Interface::FITKAbstractRegionMeshSize::RegionSphere:
        {
            Interface::FITKRegionMeshSizeSphere* sphereMS = dynamic_cast<Interface::FITKRegionMeshSizeSphere*>(meshData);
            if (!sphereMS)
            {
                return false;
            }

            // Get sphere param.
            double center[3]{ 0., 0., 0. };
            sphereMS->getLocation(center);
            double radius = sphereMS->getRadius();

            // Create the sphere.
            vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
            sphere->SetThetaResolution(90);
            sphere->SetPhiResolution(90);
            sphere->SetCenter(center);
            sphere->SetRadius(radius);
            sphere->Update();

            // Copy data.
            m_ugrid->DeepCopy(sphere->GetOutput());

            break;
        }
        case Interface::FITKAbstractRegionMeshSize::RegionNone:
        case Interface::FITKAbstractRegionMeshSize::RigonFromFile:
        case Interface::FITKAbstractRegionMeshSize::RegionUserDef1:
        case Interface::FITKAbstractRegionMeshSize::RegionUserDef2:
        case Interface::FITKAbstractRegionMeshSize::RegionUserDef3:
        case Interface::FITKAbstractRegionMeshSize::RegionUserDef4:
        case Interface::FITKAbstractRegionMeshSize::RegionUserDef5:
        default:
            return false;
        }

        return true;
    }

    void FITKFluidVTKGraphObjectRegionMesh::generateGraph()
    {
        if (!m_ugrid)
        {
            return;
        }

        // Get color.
        double colorFace3[3]{ 0., 0., 0. };
        FITKFluidVTKCommons::QColorToDouble3(FITKFluidVTKCommons::s_regionMeshFaceColor, colorFace3);

        double colorEdge3[3]{ 0., 0., 0. };
        FITKFluidVTKCommons::QColorToDouble3(FITKFluidVTKCommons::s_regionMeshEdgeColor, colorEdge3);

#ifdef Q_OS_WIN32
        // Create actor.
        m_fActor = FITKGraphActor::New();
        m_fActor->setGraphObject(this);
        m_fActor->setActorType(ActorType::SurfaceActor);
        m_fActor->SetPickable(false);
        m_fActor->SetVisibility(true);
        m_fActor->setScalarVisibility(false);
        m_fActor->GetProperty()->SetRepresentation(2);
        m_fActor->GetProperty()->SetEdgeVisibility(false);
        m_fActor->GetProperty()->SetColor(colorFace3);
        m_fActor->GetProperty()->SetEdgeColor(colorEdge3);
        m_fActor->GetProperty()->SetOpacity(1 - FITKFluidVTKCommons::s_transparency);
        m_fActor->setInputDataObject(m_ugrid);
#endif
#ifdef Q_OS_LINUX
        vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputData(m_ugrid);
        m_fActor = vtkActor::New();
        m_fActor->SetMapper(mapper);
        m_fActor->SetPickable(false);
        m_fActor->SetVisibility(true);
        m_fActor->GetProperty()->SetRepresentation(2);
        m_fActor->GetProperty()->SetEdgeVisibility(false);
        m_fActor->GetProperty()->SetColor(colorFace3);
        m_fActor->GetProperty()->SetEdgeColor(colorEdge3);
        m_fActor->GetProperty()->SetOpacity(1 - FITKFluidVTKCommons::s_transparency);
#endif
        addActor(m_fActor);
    }
}   // namespace Exchange
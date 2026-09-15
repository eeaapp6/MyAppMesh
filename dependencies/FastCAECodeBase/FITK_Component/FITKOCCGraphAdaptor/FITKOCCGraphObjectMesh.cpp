/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectMesh.h"

// OCC
#include <Standard_Handle.hxx>
#include <AIS_InteractiveContext.hxx>
#include <MeshVS_Mesh.hxx>
#include <MeshVS_DataSource.hxx>
#include <MeshVS_Drawer.hxx>
#include <MeshVS_NodalColorPrsBuilder.hxx>
#include <MeshVS_DrawerAttribute.hxx>
#include <MeshVS_SelectionModeFlags.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>
#include <MeshVS_DataMapOfIntegerColor.hxx>
#include <MeshVS_NodalColorPrsBuilder.hxx>
#include <MeshVS_ElementalColorPrsBuilder.hxx>

// Data
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKStructuredMesh.h"

// Commons
#include "FITKOCCMeshVS_Mesh.h"
#include "FITKOCCUnstructuredMeshVS.h"
#include "FITKOCCStructuredMeshVS.h"

namespace Exchange
{
    FITKOCCGraphObjectMesh::FITKOCCGraphObjectMesh(Interface::FITKAbstractMesh* meshData)
        : FITKOCCGraphObject3D(meshData)
    {
        if (!meshData)
        {
            return;
        }

        // Create mesh data source.
        //@{
        bool valid = false;

        // Unstructured grid.
        Interface::FITKUnstructuredMesh* unstrcturedMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(meshData);
        if (unstrcturedMesh && !valid)
        {
            m_meshSource = new FITKOCCUnstructuredMeshVS(unstrcturedMesh);
            valid = true;
        }

        // Structured grid.
        Interface::FITKStructuredMesh* strcturedMesh = dynamic_cast<Interface::FITKStructuredMesh*>(meshData);
        if (strcturedMesh && !valid)
        {
            m_meshSource = new FITKOCCStructuredMeshVS(strcturedMesh);
            valid = true;
        }
        //@}

        // Generate the interactive objects.
        generateShape();

        // Save the data type.
        m_graphInfo.Type = FITKOCCCommons::MeshGraph;

        // Initialize select modes.
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShape] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeVertex] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeEdge] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeWire] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeFace] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShell] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeSolid] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshNode] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshElement] = true;

        // Save the fixed size flag.
        m_hasFixedSize = true;
    }

    FITKOCCGraphObjectMesh::~FITKOCCGraphObjectMesh()
    {
        // Don't need to delete pointers here.
        m_meshSource = nullptr;
        m_meshVS = nullptr;
        m_meshVS = nullptr;
    }

    void FITKOCCGraphObjectMesh::updateInteractiveObjs()
    {
        if (m_context && m_meshVS)
        {
            // Check if the interactive object is visible.
            if (m_context->DisplayStatus(m_meshVS) == AIS_DisplayStatus::AIS_DS_Displayed)
            {
                m_context->Redisplay(m_meshVS, true);
            }
        }
    }

    void FITKOCCGraphObjectMesh::setViewMode(FITKOCCCommons::ShapeMeshViewMode type, bool visible)
    {
        if (!m_meshVS)
        {
            return;
        }

        switch (type)
        {
        case FITKOCCCommons::SMVM_Shade:
        {
            // MeshVS_Mesh does not support this function!
            break;
        }
        case FITKOCCCommons::SMVM_Wireframe:
        {
            m_meshVS->GetDrawer()->SetBoolean(MeshVS_DrawerAttribute::MeshVS_DA_ShowEdges, visible);
            break;
        }
        case FITKOCCCommons::SMVM_Vertex:
        {
            m_meshVS->GetDrawer()->SetBoolean(MeshVS_DrawerAttribute::MeshVS_DA_DisplayNodes, visible);
            break;
        }
        default:
            break;
        }

        // Save the visibility.
        FITKOCCGraphObject3D::setViewMode(type, visible);
    }

    void FITKOCCGraphObjectMesh::setColor(QColor color, FITKOCCCommons::ShapeMeshType type, int index)
    {
        if (!m_colorMap || !color.isValid())
        {
            return;
        }

        switch (type)
        {
        case FITKOCCCommons::SMT_MeshElement:
        {
            // Check the index.
            MeshVS_DataMapOfIntegerColor colors = m_colorMap->GetColors1();
            if (index < 1 || index > colors.Size())
            {
                return;
            }

            // Rebind the color to color map.
            colors.Bind(index, FITKOCCCommons::QColorToQuan(color));
            m_colorMap->SetColors1(colors);
            
            break;
        }
        default:
            break;
        }
    }

    void FITKOCCGraphObjectMesh::setColor(QColor color, FITKOCCCommons::ShapeMeshType type, QList<int> indice)
    {
        if (!m_colorMap || !color.isValid())
        {
            return;
        }

        Quantity_Color occColor = FITKOCCCommons::QColorToQuan(color);

        switch (type)
        {
        case FITKOCCCommons::SMT_MeshElement:
        {
            for (const int & index : indice)
            {
                // Check the index.
                MeshVS_DataMapOfIntegerColor colors = m_colorMap->GetColors1();
                if (index < 1 || index > colors.Size())
                {
                    return;
                }

                // Rebind the color to color map.
                colors.Bind(index, occColor);
                m_colorMap->SetColors1(colors);
            }

            break;
        }
        default:
            break;
        }
    }

    void FITKOCCGraphObjectMesh::setColor(QColor color)
    {
        if (!m_colorMap || !color.isValid())
        {
            return;
        }

        Quantity_Color occColor = FITKOCCCommons::QColorToQuan(color);

        // Bind new color to color map.
        MeshVS_DataMapOfIntegerColor colors = m_colorMap->GetColors1();
        for (int i = 1; i <= colors.Size(); i++)
        {
            colors.Bind(i, occColor);
        }

        m_colorMap->SetColors1(colors);
    }

    void FITKOCCGraphObjectMesh::setEnableTransparency(bool isOn)
    {
        if (!m_meshVS)
        {
            return;
        }

        m_meshVS->SetTransparency(isOn ? FITKOCCCommons::s_transpacency : 0.);
    }

    void FITKOCCGraphObjectMesh::highlight(bool update)
    {
        if (!m_context || !m_meshVS || m_isHighlighting)
        {
            return;
        }

        // Highlight the whole object.
        m_context->HilightWithColor(m_meshVS, m_context->HighlightStyle(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_LocalSelected), update);

        // Save the highlight flag.
        FITKOCCGraphObject3D::highlight(update);
    }

    void FITKOCCGraphObjectMesh::disHighlight(bool update)
    {
        if (!m_context || !m_meshVS || !m_isHighlighting)
        {
            return;
        }

        // Dis-highlight the whole object.
        m_context->Unhilight(m_meshVS, update);

        // Save the highlight flag.
        FITKOCCGraphObject3D::disHighlight(update);
    }

    void FITKOCCGraphObjectMesh::setVisible(bool visibility)
    {
        if (!m_meshVS || !m_context)
        {
            return;
        }

        if (visibility)
        {
            // Show the object.
            m_context->Display(m_meshVS, true);
        }
        else
        {
            // Hide the object.
            m_context->Erase(m_meshVS, true);
        }
    }

    bool FITKOCCGraphObjectMesh::test(QVariant& details)
    {
        Q_UNUSED(details);

        if (!m_meshSource)
        {
            return false;
        }

        // Generate the shape interactive objects.
        generateShape();

        return true;
    }

    void FITKOCCGraphObjectMesh::generateShape()
    {
        if (!m_meshSource || !_dataObj)
        {
            return;
        }

        // Create mesh interactive object.
        // Handle(MeshVS_Mesh) meshVS = new MeshVS_Mesh;
        m_meshVS = new FITKOCCMeshVS_Mesh;
        m_meshVS->SetDataSource(m_meshSource);

        // Save data information.
        m_meshVS->setGraphObject(this);
        m_meshVS->setDataObjectId(_dataObj->getDataObjectID());

        // Create handle.
        Handle(MeshVS_Mesh) meshHandle(m_meshVS);

        // Add default presentation builder.
        Handle(MeshVS_MeshPrsBuilder) builder = new MeshVS_MeshPrsBuilder(meshHandle);
        meshHandle->AddBuilder(builder, Standard_True);

        // Properties.
        m_meshVS->GetDrawer()->SetColor(MeshVS_DrawerAttribute::MeshVS_DA_EdgeColor, Quantity_NOC_BLACK);
        m_meshVS->GetDrawer()->SetColor(MeshVS_DrawerAttribute::MeshVS_DA_VectorColor, Quantity_NOC_BLACK);
        // m_meshVS->GetDrawer()->SetColor(MeshVS_DrawerAttribute::MeshVS_DA_InteriorColor, FITKOCCCommons::QColorToQuan(FITKOCCCommons::s_meshFaceColor));
        m_meshVS->GetDrawer()->SetBoolean(MeshVS_DrawerAttribute::MeshVS_DA_ShowEdges, Standard_True);
        m_meshVS->GetDrawer()->SetBoolean(MeshVS_DrawerAttribute::MeshVS_DA_DisplayNodes, Standard_False);
        m_meshVS->GetDrawer()->SetBoolean(MeshVS_DrawerAttribute::MeshVS_DA_Reflection, Standard_False);
        m_meshVS->SetDisplayMode(MeshVS_DMF_Shading);
        m_meshVS->SetMeshSelMethod(MeshVS_MeshSelectionMethod::MeshVS_MSM_NODES);

        // Optimization.
        // m_meshVS->GetDrawer()->SetBoolean(MeshVS_DA_SmoothShading, Standard_True);

        // Generate the face color map for original face color.
        int nElement = m_meshSource->GetAllElements().Extent();
        MeshVS_DataMapOfIntegerColor faceColorMap = FITKOCCCommons::QColorToColorMap(FITKOCCCommons::s_meshFaceColor, nElement);

        // Create element color builder.
        m_colorMap = new MeshVS_ElementalColorPrsBuilder(meshHandle, MeshVS_DMF_NodalColorDataPrs | MeshVS_DMF_OCCMask);
        Handle(MeshVS_ElementalColorPrsBuilder) elementColorBuilder(m_colorMap);
        elementColorBuilder->SetColors1(faceColorMap);
        meshHandle->AddBuilder(elementColorBuilder);

        // Add to the interactive object list.
        addInteractiveObj(meshHandle);
    }
}   // namespace Exchange
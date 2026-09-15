/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectDimensionToolBase.h"

// OCC
#include <Standard_Handle.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_ColoredShape.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_PointAspect.hxx>

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"

#include <QList>
#include <QVariant>

namespace Exchange
{
    FITKOCCGraphObjectDimensionToolBase::FITKOCCGraphObjectDimensionToolBase()
    {
        // Save the data type.
        m_graphInfo.Type = FITKOCCCommons::OtherGraph;

        // Initialize select modes.
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeVertex] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeEdge] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeWire] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeFace] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShell] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeSolid] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshNode] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshElement] = false;

        // Initialize shape object.
        createPreviewShape();

        // Save the default input role.
        m_defaultInputRole = ROLE_INPUT_MEASURE_IDS;
    }

    FITKOCCGraphObjectDimensionToolBase::~FITKOCCGraphObjectDimensionToolBase()
    {
        // Clear measured shape.
        if (m_aisShape)
        {
            m_aisShape->SetShape(TopoDS_Shape());
        }

        // Clear the pointer.
        m_aisShape = nullptr;
    }

    void FITKOCCGraphObjectDimensionToolBase::clearCache()
    {
        // Clear input and output.
        m_userInfo.clear();
    }

    void FITKOCCGraphObjectDimensionToolBase::update(bool forceUpdate)
    {
        // This kind of graph object need to be forced updated every time.
        Q_UNUSED(forceUpdate);

        // Update the geometry list.
        bool flag = updateMeasureGeometrys();
        if (!flag)
        {
            // Auto hide the measure tool if the flag is false.
            setVisible(false);
            return;
        }

        // Resize the measure tool.
        rescale();

        // Update the graph object.
        updateInteractiveObjs();
    }

    void FITKOCCGraphObjectDimensionToolBase::setVirtualTopoIds(QList<int> ids)
    {
        QVariant var = QVariant::fromValue<QList<int>>(ids);
        this->setInputVariantData(var);
    }

    void FITKOCCGraphObjectDimensionToolBase::clearData()
    {
        // Clear ids.
        m_userInfo.clear();

        // Hide the object.
        setVisible(false);
    }

    void FITKOCCGraphObjectDimensionToolBase::createPreviewShape()
    {
        // Create the shape for previewing.
        //@{
        m_aisShape = new AIS_ColoredShape(TopoDS_Shape());

        // Drawer properties.
        //@{
        m_aisShape->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);
        m_aisShape->SetTransparency(FITKOCCCommons::s_transpacency);

        Quantity_Color color = FITKOCCCommons::QColorToQuan(Qt::yellow);
        double lineWidth = FITKOCCCommons::s_toolLineWidth;

        // Initialize line width and default color.
        m_aisShape->SetWidth(lineWidth);
        m_aisShape->SetColor(color);

        Handle(Prs3d_Drawer) properties = m_aisShape->Attributes();

        // Face properties.
        properties->SetupOwnShadingAspect();
        properties->ShadingAspect()->SetColor(color);

        // Point properties.
        properties->SetupOwnPointAspect();
        properties->SetVertexDrawMode(Prs3d_VertexDrawMode::Prs3d_VDM_All);
        properties->PointAspect()->SetScale(3.);
        properties->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_O_PLUS);
        //@}

        // Add to object list.
        Handle(AIS_Shape) shapeHandle(m_aisShape);
        addInteractiveObj(shapeHandle);
        //@}
    }

    QList<int> FITKOCCGraphObjectDimensionToolBase::getInputItems()
    {
        return getUserDataAs<QList<int>>(ROLE_INPUT_MEASURE_IDS);
    }
}   // namespace Exchange
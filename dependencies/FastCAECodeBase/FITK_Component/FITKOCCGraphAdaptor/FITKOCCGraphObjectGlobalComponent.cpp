/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectGlobalComponent.h"

// OCC
#include <Standard_Handle.hxx>
#include <AIS_ColoredShape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <BRep_Builder.hxx>

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGlobalGeoComponent.h"

namespace Exchange
{
    FITKOCCGraphObjectGlobalComponent::FITKOCCGraphObjectGlobalComponent(Interface::FITKGlobalGeoComponent* globalComp)
        : FITKOCCGraphObjectTopoBase(globalComp)
    {
        if (!globalComp)
        {
            return;
        }

        // Initialize.
        bool flag = initShapeData();
        if (!flag)
        {
            return;
        }

        // Generate the interactive objects.
        generateShape();

        // Set the render layer to TopMost.
        enableTopMost(true);
    }

    FITKOCCGraphObjectGlobalComponent::~FITKOCCGraphObjectGlobalComponent()
    {
        // Don't need to delete pointers here.
        m_aisShape = nullptr;
    }

    void FITKOCCGraphObjectGlobalComponent::update(bool forceUpdate)
    {
        Q_UNUSED(forceUpdate);

        if (!m_aisShape)
        {
            return;
        }

        // Re-generate the shape.
        TopoDS_Shape shape;
        generateTopoShape(shape);

        // Set the shape to the ais shape.
        m_aisShape->SetShape(shape);
        m_aisShape->SetToUpdate();

        // Update the interactive objects.
        this->Superclass::update(true);
    }

    bool FITKOCCGraphObjectGlobalComponent::initShapeData()
    {
        Interface::FITKGlobalGeoComponent* globalComp = dynamic_cast<Interface::FITKGlobalGeoComponent*>(_dataObj);
        if (!globalComp)
        {
            return false;
        }

        // Get the sub-component count.
        return globalComp->getDataCount() != 0;
    }

    void FITKOCCGraphObjectGlobalComponent::generateTopoShape(TopoDS_Shape & resultShape)
    {
        Interface::FITKGlobalGeoComponent* globalComp = dynamic_cast<Interface::FITKGlobalGeoComponent*>(_dataObj);
        if (!globalComp)
        {
            return;
        }

        // Create a compound.
        BRep_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);

        // Get the shape from all sub-component.
        int nComp = globalComp->getDataCount();
        for (int i = 0; i < nComp; i++)
        {
            Interface::FITKGeoComponent* subComp = globalComp->getDataByIndex(i);
            if (!subComp)
            {
                continue;
            }

            // Get the topo type and shape agent.
            Interface::FITKModelEnum::FITKModelSetType type = subComp->getGeoType();

            Interface::FITKAbsGeoShapeAgent* shapeAgent = subComp->getAbstractModelAs<Interface::FITKAbsGeoShapeAgent>();
            if (!shapeAgent)
            {
                continue;
            }

            Interface::FITKGeoEnum::VTopoShapeType vsType;
            switch (type)
            {
            case Interface::FITKModelEnum::FMSPoint:
                vsType = Interface::FITKGeoEnum::VTopoShapeType::VSPoint;
                break;
            case Interface::FITKModelEnum::FMSEdge:
                vsType = Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
                break;
            case Interface::FITKModelEnum::FMSSurface:
                vsType = Interface::FITKGeoEnum::VTopoShapeType::VSFace;
                break;
            case Interface::FITKModelEnum::FMSSolid:
                vsType = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
                break;
            default:
                continue;
            }

            // Get the topo indice and virtual topo manager.
            QList<int> members = subComp->getMember();
            Interface::FITKVirtualTopoManager* vTopMgr = shapeAgent->getVirtualTopoManager();
            if (!vTopMgr)
            {
                continue;
            }

            Interface::FITKShapeVirtualTopoManager* vsTopoMgr = vTopMgr->getShapeVirtualTopoManager(vsType);
            if (!vsTopoMgr)
            {
                continue;
            }

            // Get the shape from topo members.
            for (const int & member : members)
            {
                Interface::FITKAbsVirtualTopo* vTopo = vsTopoMgr->getDataByIndex(member);
                if (!vTopo)
                {
                    continue;
                }

                OCC::FITKOCCTopoShape* occShape = vTopo->getShapeT<OCC::FITKOCCTopoShape>();
                if (!occShape)
                {
                    continue;
                }

                // Get the shape.
                TopoDS_Shape shape = occShape->getTopoShape();
                if (shape.IsNull())
                {
                    continue;
                }

                builder.Add(compound, shape);
            }
        }

        resultShape = compound;
    }

    void FITKOCCGraphObjectGlobalComponent::generateShape()
    {
        // Generate the shape.
        TopoDS_Shape shape;
        generateTopoShape(shape);

        // Create the ais shape and handle.
        m_aisShape = new AIS_ColoredShape(shape);
        Handle(AIS_Shape) shapeHandle(m_aisShape);

        Quantity_Color color = FITKOCCCommons::QColorToQuan(Qt::yellow);
        double lineWidth = FITKOCCCommons::s_toolLineWidth;

        // Initialize line width and default color.
        m_aisShape->SetWidth(lineWidth);
        m_aisShape->SetColor(color);

        // Drawer properties.
        //@{
        m_aisShape->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);
        m_aisShape->SetTransparency(FITKOCCCommons::s_transpacency);

        Handle(Prs3d_Drawer) properties = m_aisShape->Attributes();

        // Face properties.
        properties->SetupOwnShadingAspect();
        properties->ShadingAspect()->SetColor(color);
        //@}

        // Add to the interactive object list.
        addInteractiveObj(shapeHandle);
    }
}   // namespace Exchange
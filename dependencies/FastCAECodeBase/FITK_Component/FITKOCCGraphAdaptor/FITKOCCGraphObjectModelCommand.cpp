/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectModelCommand.h"

// OCC
#include <TopExp.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

// Graph
#include "FITKOCCAIS_ColoredShape.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"

// Test
//@{
#include <QStandardPaths>
#include <QFileDialog>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <STEPControl_Reader.hxx>
#include <IGESControl_Reader.hxx>
#include <StlAPI.hxx>
//@}

namespace Exchange
{
    FITKOCCGraphObjectModelCommand::FITKOCCGraphObjectModelCommand(Interface::FITKAbsGeoCommand* modelData)
        : FITKOCCGraphObjectModelBase(modelData)
    {
        if (!modelData)
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
    }

    FITKOCCGraphObjectModelCommand::~FITKOCCGraphObjectModelCommand()
    {
        // Don't need to delete pointers here.
        m_fAisShape = nullptr;
        m_tpShape = nullptr;
    }

    bool FITKOCCGraphObjectModelCommand::initShapeData()
    {
        Interface::FITKAbsGeoCommand* modelData = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (!modelData)
        {
            return false;
        }

        OCC::FITKAbstractOCCModel* modelOCC = modelData->getTShapeAgent<OCC::FITKAbstractOCCModel>();
        if (!modelOCC)
        {
            return false;
        }

        // Get the model shape.( The sketch don't need to check the shape data. )
        m_tpShape = modelOCC->getShape();
        if (!m_tpShape)
        {
            return false;
        }

        // Create shape indice map.
        m_indiceMap = new _ShapeIndiceMap;

        if (m_useIndexMap)
        {
            initShapeIndiceMap(*m_tpShape, m_indiceMap);
        }

        return true;
    }

    Interface::FITKAbsGeoCommand* FITKOCCGraphObjectModelCommand::getGeoCommand()
    {
        return dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
    }

    bool FITKOCCGraphObjectModelCommand::getDataVisibility()
    {
        // Get the command status first.
        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (!cmd)
        {
            return false;
        }

        // Then check if the command is in the geometry command( undo ) list.
        bool visible = cmd->getEnableReferenceList();

        // The modified and deleted command must be hidden.
        visible &= cmd->getCommandStatus() != Interface::FITKGeoEnum::FITKGeoStatus::FGSDeleted && 
            cmd->getCommandStatus() != Interface::FITKGeoEnum::FITKGeoStatus::FGSModified;

        // Check the reference count.
        visible &= (cmd->getReferencedCmdCount() == 0);

        // Get the data visibility.
        visible &= FITKOCCGraphObject3D::getDataVisibility();

        return visible;
    }

    void FITKOCCGraphObjectModelCommand::update(bool forceUpdate)
    {
        if (!m_fAisShape)
        {
            return;
        }

        Interface::FITKAbsGeoCommand* modelData = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (!modelData)
        {
            return;
        }

        OCC::FITKAbstractOCCModel* modelOCC = modelData->getTShapeAgent<OCC::FITKAbstractOCCModel>();
        if (!modelOCC)
        {
            return;
        }

        // Update the visibility first.
        updateVisibility();

        // Check if the shape has been changed.
        TopoDS_Shape* newShape = modelOCC->getShape();
        if (!newShape)
        {
            return;
        }

        if (newShape->HashCode(INT_MAX) != m_hashCode || forceUpdate)
        {
            // Reset the shape.
            m_tpShape = newShape;
            m_fAisShape->SetShape(*m_tpShape);
            m_fAisShape->SetToUpdate();

            // Save the hash code.
            m_hashCode = m_tpShape->HashCode(INT_MAX);

            // Update interactive object if necessary.
            updateInteractiveObjs();
        }

        // Update base.
        FITKOCCGraphObject3D::update(forceUpdate);
    }
}   // namespace Exchange
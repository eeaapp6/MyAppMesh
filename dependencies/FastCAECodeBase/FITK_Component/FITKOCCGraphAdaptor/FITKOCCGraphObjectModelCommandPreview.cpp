/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectModelCommandPreview.h"

// OCC
#include <TopoDS_Shape.hxx>

// Graph
#include "FITKOCCAIS_ColoredShape.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"

namespace Exchange
{
    FITKOCCGraphObjectModelCommandPreview::FITKOCCGraphObjectModelCommandPreview(Interface::FITKAbsGeoCommand* modelData)
        : FITKOCCGraphObjectModelPreviewBase(modelData)
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

    FITKOCCGraphObjectModelCommandPreview::~FITKOCCGraphObjectModelCommandPreview()
    {
        // Don't need to delete pointers here.
        m_fAisShape = nullptr;
        m_tpShape = nullptr;
    }

    bool FITKOCCGraphObjectModelCommandPreview::initShapeData()
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

        // Get the model shape.
        m_tpShape = modelOCC->getShape();
        if (!m_tpShape)
        {
            return false;
        }

        // Check the shape.
        if (m_tpShape->IsNull())
        {
            return false;
        }

        return true;
    }

    void FITKOCCGraphObjectModelCommandPreview::update(bool forceUpdate)
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

        if (newShape->HashCode(INT_MAX) == m_hashCode && !forceUpdate)
        {
            return;
        }

        // Reset the shape.
        m_tpShape = newShape;
        m_fAisShape->SetShape(*m_tpShape);
        m_fAisShape->SetToUpdate();

        // Save the hash code.
        m_hashCode = m_tpShape->HashCode(INT_MAX);

        // Update interactive object if necessary.
        updateInteractiveObjs();
    }
}   // namespace Exchange
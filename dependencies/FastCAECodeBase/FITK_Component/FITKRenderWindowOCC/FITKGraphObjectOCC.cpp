/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraphObjectOCC.h"

// Data base
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
 
// OCC
// Base
#include <AIS_InteractiveObject.hxx>
#include <AIS_InteractiveContext.hxx>

// Shape
#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>

// Mesh
#include <MeshVS_DataSource.hxx>
#include <MeshVS_Mesh.hxx>

namespace Render
{
    FITKGraphObjectOCC::FITKGraphObjectOCC(Core::FITKAbstractDataObject * dataObject):
        Core::FITKAbstractGraphObject(dataObject)
    {
        // Initialize select modes.
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_None] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShape] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeVertex] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeEdge] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeWire] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeFace] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShell] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeSolid] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = true;

        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshNode] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshElement] = true;
    }

    FITKGraphObjectOCC::FITKGraphObjectOCC() :
        Core::FITKAbstractGraphObject(nullptr)
    {
        // Nothing to do here.
    }

    FITKGraphObjectOCC::~FITKGraphObjectOCC()
    {
        // Remove from the graph widget.
        removeFromGraphWidget();

        // Delete all interactive objects.
        //@{
        //for (opencascade::handle<AIS_InteractiveObject> obj : m_objs)
        //{
        //    if (!obj)
        //    {
        //        continue;
        //    }

        //    obj->Delete();
        //}

        m_objs.clear();
        //@}

        m_context = nullptr;
    }

    void FITKGraphObjectOCC::update(bool forceUpdate)
    {
        Q_UNUSED(forceUpdate);

        // Reset the select mode.
        updateSelectModes();
    }

    void FITKGraphObjectOCC::setInteractiveContext(AIS_InteractiveContext* context)
    {
        m_context = context;
    }

    void FITKGraphObjectOCC::setView(V3d_View* view)
    {
        m_view = view;
    }

    void FITKGraphObjectOCC::clearInteractiveObjects()
    {
        // Try to remove from the context first.
        if (m_context)
        {
            for (opencascade::handle<AIS_InteractiveObject> obj : m_objs)
            {
                m_context->Remove(obj, false);
            }
        }

        m_objs.clear();
    }

    void FITKGraphObjectOCC::addInteractiveObj(opencascade::handle<AIS_InteractiveObject> obj)
    {
        // Check the pointer.
        if (!obj || m_objs.contains(obj))
        {
            return;
        }
        
        m_objs.push_back(obj);

        // Add to the context if the graph object is displaying.
        if (m_context)
        {
            m_context->Display(obj, obj->DisplayMode());
        }

        // Update the select mode.
        updateSelectModes();
    }

    opencascade::handle<AIS_InteractiveObject> FITKGraphObjectOCC::getInteractiveObjAt(int index)
    {
        // Get the indexed interactive object.
        if (m_objs.count() <= index && index >= 0)
        {
            return nullptr;
        }

        return m_objs[index];
    }

    int FITKGraphObjectOCC::getNumberOfInteractiveObjs()
    {
        return m_objs.count();
    }

    bool FITKGraphObjectOCC::getDataVisibility()
    {
        // Check the data object.
        if (!_dataObj)
        {
            return false;
        }

        // Get the visibility from the data object.
        return _dataObj->isEnable();
    }

    bool FITKGraphObjectOCC::getVisibility()
    {
        // Check the data first.
        bool dataVisible = getDataVisibility();
        if (!dataVisible || !m_context)
        {
            return false;
        }

        // Check the display status.
        bool displayed = true;
        for (opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (!obj)
            {
                continue;
            }

            displayed &= m_context->DisplayStatus(obj) == AIS_DisplayStatus::AIS_DS_Displayed;
        }

        return displayed && !m_objs.isEmpty();
    }

    void FITKGraphObjectOCC::addSelectMode(CommonOCC::SelectMode mode, bool force)
    {
        // Clear select modes.
        if (mode == CommonOCC::SelectMode::S_None)
        {
            clearSelectMode();
            return;
        }

        if (!m_context)
        {
            return;
        }

        // Check the select mode.
        CommonOCC::SelectMode realMode = CommonOCC::SelectMode::S_None;
        if (m_supportedSelectModeHash[mode] || force)
        {
            realMode = mode;
        }

        if (m_currSelectModes.contains(realMode))
        {
            return;
        }

        // Save the mode.
        if (realMode != CommonOCC::SelectMode::S_None)
        {
            m_currSelectModes.push_back(realMode);
        }

        // Set all objects'select mode by default.
        for (opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (!obj)
            {
                continue;
            }

            if (realMode == CommonOCC::SelectMode::S_None)
            {
                m_context->Deactivate(obj);
            }
            else
            {
                // Check if the object is visible.
                if (getVisibility())
                {
                    m_context->Load(obj, -1);
                    m_context->Activate(obj, realMode, true);
                }
            }
        }
    }

    void FITKGraphObjectOCC::clearSelectMode()
    {
        // Clear the modes.
        m_currSelectModes.clear();

        if (!m_context)
        {
            return;
        }

        // Deactive all objects'select mode by default.
        for (opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (!obj)
            {
                continue;
            }

            m_context->Deactivate(obj);
        }
    }

    void FITKGraphObjectOCC::updateSelectModes()
    {
        if (!m_context || !getVisibility())
        {
            return;
        }

        // Set all objects'select mode by default.
        for (opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (!obj)
            {
                continue;
            }

            // Deactive first.
            m_context->Deactivate(obj);
            m_context->Load(obj, -1);

            // Enable modes.
            for (const CommonOCC::SelectMode & mode : m_currSelectModes)
            {
                m_context->Activate(obj, mode, true);
            }
        }
    }

    void FITKGraphObjectOCC::updateByMouseMove(int mx, int my)
    {
        // Override if necessary.
        Q_UNUSED(mx);
        Q_UNUSED(my);
    }

    void FITKGraphObjectOCC::updateByMousePick(int mx, int my)
    {
        // Override if necessary.
        Q_UNUSED(mx);
        Q_UNUSED(my);
    }

    bool FITKGraphObjectOCC::hasFixedSize()
    {
        return m_hasFixedSize;
    }
}

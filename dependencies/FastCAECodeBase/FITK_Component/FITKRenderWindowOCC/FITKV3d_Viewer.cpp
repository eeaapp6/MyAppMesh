/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKV3d_Viewer.h"

// OCC
#include <Standard_Handle.hxx>
#include <V3d_View.hxx>
#include <Aspect_Grid.hxx>

#include "FITKV3d_RectangularGrid.h"

FITKV3d_Viewer::FITKV3d_Viewer(const Handle(Graphic3d_GraphicDriver)& theDriver) : V3d_Viewer(theDriver)
{
    // Initialize the hit point aspects.
    m_hitAsp = new Graphic3d_AspectMarker3d(Aspect_TypeOfMarker::Aspect_TOM_STAR,
        Quantity_NameOfColor::Quantity_NOC_DODGERBLUE1, 3.);
    this->SetGridEcho(m_hitAsp);

    // Create the user defined grid.
    createGrid();
}

void FITKV3d_Viewer::EnableCustomGrid()
{
    // Show the grid.
    m_grid->Display();

    // Set the user defined grid to all view.
    for (this->InitActiveViews(); this->MoreActiveViews(); this->NextActiveViews())
    {
        const Handle(V3d_View) & view = this->ActiveView();
        if (!view)
        {
            continue;
        }

        view->SetGrid(this->PrivilegedPlane(), m_grid);
    }
}

void FITKV3d_Viewer::DisbaleCustomGrid()
{
    m_grid->Erase();
    m_grid->Deactivate();

    // Hide all views'grid.
    for (this->InitActiveViews(); this->MoreActiveViews(); this->NextActiveViews())
    {
        const Handle(V3d_View) & view = this->ActiveView();
        if (!view)
        {
            continue;
        }

        view->SetGridActivity(Standard_False);
    }
}

void FITKV3d_Viewer::createGrid()
{
    // Create the grid.
    m_grid = new FITKV3d_RectangularGrid(this, Quantity_Color(Quantity_NOC_GRAY50), Quantity_Color(Quantity_NOC_GRAY60));
}

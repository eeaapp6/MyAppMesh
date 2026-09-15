/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKV3d_View.h"

// OCC
#include <Standard_Handle.hxx>

FITKV3d_View::FITKV3d_View(const Handle(V3d_Viewer)& theViewer, const V3d_TypeOfView theType)
    : V3d_View(theViewer, theType)
{
    // Nothing to de here.
}

FITKV3d_View::FITKV3d_View(const Handle(V3d_Viewer)& theViewer, const Handle(V3d_View)& theView)
    : V3d_View(theViewer, theView)
{
    // Nothing to de here.
}

void FITKV3d_View::Redraw() const
{
    V3d_View::Redraw();

    // Emit redrawed signal.
    emit sig_viewRedrawed();
}

void FITKV3d_View::RedrawImmediate() const
{
    V3d_View::RedrawImmediate();

    // Emit redrawed signal.
    emit sig_viewRedrawed();
}

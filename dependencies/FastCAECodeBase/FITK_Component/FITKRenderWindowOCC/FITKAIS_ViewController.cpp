/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAIS_ViewController.h"

// OCC
#include <Standard_Handle.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

void FITKAIS_ViewController::EnableSelectInView(bool flag)
{
    // Save the flag.
    m_enableSelect = flag;
}

void FITKAIS_ViewController::OnSelectionChanged(const Handle(AIS_InteractiveContext)& theCtx,
    const Handle(V3d_View)& theView)
{
    // Handle the selection and emit signal.
    AIS_ViewController::OnSelectionChanged(theCtx, theView);

    if (m_enableSelect)
    {
        emit sig_pickPerformed();
    }
}
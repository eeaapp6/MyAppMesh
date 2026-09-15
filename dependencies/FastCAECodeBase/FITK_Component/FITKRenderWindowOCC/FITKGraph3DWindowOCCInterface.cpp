/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraph3DWindowOCCInterface.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"

// Graph window
#include "FITKGraph3DWindowOCC.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

FITKLIBINFOREGISTER(FITKRenderWindowOCC, FITKRENDERWINDOWOCC_VERSION);

namespace Render
{
    QWidget* FITKGraph3DWindowOCCInterface::getWidget(const int port)
    {
        Q_UNUSED(port);

        // Get the application's settings.
        AppFrame::FITKRunTimeSetting* rSettings = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (rSettings)
        {
            // QVariant val = runTimeSetting->getValue();
        }

        // Create the OCC graph window.
        FITKGraph3DWindowOCC* w = new FITKGraph3DWindowOCC;
        return w;
    }

    QString FITKGraph3DWindowOCCInterface::getComponentName()
    {
        return "Graph3DWindowOCC";
    }
}
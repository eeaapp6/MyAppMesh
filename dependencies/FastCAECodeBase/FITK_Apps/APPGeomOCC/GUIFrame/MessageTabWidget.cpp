/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MessageTabWidget.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"

// Component msg
#include "FITK_Component/FITKCompMessageWidget/FITKConsoleComponent.h"

// Python
#include "FITK_Kernel/FITKPython/FITKPythonInterface.h"

namespace GUI
{
    MessageTabWidget::MessageTabWidget(QWidget * parent)
    {
        // 创建信息窗口页面。
        AppFrame::FITKComponentInterface* compInter = FITKAPP->getComponents()->getComponentTByName<Comp::ConsoleComponent>("MessageConsole");
        if (compInter)
        {
            addTab(compInter->getWidget(0), tr("Information"));
        }

        // 创建Python脚本页面。
        Python::FITKPythonInterface* pyInter = Python::FITKPythonInterface::getInstance();
        addTab(pyInter->getPyWidget(), "Python");
    }
}


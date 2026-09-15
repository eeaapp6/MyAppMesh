/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorChangeGraph.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/CentralWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace ModelOper
{
    bool OperatorChangeGraph::execGUI()
    {
        if (_emitter == nullptr) {
            return false;
        }
        //主界面
        GUI::MainWindow* mw = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mw == nullptr) {
            return false;
        }
        GUI::CentralWidget* centralWidget =mw->getCentralWidget();
        if (centralWidget == nullptr) {
            return false;
        }

        //文件列表
        QStringList files = {};
        QString actionName = _emitter->objectName();
        if (actionName == "OperatorChangePreGraph") {
            centralWidget->setCurGraphWidgetToPre();
        }
        else if (actionName == "OperatorChangePostGraph") {
            centralWidget->setCurGraphWidgetToPost();
        }
        return true;
    }
}
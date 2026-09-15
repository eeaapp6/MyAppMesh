/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "AIAssistantDockWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace GUI
{
    AIAssistantDockWidget::AIAssistantDockWidget(QWidget* parent)
        : QDockWidget(parent)
    {
        this->setWindowTitle(tr("AI Assistant"));
        this->setObjectName("AIAssistantDock");
        this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

        AppFrame::FITKComponentInterface* aiComp = FITKAPP->getComponents()->getComponentByName("FITKAIAssistant");
        if (aiComp == nullptr) return;

        QWidget* aiWidget = aiComp->getWidget(0);
        if (aiWidget == nullptr) return;

        this->setWidget(aiWidget);
        _valid = true;
    }

    bool AIAssistantDockWidget::isValid() const
    {
        return _valid;
    }
}

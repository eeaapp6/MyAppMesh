/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsWidget.h"

#include "GUIFrame/MainWindow.h"
#include "GUIWidget/InformationWidget.h"
#include "GUIWidget/PropertyWidget.h"
#include "GUIWidget/ToolBarAnimation.h"
#include "GUIWidget/ToolBarColorMapper.h"
#include "GUIWidget/ToolBarFilter.h"
#include "GUIWidget/ToolBarEvent.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace OperGUI
{
    OperatorsWidget::OperatorsWidget()
    {
        _mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
    }

    void OperatorsWidget::CurrentObjectChange(int objID)
    {
        //更新动画界面
        GUI::ToolBarAnimation* animationWidget = _mainWin->getToolBarAnimation();
        if (animationWidget) {
            animationWidget->updateCurrentGraphObj(objID);
        }

        //更新colormapper
        GUI::ToolBarColorMapper* colorMapperWidget = _mainWin->getToolBarColorMapper();
        if (colorMapperWidget) {
            colorMapperWidget->updateCurrentGraphObj(objID);
        }

        GUI::ToolBarFilter* filterWidget = _mainWin->getToolFilter();
        if (filterWidget) {
            filterWidget->updateCurrentGraphObj(objID);
        }

        //更新事件
        GUI::ToolBarEvent* eventWidget = _mainWin->getToolEvent();
        if (eventWidget) {
            eventWidget->updateCurrentGraphObj(objID);
        }

        //更新信息界面
        GUI::InformationWidget* infoWidget = _mainWin->getInfoWidget();
        if (infoWidget) {
            infoWidget->updateCurrentGraphObj(objID);
        }

        //更新属性界面
        GUI::PropertyWidget* propertyWidget = _mainWin->getPropertyWidget();
        if (propertyWidget) {
            propertyWidget->updateCurrentGraphObj(objID);
        }
    }
}
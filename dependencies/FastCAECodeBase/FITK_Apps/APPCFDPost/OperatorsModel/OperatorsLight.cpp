/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsLight.h"

#include "GUIFrame/MainWindow.h"
#include "GUIDialog/LightWidget.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"
#include "PostGraphAdaptor/PostGraphObjectLight.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostLight.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostLightManager.h"

namespace OperModel
{
    bool OperatorsLight::execGUI()
    {
        GUI::MainWindow* mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWin == nullptr)return false;
        Comp::FITKTabWidget* eventWidget = mainWin->getEventWidget();
        if (eventWidget == nullptr)return false;
        if (_senderName == "actionLight") {
            if (_isChecked) {
                GUI::LightWidget* lightWidget = new GUI::LightWidget(this);
                eventWidget->addTab(lightWidget, tr("Light"));
            }
            else {
                int index = eventWidget->findTabIndex(tr("Light"));
                QWidget* widget = eventWidget->widget(index);
                if (widget) {
                    delete widget;
                    widget = nullptr;
                }
                eventWidget->removeTab(index);
            }

            if (eventWidget->count() > 0)eventWidget->show();
            else eventWidget->hide();
        }
        else if (_senderName == "actionLightDelete") {
            int objID = -1;
            argValue(objID_String, objID);
            if (objID <= 0)return false;

            //移除灯光可视化
            Interface::PostGraphObjectManager* actorManager = Interface::PostGraphObjectManager::getInstance();
            if (actorManager == nullptr)return false;
            Interface::PostGraphLightManager* lightObjManager = actorManager->getGraphLightManager();
            if (lightObjManager == nullptr)return false;
            for (int i = 0; i < lightObjManager->getDataCount(); i++) {
                Interface::PostGraphObjectLight* lightObj = lightObjManager->getDataByIndex(i);
                if(lightObj == nullptr)continue;
                if (lightObj->getLightData() == nullptr)continue;
                if (objID == lightObj->getLightData()->getDataObjectID()) {
                    lightObjManager->removeDataObj(lightObj);
                }
            }

            //移除灯光数据
            Interface::FITKCFDPostLightManager* lightDataManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostLightManager();
            if (lightDataManager == nullptr)return false;
            lightDataManager->removeDataByID(objID);
        }
        return false;
    }

    bool OperatorsLight::execProfession()
    {
        return false;
    }
}


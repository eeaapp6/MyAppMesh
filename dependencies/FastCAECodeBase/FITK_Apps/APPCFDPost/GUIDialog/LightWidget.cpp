/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "LightWidget.h"
#include "ui_LightWidget.h"
#include "LightSubWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostLight.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostLightManager.h"

namespace GUI
{
    LightWidget::LightWidget(EventOper::ParaWidgetInterfaceOperator * oper):
        GUIWidgetBase(FITKAPP->getGlobalData()->getMainWindow())
    {
        _ui = new Ui::LightWidget();
        _ui->setupUi(this);
        init();
    }

    LightWidget::~LightWidget()
    {
        //清除全部子界面
        QLayoutItem* item;
        while ((item = _ui->verticalLayout_Sub->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater(); // 推荐使用 deleteLater，以确保小部件在适当时机被删除
            }
            delete item; // 删除布局项
        }

        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void LightWidget::init()
    {
        for (int i = 0; i < _postLightManager->getDataCount(); i++) {
            Interface::FITKCFDPostLight* lightData = _postLightManager->getDataByIndex(i);
            if(lightData == nullptr)continue;
            LightSubWidget* subWidget = new LightSubWidget(lightData->getDataObjectID(), this);
            _ui->verticalLayout_Sub->addWidget(subWidget);
        }
    }

    void LightWidget::on_pushButton_clicked()
    {
        Interface::FITKCFDPostLight* light = new Interface::FITKCFDPostLight();
        _postLightManager->appendDataObj(light);

        LightSubWidget* subWidget = new LightSubWidget(light->getDataObjectID(), this);
        _ui->verticalLayout_Sub->addWidget(subWidget);
    }
}


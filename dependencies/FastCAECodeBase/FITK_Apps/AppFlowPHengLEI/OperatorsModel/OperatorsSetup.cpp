/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsSetup.h"

#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIDialog/GUICalculateDialog/SetupWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include <QFile>
#include <QDir>

namespace ModelOper 
{
    OperatorsSetup::OperatorsSetup()
    {

    }

    OperatorsSetup::~OperatorsSetup()
    {

    }

    bool OperatorsSetup::execGUI()
    {
        GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWindow == nullptr)return false;
        GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
        if (propertyWidget == nullptr)return false;

        GUI::SetupWidget* widget = new GUI::SetupWidget(this, FITKAPP->getGlobalData()->getMainWindow());
        propertyWidget->setWidget(widget);

        return true;
    }

    bool OperatorsSetup::execProfession()
    {
        // 获取模型树控制器
        auto treeOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::TreeEventOperator>("ModelTreeEvent");
        if (treeOper == nullptr) return false;
        treeOper->updateTree();

		// 复制求解器程序
		QString PHengLEIDir = QApplication::applicationDirPath() + "/../PHengLEI";
		PHengLEIDir = QDir(PHengLEIDir).absolutePath();
		if (PHengLEIDir.isEmpty() || !Core::CreateDir(PHengLEIDir)) return false;
		QFile::copy(":/","");

        return true;
    }
}


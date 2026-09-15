/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MainWindowGenerator.h"

// APP
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

#include "GUIFrame/MainWindow.h"

MainWindowGenerator::MainWindowGenerator()
{
    //    this->setStyle("://Structural.qss");
    //    this->showMaximize(false);
}

MainWindowGenerator::~MainWindowGenerator()
{
//     delete m_MainWindow;
//     m_MainWindow = nullptr;
}

QWidget * MainWindowGenerator::genMainWindow()
{
    if (m_MainWindow == nullptr) {
        m_MainWindow = new GUI::MainWindow;
    }
    return m_MainWindow;
}

QStringList MainWindowGenerator::getTranslatorQmFiles()
{
    AppFrame::FITKAppSettings* settings = FITKAPP->getAppSettings();
    if (settings == nullptr) return QStringList();

    const QString language = settings->getValue<QString>("Language/Value").toLower();
    if (language == "chinese")
        return this->getCnTranslatorFiles();
    else
        return this->getEnTranslatorFiles();
}

QStringList MainWindowGenerator::getEnTranslatorFiles() const
{
    return
    {
        ":/translations/Translate_GUIDialog_EN.qm",
        ":/translations/Translate_GUIFrame_EN.qm",
        ":/translations/Translate_GUIWidget_EN.qm",
        ":/translations/Translate_FITKGeoCompOCC_EN.qm",
        ":/translations/Translate_FITKCompMessageWidget_EN.qm",
        ":/translations/Translate_FITKRenderWindowOCC_EN.qm",
    };
}

QStringList MainWindowGenerator::getCnTranslatorFiles() const
{
    return
    {
        ":/translations/Translate_GUIDialog_CN.qm",
        ":/translations/Translate_GUIFrame_CN.qm",
        ":/translations/Translate_GUIWidget_CN.qm",
        ":/translations/Translate_FITKGeoCompOCC_CN.qm",
        ":/translations/Translate_FITKCompMessageWidget_CN.qm",
        ":/translations/Translate_FITKRenderWindowOCC_CN.qm",
    };
}
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MainWindowGenerator.h"
#include "GUIFrame/MainWindow.h"
#include <QMainWindow>

MainWindowGenerator::MainWindowGenerator()
{
}

QWidget * MainWindowGenerator::genMainWindow()
{
    return new GUI::MainWindow;
}

bool MainWindowGenerator::showPreSettingPage()
{
#ifndef QT_DEBUG
//     GUI::WorkingDirDlg d;
//     return d.exec() == QDialog::Accepted;
#else
    return true;
#endif // !QT_DEBUG 
    return true;
}

QString MainWindowGenerator::getQssStyle()
{
 //   return QString("://IGA.qss");
	return QString();
}

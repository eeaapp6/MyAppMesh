/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MainTreeWidget.h"
#include "MainWindow.h"
#include "GUIWidget/TreeWidget.h"

#include <QVBoxLayout>

namespace GUI
{
    MainTreeWidget::MainTreeWidget(QWidget * parent) :
        Core::FITKWidget(parent)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);
        _treeWidget = new TreeWidget(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(_treeWidget);
    }

    MainTreeWidget::~MainTreeWidget()
    {

    }

    TreeWidget * MainTreeWidget::getTreeWidget()
    {
        return _treeWidget;
    }
}


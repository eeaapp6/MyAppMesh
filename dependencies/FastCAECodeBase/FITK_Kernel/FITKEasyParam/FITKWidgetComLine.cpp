/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWidgetComLine.h"
#include "ui_FITKWidgetComLine.h"
#include "FITKAbstractEasyParam.h"
#include "FITKEasyParamWidgetFactory.h"

#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QToolBox>

namespace Core
{
    FITKWidgetComLine::FITKWidgetComLine(FITKAbstractEasyParam* data, QWidget * parent) :
        QWidget(parent), _data(data)
    {
        _ui = new Ui::FITKWidgetComLine();
        _ui->setupUi(this);

        init();
    }

    FITKWidgetComLine::~FITKWidgetComLine()
    {
        if (_ui)delete _ui;
    }

    void FITKWidgetComLine::init()
    {
        if (_data == nullptr)return;
        _label = new QLabel(this);
        _label->setText(_data->getDataObjectName());
        int width = this->width();
        _label->setMinimumWidth(width*0.4);
        _label->setMaximumWidth(width*0.4);
        _ui->horizontalLayout->addWidget(_label);

        _paramWidget = FITKEasyParamWidgetFactory::createWidget(_data, this);
        if (_paramWidget == nullptr)return;
        _ui->horizontalLayout->addWidget(_paramWidget);
    }

    QToolBox * FITKWidgetComLine::CreateToolBox(QWidget * parent)
    {
        QToolBox* toolBox = new QToolBox(parent);
        toolBox->setStyleSheet(
            "QToolBox::tab {"
            "    background-color: #d3d3d3;" /* 淡浅灰色背景 */
            "}"
            "QToolBox::tab:selected {"
            "    background-color: #a9a9a9;" /* 选中时的背景色，稍深的灰色 */
            "}"
        );

        return toolBox;
    }

    FITKAbstractEasyParam * FITKWidgetComLine::getData()
    {
        return _data;
    }

    QLabel* FITKWidgetComLine::getLabel()
    {
        return _label;
    }

    QWidget* FITKWidgetComLine::getParamWidget()
    {
        return _paramWidget;
    }
}


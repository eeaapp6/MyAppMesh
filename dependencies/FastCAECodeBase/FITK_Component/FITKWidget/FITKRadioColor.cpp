/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadioColor.h"
#include "ui_FITKRadioColor.h"

FITKRadioColor::FITKRadioColor(QWidget *parent) : QWidget(parent), _ui(new Ui::FITKRadioColor)
{
    _ui->setupUi(this);
}

FITKRadioColor::~FITKRadioColor()
{
    delete _ui;
}

void FITKRadioColor::init(QString radioName, QColor backGroundColor, bool isEnable, bool isShowColor)
{
    _ui->RB_Temp->setText(radioName);
    _ui->RB_Temp->setEnabled(isEnable);

    _ui->label->setAutoFillBackground(isShowColor);

    if (!isShowColor) return;
    int r = backGroundColor.red();
    int g = backGroundColor.green();
    int b = backGroundColor.blue();
    _ui->label->setStyleSheet(QString("border:1px solid black;background-color:rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
    QPalette palette;
    palette.setColor(QPalette::Background, backGroundColor);
    _ui->label->setPalette(palette);
}

void FITKRadioColor::setEnable(bool isEnable)
{
    _ui->RB_Temp->setEnabled(isEnable);
}

void FITKRadioColor::setClick(bool isClick)
{
    _ui->RB_Temp->setChecked(isClick);
}

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPushButtonColor.h"

#include <QColorDialog>

FITKPushButtonColor::FITKPushButtonColor(QWidget* parent) :
    QPushButton(parent)
{
    connect(this, &QPushButton::clicked, this, &FITKPushButtonColor::slotButtonClicked);
    updateButtonColor();
}

void FITKPushButtonColor::setCurrentColor(const QColor& color)
{
    if (color.isValid() == false)
    {
        return;
    }

    if (_currentColor == color)
    {
        return;
    }

    _currentColor = color;
    updateButtonColor();
    emit sigColorChanged(_currentColor);
}

QColor FITKPushButtonColor::getCurrentColor() const
{
    return _currentColor;
}

void FITKPushButtonColor::slotButtonClicked()
{
    QColor selectedColor = QColorDialog::getColor(_currentColor, nullptr, tr("Select Color"));
    if (selectedColor.isValid() == false)
    {
        return;
    }

    setCurrentColor(selectedColor);
}

void FITKPushButtonColor::updateButtonColor()
{
    QString styleSheet = QString("QPushButton{background-color:%1;border:1px solid #808080;}")
        .arg(_currentColor.name(QColor::HexRgb));
    setStyleSheet(styleSheet);
}
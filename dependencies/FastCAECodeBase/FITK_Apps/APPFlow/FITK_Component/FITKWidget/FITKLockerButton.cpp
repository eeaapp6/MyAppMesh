/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKLockerButton.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPixmap>

namespace Comp 
{
    FITKLockerButton::FITKLockerButton(QWidget* parent) :
        QPushButton(parent)
    {
        this->setObjectName("DrawerWidgetButton");
        //标题label初始化
        m_textLabel = new QLabel(this);
        m_textLabel->setText("DrawerWidgetLabel");
        m_textLabel->setObjectName("DrawerWidgetLabel");
        //图片label初始化
        m_iconLabel = new QLabel(this);
        m_iconLabel->setText("DrawerWidgetIcon");
        initialize();
    }

    FITKLockerButton::~FITKLockerButton()
    {
        if (m_textLabel != nullptr) delete m_textLabel;
        if (m_iconLabel != nullptr) delete m_iconLabel;
    }

    void FITKLockerButton::setText(const QString& text)
    {
        //设置标题
        m_textLabel->setText(text);
    }

    void FITKLockerButton::setImage(const QPixmap& image)
    {
        //设置图标
        m_iconLabel->setPixmap(image);
    }

    void FITKLockerButton::initialize()
    {
        m_iconLabel->setFixedWidth(16);
        m_iconLabel->setFixedHeight(16);
        m_iconLabel->setScaledContents(true);
        m_textLabel->setText(tr("Page"));

        //界面布局添加
        QHBoxLayout* hLayout = new QHBoxLayout;
        hLayout->setObjectName("LockerButtonHBoxLayout");
        hLayout->addWidget(m_iconLabel);
        hLayout->addWidget(m_textLabel);
        hLayout->setMargin(0);
        hLayout->setSpacing(6);
        hLayout->setContentsMargins(10, 0, 0, 0);
        this->setLayout(hLayout);
    }
}
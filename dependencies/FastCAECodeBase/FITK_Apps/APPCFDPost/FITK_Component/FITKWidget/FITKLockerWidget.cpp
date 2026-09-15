/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKLockerWidget.h"
#include "FITKLockerButton.h"
#include "FITKDrawerWidget.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QDebug>
#include <QStyle>

namespace Comp
{
    FITKLockerWidget::FITKLockerWidget(int id, FITKDrawerWidget* parent) :
        Core::FITKWidget(parent)
    {
        //参数初始化
        m_button = new FITKLockerButton(this);
        m_widget = new QWidget;
        m_widgetLayout = new QVBoxLayout;
        m_widgetLayout->setSpacing(0);
        m_widgetLayout->setMargin(0);
        m_scrollArea = new QScrollArea;

        m_fold = true;
        m_id = id;

        initialize();
        //折叠信号连接
        connect(this, SIGNAL(foldWidgetSig(int, bool)), parent, SLOT(foldWidget(int, bool)));

        // 获取展开图标（向下箭头）Qt默认图标
        m_unfoldPixmap = this->style()->standardPixmap(QStyle::SP_ArrowDown);
        // 获取折叠图标（向右箭头）Qt默认图标
        m_foldPixmap = this->style()->standardPixmap(QStyle::SP_ArrowRight);
        //设置展开图标
        setFoldPixmap(m_foldPixmap);
        //设置折叠图标
        setUnFoldPixmap(m_unfoldPixmap);
    }

    FITKLockerWidget::~FITKLockerWidget()
    {
        if (m_button != nullptr) delete m_button;
        if (m_widgetLayout != nullptr) delete m_widgetLayout;
        if (m_widget != nullptr) delete m_widget;
    }

    void FITKLockerWidget::setText(const QString& text)
    {
        //设置locker中按钮标题
        m_button->setText(text);
    }

    void FITKLockerWidget::addWidget(QWidget* widget, bool isStretch)
    {
        if (widget == nullptr) return;

        auto count = m_widgetLayout->count();
        auto item = m_widgetLayout->itemAt(count - 1);
        if (item != nullptr)
        {
            auto spacer = item->spacerItem();
            if (spacer != nullptr)
            {
                m_widgetLayout->removeItem(item);
            }
        }

        m_widgetLayout->addWidget(widget);
        if (isStretch) m_widgetLayout->addStretch();
    }

    int FITKLockerWidget::getID()
    {
        return m_id;
    }

    void FITKLockerWidget::setFoldable(bool visible)
    {
        m_fold = visible;

        //设置折叠与展开图标
        if (m_fold)
        {
            m_button->setImage(QPixmap(m_foldPixmap));
            m_widget->setVisible(false);
        }
        else
        {
            m_button->setImage(QPixmap(m_unfoldPixmap));
            m_widget->setVisible(true);
        }
    }

    bool FITKLockerWidget::getFoldable()
    {
        return m_fold;
    }

    void FITKLockerWidget::setFoldPixmap(const QPixmap& foldImage)
    {
        m_foldPixmap = foldImage;
        if (m_fold) m_button->setImage(m_foldPixmap);
    }

    void FITKLockerWidget::setUnFoldPixmap(const QPixmap& unFoldImage)
    {
        m_unfoldPixmap = unFoldImage;
        if (!m_fold) m_button->setImage(m_unfoldPixmap);
    }

    void FITKLockerWidget::initialize()
    {
        m_widgetLayout->setMargin(0);
        m_widgetLayout->setSpacing(0);
        m_widgetLayout->addStretch();

        //主控区域初始化
        QWidget* scrollWidget = new QWidget;
        scrollWidget->setLayout(m_widgetLayout);

        m_scrollArea->setWidgetResizable(true);
        m_scrollArea->setWidget(scrollWidget);

        QGridLayout* gridLayout = new QGridLayout;
        gridLayout->setMargin(0);
        gridLayout->setSpacing(0);
        gridLayout->addWidget(m_scrollArea);

        m_widget->setVisible(false);
        m_widget->setLayout(gridLayout);
        m_widget->setAutoFillBackground(true);

        //布局添加
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(m_button);
        layout->addWidget(m_widget);
        layout->setMargin(0);
        layout->setSpacing(0);
        this->setLayout(layout);

        connect(m_button, &FITKLockerButton::clicked, [this] { emit this->foldWidgetSig(m_id, !m_fold); });
    }
}
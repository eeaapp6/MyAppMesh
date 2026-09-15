/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKDrawerWidget.h"
#include "ui_FITKDrawerWidget.h"
#include "FITKLockerWidget.h"
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>

namespace Comp
{
    //组件ID
    int FITKDrawerWidget::m_widgetNum = 0;
     
    FITKDrawerWidget::FITKDrawerWidget(QWidget *parent):
        Core::FITKWidget(parent), m_ui(new Ui::FITKDrawerWidget)
    {
        m_ui->setupUi(this);
        //
        m_ui->verticalLayout->setMargin(0);
        //间隔设置
        m_ui->verticalLayout->setSpacing(2);
    }

    FITKDrawerWidget::~FITKDrawerWidget()
    {
        delete m_ui;
    }

    void FITKDrawerWidget::setAllItemFold(bool fold)
    {
        m_allItemFold = fold;
    }

    void FITKDrawerWidget::setItemExpand(bool expand)
    {
        m_itemExpand = expand;
        if (m_itemExpand) return;
        removeLastCompStretch();
    }

    bool FITKDrawerWidget::getItemExpand()
    {
        return m_itemExpand;
    }

    int FITKDrawerWidget::addWidget(QWidget* widget, QString title, bool isStretch)
    {
        if (widget == nullptr) return -1;

        if (m_itemExpand) removeLastCompStretch();

        auto lWidget = new FITKLockerWidget(++m_widgetNum, this);
        lWidget->addWidget(widget, isStretch);
        //设置标题
        lWidget->setText(title);
        lWidget->setFoldable(true);

        //判断存储的图标是否有效，设置locker界面折叠与展开图标
        if (m_foldImage != QPixmap()) {
            lWidget->setFoldPixmap(m_foldImage);
        }
        if (m_unFoldImage != QPixmap()) {
            lWidget->setUnFoldPixmap(m_unFoldImage);
        }
        if (m_LockerWidgetList.size() == 0) lWidget->setFoldable(false);

        m_LockerWidgetList.append(lWidget);
        m_ui->verticalLayout->addWidget(lWidget);

        return m_widgetNum;
    }

    FITKLockerWidget* FITKDrawerWidget::getLockerWidgetByID(int id)
    {
        //通过ID获取locker界面
        for (FITKLockerWidget* widget : m_LockerWidgetList)
        {
            if (widget == nullptr) continue;
            if (widget->getID() == id)
                return widget;
        }
        return nullptr;
    }

    void FITKDrawerWidget::setLockerWidgetIcon(const QPixmap & foldImage,const QPixmap & unFoldImage)
    {
        //记录设置的图标
        m_foldImage = foldImage;
        m_unFoldImage = unFoldImage;

        //修改所有locker中的图标
        for (auto w : m_LockerWidgetList) {
            if (w == nullptr)continue;
            w->setFoldPixmap(m_foldImage);
            w->setUnFoldPixmap(m_unFoldImage);
        }
    }

    void FITKDrawerWidget::foldWidget(int id, bool fold)
    {
        Comp::FITKLockerWidget* widget = this->getLockerWidgetByID(id);
        if (widget == nullptr) return;

        //局部函数
        auto getAllItemFoldState = [=] {
            for (auto w : m_LockerWidgetList)
            {
                if (!w->getFoldable()) return false;
            }

            return true;
        };

        if (!m_itemExpand)
        {
            if (m_allItemFold && !fold)
            {
                removeLastCompStretch();
            }
            else if (m_allItemFold && fold)
            {
                widget->setFoldable(fold);
                if (getAllItemFoldState()) m_ui->verticalLayout->addStretch();
                else removeLastCompStretch();
            }

            if (!widget->getFoldable()) return;

            for (auto w : m_LockerWidgetList)
            {
                w->setFoldable(true);
            }
        }

        widget->setFoldable(fold);

        if (m_itemExpand)
        {
            if (getAllItemFoldState()) m_ui->verticalLayout->addStretch();
            else removeLastCompStretch();
        }
    }

    void FITKDrawerWidget::initialize()
    {
    }

    void FITKDrawerWidget::removeLastCompStretch()
    {
        //移除最后一个组件的空白区域，使其拾取底部拉伸，下沉到界面最底部
        QVBoxLayout* vLayout = m_ui->verticalLayout;
        int count = vLayout->count();
        QLayoutItem* item = vLayout->itemAt(count - 1);
        if (item != nullptr)
        {
            //获取空白区域
            QSpacerItem* spacer = item->spacerItem();
            //判断是否有空白区域，有则删除
            if (spacer != nullptr){
                //移除空白区域
                vLayout->removeItem(item);
            }
        }
    }
}

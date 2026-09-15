/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTabWidget.h"

#include <QFontMetrics>

namespace Comp
{
    FITKTabWidget::FITKTabWidget(FITKTabWidgetType type, QWidget * parent) :
        QTabWidget(parent), _type(type)
    {
        switch (_type) {
        case Comp::FITKTabWidgetType::FITKTab_None: break;
        case Comp::FITKTabWidgetType::FITKTab_Fill: break;
        case Comp::FITKTabWidgetType::FITKTab_Auto: {
            FITKTabBar* customTabBar = new FITKTabBar(this);
            this->setTabBar(customTabBar);
            break;
        }
        }
    }

    FITKTabWidget::~FITKTabWidget()
    {

    }

    int FITKTabWidget::findTabIndex(QString tabName)
    {
        int tabCount = this->count();
        for (int i = 0; i < tabCount; i++) {
            QString name = this->tabText(i);
            if (name == tabName)return i;
        }
        return -1;
    }

    void FITKTabWidget::showEvent(QShowEvent * event)
    {
        QTabWidget::showEvent(event);
        changeStyleSheet();
    }

    void FITKTabWidget::resizeEvent(QResizeEvent * event)
    {
        QTabWidget::resizeEvent(event);
        changeStyleSheet();
    }

    void FITKTabWidget::changeStyleSheet()
    {
        QString styleSheet = "";
        switch (_type) {
        case Comp::FITKTabWidgetType::FITKTab_None:return;
        case Comp::FITKTabWidgetType::FITKTab_Fill: {
            int width = this->width();
            int tabCount = this->count();
            if (tabCount == 0)
            {
                return;
            }

            int tabWidth = width / tabCount;
            styleSheet = QString(
                "QTabBar::tab { "
                "   width:%1px;"
                "   height:30px;"
                "   left: 0px;"
                "   right: 0px;"
                "}"
                "QTabBar::tab:selected {"
                "   border-bottom: 2px solid #007bff;" // 选中的标签页底部使用蓝色边框
                "}"
                "QTabBar::tab:hover {"
                "   border-bottom: 2px solid #d3d3d3;" // 悬浮时标签页底部使用灰色边框
                "}"
            ).arg(tabWidth);
            this->tabBar()->setStyleSheet(styleSheet);
            break;
        }
        case Comp::FITKTabWidgetType::FITKTab_Auto: {
            styleSheet =
                "QTabBar::tab {"
                "   background: lightgray;"      // 标签页背景色
                "   border: 1px solid darkgray ;"   // 标签页边框
                "   left: 0px;"
                "   right: 0px;"
                "}"
                "QTabBar::tab:selected {"
                "   background: white;"          // 选中的标签页背景色
                "   border-bottom: none;"        // 移除底部边框以避免与选中状态的边框重叠
                "}"
                ;
            this->tabBar()->setStyleSheet(styleSheet);
            styleSheet = "QTabWidget::pane {"
                "   border: 1px solid #d0d0d0;"  // QTabWidget 的边框
                "}";
            this->setStyleSheet(styleSheet);
            break;
        }
        }
    }

    //---------------------------------------------------------------------------------------------------------------

    FITKTabBar::FITKTabBar(QWidget * parent) :
        QTabBar(parent)
    {

    }

    FITKTabBar::FITKTabBar()
    {

    }

    QSize FITKTabBar::tabSizeHint(int index) const
    {
        // 根据每个标签页的标题计算宽度
        QString text = tabText(index);
        QFontMetrics fm(font());
        int textWidth = fm.horizontalAdvance(text);
        // 添加一些填充
        int padding = 20;
        // 设置宽度和默认高度
        return QSize(textWidth + padding, 30);
    }
}
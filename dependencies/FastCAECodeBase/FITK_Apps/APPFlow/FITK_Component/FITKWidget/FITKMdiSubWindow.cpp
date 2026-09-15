/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMdiSubWindow.h"
#include "FITKMdiArea.h"

#include <QDrag>
#include <QStyle>
#include <QMimeData>
#include <QMouseEvent>
#include <QApplication>
#include <QMenu>

namespace Comp
{
    //互斥锁初始化
    QMutex FITKMdiSubWindow::_mutex;

    FITKMdiSubWindow::FITKMdiSubWindow(QWidget * parent) 
    {
        //设置窗口外部移动属性
        QMenu* menu = this->systemMenu();
        //添加外部移动动作-将内部窗口移除
        QAction* eMoveAction = menu->addAction("ExteriorMove");
        connect(eMoveAction, &QAction::triggered, this, &FITKMdiSubWindow::widgetExteriorMove);
    }

    FITKMdiSubWindow::~FITKMdiSubWindow()
    {
    }

    void FITKMdiSubWindow::setWidget(QWidget *widget)
    {
        m_widget = widget;
        this->QMdiSubWindow::setWidget(widget);
        this->initWidget();
    }

    QWidget *FITKMdiSubWindow::widget() const
    {
        return m_widget;
    }

    void FITKMdiSubWindow::setParent(FITKMdiArea* parent)
    {
        m_parent = parent;
    }

    void FITKMdiSubWindow::initWidget()
    {
        if (!m_widget)return;


        m_widget->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
        //右键 不可少否则右键无反应
        m_widget->setContextMenuPolicy(Qt::CustomContextMenu);

        //创建右键移回操作
        connect(m_widget, &QWidget::customContextMenuRequested, [=]() {
            if (m_widget == this->QMdiSubWindow::widget())
                return;
            QMenu menu;
            QAction* moveRAction = menu.addAction("MoveRestore");
            connect(moveRAction, &QAction::triggered, this, &FITKMdiSubWindow::widgetMoveRestore);
            menu.exec(QCursor::pos());
        });
    }

    void FITKMdiSubWindow::mousePressEvent(QMouseEvent *event)
    {
        QMdiSubWindow::mousePressEvent(event);
    }

    void FITKMdiSubWindow::mouseMoveEvent(QMouseEvent *event)
    {
        QMdiSubWindow::mouseMoveEvent(event);
    }

    void FITKMdiSubWindow::mouseReleaseEvent(QMouseEvent *event)
    {
        QMdiSubWindow::mouseReleaseEvent(event);
    }

    void FITKMdiSubWindow::widgetExteriorMove()
    {
        if (!m_widget)return;
        this->QMdiSubWindow::setWidget(nullptr);
        m_widget->show();
        this->hide();
    }
    void FITKMdiSubWindow::changeEvent(QEvent * changeEvent)
    {
        QMdiSubWindow::changeEvent(changeEvent);
        if (this->isMaximized()) {
            emit this->sigWinStateMax();
        }
    }

    void FITKMdiSubWindow::widgetMoveRestore()
    {
        this->QMdiSubWindow::setWidget(m_widget);
        if (!m_widget || !m_parent)return;
        QList<QMdiSubWindow *> list = m_parent->subWindowList();
        if (!list.contains(this))
            m_parent->addSubWindow(this);
        m_widget->show();
        this->show();
    }
}

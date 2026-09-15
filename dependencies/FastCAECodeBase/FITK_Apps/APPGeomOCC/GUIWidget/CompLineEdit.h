/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef CompLineEdit_H
#define CompLineEdit_H

#include "GUIWidgetAPI.h"
#include <QLineEdit>

namespace GUI
{
    class GUIWIDGETAPI CompLineEdit : public QLineEdit
    {
        Q_OBJECT;
    public:
        CompLineEdit(QWidget* widget);
        ~CompLineEdit();

    signals:
        ;
        void sigEditStart();
        void sigEditFinish();
    private:
        //鼠标双击事件重写
        void mouseDoubleClickEvent(QMouseEvent* event) override;
        //失焦事件重写
        void focusOutEvent(QFocusEvent* event) override;
        //回车事件
        void keyPressEvent(QKeyEvent* event) override;
    private:
        bool _isEdit = false;
    };
}

#endif

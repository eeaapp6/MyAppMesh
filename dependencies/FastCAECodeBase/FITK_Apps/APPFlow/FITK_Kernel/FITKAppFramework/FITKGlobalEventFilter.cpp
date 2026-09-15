/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGlobalEventFilter.h"
#include "FITKKeyMouseStates.h"
#include "FITKAppFramework.h"
#include "FITKGlobalData.h"
#include "FITKKeyMouseStates.h"
#include "FITKAbstractEventFilter.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

namespace AppFrame
{

    FITKGlobalEventFilter::FITKGlobalEventFilter()
    {
        _keyMouseStates = FITKAPP->getGlobalData()->getKeyMouseStates();
    }

    FITKGlobalEventFilter::~FITKGlobalEventFilter()
    {
        for (FITKAbstractEventFilter* f : _eventFilters)
        {
            if(f) delete f;
        }
        _eventFilters.clear();
    }

    bool FITKGlobalEventFilter::eventFilter(QObject *Object, QEvent *event)
    {
        //程序事件过滤
        QEvent::Type eType = event->type();
        switch (eType)
        {
        case QEvent::KeyPress:
            this->keyPressEvent(Object, dynamic_cast<QKeyEvent*>(event));
            break;
        case QEvent::KeyRelease:
            this->keyReleaseEvent(Object, dynamic_cast<QKeyEvent*>(event));
            break;
        case QEvent::MouseButtonPress:
            this->mousePressEvent(Object, dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            this->mouseReleaseEvent(Object, dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseMove:
            this->mouseMoveEvent(Object, dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::ApplicationActivated:
        case QEvent::ApplicationDeactivate:
            this->applicationActiveEvent();
            break;
        default: break;
        }

        bool ok = QObject::eventFilter(Object, event);
        return ok;
    }

    void FITKGlobalEventFilter::addEventFilter(FITKAbstractEventFilter* filter)
    {
        _eventFilters.append(filter);
    }




    bool FITKGlobalEventFilter::keyPressEvent(QObject *Object, QKeyEvent* e)
    {
        if (e == nullptr) return false;

        //判断键盘按下类型
        switch (e->key())
        {
        case Qt::Key_Control:
        case Qt::Key_Shift:
            //记录键盘按键状态
            _keyMouseStates->keyPressed((Qt::Key)e->key(), true);
        default:
            break;
        }
        return this->addedEventFilterProcess(Object, e);
    }


    bool FITKGlobalEventFilter::keyReleaseEvent(QObject *Object, QKeyEvent* e)
    {
        if (e == nullptr) return false;

        //判断键盘抬起类型
        switch (e->key())
        {
        case Qt::Key_Control:
        case Qt::Key_Shift:
            //记录键盘按键状态
            _keyMouseStates->keyPressed((Qt::Key)e->key(), false);
        default:
            break;
        }
        return this->addedEventFilterProcess(Object, e);

    }

    bool FITKGlobalEventFilter::mousePressEvent(QObject *Object, QMouseEvent* e)
    {
        if (e == nullptr) return false;
        //判断鼠标按下类型
        switch (e->button())
        {
        case Qt::LeftButton:
        case Qt::MiddleButton:
        case Qt::RightButton:
            //记录鼠标按键状态
            _keyMouseStates->mousePressed(e->button(), true);
            break;
        default:
            break;
        }
        bool flag = this->addedEventFilterProcess(Object, e);
        _keyMouseStates->mouseMoved(false);
        return flag;
    }

    bool FITKGlobalEventFilter::mouseReleaseEvent(QObject *Object, QMouseEvent* e)
    {
        if (e == nullptr) return false;

        //判断鼠标抬起类型
        switch (e->button())
        {
        case Qt::LeftButton:
        case Qt::MiddleButton:
        case Qt::RightButton:
            //记录鼠标按键状态
            _keyMouseStates->mousePressed(e->button(), false);
            break;
        default:
            break;
        }
        return this->addedEventFilterProcess(Object, e);
//        _keyMouseStates->mouseMoved(false);

    }

    bool FITKGlobalEventFilter::mouseMoveEvent(QObject *Object, QMouseEvent* e)
    {
        //有鼠标按下处理
        if (!_keyMouseStates->isMousePressed()) return false;

        _keyMouseStates->mouseMoved(true);
        return this->addedEventFilterProcess(Object, e);
    }

    void FITKGlobalEventFilter::applicationActiveEvent()
    {
        _keyMouseStates->keyPressed(Qt::Key_Control, false);
        _keyMouseStates->keyPressed(Qt::Key_Shift, false);
    }

    bool FITKGlobalEventFilter::addedEventFilterProcess(QObject *Object, QEvent *event)
    {
        //执行自定义事件处理
        for (FITKAbstractEventFilter* f : _eventFilters)
        {
            if (f)
            {
                f->eventFilter(Object, event);
//                 if (f->eventFilter(Object, event))
//                 {
//                     // return true;
//                 }
            }
        }

        return false;
    }

}

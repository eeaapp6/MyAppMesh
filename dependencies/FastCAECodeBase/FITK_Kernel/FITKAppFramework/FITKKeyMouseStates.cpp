/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKKeyMouseStates.h"
#include <QDebug>
#include <QMutexLocker>

namespace AppFrame
{
    
     
    void FITKKeyMouseStates::keyPressed(Qt::Key key,bool p)
    {
        //记录键盘按键点击状态
        _keyStates[key] = p;
    }

    bool FITKKeyMouseStates::keyPressed(Qt::Key key)
    {
        //获取键盘按键点击状态
        if (_keyStates.contains(key))
            return _keyStates[key];
        return false;
    }

    void FITKKeyMouseStates::mousePressed(Qt::MouseButton button, bool p)
    {
        //记录鼠标按键点击状态
        _mouseButtonStates[button] = p;
    }

    bool FITKKeyMouseStates::mousePressed(Qt::MouseButton button)
    {
        //获取鼠标按键点击状态
        if (_mouseButtonStates.contains(button))
            return _mouseButtonStates[button];
        return false;
    }

    bool FITKKeyMouseStates::isMousePressed()
    {
        //获取全部状态，有一个键按下则返回真
        QList<bool> states = _mouseButtonStates.values();
        return states.contains(true);
    }

    bool FITKKeyMouseStates::isMouseMoved() const
    {
        return _mouseMoved;
    }

    void FITKKeyMouseStates::mouseMoved(const bool& m)
    {
        _mouseMoved = m;

    }

}

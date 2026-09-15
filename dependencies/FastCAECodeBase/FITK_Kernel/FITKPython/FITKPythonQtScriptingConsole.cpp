/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPythonQtScriptingConsole.h"

#include <QKeyEvent>

namespace Python
{
    FITKPythonQtScriptingConsole::FITKPythonQtScriptingConsole(QWidget * parent, const PythonQtObjectPtr & context, Qt::WindowFlags i) :
        PythonQtScriptingConsole(parent, context, i)
    {

    }

    FITKPythonQtScriptingConsole::~FITKPythonQtScriptingConsole()
    {

    }

    void FITKPythonQtScriptingConsole::keyPressEvent(QKeyEvent * event)
    {
        //Ctrl + Z崩溃
        if ((event->modifiers()& Qt::ControlModifier) != 0 && event->key() == Qt::Key_Z){
            return;
        }
        else {
            PythonQtScriptingConsole::keyPressEvent(event);
        }
    }
}
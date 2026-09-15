/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPythonQtScriptingConsole.h
 * @brief  Python控制台重写
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-03
 *********************************************************************/
#ifndef FITKPythonQtScriptingConsole_H
#define FITKPythonQtScriptingConsole_H

#include <PythonQtScriptingConsole.h>

class QWidget;
class PythonQtObjectPtr;

namespace Python
{
    /**
     * @brief  Python控制台重写
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-03
     */
    class FITKPythonQtScriptingConsole :public PythonQtScriptingConsole
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent               父对象
         * @param[i] context              Python模块
         * @param[i] i                    窗口类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-03
         */
        FITKPythonQtScriptingConsole(QWidget* parent, const PythonQtObjectPtr& context, Qt::WindowFlags i = Qt::WindowFlags());
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-03
         */
        ~FITKPythonQtScriptingConsole() override;
    public slots:
        ;
        /**
         * @brief    键盘事件重写
         * @param[i] event                 事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-03
         */
        void keyPressEvent(QKeyEvent * event) override;
    };
}

#endif
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    MessageMessageTabWidget.h
 * @brief   日志与脚本分页窗口。

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-27
 *********************************************************************/

#ifndef MESSAGETABWIDGET_H
#define MESSAGETABWIDGET_H

#include <QTabWidget>

#include "GUIFrameAPI.h"

namespace GUI
{
    /**
     * @brief   日志与脚本分页窗口。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-27
     */
    class GUIFRAMEAPI MessageTabWidget :public QTabWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief   构造函数。
         * @param   parent：窗口父指针
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-27
         */
        MessageTabWidget(QWidget* parent);

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-27
         */
        ~MessageTabWidget() = default;
    };
}

#endif //!MESSAGETABWIDGET_H

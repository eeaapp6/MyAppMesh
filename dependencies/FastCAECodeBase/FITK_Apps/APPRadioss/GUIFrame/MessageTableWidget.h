/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file MessageTableWidget.h
 * @brief 信息输出窗口与Python工具栏
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-11-05
 * 
 */
#ifndef _MESSAGETAB_Widget_H__
#define _MESSAGETAB_Widget_H__

#include "GUIFrameAPI.h"
#include <QTableWidget>

namespace GUI
{
    /**
     * @brief 信息输出窗口与Python工具栏
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-11-05
     */
    class GUIFRAMEAPI MessageTabWidget :public QTabWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Message Tab Widget object
         * @param[i]  parent         父窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-05
         */
        explicit MessageTabWidget(QWidget* parent);
        /**
         * @brief Destroy the Message Tab Widget object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-05
         */
        virtual ~MessageTabWidget();
    };
}

#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file AIAssistantDockWidget.h
 * @brief AI 助手停靠窗口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2026-05-14
 *
 */
#ifndef __AIASSISTANT_DOCK_WIDGET_H__
#define __AIASSISTANT_DOCK_WIDGET_H__

#include "GUIFrameAPI.h"

#include <QWidget>

namespace GUI
{
    /**
     * @brief AI 助手停靠窗口
     */
    class GUIFRAMEAPI AIAssistantDockWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit AIAssistantDockWidget(QWidget* parent = nullptr);
        virtual ~AIAssistantDockWidget() = default;

        /**
         * @brief 当前停靠窗口是否已成功加载助手界面
         */
        bool isValid() const;

    private:
        bool _valid{ false };
    };
}

#endif

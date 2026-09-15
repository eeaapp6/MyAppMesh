/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 控制台插件声明
 * @file FITKConsoleComponent.h
 * @brief 
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-03-05
 * 
 */
#ifndef _FITKConsoleComponent_H_
#define _FITKConsoleComponent_H_

#include <QObject>
#include "FITKCompMessageWidgetAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace Comp
{
    class FITKConsole;
    /**
     * @brief 控制台插件接口声明
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-03-05
     */
    class CompMesWidgetAPI ConsoleComponent :public QObject,
        public AppFrame::FITKComponentInterface
        
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new Console Component object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-05
         */
        ConsoleComponent(QWidget* parent = nullptr);
        /**
         * @brief Destroy the Console Component object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-05
         */
        ~ConsoleComponent();
        /**
         * @brief 是否接管qt的消息，qdebug等
         * @param[i] takeover 是否接管
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        void takeoverQtMessage(bool takeover = true);
        /**
         * @brief 获取控制台界面
         * @param[i]  indexPort      索引值
         * @return QWidget* 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-05
         */
        QWidget* getWidget(const int indexPort = 0) override;
        /**
         * @brief 获取控制台名称
         * @return QString 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-05
         */
        QString getComponentName() override;
    private:
        /**
         * @brief       端口-窗口字典。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-27
         */
        QHash<int, FITKConsole*> m_widgetHash;
        /**
        * @brief 父对象
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-03-26
        */
        QWidget* _parent{};
        /**
        * @brief 是否接管qt的消息，qdebug等
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-03-26
        */
        bool _takeoverQt = false;
    };
}

#endif

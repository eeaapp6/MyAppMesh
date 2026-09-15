/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKConsole.h
 * @brief 控制台
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-03-04
 *
 */
#ifndef _FITKConsole_H_
#define _FITKConsole_H_

#include "FITKCompMessageWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIObject.h"
#include <QTextEdit>

class QAction;
class QMutex;
class QContextMenuEvent;

namespace Comp
{
    /**
     * @brief 控制台信息类型定义
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-03-04
     */
    enum MessageType
    {
        None = 0,
        Normal,
        Warning,
        Error,
        Info,
    };
    /**
     * @brief 控制台接口类
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-03-04
     */
    class CompMesWidgetAPI FITKConsole :public QTextEdit, public Core::FITKAbstractGUIObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKConsole object
         * @param[i]  parent         My Param doc
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-04
         */
        FITKConsole(bool qtMessage, QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKConsole object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-04
         */
        ~FITKConsole();
        /**
         * @brief 控制台信息显示接口
         * @param[i]  type           信息类型(枚举)
         * @param[i]  message        信息
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-04
         */
        void displayMessage(MessageType type, const QString message);

        /**
         * @brief       设置提示文本颜色。
         * @param[in]   color：文本颜色
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        void setNormalTextColor(QColor color);

        /**
         * @brief       设置警告文本颜色。
         * @param[in]   color：文本颜色
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        void setWarningTextColor(QColor color);

        /**
         * @brief       设置错误文本颜色。
         * @param[in]   color：文本颜色
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        void setErrorTextColor(QColor color);

    protected:
        /**
         * @brief 右键菜单事件重写
         * @param[i]  event          事件对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-04
         */
        void contextMenuEvent(QContextMenuEvent *event) override;
    private:
        /**
         * @brief Qt调试消息回调函数
         * @param[i]  type           消息类型
         * @param[i]  context        调试信息（文件名、行数等）
         * @param[i]  msg            调试内容
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-05
         */
        static void messageCallBack(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    private slots:
        /**
        * @brief 保存控制台信息
        * @author BaGuijun (baguijun@163.com)
        * @date 2024-03-04
        */
        void save();
        /**
        * @brief 控制台信息显示接口
        * @param[i]  type           信息类型(整形)
        * @param[i]  message        信息
        * @author BaGuijun (baguijun@163.com)
        * @date 2024-03-04
        */
        void displayMessage(int type, const QString message);
    private:
        /**
         * @brief 线程锁
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-06
         */
        static QMutex m_mutex;
        /**
         * @brief 是否接管qt输出信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        bool _qtMessage{ false };

        /**
         * @brief       普通文本颜色。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        QColor _normalColor;

        /**
         * @brief       警告文本颜色。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        QColor _warningColor;

        /**
         * @brief       错误文本颜色。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-13
         */
        QColor _errorColor;

    };

}

#endif

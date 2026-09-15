/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractGUI.h
 * @brief GUI控件基类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-02
 *
 */
#ifndef  ___FITKABSTRACTGUI_H__
#define ___FITKABSTRACTGUI_H__

#include <QWidget>
#include <QDialog>
#include <QMainWindow>
#include <QPoint>
#include "FITKAbstractGUIDialog.h"

class QCloseEvent;

namespace Core
{
    class FITKAbstractOperator;
    /**
     * @brief GUI WIDGET声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    class FITKCoreAPI FITKWidget : public QWidget ,public FITKAbstractGUIObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 使用全部基类构造函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        using QWidget::QWidget;
        virtual ~FITKWidget() = 0;
        /**
         * @brief 强制刷新窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-17
         */
        virtual void flush();
        
        /**
         * @brief  获取操作器
         * @param[i]  operName      操作器名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        virtual void initOperator(const QString & operName);

        template<class T>
        /**
         * @brief  获取操作器并进行类型转换
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        T* getTOperator()
        {
            return dynamic_cast<T*>(_oper);
        }

    protected:
        /**
         * @brief 操作器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        FITKAbstractOperator* _oper{};


    };

    /**
     * @brief Mainwindow声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    class FITKCoreAPI FITKMainWindow : public QMainWindow, public FITKAbstractGUIObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 使用全部基类构造函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        using QMainWindow::QMainWindow;
        virtual ~FITKMainWindow() = 0;
        /**
         * @brief 重写关闭事件
         * @param[i]  event         关闭事件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        virtual void closeEvent(QCloseEvent *event) override;
        /**
         * @brief 强制刷新窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-17
         */
        virtual void flush();

    };

    /**
     * @brief Mainwindow声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    class FITKCoreAPI FITKRibbonMainWindow : public QMainWindow, public FITKAbstractGUIObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 使用全部基类构造函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-02
         */
        using QMainWindow::QMainWindow;
        virtual ~FITKRibbonMainWindow() = 0;
        /**
         * @brief 重写关闭事件
         * @param[i]  event         关闭事件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        virtual void closeEvent(QCloseEvent *event) override;
        /**
         * @brief 强制刷新窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-17
         */
        virtual void flush();

    };


}




#endif

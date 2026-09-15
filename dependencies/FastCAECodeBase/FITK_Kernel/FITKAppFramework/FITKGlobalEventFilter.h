/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKGlobalEventFilter.h
 * @brief 全局事件处理
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-03-19
 * 
 */
#ifndef __GLOBALEVENTFILTER_H__
#define __GLOBALEVENTFILTER_H__

#include <QObject>
#include <QList>
#include "FITKAppFrameworkAPI.h"

class QKeyEvent;
class QMouseEvent;

namespace AppFrame
{
    class FITKKeyMouseStates;
    class FITKAbstractEventFilter;
    /**
     * @brief 全局事件处理,捕获键盘，鼠标等
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-03-19
     */
    class FITKAppFrameworkAPI FITKGlobalEventFilter : public QObject
    {
    public:
        /**
         * @brief Construct a new FITKGlobalEventFilter object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        FITKGlobalEventFilter();
        /**
         * @brief Destroy the FITKGlobalEventFilter object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        ~FITKGlobalEventFilter();
        /**
         * @brief 事件过滤
         * @param[i]  Object         事件发出者
         * @param[i]  event          事件
         * @return true 
         * @return false 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        bool eventFilter(QObject *Object, QEvent *event) override;
        /**
         * @brief 追加事件处理器
         * @param[i]  filter         事件处理器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        void addEventFilter(FITKAbstractEventFilter* filter);
    private:
        /**
         * @brief 键盘按下事件
         * @param[i]  e              My Param doc
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        bool keyPressEvent(QObject *Object, QKeyEvent* e);
        /**
         * @brief 键盘释放事件
         * @param[i]  e              键盘事件
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        bool keyReleaseEvent(QObject *Object, QKeyEvent* e);
        /**
         * @brief 鼠标按下事件
         * @param[i]  e              鼠标事件
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        bool mousePressEvent(QObject *Object, QMouseEvent* e);
        /**
         * @brief 鼠标释放事件
         * @param[i]  e              鼠标事件
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        bool mouseReleaseEvent(QObject *Object, QMouseEvent* e);
        /**
         * @brief 鼠标移动
         * @param[i]  e              鼠标事件
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-05-09
         */
        bool mouseMoveEvent(QObject *Object, QMouseEvent* e);
        /**
         * @brief app被激活或者不激活
         * @author libaojun (libaojunqd@foxmai.com)
         * @date 2024-05-23
         */
        void applicationActiveEvent();

        /**
         * @brief 事件过滤
         * @param[i]  Object         事件发出者
         * @param[i]  event          事件
         * @return true
         * @return false
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        bool addedEventFilterProcess(QObject *Object, QEvent *event);
    private:
        /**
         * @brief 键盘鼠标事件
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        FITKKeyMouseStates* _keyMouseStates{};
        /**
         * @brief 自定义事件处理器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        QList<FITKAbstractEventFilter*> _eventFilters{};
    };

}

#endif

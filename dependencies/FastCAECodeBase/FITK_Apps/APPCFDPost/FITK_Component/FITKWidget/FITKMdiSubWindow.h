/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMdiSubWindow.h
 * @brief  多视口组件类的子组件类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-10-18
 *********************************************************************/
#ifndef _FITMdiSubWindow_H
#define _FITMdiSubWindow_H

#include "FITKWidgetAPI.h"
#include <QMdiSubWindow>
#include <QPoint>
#include <QMutex>

namespace Comp
{
    class FITKMdiArea;
    /**
     * @brief 多视口组件类的子组件类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-10-18
     */
    class FITKWidgetAPI FITKMdiSubWindow :public QMdiSubWindow
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKMdiSubWindow object
         * @param[i]  parent         My Param doc
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-18
         */
        explicit FITKMdiSubWindow(QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKMdiSubWindow object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-18
         */
        virtual ~FITKMdiSubWindow();
        /**
         * @brief    设置窗口
         * @param[i] widget 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-22
         */
        void setWidget(QWidget *widget);
        /**
         * @brief    获取窗口
         * @return   QWidget *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-22
         */
        QWidget *widget() const;
        /**
         * @brief    
         * @param[i] parent 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-22
         */
        void setParent(FITKMdiArea* parent);
    signals:
        ;
        /**
         * @brief    窗口最大化信号
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-06-05
         */
        void sigWinStateMax();
    protected:
        /**
         * @brief    初始化窗口
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-22
         */
        void initWidget();
        /**
         * @brief    鼠标点击事件
         * @param[i] event 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-18
         */
        void mousePressEvent(QMouseEvent *event) override;
        /**
         * @brief    鼠标移动事件
         * @param[i] event 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-18
         */
        void mouseMoveEvent(QMouseEvent *event) override;
        /**
         * @brief    鼠标松开事件
         * @param[i] event 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-18
         */
        void mouseReleaseEvent(QMouseEvent *event) override;
        /**
         * @brief    更改事件重写
         * @param[i] changeEvent                修改事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-06-05
         */
        void changeEvent(QEvent *changeEvent) override;
    public slots:
        void widgetExteriorMove();
        void widgetMoveRestore();

    private:
        /**
         * @brief 互斥锁对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-18
         */
        static QMutex _mutex;
        /**
         * @brief  映射，用于从外部移回内部的窗口
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-22
         */
        QWidget* m_widget{};
        FITKMdiArea* m_parent{};
    };
}

#endif

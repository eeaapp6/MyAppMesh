/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKTabWidget.h
 * @brief Tab页组件添加
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-06-07
 * 
 */
#ifndef FITKTabWidget_H_
#define FITKTabWidget_H_

#include <QTableWidget>
#include <QProxyStyle>
#include "FITKWidgetAPI.h"

namespace Comp
{
    enum class FITKTabWidgetType{
        FITKTab_None,       //QtabWidget样式
        FITKTab_Fill,       //填充样式
        FITKTab_Auto,       //自适应大小样式
    };
    /**
     * @brief Tab页组件添加
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-06-07
     */
    class FITKWidgetAPI FITKTabWidget :public QTabWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKTabWidget object
         * @param[i]  parent         My Param doc
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-07
         */
        FITKTabWidget(FITKTabWidgetType type = FITKTabWidgetType::FITKTab_None, QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKTabWidget object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-07
         */
        virtual ~FITKTabWidget();
        /**
         * @brief    查找tab的索引
         * @param[i] tabName   tab名称 
         * @return   int       索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        int findTabIndex(QString tabName);
    protected:
        /**
         * @brief    显示事件
         * @param[i] event  事件对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-02
         */
        void showEvent(QShowEvent * event);
        /**
         * @brief    移动事件
         * @param[i] event  事件对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-02
         */
        void resizeEvent(QResizeEvent * event);
    private:
        /**
         * @brief    修改样式
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-02
         */
        void changeStyleSheet();
    private:
        /**
         * @brief  标签界面样式
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-02
         */
        FITKTabWidgetType _type = FITKTabWidgetType::FITKTab_None;
    };

    class FITKTabBar : public QTabBar 
    {
        Q_OBJECT;
    public:
        FITKTabBar(QWidget *parent = nullptr);  
        FITKTabBar();
    protected:
        /**
         * @brief    函数重写重新设置每个Tab标签大小
         * @param[i] index   Tab标签索引
         * @return   QSize   大小
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-03
         */
        QSize tabSizeHint(int index) const override;
    };
}

#endif
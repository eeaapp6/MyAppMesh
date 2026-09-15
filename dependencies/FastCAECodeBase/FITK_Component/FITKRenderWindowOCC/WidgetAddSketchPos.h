/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        WidgetAddSketchPos.h
 * @brief       添加草图坐标窗口。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-11-18
 *********************************************************************/

#ifndef __WIDGETADDSKETCHPOS_H__
#define __WIDGETADDSKETCHPOS_H__

#include <QDialog>

namespace Ui {
    class WidgetAddSketchPos;
}

// 前置声明。
namespace Render
{
    class FITKGraph3DWindowOCC;
}

namespace GUI 
{
    class WidgetAddSketchPos : public QDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        explicit WidgetAddSketchPos(QWidget *parent = nullptr);

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        ~WidgetAddSketchPos();

        /**
         * @brief       设置渲染窗口。
         * @param[in]   rw：渲染窗口
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        void setRenderWindow(Render::FITKGraph3DWindowOCC* rw);

    private slots:
        /**
         * @brief       槽函数 - 确定添加坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        void slot_confirm();

    private:
        /**
         * @brief       UI.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        Ui::WidgetAddSketchPos* m_Ui{};

        /**
         * @brief       渲染窗口。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        Render::FITKGraph3DWindowOCC* m_rw{ nullptr };

    };
}

#endif // __WIDGETADDSKETCHPOS_H__

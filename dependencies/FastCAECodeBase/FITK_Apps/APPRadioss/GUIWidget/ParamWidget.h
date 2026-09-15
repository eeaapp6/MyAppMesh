/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file ParamWidget.h
 * @brief  参数窗口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-12-28
 * 
 */
#ifndef _PARAMWIDGET_H___
#define _PARAMWIDGET_H___
 
#include "GUIWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace Ui
{
    class ParamWidget;
}

namespace GUI
{
    /**
     * @brief 属性窗口
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-12-28
     */
    class GUIWIDGETAPI ParamWidget : public Core::FITKWidget
    {
    public:
        /**
         * @brief Construct a new Param Widget object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-28
         */
        explicit ParamWidget(QWidget* parent = nullptr);
        /**
         * @brief Destroy the Param Widget object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-28
         */
        virtual ~ParamWidget() = default;
         /**
          * @brief 设置显示的窗口对象
          * @param[i]  widget         显示窗口
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-12-28
          */
        void setWidget(QWidget* widget);
        /**
         * @brief 清空窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-28
         */ 
        void clear();

        QWidget* getCurrentWidget();

    private:
        /**
         * @brief UI对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-28
         */
        Ui::ParamWidget* _ui{};
        /**
         * @brief 当前显示的窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-28
         */
        QWidget* _widget{};
 
    };
}


#endif

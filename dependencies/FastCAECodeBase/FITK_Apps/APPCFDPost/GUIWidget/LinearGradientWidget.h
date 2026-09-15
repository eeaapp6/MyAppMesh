/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   LinearGradientWidget.h
 * @brief  颜色径向渐变界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-25
 *********************************************************************/
#ifndef _LinearGradientWidget_H
#define _LinearGradientWidget_H

#include "GUIWidgetAPI.h"
#include "GUIWidgetBase.h"

namespace Interface {
    class FITKCFDPostColorLibaryData;
}

namespace GUI
{
    /**
     * @brief  颜色径向渐变界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-25
     */
    class GUIWidgetAPI LinearGradientWidget :public GUIWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent      父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        LinearGradientWidget(Interface::FITKCFDPostColorLibaryData* colorLibData, QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        ~LinearGradientWidget();
        /**
         * @brief    更新界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void updateWidget();
    protected:
        void paintEvent(QPaintEvent *event) override;
    private:
        /**
         * @brief  色库数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-29
         */
        Interface::FITKCFDPostColorLibaryData* _colorLibData = nullptr;
    };
}

#endif

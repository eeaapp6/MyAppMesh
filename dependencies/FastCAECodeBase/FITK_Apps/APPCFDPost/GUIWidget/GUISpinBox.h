/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUISpinBox.h
 * @brief  封装SpinBox，解决滚轮问题
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-11
 *********************************************************************/
#ifndef _GUISpinBox_H_
#define _GUISpinBox_H_

#include <QSpinBox>
#include "GUIWidgetAPI.h"

namespace GUI
{
    class GUIWidgetAPI GUISpinBox : public QSpinBox
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent     父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        explicit GUISpinBox(QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        virtual ~GUISpinBox();
    protected:
        void wheelEvent(QWheelEvent *event) override;
    };
}

#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIComboBox.h
 * @brief  封装ComboBox，解决ComboBox滚轮问题
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-11
 *********************************************************************/
#ifndef _GUIComboBox_H_
#define _GUIComboBox_H_

#include <QComboBox>
#include "GUIWidgetAPI.h"

namespace GUI
{
    /**
     * @brief  封装ComboBox，解决ComboBox滚轮问题
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-11
     */
    class GUIWidgetAPI GUIComboBox : public QComboBox
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent      父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        explicit GUIComboBox(QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        virtual ~GUIComboBox();
    protected:
        /**
         * @brief    滚轮事件重写
         * @param[i] e          事件对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void wheelEvent(QWheelEvent *e) override;
    };
}
#endif

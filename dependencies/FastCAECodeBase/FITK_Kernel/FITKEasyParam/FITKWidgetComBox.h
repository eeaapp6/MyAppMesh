/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKWidgetComBox.h
 * @brief combox类型控件
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _GUIWidgetComBox_H
#define _GUIWidgetComBox_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParamWidget.h"
#include <QComboBox>

namespace Core
{
    class FITKAbstractEasyParam;
    class FITKParamCombox;
    /**
     * @brief combox类型控件
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class FITKEasyParamAPI FITKWidgetComBox :public QComboBox, public FITKAbstractEasyParamWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKWidgetComBox object
         * @param[i]  data           数据对象
         * @param[i]  parent         父对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        FITKWidgetComBox(FITKAbstractEasyParam* data, QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKWidgetComBox object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~FITKWidgetComBox();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
    protected:
        /**
         * @brief    鼠标滚轮事件
         * @param[i] event 事件对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        void wheelEvent(QWheelEvent *event) override;
    private slots:
        ;
        /**
         * @brief 数据更改槽函数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void slotDataChange();
    protected:
        /**
         * @brief combox数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        FITKParamCombox* _value = nullptr;
    };
}
#endif

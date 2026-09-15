/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKWidgetString.h
 * @brief 字符串型控件
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _GUIWidgetString_H
#define _GUIWidgetString_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParamWidget.h"
#include <QLineEdit>

namespace Core
{
    class FITKAbstractEasyParam;
    class FITKParamString;

    class FITKEasyParamAPI FITKWidgetString :public QLineEdit, public FITKAbstractEasyParamWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKWidgetString object
         * @param[i]  data           数据对象
         * @param[i]  parent         父对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        FITKWidgetString(FITKAbstractEasyParam* data, QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKWidgetString object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~FITKWidgetString();
        /**
         * @brief 初始化参数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
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
         * @brief 数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        FITKParamString* _value = nullptr;
    };
}

#endif

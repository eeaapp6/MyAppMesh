/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKWidgetBool.h
 * @brief 布尔类型控件
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _GUIWidgetBool_H
#define _GUIWidgetBool_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParamWidget.h"
#include <QCheckBox>

namespace Core
{
    class FITKAbstractEasyParam;
    class FITKParamBool;
    /**
     * @brief 布尔类型控件
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class FITKEasyParamAPI FITKWidgetBool : public QCheckBox ,public FITKAbstractEasyParamWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKWidgetBool object
         * @param[i]  data           数据对象
         * @param[i]  parent         父对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        FITKWidgetBool(FITKAbstractEasyParam* data, QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKWidgetBool object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~FITKWidgetBool();
        /**
         * @brief 初始化
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
    private slots:
        ;
        /**
         * @brief 数据更改
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
        FITKParamBool* _value = nullptr;
    };
}

#endif

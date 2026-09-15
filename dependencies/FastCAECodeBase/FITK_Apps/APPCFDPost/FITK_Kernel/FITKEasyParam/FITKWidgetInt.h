/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKWidgetInt.h
 * @brief 整型类型控件
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _GUIWidgetInt_H
#define _GUIWidgetInt_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParamWidget.h"
#include <QSpinBox>

namespace Interface {
    class FITKAbstractEasyParam;
    class FITKFlowDataInt;
}

namespace Core
{
    class FITKAbstractEasyParam;
    class FITKParamInt;
    /**
     * @brief 整型类型控件
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class FITKEasyParamAPI FITKWidgetInt :public QSpinBox, public FITKAbstractEasyParamWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKWidgetInt object
         * @param[i]  data           数据对象
         * @param[i]  parent         父对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        FITKWidgetInt(FITKAbstractEasyParam* data, QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKWidgetInt object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~FITKWidgetInt();
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
         * @brief 数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        FITKParamInt* _value = nullptr;
    };
}

#endif

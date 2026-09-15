/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKWidgetDouble.h
 * @brief 双精度型控件
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-08-14
 * 
 */
#ifndef _GUIWidgetDouble_H
#define _GUIWidgetDouble_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParamWidget.h"
#include "FITKWidgetSciNotation.h"
#include <QDoubleSpinBox>

namespace Interface {
    class FITKAbstractEasyParam;
    class FITKFlowDataDouble;
}

namespace Core
{
    class FITKAbstractEasyParam;
    class FITKParamDouble;
    /**
     * @brief 双精度型控件
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-08-14
     */
    class FITKEasyParamAPI FITKWidgetDouble :public FITKWidgetSciNotation, public FITKAbstractEasyParamWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new FITKWidgetDouble object
         * @param[i]  data           数据对象
         * @param[i]  parent         父对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        FITKWidgetDouble(FITKAbstractEasyParam* data, QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKWidgetDouble object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        ~FITKWidgetDouble();
        /**
         * @brief 初始化参数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        void init();
    protected:
        //鼠标滚轮事件
        void wheelEvent(QWheelEvent *event) override;
    private slots:
        ;
        /**
         * @brief    数据更改槽函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-27
         */
        void slotDataChangeFinished();
    protected:
        /**
         * @brief 数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-14
         */
        FITKParamDouble* _value = nullptr;
    };
}

#endif

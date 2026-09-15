/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKSciNotationLineEdit.h
 * @brief 科学计数法组件声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-06-11
 * 
 */
#ifndef _FITKSciNotationLineEdit_H
#define _FITKSciNotationLineEdit_H

#include "FITKWidgetAPI.h"
#include <QLineEdit>

namespace Comp
{
    /**
     * @brief 科学计数法组件声明
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-06-11
     */
    class FITKWidgetAPI FITKSciNotationLineEdit : public QLineEdit
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKWidgetSciNotation object
         * @param[i]  parent         My Param doc
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-11
         */
        FITKSciNotationLineEdit(QWidget* parent);
        /**
         * @brief Destroy the FITKWidgetSciNotation object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-11
         */
        virtual ~FITKSciNotationLineEdit();
        /**
         * @brief    设置范围
         * @param[i] range  数据范围
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-27
         */
        void setRange(double* range);
        /**
         * @brief 文本是否是有效的科学计数法
         * @param[i]  message        文本对象
         * @param[o]  value          文本装换为double后的数据
         * @return true   有效
         * @return false   无效
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-11
         */
        static bool isValidText(QString message, double& value);
        /**
         * @brief 获取当前有效数据（字符串类型）
         * @return QString   有效文本
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-11
         */
        QString getCurrentValidText();
        /**
         * @brief 获取当前有效数据（double类型）
         * @param[o]  value          有效数据
         * @return true   有效
         * @return false   无效
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-11
         */
        bool getCurrentValidValue(double& value);
        /**
         * @brief   设置当前有效数据
         * @param[i] value  有效数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-27
         */
        void setCurrentValidValue(double value);

        /**
         * @brief       设置输入数值。
         * @param[in]   val：数值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        void setValue(double val);

        /**
         * @brief       获取数值。
         * @return      数值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        double value();

    signals:
        ;
        /**
         * @brief 当前LineEdit中的数据发生更改且数据有效发送该信号
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-11
         */
        void dataChanged();
    private slots:
        ;
        /**
         * @brief LineEdit中文本发生更改槽函数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-11
         */
        void slot_textChanged();

        /**
         * @brief       输入文本发生编辑。
         * @param[in]   text：输入框文本
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        void slot_textEdited(const QString & text);

    protected:
        /**
         * @brief 当前且科学计数法有效文本
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-11
         */
        QString _currentValidText = "";
        /**
         * @brief  数据范围
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-27
         */
        double _range[2] = { -9e99,9e99 };

        /**
         * @brief       实际数值。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        double _value = 0.;
    };
}

#endif

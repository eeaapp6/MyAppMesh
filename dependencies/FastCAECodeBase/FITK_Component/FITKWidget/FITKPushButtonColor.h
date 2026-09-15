/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKPushButtonColor.h
 * @brief  颜色按钮控件声明
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-04-28
 */
#ifndef _FITKPushButtonColor_H_
#define _FITKPushButtonColor_H_

#include "FITKWidgetAPI.h"

#include <QColor>
#include <QPushButton>

 /**
     * @brief  颜色按钮控件
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-04-28
     */
class FITKWidgetAPI FITKPushButtonColor : public QPushButton
{
    Q_OBJECT;
public:
    /**
     * @brief    构造函数
     * @param[i] parent 父控件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-04-28
     */
    explicit FITKPushButtonColor(QWidget* parent = nullptr);
    /**
     * @brief    析构函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-04-28
     */
    virtual ~FITKPushButtonColor() = default;
    /**
     * @brief    设置当前颜色
     * @param[i] color 颜色
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-04-28
     */
    void setCurrentColor(const QColor& color);
    /**
     * @brief    获取当前颜色
     * @return   当前颜色
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-04-28
     */
    QColor getCurrentColor() const;

signals:
    /**
     * @brief    颜色改变信号
     * @param[i] color 当前颜色
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-04-28
     */
    void sigColorChanged(const QColor& color);

private slots:
    /**
     * @brief    按钮点击槽函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-04-28
     */
    void slotButtonClicked();

private:
    /**
     * @brief    刷新按钮显示颜色
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-04-28
     */
    void updateButtonColor();

private:
    /**
     * @brief    当前颜色
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-04-28
     */
    QColor _currentColor = QColor(Qt::white);
};

#endif // _FITK_PUSH_BUTTON_COLOR_H_
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKComboBox.h
 * @brief  封装qcombobox，添加ListView
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-20
 *
 */
#ifndef __FITK__COMBO_BOX_H___
#define __FITK__COMBO_BOX_H___

#include <QComboBox>
#include "FITKWidgetAPI.h"

/**
 * @brief 封装qcombobox，添加ListView
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-20
 */
class FITKWidgetAPI FITKComboBox : public QComboBox
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new FITKComboBox object
     * @param[i]  parent         父对象
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-20
     */
    explicit FITKComboBox(QWidget* parent = nullptr);
    /**
     * @brief Destroy the FITKComboBox object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-20
     */
    virtual ~FITKComboBox();
    /**
     * @brief    设置是否禁用滚轮事件
     * @param[i] isDisable                  是否禁用
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-07-31
     */
    void setWheelDisable(bool isDisable);
signals:
    ;
    /**
     * @brief    点击信号
     * @param[i]  
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-27
     */
    void clicked(bool);
protected:
    /**
     * @brief    鼠标点击事件重写
     * @param[i] event           事件
     * @author   BaGuijun (baguijun@163.com)
     * @date     2024-11-27
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief    滚轮事件重写
     * @param[i] e                          事件对象
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-07-31
     */
    void wheelEvent(QWheelEvent *e) override;
protected:
    /**
     * @brief  是否禁用滚轮事件
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-07-31
     */
    bool _isDisableWheel = false;
};



#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadioColor.h
 * @brief  单选框与颜色显示框
 * @author fulipeng (fulipengqd@yeah.net)
 * @date   2024-12-24
 *********************************************************************/

#ifndef FITKRADIOCOLOR_H
#define FITKRADIOCOLOR_H

#include "FITKWidgetAPI.h"
#include <QWidget>

namespace Ui
{
    class FITKRadioColor;
}
/**
 * @brief 单选框与颜色显示框
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-12-24
 */
class FITKWidgetAPI FITKRadioColor : public QWidget
{
    Q_OBJECT

public:
    explicit FITKRadioColor(QWidget *parent = nullptr);
    virtual ~FITKRadioColor();

    /**
     * @brief    初始化单选框名称与颜色，是否启用等。
     * @param[i] radioName      :  单选框名称
     * @param[i] backGroundColor:  颜色框颜色
     * @param[i] isEnable       :  是否启用
     * @param[i] isShowColor    :  是否显示颜色
     * @author   fulipeng (fulipengqd@yeah.net)
     * @date     2024-12-24
     */
    void init(QString radioName, QColor backGroundColor, bool isEnable = true, bool isShowColor = true);
    /**
     * @brief    设置启用单选
     * @param[i] isEnable       :  是否启用
     * @author   fulipeng (fulipengqd@yeah.net)
     * @date     2024-12-24
     */
    void setEnable(bool isEnable);
    /**
     * @brief    设置单击选择
     * @param[i] isClick        :  是否选择
     * @author   fulipeng (fulipengqd@yeah.net)
     * @date     2024-12-24
     */
    void setClick(bool isClick);

private:
    Ui::FITKRadioColor *_ui;
};

#endif // FITKRADIOCOLOR_H

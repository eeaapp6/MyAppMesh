/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKWidgetVBox.h
 * @brief  水平布局组件界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-08-22
 *********************************************************************/
#ifndef _FITKWidgetVBox_H
#define _FITKWidgetVBox_H

#include "FITKEasyParamAPI.h"
#include <QWidget>

namespace Ui {
    class FITKWidgetVBox;
}

class QRadioButton;

namespace Core
{
    /**
     * @brief  水平布局组件界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-08-22
     */
    class FITKEasyParamAPI FITKWidgetVBox : public QWidget
    {
        Q_OBJECT;
    public:
        FITKWidgetVBox(QList<QWidget*> widgetList, QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-22
         */
        ~FITKWidgetVBox();
        /**
         * @brief    设置子项界面不可显示
         * @param[i] enable     是否显示
         * @param[i] isFirst    首项是否跳过
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-22
         */
        void setSubWidgetEnable(bool enable, bool isFirst = true);
        /**
         * @brief    获取子界面
         * @return   QList<QWidget*>    子界面（界面顺序为添加的顺序）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        QList<QWidget*> getSubWidget();
    private:
        /**
         * @brief  ui对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-22
         */
        Ui::FITKWidgetVBox* _ui = nullptr;
        /**
         * @brief  子界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-12
         */
        QList<QWidget*> _subWidgets = {};
    };
}

#endif // !_FITKWidgetVBox_H

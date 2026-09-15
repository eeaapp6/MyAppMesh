/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PropertyWidget.h
 * @brief  属性界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-11
 *********************************************************************/
#ifndef _PropertyWidget_H_
#define _PropertyWidget_H_

#include "GUIWidgetAPI.h"
#include "GUIWidgetBase.h"

namespace Ui {
    class PropertyWidget;
}

namespace GUI
{
    class GUIWidgetAPI PropertyWidget : public GUIWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent    父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        PropertyWidget(QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        ~PropertyWidget();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void init();
        /**
         * @brief    更新界面
         * @param[i] postDataID      后处理数据id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void updateCurrentGraphObj(int postDataID);
    private slots:
        ;
        /**
         * @brief    不透明度精度条抬起事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void on_horizontalSlider_Opacity_valueChanged(int value);
        /**
         * @brief    不透明度数据修改事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void on_spinBox_Opacity_editingFinished();

    private:
        /**
         * @brief    更新不透明度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void updateOpacity();
    private:
        /**
         * @brief  ui文件
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        Ui::PropertyWidget* _ui = nullptr;
    };
}

#endif

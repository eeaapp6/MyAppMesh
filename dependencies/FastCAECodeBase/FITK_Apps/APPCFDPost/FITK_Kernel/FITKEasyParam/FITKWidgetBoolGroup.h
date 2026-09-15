/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKWidgetBoolGroup.h
 * @brief  布尔组界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-08-23
 *********************************************************************/
#ifndef _GUIWidgetBoolGroup_H
#define _GUIWidgetBoolGroup_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParamWidget.h"
#include <QWidget>

namespace Ui {
    class FITKWidgetBoolGroup;
}

namespace Core 
{
    class FITKParamBoolGroup;
    /**
     * @brief  布尔组界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-08-23
     */
    class FITKEasyParamAPI FITKWidgetBoolGroup :public QWidget, public FITKAbstractEasyParamWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] dataBase 
         * @param[i] parent
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-23
         */
        FITKWidgetBoolGroup(FITKParamBoolGroup* dataBase, QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-23
         */
        ~FITKWidgetBoolGroup();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-23
         */
        void init();
    private slots:
        ;
        void on_checkBox_clicked();
    private:
        /**
         * @brief    更新界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-23
         */
        void updateWidget();
    private:
        /**
         * @brief  UI
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-23
         */
        Ui::FITKWidgetBoolGroup* _ui = nullptr;
        /**
         * @brief  布尔组数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-23
         */
        FITKParamBoolGroup* _dataBase = nullptr;
    };
}
#endif
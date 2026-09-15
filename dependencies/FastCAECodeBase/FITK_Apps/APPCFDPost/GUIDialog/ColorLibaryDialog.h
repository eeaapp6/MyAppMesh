/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ColorLibaryDialog.h
 * @brief  色库对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-25
 *********************************************************************/
#ifndef _ColorLibaryDialog_H
#define _ColorLibaryDialog_H

#include "GUIDialogAPI.h"
#include "GUIDialogBase.h"

namespace Ui {
    class ColorLibaryDialog;
}

namespace GUI
{
    /**
     * @brief  色库对话框
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-25
     */
    class GUIDialogAPI ColorLibaryDialog :public GUIDialogBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    g构造函数
         * @param[i] oper          操作器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        ColorLibaryDialog(EventOper::ParaWidgetInterfaceOperator* oper);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        ~ColorLibaryDialog();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        void init();
        /**
         * @brief    更新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        void update();
    private slots:
        ;
        /**
         * @brief    色库应用事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void on_pushButton_Apply_clicked();
        /**
         * @brief    色库导入事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void on_pushButton_Import_clicked();
        /**
         * @brief    色库导出事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void on_pushButton_Export_clicked();
        /**
         * @brief    关闭事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void on_pushButton_Close_clicked();
    private:
        /**
         * @brief  ui界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-25
         */
        Ui::ColorLibaryDialog* _ui = nullptr;
    };
}

#endif

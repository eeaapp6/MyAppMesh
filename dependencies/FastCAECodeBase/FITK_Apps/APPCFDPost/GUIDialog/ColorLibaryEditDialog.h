/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ColorLibaryEditDialog.h
 * @brief  颜色编辑对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-29
 *********************************************************************/
#ifndef _ColorLibaryEditDialog_H
#define _ColorLibaryEditDialog_H

#include "GUIDialogAPI.h"
#include "GUIDialogBase.h"

namespace Ui {
    class ColorLibaryEditDialog;
}

namespace Interface {
    class FITKCFDPostColorLibaryData;
}

namespace GUI
{
    class LinearGradientWidget;
    /**
     * @brief  颜色编辑对话框
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-29
     */
    class GUIDialogAPI ColorLibaryEditDialog :public GUIDialogBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] currentDataID  当前数据id 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        ColorLibaryEditDialog(int currentDataID, EventOper::ParaWidgetInterfaceOperator* oper);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        ~ColorLibaryEditDialog();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void init();
    private slots:
        ;
        /**
         * @brief    OK按钮点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    Cancel按钮点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void on_pushButton_Cancel_clicked();
        void on_pushButton_Append_clicked();
        void on_pushButton_Remove_clicked();
        void on_pushButton_Clear_clicked();
    private:
        /**
         * @brief    更新表格
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void updateTableWidget();
    private:
        /**
         * @brief  ui
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-29
         */
        Ui::ColorLibaryEditDialog* _ui = nullptr;
        /**
         * @brief  颜色过渡界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-29
         */
        LinearGradientWidget* _linearWidget = nullptr;

        int _currentDataID = -1;
        /**
         * @brief  色库临时数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-29
         */
        Interface::FITKCFDPostColorLibaryData* _copyColorLibData = nullptr;
    };
}

#endif

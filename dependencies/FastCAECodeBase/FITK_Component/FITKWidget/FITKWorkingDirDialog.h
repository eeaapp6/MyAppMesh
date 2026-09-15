/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKWorkingDirDialog.h
 * @brief 设置工作路径对话框声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2023-06-26
 *
 */
#ifndef _FITKWorkingDirDialog_H__
#define _FITKWorkingDirDialog_H__

#include "FITKWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace Ui {
    class FITKWorkingDirDialog;
}

namespace Comp
{
    /**
     * @brief 设置工作路径对话框
     * @author BaGuijun (baguijun@163.com)
     * @date 2023-06-26
     */
    class FITKWidgetAPI FITKWorkingDirDialog : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        FITKWorkingDirDialog(QWidget* parent);
        virtual ~FITKWorkingDirDialog();

    private slots:
        ;
        /**
         * @brief 选择新路径
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-07
         */
        void on_browseButton_clicked();
        /**
         * @brief 确定按钮事件
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-07
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief 取消按钮事件
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-07
         */
        void on_pushButton_Cancel_clicked();
    private:
        /**
         * @brief  UI
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-07
         */
        Ui::FITKWorkingDirDialog* _ui = nullptr;
    };
}  // namespace GUI

#endif
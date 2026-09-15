/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIEOSTypeDialog.h
 * @brief  状态方程类型选择对话框声明
 * @author wangning (2185896382@qq.com)
 * @date   2025-05-23
 *********************************************************************/
#ifndef _GUIEOSTypeDialog_H
#define _GUIEOSTypeDialog_H

#include <QDialog>
#include "GUIAnalysisDialogAPI.h"

namespace Ui
{
    class GUIEOSTypeDialog;
}
namespace GUI
{
    /**
     * @brief 状态方程类型选择对话框
     */
    class GUIAnalysisDialogAPI GUIEOSTypeDialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit GUIEOSTypeDialog(QWidget* parent = nullptr);
        ~GUIEOSTypeDialog();

        /**
         * @brief 获取用户选择的类型
         * @return 类型
         */
        int selectedEOSType() const;
        /**
         * @brief    获取名称
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        QString getEOSName();

    private slots:

        /**
        * @brief OK按钮信号
        * @return
        */
        void on_pushButtonOK_clicked();
        /**
        * @brief cancle按钮信号
        * @return
        */
        void on_pushButtonCancel_clicked();

    private:
        Ui::GUIEOSTypeDialog* _ui{};
    };
}

#endif

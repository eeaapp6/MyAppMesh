/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIContactCreateDialog.h
 * @brief  创建接触窗口类
 * @author wangning (2185896382@qq.com)
 * @date   2026-02-06
 *********************************************************************/
#ifndef _GUIContactCreateDialog_H
#define _GUIContactCreateDialog_H


#include <QDialog>
#include "GUIAnalysisDialogAPI.h"

namespace Ui
{
    class GUIContactCreateDialog;
}

namespace GUI
{
    /**
     * @brief 材料类型选择对话框
     */
    class GUIAnalysisDialogAPI GUIContactCreateDialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit GUIContactCreateDialog(QWidget* parent = nullptr);
        ~GUIContactCreateDialog();

        /**
         * @brief 获取用户选择的连接类型
         * @return 连接类型
         */
        int selectedContactType() const;
        /**
         * @brief    获取连接名称
         */
        QString getContactName();

    private slots:
        /**
        * @brief OK按钮信号
        * @return
        */
        void on_pushButton_OK_clicked();
        /**
        * @brief cancle按钮信号
        * @return
        */
        void on_pushButton_Cancel_clicked();

    private:
        /**
         * @brief  ui
         */
        Ui::GUIContactCreateDialog* _ui{};
    };
}

#endif

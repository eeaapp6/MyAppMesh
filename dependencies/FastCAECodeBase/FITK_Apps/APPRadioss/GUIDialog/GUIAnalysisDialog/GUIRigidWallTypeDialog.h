/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIRigidWallTypeDialog.h
 * @brief  创建刚性壁类型选择对话框
 * @author guqingtao (15598887859@163.com)
 * @date   2025-08-26
 *********************************************************************/
#ifndef _GUIRigidWallTypeDialog_H
#define _GUIRigidWallTypeDialog_H

#include <QDialog>
#include "GUIAnalysisDialogAPI.h"

namespace Ui
{
    class GUIRigidWallTypeDialog;
}

namespace GUI
{
    /**
     * @brief  创建刚性壁类型选择对话框
     * @author guqingtao (15598887859@163.com)
     * @date   2025-08-26
     */
    class GUIAnalysisDialogAPI GUIRigidWallTypeDialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit GUIRigidWallTypeDialog(QWidget* parent = nullptr);
        ~GUIRigidWallTypeDialog();
        /**
         * @brief    获取选择的刚性壁类型
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        int selectedRigidWallType() const;
        /**
         * @brief    获取设置的刚性壁名字
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        QString getRigidWallName();
    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        void on_pushButtonOK_clicked();
        /**
         * @brief    取消按钮
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        void on_pushButtonCancel_clicked();
        /**
         * @brief    下拉框改变
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        void on_comboBox_type_currentIndexChanged(int index);

    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        Ui::GUIRigidWallTypeDialog* _ui{};
    };
}

#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUICreateSolutionDialog.h
 * @brief  解决方案确认
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-17
 *********************************************************************/
#ifndef _GUICreateSolutionDialog_H
#define _GUICreateSolutionDialog_H

#include <QDialog>
#include "GUIAnalysisDialogAPI.h"

namespace Ui
{
    class GUICreateSolutionDialog;
}

namespace GUI
{
    /**
     * @brief 材料类型选择对话框
     */
    class GUIAnalysisDialogAPI GUICreateSolutionDialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit GUICreateSolutionDialog(QWidget* parent = nullptr);
        ~GUICreateSolutionDialog();

        /**
         * @brief    获取解决方案名称
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-17
         */
        QString getSolutionName();

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

        Ui::GUICreateSolutionDialog* _ui{};
    };
}

#endif

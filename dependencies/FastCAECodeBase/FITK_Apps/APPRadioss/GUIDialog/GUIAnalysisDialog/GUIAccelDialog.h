/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIAccelDialog.h
 * @brief  加速度计对话框声明
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-23
 *********************************************************************/
#ifndef _GUIAccelDialog_H
#define _GUIAccelDialog_H

#include <QDialog>
#include "GUIAnalysisDialogAPI.h"

namespace Ui
{
    class GUIAccelDialog;
}
namespace Core
{
    class FITKAbstractOperator;
    class FITKActionOperator;
}
namespace Radioss
{
    class FITKProbeAcceleration;
}
namespace GUI
{
    /**
     * @brief 加速度计对话框声明
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-23
     */
    class GUIAnalysisDialogAPI GUIAccelDialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit GUIAccelDialog(Radioss::FITKProbeAcceleration* accel, Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        ~GUIAccelDialog();

    private:
        //初始化UI
        void init();
        //初始化集合
        void initSet();
        //写入Python脚本
        void writePythonScript(const QString& oldName = QString());

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
        /**
        * @brief 拾取按钮信号
        * @return
        */
        void on_pushButtonPick_clicked();

        //刷新下拉框
        void onComboBoxAboutToShow(Core::FITKAbstractOperator * oper);

    private:
        Ui::GUIAccelDialog* _ui{};

        Radioss::FITKProbeAcceleration* _accel{};

        Core::FITKActionOperator* _oper = nullptr;
    };
}

#endif

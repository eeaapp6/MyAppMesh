/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIPropertySandwitchLayerDialog.h
 * @brief  夹层属性层设置对话框声明
 * @author wangning (2185896382@qq.com)
 * @date   2025-11-04
 *********************************************************************/
#ifndef _GUIPropertySandwitchLayerDialog_H
#define _GUIPropertySandwitchLayerDialog_H

#include <QDialog>
#include "GUIAnalysisDialogAPI.h"
class QComboBox;

namespace Ui
{
    class GUIPropertySandwitchLayerDialog;
}
namespace Radioss
{
    class FITKPropSandwichShell;
}
namespace GUI
{
    /**
     * @brief 夹层属性层设置对话框声明
     */
    class GUIAnalysisDialogAPI GUIPropertySandwitchLayerDialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit GUIPropertySandwitchLayerDialog(Radioss::FITKPropSandwichShell* obj,QWidget* parent = nullptr);
        ~GUIPropertySandwitchLayerDialog();

    private:
        void init();
        QComboBox* createMatBox(int matID= -1);

    private slots:
        /**
        * @brief OK按钮信号
        * @return
        */
        void on_pushButtonOK_clicked();
        /**
        * @brief cancel按钮信号
        * @return
        */
        void on_pushButtonCancel_clicked();
        /**
        * @brief add按钮信号
        * @return
        */
        void on_pushButtonAdd_clicked();
        /**
        * @brief deleta按钮信号
        * @return
        */
        void on_pushButtonDelete_clicked();

    private:
        Ui::GUIPropertySandwitchLayerDialog* _ui;
        Radioss::FITKPropSandwichShell* _obj{};
    };
}

#endif

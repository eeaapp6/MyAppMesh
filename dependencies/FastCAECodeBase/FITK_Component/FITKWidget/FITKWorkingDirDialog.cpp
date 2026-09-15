/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWorkingDirDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "ui_FITKWorkingDirDialog.h"

#include <QFileDialog>
#include <QMessageBox>

namespace Comp
{
    FITKWorkingDirDialog::FITKWorkingDirDialog(QWidget* parent) :
        Core::FITKDialog(parent)
    {
        _ui = new Ui::FITKWorkingDirDialog;
        _ui->setupUi(this);
        ///获取工作路径
        AppFrame::FITKAppSettings* settings = FITKAPP->getAppSettings();
        if (settings == nullptr) return;
        QString wk = settings->getWorkingDir();
        //设置到界面
        _ui->workingDirlineEdit->setText(wk);
        _ui->currentWorkingDirLabel->setText(wk);
    }

    FITKWorkingDirDialog::~FITKWorkingDirDialog()
    {
        if (_ui != nullptr) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void FITKWorkingDirDialog::on_browseButton_clicked()
    {
        //获取工作路径
        AppFrame::FITKAppSettings* settings = FITKAPP->getAppSettings();
        if (settings == nullptr) {
            QMessageBox::critical(this, tr("Error"), tr("AppSetting is nullprt!"), tr("OK"));
            return;
        }
        QString wk = settings->getWorkingDir();
        //弹出对话框
        QString dir = QFileDialog::getExistingDirectory(this, tr("Working Dir"), wk);
        if (dir.isEmpty()) return;
        _ui->workingDirlineEdit->setText(dir);
    }

    void FITKWorkingDirDialog::on_pushButton_OK_clicked()
    {
        //点击确定
        QString d = _ui->workingDirlineEdit->text();
        AppFrame::FITKAppSettings* settings = FITKAPP->getAppSettings();
        if (settings == nullptr) {
            QMessageBox::critical(this, tr("Error"), tr("AppSetting is nullprt!"), tr("OK"));
            return;
        }

        //存储信息
        settings->setWorkingDir(d);

        this->accept();
    }

    void FITKWorkingDirDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }
}  // namespace GUI
/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file LicenseCheckDialog.cpp
 * @brief 许可证检查对话框的实现文件
 */

#include "FITKLicenseFileCheckDialog.h"
#include "ui_FITKLicenseFileCheckDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractLicenseChecker.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QFileDialog>
#include <QClipboard>


namespace Comp
{   
    /**
 * @brief 构造函数实现
 * @param checker 许可证检查器指针
 * @param errorMessage 错误信息
 * 
 * 初始化对话框，设置窗口属性，显示系统标识符和错误信息
 */
FITKLicenseFileCheckDialog::FITKLicenseFileCheckDialog(AppFrame::FITKAbstractLicenseChecker* checker, 
    const QString& errorMessage, const QStringList& licenseCopyto)
    : QDialog(nullptr), _licenseCopyto(licenseCopyto), _checker(checker)
{
    _ui = new Ui::FITKLicenseFileCheckDialog;

    // 禁用窗口关闭按钮，强制用户通过对话框按钮进行操作
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);
    _ui->setupUi(this);
    _ui->labelError->setText(errorMessage);

    if(_checker == nullptr) return;

    // 获取并显示系统标识符
    const QString& id = _checker->getSystemIdentifier();
    _ui->lineEditIden->setEnabled(false);
    _ui->lineEditIden->setText(id);
}

/**
 * @brief 析构函数实现
 * 
 * 清理UI资源
 */
FITKLicenseFileCheckDialog::~FITKLicenseFileCheckDialog()
{
    if(_ui)
    {
        delete _ui;
        _ui = nullptr;
    }
}

/**
 * @brief 文件浏览器按钮点击事件处理
 * 
 * 打开文件选择对话框，仅允许选择.lic格式的许可证文件
 */
void FITKLicenseFileCheckDialog::on_BrowserButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString("Select a license file"), "", 
                       QString("License File (%1)").arg(_licenseFileSuffix));
    if(fileName.isEmpty()) return;
    _ui->lineEditLicenseFile->setText(fileName);
}

/**
 * @brief 复制按钮点击事件处理
 * 
 * 将系统标识符复制到系统剪贴板
 */
void FITKLicenseFileCheckDialog::on_CopyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(_ui->lineEditIden->text());
}

/**
 * @brief 导入按钮点击事件处理
 * 
 * 导入许可证文件并复制到应用程序目录
 * 设置许可证检查器的许可证文件路径
 */
void FITKLicenseFileCheckDialog::on_ImportButton_clicked()
{
    QString fileName = _ui->lineEditLicenseFile->text();
    if(fileName.isEmpty() || !QFile::exists(fileName)) return;

    if(_licenseCopyto.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "No license copy path specified.");
        this->reject();
        return;
    }

    // 拷贝文件到指定路径
    for(const QString& dir : _licenseCopyto)
    {
        // 检查目录是否存在，如果不存在则创建
        if(!Core::CreateDir(dir))
        {
            QMessageBox::warning(this, "Warning", QString("Failed to create directory: %1").arg(dir));
            this->reject();
            return;
        }
        QFile::remove(dir + "/" + _licenseFileName);
        QFile::copy(fileName, dir + "/" + _licenseFileName);
        // 检查文件是否成功复制
        if(!QFile::exists(dir + "/" + _licenseFileName))
        {
            QMessageBox::warning(this, "Warning", QString("Failed to copy license file to: %1").arg(dir));
            this->reject();
            return;
        }
    }

    this->accept();
}

// void FITKLicenseFileCheckDialog::reject()
// {
//     QApplication::quit();
// }

/**
 * @brief 退出按钮点击事件处理
 * 
 * 关闭应用程序
 */
void FITKLicenseFileCheckDialog::on_QuitBitton_clicked()
{
    QApplication::quit();   
}

void FITKLicenseFileCheckDialog::setLicenseFileName(const QString &fileName)
{
    _licenseFileName = fileName;
}

void FITKLicenseFileCheckDialog::setLicenseFileSuffix(const QString &fileSuffix)
{
    _licenseFileSuffix = fileSuffix;
}
void FITKLicenseFileCheckDialog::canBeCanceled(bool canBeCanceled)
{
    if(canBeCanceled)
    {
        // 不禁用窗口关闭按钮
        this->setWindowFlags(this->windowFlags() &  Qt::WindowCloseButtonHint);
    }
    else
    {
        // 禁用窗口关闭按钮，强制用户通过对话框按钮进行操作
        this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);
    }
}
}

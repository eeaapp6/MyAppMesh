/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file LicenseCheckDialog.h
 * @brief 许可证检查对话框的头文件
 */

#ifndef LICENSECHECKDIALOG_H
#define LICENSECHECKDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include "FITKWidgetAPI.h"

namespace Ui 
{
    class FITKLicenseFileCheckDialog;
}

namespace AppFrame
{
    class FITKAbstractLicenseChecker;
}


namespace Comp
{
        /**
     * @class LicenseCheckDialog
     * @brief 许可证检查对话框类，用于处理软件许可证的验证和导入
     * 
     * 该类提供了一个用户界面，允许用户：
     * 1. 查看系统标识符
     * 2. 复制系统标识符
     * 3. 选择并导入许可证文件
     * 4. 退出应用程序
     */

    class FITKWidgetAPI FITKLicenseFileCheckDialog : public QDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief 构造函数
         * @param checker 许可证检查器指针
         * @param errorMessage 错误信息
         * @param licenseCopyto 许可证复制路径列表 注意输入的是目录的绝对路径
         */
        explicit FITKLicenseFileCheckDialog(AppFrame::FITKAbstractLicenseChecker* checker, const QString& errorMessage, 
                                    const QStringList& licenseCopyto);
        
        /**
         * @brief 析构函数
         */
        virtual ~FITKLicenseFileCheckDialog();
        
        /**
         * @brief 设置许可证文件名称，用于导入许可证文件复制后重命名
         * 
         * @param fileName 许可证文件名称
         */
        void setLicenseFileName(const QString& fileName);
        /**
         * @brief 设置许可证文件后缀名
         * 
         * @param fileSuffix 许可证文件后缀名。例如*.lic   *.lic1;;*.lic2
         * @note 该后缀名用于文件浏览器选择许可证文件时的过滤器
         */
        void setLicenseFileSuffix(const QString& fileSuffix) ;
        /**
         * @brief 设置是否可以取消对话框
         * 
         * @param canBeCanceled 是否可以取消对话框
          
          * @note 该函数用于设置对话框是否可以被取消，默认情况下对话框是不可以被取消的
         */
        void canBeCanceled(bool canBeCanceled)  ;
        
    private slots: 
        /**
         * @brief 复制系统标识符到剪贴板
         */
        void on_CopyButton_clicked();
        
        /**
         * @brief 打开文件浏览器选择许可证文件
         */
        void on_BrowserButton_clicked();
        
        /**
         * @brief 退出应用程序
         */
        void on_QuitBitton_clicked();
        
        /**
         * @brief 导入许可证文件
         */
        void on_ImportButton_clicked();

//     private:
//         void reject() override;

    private:
        Ui::FITKLicenseFileCheckDialog *_ui{};        ///< UI界面指针
        AppFrame::FITKAbstractLicenseChecker* _checker{};  ///< 许可证检查器指针
        QStringList _licenseCopyto{}; ///< 许可证复制路径列表
        QString _licenseFileName{"license.lic"}; ///< 许可证文件路径
        QString _licenseFileSuffix{"*.lic"}; ///< 许可证文件后缀名
    };



}

#endif // LICENSECHECKDIALOG_H

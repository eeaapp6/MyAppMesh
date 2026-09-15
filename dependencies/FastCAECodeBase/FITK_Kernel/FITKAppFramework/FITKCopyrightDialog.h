/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKCopyrightDialog.h
 * @brief 版权信息展示
 * @author libaojun (libaojunqd@Foxmail.com)
 * @date 2024-05-16
 *
 */
#ifndef __FITKCOPYRIGHT_H___
#define __FITKCOPYRIGHT_H___

#include <QDialog>
#include <QString>

namespace Ui
{
    class DlgCopyright;
}

class QTextBrowser;
/**
 * @brief 第三方库版权信息窗口
 * @author libaojun (libaojunqd@Foxmail.com)
 * @date 2024-05-16
 */
class FITKCopyrigntInfoDlg : public QDialog
{
public:
    /**
     * @brief Construct a new FITKCopyrigntInfoDlg object
     * @param[i]  parent        父亲对象
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-05-16
     */
    FITKCopyrigntInfoDlg(QWidget* parent = nullptr);
    /**
     * @brief Destroy the FITKCopyrigntInfoDlg object
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-05-16
     */
    virtual ~FITKCopyrigntInfoDlg() = default;
    /**
     * @brief 设置显示字符内容
     * @param[i]  text           显示字符内容
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-05-16
     */
    void setText(const QString & text);

private:
    /**
     * @brief 窗口对象
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-05-16
     */
    QTextBrowser* _text{};
};


class FITKCopyRightDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new FITKCopyRightDialog object
     * @param[i]  parent         父对象
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-05-16
     */
    explicit FITKCopyRightDialog(QWidget* parent = nullptr);
    /**
     * @brief Destroy the FITKCopyRightDialog object
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-05-16
     */
    virtual ~FITKCopyRightDialog();

private slots:
    /**
     * @brief 表格被点击
     * @param[i]  r              行索引
     * @param[i]  w              列索引
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-05-16
     */
    void onTableItemClickedSlot(int r, int w);

private:
    /**
     * @brief 查看第row行的许可证
     * @param[i]  row           行号
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-05-16
     */
    void browseLicense(int row);
    /**
     * @brief 打开网址
     * @param[i]  row            点击对象行号
     * @param[i]  col            点击对象列号
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-05-16
     */
    void openUrl(int row, int col);

private:
    /**
     * @brief UI对象
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-05-16
     */
    Ui::DlgCopyright* _ui{};
};

#endif

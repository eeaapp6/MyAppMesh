/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCopyrightDialog.h"
#include "ui_FITKCopyrightDialog.h"
#include <QTextBrowser>
#include <QFile>
#include <QDesktopServices>

/**
 * 构造函数: 创建FITKCopyrigntInfoDlg对话框
 *
 * @param parent 父QWidget，默认为nullptr。指定了此对话框的父级窗口。
 */
FITKCopyrigntInfoDlg::FITKCopyrigntInfoDlg(QWidget* parent /*= nullptr*/)
{
    // 创建并设置网格布局
    auto gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    // 创建文本浏览器并添加到布局中
    _text = new QTextBrowser(this);
    gridLayout->addWidget(_text, 0, 0, 1, 1);
    // 设置文本浏览器作为对话框的主要内容
}

/**
 * 设置文本框的文本内容。
 *
 * @param text 要设置的文本内容，类型为QString。
 *
 * 该函数首先检查_text指针是否为nullptr，如果是，则直接返回，
 * 避免空指针解引用。接着清空当前的文本内容，然后设置新的文本。
 */
void FITKCopyrigntInfoDlg::setText(const QString & text)
{
    if (_text == nullptr) return; // 检查指针是否为空，防止空指针异常

    _text->clear(); // 清除当前文本框的内容
    _text->setText(text); // 设置新的文本内容
}

/**
 * 构造函数：FITKCopyRightDialog
 * 用于创建一个版权信息对话框。
 *
 * @param parent 父QWidget，默认为nullptr。指定了此对话框的父级窗口。
 */
FITKCopyRightDialog::FITKCopyRightDialog(QWidget* parent /*= nullptr*/)
    :QDialog(parent)
{
    // 初始化UI界面
    _ui = new Ui::DlgCopyright;
    _ui->setupUi(this);

    // 设置表格头部的自适应拉伸模式
    _ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 遍历表格，设置单元格文本的居中对齐
    const int nR = _ui->tableWidget->rowCount();
    const int nC = _ui->tableWidget->columnCount();

    for (int i = 0; i < nR; i++)
    {
        for (int j = 0; j < nC; j++)
        {
            _ui->tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }

    // 连接表格单元格双击事件到相应槽
    connect(_ui->tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onTableItemClickedSlot(int, int)));

    // 禁止表格的编辑触发器
    _ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->tabWidget->setCurrentIndex(0);
}

FITKCopyRightDialog::~FITKCopyRightDialog()
{
    if (_ui) delete _ui;
}



void FITKCopyRightDialog::onTableItemClickedSlot(int row, int col)
{
    if (col == 1)
    {
        this->browseLicense(row);
    }
    else if (col == 3)
    {
        this->openUrl(row, col);
    }
    return;
}

/**
 * 浏览指定行的许可证信息。
 *
 * @param row 表格中的行号，指定要浏览许可证信息的行。
 *
 * 该函数首先从表格中获取指定行的文本项，然后使用这个文本作为文件名的一部分来查找许可证文件。
 * 如果文件存在且可以打开，其内容将被读取，并在一个新的对话框中显示给用户。
 */
void FITKCopyRightDialog::browseLicense(int row)
{
    QTableWidgetItem* item = _ui->tableWidget->item(row, 0); // 尝试获取指定行的首项
    if (item == nullptr) return; // 如果项不存在，则直接返回
    QString text = item->text(); // 获取项的文本
    QString file = QString(":/License/License/%1.txt").arg(text); // 生成许可证文件的路径，位于资源文件
    QFile f(file);
    if (!f.exists()) return; // 如果文件不存在，直接返回
    if(!f.open(QIODevice::ReadOnly)) return; // 如果无法以只读模式打开文件，直接返回
    QString alltext = f.readAll(); // 读取文件的全部内容
    f.close(); // 关闭文件
    FITKCopyrigntInfoDlg dlg(this); // 创建显示许可证信息的对话框
    dlg.setText(alltext); // 设置对话框中显示的内容为文件内容
    dlg.setWindowTitle(text); // 设置对话框的标题为文件名
    dlg.exec(); // 显示对话框并等待用户交互
}

/**
 * @brief 打开指定单元格中的URL。
 *
 * 该函数首先获取指定行和列的单元格项，如果该项不存在则返回。接着，检查单元格内容是否以"http"开头，
 * 如果不是，则不进行任何操作。最后，若满足条件，则使用QDesktopServices打开该URL。
 *
 * @param row 行索引，指定要操作的单元格所在的行。
 * @param col 列索引，指定要操作的单元格所在的列。
 */
void FITKCopyRightDialog::openUrl(int row, int col)
{
    // 尝试获取指定位置的单元格项
    QTableWidgetItem* item = _ui->tableWidget->item(row, col);
    if (item == nullptr) return; // 如果单元格项不存在，则直接返回

    QString text = item->text(); // 获取单元格项的文本内容
    if (!text.startsWith("http")) return; // 如果文本不以"http"开头，则不进行后续操作

    // 使用QDesktopServices打开URL
    QDesktopServices::openUrl(QUrl(text));
}


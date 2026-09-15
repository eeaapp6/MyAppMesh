/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   TableWidgetFileReadFileDialog.cpp
 * @brief  TimePoint文件读取对话框
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-04-19
 */
#include "TableWidgetFileReadFileDialog.h"
#include "ui_TableWidgetFileReadFileDialog.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

namespace Comp
{
    TableWidgetFileReadFileDialog::TableWidgetFileReadFileDialog(QList< int > range, QWidget* parent) : QDialog(parent),
        _ui(new Ui::TableWidgetFileReadFileDialog)
    {
        _ui->setupUi(this);
        if (range.size() == 4)
        {
            _totalRow = range.at(0);
            _totalColumn = range.at(1);
            _startRow = range.at(2) + 1;
            _startColumn = range.at(3) + 1;
        }
        _ui->le_row->setText(QString("%1").arg(_startRow));
        _ui->le_column->setText(QString("%1").arg(_startColumn));
    }

    TableWidgetFileReadFileDialog::~TableWidgetFileReadFileDialog()
    {
        delete _ui;
    }
    /**
     * @brief 文件选择按钮槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-08-14
     */
    void TableWidgetFileReadFileDialog::on_pb_file_clicked()
    {
        QString file = QFileDialog::getOpenFileName(this, tr("ASCII File Selection"));
        if (!file.isEmpty())
        {
            _ui->le_file->setText(file);
        }
    }
    /**
     * @brief 获取文件路径
     * @return 选择的文件路径
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-08-14
     */
    QString TableWidgetFileReadFileDialog::getFile() const
    {
        return _filepath;
    }
    /**
     * @brief 获取开始填充的行数
     * @return 开始填充的行数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-08-14
     */
    int TableWidgetFileReadFileDialog::getStartRow() const
    {
        return _startRow;
    }
    /**
     * @brief 获取开始填充的列数
     * @return 开始填充的列数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-08-14
     */
    int TableWidgetFileReadFileDialog::getStartColumn() const
    {
        return _startColumn;
    }
    /**
     * @brief ok按钮槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-08-14
     */
    void TableWidgetFileReadFileDialog::on_pb_ok_clicked()
    {
        QFileInfo fileInfo(_ui->le_file->text());
        if (!fileInfo.exists())
        {
            QMessageBox::critical(this, tr("Error Information"), tr("File does not exist."), tr("OK"));
            return;
        }
        if (!fileInfo.isFile())
        {
            QMessageBox::critical(this, tr("Error Information"), QString(tr("%1: File not found.")).arg(fileInfo.absoluteFilePath()), tr("OK"));
            return;
        }
        bool    ok;
        QString row = _ui->le_row->text();
        int     rowInt = row.toInt(&ok);
        if (!ok || rowInt <= 0 || rowInt > _totalRow)
        {
            QMessageBox::critical(this, tr("Error Information"), QString(tr("Specified start row(%1) is out of range.")).arg(rowInt), tr("OK"));
            return;
        }
        QString column = _ui->le_column->text();
        int     columnInt = column.toInt(&ok);
        if (!ok || columnInt <= 0 || columnInt > _totalColumn)
        {
            QMessageBox::critical(this, tr("Error Information"), QString(tr("Specified start column(%1) is out of range.")).arg(columnInt), tr("OK"));
            return;
        }

        _filepath = fileInfo.absoluteFilePath();
        _startRow = rowInt;
        _startColumn = columnInt;
        this->accept();
    }
    /**
     * @brief 取消按钮槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-08-14
     */
    void TableWidgetFileReadFileDialog::on_pb_cancel_clicked()
    {
        this->reject();
    }
}  // namespace GUI

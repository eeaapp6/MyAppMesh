/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTreeTableWidget.h"
#include <QHeaderView>

namespace Comp
{
    FITKTreeTableWidget::FITKTreeTableWidget(QWidget* parent /*= nullptr*/)
        :QTableWidget(parent)
    {
        QHeaderView *header = this->verticalHeader();
        if (header) header->setHidden(true);

        //水平布局策略
        header = this->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::Stretch);
        this->resizeColumnsToContents();
    }



    FITKTreeTableWidget::~FITKTreeTableWidget()
    {
        this->clear();
    }

     
    int FITKTreeTableWidget::appendTreeLevelItem(const QString& text, const bool expandable,
        const int level /*= 0*/, const bool mergeLine /*= false */)
    {
        //根据层次调整缩进
        QString label;
        for (int i = 0; i < level; ++i) label += "  ";
        label += text;

        //创建首列对象
        QTableWidgetItem* item = new QTableWidgetItem(label);
        item->setData(Qt::UserRole, level);

        //追加一行
        const int colCount = this->columnCount();
        const int rowCount = this->rowCount();
        this->insertRow(rowCount);

        //添加到表格
        this->setItem(rowCount, 0, item);

        //合并列
        if (mergeLine)
            this->setSpan(rowCount, 0, 1, colCount);
        //返回当前行号
        return rowCount;
    }

    void FITKTreeTableWidget::setItemText(const int row, const int col, const QString& text)
    {
        //错误判断
        const int colCount = this->columnCount();
        const int rowCount = this->rowCount();
        if (row < 0 || row >= rowCount) return;
        if (col < 0 || col >= colCount) return;
        //新建对象
        QTableWidgetItem* item = new QTableWidgetItem(text);
        this->setItem(row, col, item);
    }

}


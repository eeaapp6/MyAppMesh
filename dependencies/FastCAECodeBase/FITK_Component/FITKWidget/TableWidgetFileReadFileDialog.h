/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   TableWidgetFileReadFileDialog.h
 * @brief  TimePoint文件读取对话框
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-04-19
 */
#ifndef __TABLEWIDGETFILEREADFILEDIALOG_H__
#define __TABLEWIDGETFILEREADFILEDIALOG_H__

#include "FITKWidgetAPI.h"
#include <QDialog>

namespace Ui
{
    class TableWidgetFileReadFileDialog;
}

namespace Comp
{
    /**
     * @brief TimePoint文件读取对话框类
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-08-14
     */
    class FITKWidgetAPI TableWidgetFileReadFileDialog : public QDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief 构造函数
         * @param range 时间点范围
         * @param parent 父窗口
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        TableWidgetFileReadFileDialog(QList<int> range, QWidget *parent = nullptr);
        /**
         * @brief 析构函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        virtual ~TableWidgetFileReadFileDialog();
        /**
         * @brief 获取文件路径
         * @return 选择的文件路径
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        QString getFile() const;
        /**
         * @brief 获取开始填充的行数
         * @return 开始填充的行数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        int getStartRow() const;
        /**
         * @brief 获取开始填充的列数
         * @return 开始填充的列数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        int getStartColumn() const;
    private slots:
        /**
         * @brief 文件选择按钮槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        void on_pb_file_clicked();
        /**
         * @brief ok按钮槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        void on_pb_ok_clicked();
        /**
         * @brief 取消按钮槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        void on_pb_cancel_clicked();

    private:
        /**
         * @brief ui对象
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        Ui::TableWidgetFileReadFileDialog *_ui{};
        /**
         * @brief 文件路径
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        QString _filepath{};
        /**
         * @brief 总行数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        int _totalRow{};
        /**
         * @brief 总列数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        int _totalColumn{};
        /**
         * @brief 起始行数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        int _startRow{};
        /**
         * @brief 起始列数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        int _startColumn{};
    };
} // namespace GUI

#endif // !__TABLEWIDGETFILEREADFILEDIALOG_H__

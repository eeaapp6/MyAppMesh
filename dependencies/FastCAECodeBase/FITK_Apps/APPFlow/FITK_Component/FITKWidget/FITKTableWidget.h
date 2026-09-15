/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKTableWidget.h
 * @brief
 * @author YanZhiHui(chanyuantiandao@126.com)
 * @date   2024-10-21
 */
#ifndef FITKTableWidget_H_
#define FITKTableWidget_H_

#include <QTableWidget>
#include <QClipboard>
#include <QModelIndex>
#include "FITKWidgetAPI.h"

namespace Comp
{
    /**
    * @brief 自定义表格组件类
    * @author YanZhiHui (chanyuantiandao@126.com)
    * @date 2024-04-22
    */
    class FITKWidgetAPI FITKTableWidget : public QTableWidget
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造函数
         * @param parent 父对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        explicit FITKTableWidget(QWidget *parent = nullptr);
        /**
         * @brief 析构函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        virtual ~FITKTableWidget();
        /**
         * @brief 设置按回车键添加一行
         * @param b 是否添加
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        void setAppendLineByEnterPressed(bool b);
        /**
         * @brief 设置表格是否是单行模式（默认为false）
         * @param[i]  isAlone       true：开启 false：关闭
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-06-07
         */
        void setIsAloneRow(bool isAlone);
        /**
         * @brief 表格增加一行
         * @param[in] rowNum 增加行的索引
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        void addTableRowAndItem(int rowNum);
        /**
         * @brief 移除表格所有的数据行
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        void removeAllRows();

    signals:
        /**
         * @brief 单元格按回车键被按下
         * @param index 单元格索引
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        void cellEnterPressed(QModelIndex index);

    public slots:
        /**
         * @brief 右键菜单响应槽函数
         * @param[in] 点击位置
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-07-26
         */
        virtual void customContextMenuRequestedSlot(QPoint);
        /**
         * @brief 右键菜单“剪切”响应槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-07-26
         */
        virtual void cutActionSlot();
        /**
         * @brief 右键菜单“拷贝”响应槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-07-26
         */
        virtual void copyActionSlot();
        /**
         * @brief 右键菜单“粘贴”响应槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-07-26
         */
        virtual  void pasteActionSlot();
        /**
         * @brief 右键菜单“在之前插入”响应槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-07-26
         */
        virtual void insertBeforeActionSlot();
        /**
         * @brief 右键菜单“在之后插入”响应槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-07-26
         */
        virtual void insertAfterActionSlot();
        /**
         * @brief 右键菜单“删除行”响应槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-07-26
         */
        virtual void deleteRowActionSlot();
        /**
         * @brief 右键菜单“清空内容”响应槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-07-26
         */
        virtual void clearContentsActionSlot();
        /**
         * @brief 右键菜单“清理表格”响应槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-07-26
         */
        virtual void clearTableActionSlot();
        /**
         * @brief 右键菜单“读取文件”响应槽函数
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-07-26
         */
        virtual void readFileActionSlot();
    protected:
        /**
         * @brief 重写按键事件
         * @param event 事件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        virtual void keyPressEvent(QKeyEvent *event) override;
        /**
         * @brief 重写按键释放事件
         * @param event 事件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        virtual void keyReleaseEvent(QKeyEvent *event) override;
        /**
         * @brief  重写方法，屏蔽编辑框的右键菜单
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2024-05-14
         */
        virtual bool edit(const QModelIndex &index, EditTrigger trigger = QAbstractItemView::SelectedClicked, QEvent *event = nullptr) override;

    private:
        /**
         * @brief 按回车键添加一行
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        bool _isAppendLineByEnterPressed{ true };
        /**
         * @brief 右键菜单
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        QMenu *_tableWidgetContextMenu{};
        /**
         * @brief 剪切板对象
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-14
         */
        QClipboard *_clipboard{};
        /**
         * @brief 是否为单行模式标记符
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-22
         */
        bool _isAloneRow = false;
    };
}

#endif
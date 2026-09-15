/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTableWidget.h"
#include <QKeyEvent>
#include <QLineEdit>
#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QItemDelegate>
#include "TableWidgetFileReadFileDialog.h"

namespace Comp
{

    class FITKTableWidgetLineEdit : public QLineEdit {
    public:
        FITKTableWidgetLineEdit(QWidget* parent = nullptr) : QLineEdit(parent) {
            setContextMenuPolicy(Qt::NoContextMenu);
            setStyleSheet("border{none;background-color: #CDCDCD;color: #F2F2F2;}");
        };
        ~FITKTableWidgetLineEdit() = default;

    protected:
        /**
         * @brief 重写按键事件
         * @param event 事件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        virtual void keyPressEvent(QKeyEvent *event) override {
            if ((event->modifiers()& Qt::ControlModifier) != 0 && (event->key() == Qt::Key_X || event->key() == Qt::Key_C || event->key() == Qt::Key_V)) {
                event->ignore();
                //QApplication::postEvent(this->parent(), event);
            }
            else {
                QLineEdit::keyPressEvent(event);
            }
        }

        /**
         * @brief 重写按键释放事件
         * @param event 事件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-22
         */
        virtual void keyReleaseEvent(QKeyEvent *event) override {
            if ((event->modifiers()& Qt::ControlModifier) != 0 && (event->key() == Qt::Key_X || event->key() == Qt::Key_C || event->key() == Qt::Key_V)) {
                event->ignore();
                //QApplication::postEvent(this->parent(), event);
            }
            else {
                QLineEdit::keyReleaseEvent(event);
            }
        }
    };

    class TableWidgetItemDelegate : public QItemDelegate {
    protected:
        // 创建编辑器
        virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
        {
            // 创建自己需要的控件进行返回
            QLineEdit *editor = new FITKTableWidgetLineEdit(parent);
            return editor;
        }
        // 设置编辑器数据
        virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override {
            // 将参数editor转换为对应创建的控件，再进行数据初始设置就行
            QLineEdit *cob = static_cast<QLineEdit *>(editor);
            QString value = index.model()->data(index, Qt::EditRole).toString();
            cob->setText(value);
        }
        // 更新编辑器集合属性
        virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override
        {
            // 将编辑器设置为矩形属性
            editor->setGeometry(option.rect);
        }
        // 设置模型数据
        virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
        {
            QLineEdit *comboBox = static_cast<FITKTableWidgetLineEdit *>(editor); // 类型转换
            // 模型（单元格）显示的数据
            model->setData(index, comboBox->text());
        }
    };

    /**
     * @brief 构造函数
     * @param parent 父类对象
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date 2024-04-22
     */
    FITKTableWidget::FITKTableWidget(QWidget *parent) : QTableWidget(parent)
    {
        setEditTriggers(QAbstractItemView::AllEditTriggers);
        setContextMenuPolicy(Qt::CustomContextMenu);
        _clipboard = QApplication::clipboard();
        setItemDelegate(new TableWidgetItemDelegate);
        connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequestedSlot(QPoint)));
    }
    /**
     * @brief 析构函数
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date 2024-04-22
     */
    FITKTableWidget::~FITKTableWidget()
    {
    }
    /**
     * @brief 是否通过回车操作增加行
     * @param b 布尔值
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date 2024-04-22
     */
    void FITKTableWidget::setAppendLineByEnterPressed(bool b)
    {
        _isAppendLineByEnterPressed = b;
    }

    void FITKTableWidget::setIsAloneRow(bool isAlone)
    {
        _isAloneRow = isAlone;
        _isAppendLineByEnterPressed = !_isAloneRow;
    }

    void FITKTableWidget::addTableRowAndItem(int rowNum)
    {
        if (_isAloneRow && rowCount() > 0) return;
        int columnNum = columnCount();
        insertRow(rowNum);
        for (int i = 0; i < columnNum; ++i)
        {
            setItem(rowNum, i, new QTableWidgetItem);
        }
    }

    void FITKTableWidget::removeAllRows() {
        if (rowCount() > 0) {
            removeRow(0);
        }
    }

    /**
     * @brief 右键菜单响应槽函数
     * @param[in] 点击位置
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-07-26
     */
    void FITKTableWidget::customContextMenuRequestedSlot(QPoint)
    {
        delete _tableWidgetContextMenu;
        _tableWidgetContextMenu = new QMenu();
        QAction* cutAction = new QAction(_tableWidgetContextMenu);
        cutAction->setText(tr("Cut"));
        cutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
        connect(cutAction, SIGNAL(triggered()), this, SLOT(cutActionSlot()));
        _tableWidgetContextMenu->addAction(cutAction);
        QAction* copyAction = new QAction(_tableWidgetContextMenu);
        copyAction->setText(tr("Copy"));
        copyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
        connect(copyAction, SIGNAL(triggered()), this, SLOT(copyActionSlot()));
        _tableWidgetContextMenu->addAction(copyAction);
        QAction* pasteAction = new QAction(_tableWidgetContextMenu);
        pasteAction->setText(tr("Paste"));
        pasteAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
        connect(pasteAction, SIGNAL(triggered()), this, SLOT(pasteActionSlot()));
        _tableWidgetContextMenu->addAction(pasteAction);
        _tableWidgetContextMenu->addSeparator();

        if (!_isAloneRow) {
            QAction* insertBeforeAction = new QAction(_tableWidgetContextMenu);
            insertBeforeAction->setText(tr("Insert Row Before"));
            connect(insertBeforeAction, SIGNAL(triggered()), this, SLOT(insertBeforeActionSlot()));
            _tableWidgetContextMenu->addAction(insertBeforeAction);
            QAction* insertAfterAction = new QAction(_tableWidgetContextMenu);
            insertAfterAction->setText(tr("Insert Row After"));
            connect(insertAfterAction, SIGNAL(triggered()), this, SLOT(insertAfterActionSlot()));
            _tableWidgetContextMenu->addAction(insertAfterAction);
            QAction* deleteRowAction = new QAction(_tableWidgetContextMenu);
            deleteRowAction->setText(tr("Delete Rows"));
            connect(deleteRowAction, SIGNAL(triggered()), this, SLOT(deleteRowActionSlot()));
            _tableWidgetContextMenu->addAction(deleteRowAction);
            _tableWidgetContextMenu->addSeparator();
        }

        QAction* clearContentsAction = new QAction(_tableWidgetContextMenu);
        clearContentsAction->setText(tr("Clear Contents"));
        connect(clearContentsAction, SIGNAL(triggered()), this, SLOT(clearContentsActionSlot()));
        _tableWidgetContextMenu->addAction(clearContentsAction);
        QAction* clearTableAction = new QAction(_tableWidgetContextMenu);
        clearTableAction->setText(tr("Clear Table"));
        connect(clearTableAction, SIGNAL(triggered()), this, SLOT(clearTableActionSlot()));
        _tableWidgetContextMenu->addAction(clearTableAction);
        _tableWidgetContextMenu->addSeparator();
        QAction* readFileAction = new QAction(_tableWidgetContextMenu);
        readFileAction->setText(tr("Read from file..."));
        readFileAction->setIcon(QIcon(":/icons/icoR_fileOpen.xpm"));
        connect(readFileAction, SIGNAL(triggered()), this, SLOT(readFileActionSlot()));
        _tableWidgetContextMenu->addAction(readFileAction);
        _tableWidgetContextMenu->addSeparator();

        _tableWidgetContextMenu->exec(QCursor::pos());
    }
    /**
     * @brief 右键菜单“剪切”响应槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-07-26
     */
    void FITKTableWidget::cutActionSlot()
    {
        QTableWidgetItem* tableWidgetItem = currentItem();
        if (tableWidgetItem == nullptr) return;
        _clipboard->setText(tableWidgetItem->text());
        tableWidgetItem->setText("");
    }
    /**
     * @brief 右键菜单“拷贝”响应槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-07-26
     */
    void FITKTableWidget::copyActionSlot()
    {
        QTableWidgetItem* tableWidgetItem = currentItem();
        if (tableWidgetItem == nullptr) return;
        _clipboard->setText(tableWidgetItem->text());
    }
    /**
     * @brief 右键菜单“粘贴”响应槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-07-26
     */
    void FITKTableWidget::pasteActionSlot()
    {
        QTableWidgetItem* tableWidgetItem = currentItem();
        if (tableWidgetItem == nullptr) return;
        auto txt = _clipboard->text();
        if (txt.contains('\t')) {
            QStringList rowTxts = txt.split('\n');
            int currentRow = tableWidgetItem->row();
            int currentCol = tableWidgetItem->column();
            int totalCol = columnCount();
            int maxRow = _isAloneRow ? 1 : rowTxts.size();
            for (int i = 0; i < maxRow; ++i) {
                auto row = currentRow + i;
                if (row >= rowCount()) {
                    addTableRowAndItem(row);
                }

                auto colTxts = rowTxts.at(i).split('\t');
                for (int j = 0; j < colTxts.size(); ++j) {
                    auto col = currentCol + j;

                    // 存在隐藏列的情况处理
                    int span{ 0 };
                    for (int k = 0; k <= col; ++k) {
                        if (isColumnHidden(k)) {
                            ++span;
                        }
                    }
                    col += span;


                    if (col >= totalCol) break;
                    auto it = item(row, col);
                    if (it == nullptr) break;
                    it->setText(colTxts.at(j));
                }
            }
        }
        else {
            tableWidgetItem->setText(txt);
        }
    }
    /**
     * @brief 右键菜单“在之前插入”响应槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-07-26
     */
    void FITKTableWidget::insertBeforeActionSlot()
    {
        addTableRowAndItem(currentRow());
    }
    /**
     * @brief 右键菜单“在之后插入”响应槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-07-26
     */
    void FITKTableWidget::insertAfterActionSlot()
    {
        addTableRowAndItem(currentRow() + 1);
    }
    /**
     * @brief 右键菜单“删除行”响应槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-07-26
     */
    void FITKTableWidget::deleteRowActionSlot()
    {
        if (rowCount() <= 1)
        {
            QTableWidgetItem* it = item(currentRow(), 0);
            if (it != nullptr) it->setText("");
        }
        else
        {
            removeRow(currentRow());
        }
    }
    /**
     * @brief 右键菜单“清空内容”响应槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-07-26
     */
    void FITKTableWidget::clearContentsActionSlot()
    {
        QTableWidgetItem* tableWidgetItem = currentItem();
        if (tableWidgetItem == nullptr) return;
        tableWidgetItem->setText("");
    }
    /**
     * @brief 右键菜单“清理表格”响应槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-07-26
     */
    void FITKTableWidget::clearTableActionSlot()
    {
        int rowNum = rowCount();
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < rowNum; ++j)
            {
                QTableWidgetItem* tableWidgetItem = item(j, i);
                if (tableWidgetItem == nullptr) continue;
                tableWidgetItem->setText("");
            }
        }
    }
    /**
     * @brief 右键菜单“读取文件”响应槽函数
     * @author yanzhihui (chanyuantiandao@126.com)
     * @date 2023-07-26
     */
    void FITKTableWidget::readFileActionSlot()
    {
        // 获取当前选择的表格，作为填充数据的开始位置
        QModelIndex index = currentIndex();
        int         totalRow = rowCount();
        int         totalColumn = columnCount();
        if (!index.isValid()) return;
        // 打开文件选择对话框
        auto amplitudeReadFileDialog = new TableWidgetFileReadFileDialog({ totalRow, totalColumn, index.row(), index.column() }, nullptr);
        if (amplitudeReadFileDialog->exec() != QDialog::Accepted) return;
        // 从打开文件对话框中获取最新数据
        QString filePath = amplitudeReadFileDialog->getFile();
        int     startRow = amplitudeReadFileDialog->getStartRow();
        int     startColumn = amplitudeReadFileDialog->getStartColumn();
        // 文件是否可以正常打开
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, tr("Error Information"), QString(tr("Error to read file: %1")).arg(filePath), tr("OK"));
            return;
        }

        char   buf[1024];
        qint64 lineLength = file.readLine(buf, sizeof(buf));
        // 读取文本行数
        int currentFileLine = 1;
        // 当前行的总列数
        int     columnNum = 0;
        QString errMessage{};
        // 序列号转换索引号
        int i = startRow - 1;
        while (lineLength != -1)
        {
            // 如果最大索引号超过了当前索引值，则插入新行
            if (totalRow - 1 < i)
            {
                addTableRowAndItem(i);
            }
            // 用正则匹配数据
            QStringList resultList = QString(buf).split(QRegExp("[\\t|,|\\s]"), QString::SkipEmptyParts);
            columnNum = resultList.size();
            // 文件中的数据列是否超出表格有效列
            if (errMessage.isEmpty() && columnNum > totalColumn - startColumn + 1)
            {
                errMessage = QString(tr("The data read from the file were truncated.\nRow %1 int the file contains %2 columns of data, which cannot fit starting in the table column %3."))
                    .arg(currentFileLine)
                    .arg(resultList.size())
                    .arg(startColumn);
            }
            if (startColumn == 1 && columnNum > 0)
            {
                setItem(i, 0, new QTableWidgetItem(resultList.at(0)));
                if (columnNum > 1)
                {
                    setItem(i, 1, new QTableWidgetItem(resultList.at(1)));
                }
            }
            else if (startColumn == 2 && columnNum > 0)
            {
                setItem(i, 1, new QTableWidgetItem(resultList.at(0)));
            }
            ++i;
            lineLength = file.readLine(buf, sizeof(buf));
            ++currentFileLine;
        }
        if (!errMessage.isEmpty())
        {
            QMessageBox::warning(this, tr("Warning Information"), errMessage, tr("OK"));
        }
    }
    /*
     * 处理键盘按下事件，特定按键行为实现。
     *
     * 参数:
     *   event - QKeyEvent*，指向键盘事件的指针。
     *
     * 说明:
     *   仅当按下的是Enter或Return键时，才执行特定逻辑。
     */
    void FITKTableWidget::keyPressEvent(QKeyEvent *event)
    {
        // 获取按键值，只处理enter和return键
        int key = event->key();
        if (key == Qt::Key_Enter || key == Qt::Key_Return)
        {
            // 获取当前选中单元格的索引
            QModelIndex index = currentIndex();
            if (!index.isValid())
                return;
            int row = index.row();
            int column = index.column();

            int cc = columnCount();
            // 如果当前列不是最后一列，则移动到下一列
            if (column < columnCount() - 1)
            {
                int nextVisiablecolumn = column + 1;
                while (nextVisiablecolumn < columnCount())
                {
                    if (!isColumnHidden(nextVisiablecolumn))
                    {
                        setCurrentCell(row, nextVisiablecolumn);
                        edit(index.siblingAtColumn(nextVisiablecolumn));
                        return;
                    }
                    ++nextVisiablecolumn;
                }
            }
            // 如果当前行为非最后一行，则移动到下一行
            if (row < rowCount() - 1)
            {
                setCurrentCell(row + 1, 0);
                edit(index.sibling(row + 1, 0));
            }
            // 如果配置了按下Enter键添加新行，则添加新行
            else if (_isAppendLineByEnterPressed)
            {
                insertRow(rowCount());
                for (int i = colorCount() - 1; i >= 0; --i)
                {
                    setItem(rowCount(), i, new QTableWidgetItem(""));
                }
                setCurrentCell(row + 1, 0);
                edit(index.sibling(row + 1, 0));
            }
        }
        else if ((event->modifiers()& Qt::ControlModifier) != 0 && key == Qt::Key_X) {
            cutActionSlot();
        }
        else if ((event->modifiers()& Qt::ControlModifier) != 0 && key == Qt::Key_C) {
            copyActionSlot();
        }
        else if ((event->modifiers()& Qt::ControlModifier) != 0 && key == Qt::Key_V) {
            pasteActionSlot();
        }
        else {
            QTableWidget::keyPressEvent(event);
        }
    }
    /**
     * @brief 释放按钮时执行父类函数
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date 2024-04-22
     */
    void FITKTableWidget::keyReleaseEvent(QKeyEvent *event)
    {
        QTableWidget::keyReleaseEvent(event);
    }

    bool FITKTableWidget::edit(const QModelIndex & index, EditTrigger trigger, QEvent * event)
    {
        auto result = QTableWidget::edit(index, trigger, event);
        QList<QLineEdit*> lineEdits = findChildren<QLineEdit*>();
        for (int i = 0; i < lineEdits.size(); i++)
        {
            // 重写方法，屏蔽编辑框的右键菜单
            QLineEdit* target = lineEdits.at(i);
            target->setContextMenuPolicy(Qt::NoContextMenu);
        }
        return result;
    }


} // namespace GUI

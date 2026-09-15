/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ComboboxEventOperator.h"
#include <QWidget>

namespace EventOper
{
    /**
     * ComboboxEventOperator 类的构造函数
     * 无参数构造函数，初始化 ComboboxEventOperator 对象。
     */
    ComboboxEventOperator::ComboboxEventOperator()
    {
    }

    /**
     * ComboboxEventOperator 类的析构函数
     * 释放 ComboboxEventOperator 对象所占用的资源。
     */
    ComboboxEventOperator::~ComboboxEventOperator()
    {
    }

    /**
     * 检测组合框是否发生变化
     * @return 返回一个布尔值，当组合框内容变化时返回 true，否则返回 false。
     */
    bool ComboboxEventOperator::comboboxChanged()
    {
        return false;
    }

    /**
     * 设置父级小部件
     * @param w 指向父级小部件的指针。
     */
    void ComboboxEventOperator::setParentWidget(QWidget *w)
    {
        _parentWidget = w;
    }

    /**
     * 设置当前选中的索引
     * @param index 当前选中项的索引。
     */
    void ComboboxEventOperator::setCurrentIndex(int index)
    {
        _currentIndex = index;
    }

    /**
     * 设置主窗口
     * @param mw 指向主窗口的指针。
     */
    void ComboboxEventOperator::setMainWindow(GUI::MainWindow *mw)
    {
        _mainWindow = mw;
    }
} // namespace EventOper
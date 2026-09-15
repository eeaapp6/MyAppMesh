/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsBase.h"
#include "GUIFrame/MainWindow.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

#include <QAction>

namespace OperModel
{
    /**
     * @brief OperatorsBase的构造函数
     *
     * 该构造函数初始化OperatorsBase类的一个实例。它通过动态类型转换从FITKAPP的全局数据中获取GUI::MainWindow的指针，并将其赋值给_mainWindow成员变量。
     *
     * @param args 构造函数参数列表（当前未列出具体参数，表示可能有可选的参数传递给构造函数）
     */
    OperatorsBase::OperatorsBase(/* args */)
    {
        _mainWindow = dynamic_cast<GUI::MainWindow *>(FITKAPP->getGlobalData()->getMainWindow());
    }

    /**
     * @brief OperatorsBase的析构函数
     *
     * 该析构函数释放OperatorsBase类实例的资源，是一个空函数体。
     */
    OperatorsBase::~OperatorsBase()
    {

    }

    void OperatorsBase::preArgs()
    {
        QAction* current = dynamic_cast<QAction*>(_emitter);
        if (current == nullptr)return;

        _isChecked = current->isChecked();
        _senderName = current->objectName();
    }
} // namespace ModelOper
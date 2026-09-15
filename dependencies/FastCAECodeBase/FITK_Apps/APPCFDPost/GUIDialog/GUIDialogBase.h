/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIDialogBase.h
 * @brief  对话框基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-12
 *********************************************************************/
#ifndef _GUIDialogBase_H
#define _GUIDialogBase_H

#include "GUIDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace Interface {
    class FITKCFDPost3DManager;
    class PostGraphObjectManager;
    class FITKCFDPostColorLibaryDataManager;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    class MainWindow;
    /**
     * @brief  对话框基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-12
     */
    class GUIDialogAPI GUIDialogBase : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] oper      操作器对象
         * @param[i] parent    父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        GUIDialogBase(EventOper::ParaWidgetInterfaceOperator* oper, QString dialogTitle, QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        ~GUIDialogBase();
        /**
         * @brief    刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void reRender();
    protected:
        /**
         * @brief  操作对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-12
         */
        EventOper::ParaWidgetInterfaceOperator* _oper = nullptr;
        Interface::FITKCFDPost3DManager* _postDataManager = nullptr;

        Interface::PostGraphObjectManager* _postGraphManager = nullptr;
        Interface::FITKCFDPostColorLibaryDataManager* _colorLibManager = nullptr;

        MainWindow* _mainWin = nullptr;
    };
}

#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ExtremumDialog.h
 * @brief  极值对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-12
 *********************************************************************/
#ifndef _ExtremumDialog_H
#define _ExtremumDialog_H

#include "GUIDialogAPI.h"
#include "GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostEnum.h"

namespace Ui {
    class ExtremumDialog;
}

namespace GUI
{
    /**
     * @brief  极值对话框
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-12
     */
    class GUIDialogAPI ExtremumDialog : public GUIDialogBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] dataID       数据id
         * @param[i] oper         操作器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        ExtremumDialog(int dataID, EventOper::ParaWidgetInterfaceOperator* oper);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        ~ExtremumDialog();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        void init();
        /**
         * @brief    更新界面
         * @param[i] fieldName     场量名称 
         * @param[i] fieldType     场量类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-14
         */
        void updateWidget(QString fieldName, Interface::FITKPostFieldType fieldType);
    private slots:
        /**
         * @brief  场量切换事件
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-14
         */
        void on_comboBox_Field_currentIndexChanged(int index);
    private:
        /**
         * @brief  ui
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-12
         */
        Ui::ExtremumDialog* _ui = nullptr;
        /**
         * @brief  当前数据对象id
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-24
         */
        int _currentDataID = -1;
    };
}

#endif

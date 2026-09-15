/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   SwitchCFDPostDialog.h
 * @brief  边界参数创建对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-08-26
 *********************************************************************/
#ifndef _SwitchCFDPostDialog_H
#define _SwitchCFDPostDialog_H

#include "GUIWidget/GUIDialogBase.h"
#include "GUICalculateDialogAPI.h"

namespace Ui {
    class SwitchCFDPostDialog;
}

namespace Interface {
    class FITKOFPhysicsData;
    class FITKFlowPhysicsHandlerFactory;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    /**
     * @brief  边界参数创建对话框
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-08-26
     */
    class GUICalculateDialogAPI SwitchCFDPostDialog : public GUIDialogBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] oper 操作器对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        SwitchCFDPostDialog(EventOper::ParaWidgetInterfaceOperator* oper);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        ~SwitchCFDPostDialog();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        void init();
    protected:
    private slots:
		void on_btn_Import_clicked();
        void on_pushButton_OK_clicked();

        void on_pushButton_Cancel_clicked();
	private:
		void loadPath();
		void savePath();
    private:
        /**
         * @brief  ui
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-26
         */
        Ui::SwitchCFDPostDialog* _ui = nullptr;
        /**
         * @brief  操作器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-26
         */
        EventOper::ParaWidgetInterfaceOperator* _oper = nullptr;
        /**
         * @brief  物理数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-21
         */
        Interface::FITKOFPhysicsData* _physicsData = nullptr;
        /**
         * @brief  工厂对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-21
         */
        Interface::FITKFlowPhysicsHandlerFactory* _factoryData = nullptr;
    };
}

#endif

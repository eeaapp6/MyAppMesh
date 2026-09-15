/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PHengLEINewProjectDialog.h
 * @brief  边界参数创建对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-08-26
 *********************************************************************/
#ifndef _NewProjectDialog_H
#define _NewProjectDialog_H 

#include "GUIWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace Ui {
    class NewProjectDialog;
}

namespace Interface {
    class FITKPHengLEIData;
    class FITKPhysicsHandlerFactory;
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
    class GUIWIDGETAPI NewProjectDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:


        /**
         * @brief    构造函数
         * @param[i] oper 操作器对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        NewProjectDialog();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        ~NewProjectDialog();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        void init();
    protected:
        /**
         * @brief    隐藏事件重写
         * @param[i] event 事件对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        void hideEvent(QHideEvent *event) override;
        /**
         * @brief    显示事件重写
         * @param[i] event 事件对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        void showEvent(QShowEvent *event) override;

    private slots:
		void on_btn_import_clicked();
		void on_pushButton_Start_clicked();
		void on_pushButton_Cancel_clicked();
    private:
        /**
         * @brief  ui
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-26
         */
        Ui::NewProjectDialog* _ui = nullptr;
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
        Interface::FITKPHengLEIData* _physicsData = nullptr;
        /**
         * @brief  工厂对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-21
         */
        Interface::FITKPhysicsHandlerFactory* _factoryData = nullptr;


		QPushButton *btn_import = nullptr;  // 添加btn_import按钮指针的声明

		/*QLineEdit *lineWorkingDirectory = nullptr;*/

    };
}

#endif

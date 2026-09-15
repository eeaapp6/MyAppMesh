/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PHengLEIPHengLEIBoundaryCreateDialog.h
 * @brief  边界参数创建对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-08-26
 *********************************************************************/
#ifndef _PHengLEIBoundaryCreateDialog_H
#define _PHengLEIBoundaryCreateDialog_H

#include "GUIWidget/GUIDialogBase.h"
#include "GUICalculateDialogAPI.h"

namespace Ui {
    class PHengLEIBoundaryCreateDialog;
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
    class GUICalculateDialogAPI PHengLEIBoundaryCreateDialog : public GUIDialogBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] oper 操作器对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        PHengLEIBoundaryCreateDialog(EventOper::ParaWidgetInterfaceOperator* oper);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        ~PHengLEIBoundaryCreateDialog();
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
        void on_comboBox_Boundary_activated(int index);

        void on_pushButton_OK_clicked();

        void on_pushButton_Cancel_clicked();
    private:
        /**
         * @brief    高亮网格边界
         * @param[i] meshBoundID   网格边界id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        void highlightMeshBoundary(int meshBoundID);
    private:
        /**
         * @brief  ui
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-08-26
         */
        Ui::PHengLEIBoundaryCreateDialog* _ui = nullptr;
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
    };
}

#endif

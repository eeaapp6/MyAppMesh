/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    GUIBatchSettingDialog.h
 * @brief   批量设置窗口类
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-24
 *********************************************************************/
#ifndef __GUI_BATCHSETTING_DIALOG_H__
#define __GUI_BATCHSETTING_DIALOG_H__

#include "GUIToolDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"

namespace Core {
    class FITKActionOperator;
}
namespace Ui {
    class GUIBatchSettingDialog;
}
namespace GUI
{
    /**
     * @brief  重命名窗口类
     * @author lilongyuan(lilongyuan@diso.cn)
     * @date   2025-09-24
     */
    class GUIToolDialogAPI GUIBatchSettingDialog : public GUIDialogBase
    {
        Q_OBJECT
    public:
        /**
         * @brief     构造函数
         * @param[i]  oper 操作器指针
         * @param[i]  parent 父窗口指针
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-24
         */
        GUIBatchSettingDialog(Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-24
         */
        ~GUIBatchSettingDialog();
        /**
         * @brief     构造函数
         * @param[i]  model 模式
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-24
         */
        void setModel(int model);
        /**
         * @brief   界面初始化
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-24
         */
        void init();
    private slots:
        /**
         * @brief   确定按钮点击事件
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-24
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief   取消按钮点击事件
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-24
         */
        void on_pushButton_Cancel_clicked();
    private:
        /**
         * @brief   ui指针
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-24
         */
        Ui::GUIBatchSettingDialog* _ui = nullptr;
        /**
         * @brief   操作器指针
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-24
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief   模式标志位，0为材料，1为属性
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-24
         */
        int _model = -1;
    };
}
#endif  // __GUI_BATCHSETTING_DIALOG_H__
/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    GUIInitialFieldTypeDialog.h
 * @brief   初始化场类型选择对话框
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-02
 *********************************************************************/
#ifndef __GUI_INITIAL_FIELD_TYPE_DIALOG_H__
#define __GUI_INITIAL_FIELD_TYPE_DIALOG_H__

#include "GUIAnalysisDialogAPI.h"
#include <QDialog>

namespace Ui
{
    class GUIInitialFieldTypeDialog;
}

namespace Radioss
{
    class FITKInitialFieldManager;
}

namespace GUI
{
    /**
     * @brief   初始化场类型选择对话框
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-02
     */
    class GUIAnalysisDialogAPI GUIInitialFieldTypeDialog : public QDialog
    {
        Q_OBJECT
    public:
        /**
         * @brief     构造函数
         * @param[i]  parent 父窗口
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-02
         */
        explicit GUIInitialFieldTypeDialog(QWidget* parent = nullptr);
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        ~GUIInitialFieldTypeDialog();
        /**
         * @brief   获取数据名字
         * @return  QString 数据名称
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        QString getDataName();
        /**
         * @brief   获取数据类型
         * @return  int 数据类型
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        int getDataType();
        /**
         * @brief   获取数据关键字
         * @return  QString 数据名称
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        QString getDataKeyWord();
    private slots:
        /**
         * @brief   OK按钮点击事件
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        void on_buttonOK_clicked();
        /**
         * @brief   取消按钮点击事件
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        void on_buttonCancel_clicked();
        /**
         * @brief     类型下拉框索引值改变事件
         * @param[i]  index 当前索引值
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-02
         */
        void on_comboBoxType_currentIndexChanged(int index);
    private:
        /**
         * @brief   获取初始化场管理器
         * @return  Radioss::FITKInitialFieldManager*
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        Radioss::FITKInitialFieldManager* getManagerInitialField();
    private:
        /**
         * @brief   ui指针
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        Ui::GUIInitialFieldTypeDialog* _ui{};
    };
}
#endif  // __GUI_INITIAL_FIELD_TYPE_DIALOG_H__
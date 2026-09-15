/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FaceHiddenDialog.h
 * @brief       隐藏面交互控制对话框。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-07-07
 *********************************************************************/

#ifndef __FACEHIDDENDIALOG_H__
#define __FACEHIDDENDIALOG_H__

#include "GUIWidget/GUIDialogBase.h"
#include "GUIDialogAPI.h"

namespace Ui {
    class FaceHiddenDialog;
}

namespace GUI 
{
    /**
     * @brief       隐藏面交互控制对话框。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-07
     */
    class GUIDIALOGAPI FaceHiddenDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        explicit FaceHiddenDialog(QWidget *parent = nullptr);

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        ~FaceHiddenDialog();

        /**
         * @brief       构造单例对话框窗口。[静态]
         * @param[in]   parent：父窗口
         * @param[in]   oper：操作器
         * @return      对话框
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        static FaceHiddenDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper);

    protected:
        /**
         * @brief       界面逻辑终止处理。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        void finalize() override;

        /**
         * @brief       刷新面组列表下拉框。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void refreshFaceGroupComboBox();

    private slots:
        /**
         * @brief       槽函数 - 选择隐藏方式按钮点击。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void slot_rdBtnClicked();

        /**
         * @brief       槽函数 - 拾取形状类型下拉框变化。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        void slot_pickShapeTypeCmbChanged(int index);

        /**
         * @brief       槽函数 - 隐藏形状按钮。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        void slot_btnHide();

        /**
         * @brief       槽函数 - 显示隐藏形状按钮。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        void slot_btnShowAll();

        /**
         * @brief       槽函数 - 确定按钮。（退出）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        void slot_btnOK();

    private:
        /**
         * @brief       初始化界面。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        void init();

        /**
         * @brief       初始化拾取状态。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        void initPickMode();

        /**
         * @brief       隐藏拾取到的形状。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void hidePickedShapes();

        /**
         * @brief       隐藏当前选中面组。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void hideCurrentFaceGroup();

    private:
        /**
         * @brief       UI.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        Ui::FaceHiddenDialog *ui{};

    };
}

#endif // __FACEHIDDENDIALOG_H__

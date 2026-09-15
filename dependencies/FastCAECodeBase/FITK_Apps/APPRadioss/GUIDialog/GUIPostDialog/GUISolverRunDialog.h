/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUISolverRunDialog.h
 * @brief  优化求解Model窗口
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-11
 *********************************************************************/
#ifndef _GUISolverRunDialog_H___
#define _GUISolverRunDialog_H___

#include "GUIPostDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

class QRadioButton;

namespace Ui
{
    class GUISolverRunDialog;
}

namespace Core
{
    class FITKAbstractOperator;
}

namespace GUI
{
    /**
     * @brief  求解case窗口
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-11
     */
    class GUIPostDialogAPI GUISolverRunDialog : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造求解Model对话框对象
         * @param[in] parent         父对象
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        explicit GUISolverRunDialog(Core::FITKAbstractOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief 析构求解Model对话框对象
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        virtual ~GUISolverRunDialog();

    private:
        /**
         * @brief 初始化
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        void init();
        /**
         * @brief 添加QRadioButton
         * @param[i]  label          行标题
         * @author wangning (2185896382@qq.com)
         * @date   2025-08-26
         */
        QRadioButton* addLine(const QString& label);

    private slots:
        /**
         * @brief 确定按钮事件
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief 取消按钮事件
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    模型获取模式选择
         * @param[i] index
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        void on_comboBox_Model_currentIndexChanged(int index);
        /**
         * @brief    导入模型
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        void on_pushButton_ImportModel_clicked();

    private:
        /**
         * @brief UI对象
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        Ui::GUISolverRunDialog* _ui{};
        /**
         * @brief  操作器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-19
         */
        Core::FITKAbstractOperator* _oper{};

        //当前求解方案ID
        int _setSoliutionID{};
    };
}

#endif


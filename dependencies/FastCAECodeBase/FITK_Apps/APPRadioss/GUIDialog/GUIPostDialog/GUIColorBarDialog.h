/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIColorBarDialog.h
 * @brief  色棒窗口
 * @author wangning (2185896382@qq.com)
 * @date   2025-10-20
 *********************************************************************/
#ifndef _GUIColorBarDialog_H___
#define _GUIColorBarDialog_H___

#include "GUIPostDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace Ui
{
    class GUIColorBarDialog;
}
namespace EventOper
{
    class GraphEventOperator;
}
namespace GUI
{
    /**
     * @brief  色棒窗口
     * @author wangning (2185896382@qq.com)
     * @date   2025-10-20
     */
    class GUIPostDialogAPI GUIColorBarDialog : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        explicit GUIColorBarDialog(EventOper::GraphEventOperator* oper, QWidget* parent = nullptr);
        virtual ~GUIColorBarDialog();

    private:
        /**
         * @brief 初始化
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        void init();

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
        * @brief 最大值颜色
        * @author wangning (2185896382@qq.com)
        * @date   2025-09-11
        */
        void on_pushButtonMax_clicked();
        /**
         * @brief 最小值颜色
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        void on_pushButtonMin_clicked();
        /**
       * @brief 超出最大值颜色
       * @author wangning (2185896382@qq.com)
       * @date   2025-09-11
       */
        void on_pushButtonOutMax_clicked();
        /**
         * @brief 低于最小值颜色
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        void on_pushButtonOutMin_clicked();

    private:
        /**
         * @brief UI对象
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-11
         */
        Ui::GUIColorBarDialog* _ui{};
        /**
         * @brief  操作器
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-19
         */
        EventOper::GraphEventOperator* _oper{};

        int _currentPostID = -1;
    };
}

#endif



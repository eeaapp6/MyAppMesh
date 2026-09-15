/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUITimeHistorySECTIODialog.h
 * @brief  输出请求时间历程截面力窗口
 * @author guqingtao (15598887859@163.com)
 * @date   2025-11-07
 *********************************************************************/
#ifndef _GUITimeHistorySECTIODialog_H___
#define _GUITimeHistorySECTIODialog_H___

#include "GUIPostDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistorySECTIO.h"

namespace Ui
{
    class GUITimeHistorySECTIODialog;
}
namespace Core
{
    class FITKActionOperator;
}
namespace Radioss
{
    class FITKTimeHistoryManager;
}
namespace GUI
{
    /**
     * @brief  输出请求时间历程截面力窗口
     * @author guqingtao (15598887859@163.com)
     * @date   2025-11-07
     */
    class GUIPostDialogAPI GUITimeHistorySECTIODialog : public Core::FITKDialog
    {
        Q_OBJECT
    public:

        GUITimeHistorySECTIODialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        virtual ~GUITimeHistorySECTIODialog();

    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-11-07
         */
        void init();
        /**
         * @brief    初始化表格
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-11-07
         */
        void initTableWidget();
        /**
         * @brief  加载自定义变量选中状态
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-07
         */
        void loadCustomVariables(); 
        /**
         * @brief  收集自定义变量
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-07
         */
        QList<Radioss::FITKTimeHistorySECTIO::TimeHistorySECTIOVariable> collectCustomVariables();
        /**
         * @brief  收集选中的探针ID
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-07
         */
        QList<int> collectSelectedProbeIds(); 
    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-07
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    取消按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-11-07
         */
        void on_pushButton_Cancle_clicked();
        /**
         * @brief    类型选择
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-11-07
         */
        void on_comboBox_Type_currentIndexChanged(int index);
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-07
         */
        Ui::GUITimeHistorySECTIODialog* _ui{};
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-07
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  管理器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-07
         */
        Radioss::FITKTimeHistoryManager* _mgr = nullptr;
        /**
         * @brief  时间历程数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-07
         */
        Radioss::FITKTimeHistorySECTIO* _data = nullptr;
        /**
         * @brief  是否创建
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-07
         */
        bool _isCreate = true;
    };
}

#endif


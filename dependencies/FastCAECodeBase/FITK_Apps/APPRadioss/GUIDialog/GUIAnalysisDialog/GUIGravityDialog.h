/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIGravityDialog.h
 * @brief  重力场对话框
 * @author guqingtao (15598887859@163.com)
 * @date   2025-08-27
 *********************************************************************/
#ifndef _GUIGravityDialog_H
#define _GUIGravityDialog_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include <QString>
#include  "NodeGroupWidget.h"

namespace Ui
{
    class GUIGravityDialog;
}
namespace Radioss
{
    class FITKGravity;
    class FITKGravityManager;
}

namespace GUI
{
    /**
     * @brief  重力场对话框
     * @author guqingtao (15598887859@163.com)
     * @date   2025-08-27
     */
    class GUIAnalysisDialogAPI GUIGravityDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:

        explicit GUIGravityDialog(Radioss::FITKGravity* obj, Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        virtual ~GUIGravityDialog();

    protected:
        /**
         * @brief    数据初始化
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        void init();
        /**
         * @brief    创建数据对象
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        bool createObjData();
        /**
         * @brief    编辑数据对象
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        bool editObjData();
        /**
         * @brief    检查
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        bool ckeckData();
        /**
         * @brief    从界面获取数据
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        void getDataFormWidget();

        /**
        * @brief    写入Python脚本
        * @return   void
        * @author   wangning (2185896382@qq.com)
        * @date     2026-05-14
        */
        void writePythonScript();

    protected slots:
        /**
         * @brief    OK按钮
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        void on_pushButtonOK_clicked();
        /**
         * @brief    取消按钮信号
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        void on_pushButtonCancel_clicked();
        void clearHight();
    protected:
        /**
         * @brief  管理器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        Radioss::FITKGravityManager* _mgr = nullptr;
        /**
         * @brief  数据对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        Radioss::FITKGravity* _obj = nullptr;
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  是否创建
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        bool _isCreate = false;
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        Ui::GUIGravityDialog* _ui = nullptr;
        QString _oldNameForScript{};
        NodeGroupWidget* _nodeGroup{};

    };
}

#endif

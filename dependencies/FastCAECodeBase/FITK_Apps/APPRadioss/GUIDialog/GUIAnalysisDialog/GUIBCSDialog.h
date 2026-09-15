/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIBCSDialog.h
 * @brief  边界对话框
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-24
 *********************************************************************/
#ifndef _GUIBCSDialog_H
#define _GUIBCSDialog_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include  "NodeGroupWidget.h"

namespace Ui
{
    class GUIBCSDialog;
}
namespace Radioss
{
    class FITKRadiossBCS;
    class FITKRadiossBCSManager;
}

namespace GUI
{
    /**
     * @brief  边界对话框
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-10-24
     */
    class GUIAnalysisDialogAPI GUIBCSDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief    创建边界对话框
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-24
         */
        explicit GUIBCSDialog(Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief    编辑边界对话框
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-24
         */
        explicit GUIBCSDialog(Radioss::FITKRadiossBCS* obj, Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief    释放对话框
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-24
         */
        virtual ~GUIBCSDialog();

    protected:
        /**
         * @brief    数据初始化
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-27
         */
        void init();
        /**
         * @brief    检查
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-27
         */
        bool ckeckData();
        /**
         * @brief    从界面获取数据
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-27
         */
        void getDataFormWidget();
        /**
         * @brief    写入Python脚本
         * @param[i] oldName 编辑模式下修改前名称
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-22
         */
        void writePythonScript(const QString& oldName = QString());

    protected slots:
        /**
         * @brief    OK按钮
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-27
         */
        void on_pushButtonOK_clicked();
        /**
         * @brief    取消按钮信号
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-27
         */
        void on_pushButtonCancel_clicked();

        void clearHight();
    protected:
        /**
         * @brief  ui
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-26
         */
        Ui::GUIBCSDialog* _ui = nullptr;
        /**
         * @brief  管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-26
         */
        Radioss::FITKRadiossBCSManager* _mgr = nullptr;
        /**
         * @brief  数据对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-26
         */
        Radioss::FITKRadiossBCS* _obj = nullptr;
        /**
         * @brief  操作器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-26
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  是否创建
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-26
         */
        bool _isCreate = false;
        /**
         * @brief  节点集合
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-24
         */
        NodeGroupWidget* _nodeGroup{};

    };
}

#endif

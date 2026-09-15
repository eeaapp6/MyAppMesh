/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUISectionDialog.h
 * @brief  截面探针对话框声明
 * @author wangning (2185896382@qq.com)
 * @date   2025-11-06
 *********************************************************************/
#ifndef _GUISectionDialog_H
#define _GUISectionDialog_H

#include <QDialog>
#include "GUIAnalysisDialogAPI.h"

class QComboBox;
namespace Ui
{
    class GUISectionDialog;
}
namespace Core
{
    class FITKAbstractOperator;
    class FITKActionOperator;
}
namespace Radioss
{
    class FITKProbeSection;
}
namespace GUI
{
    /**
     * @brief 截面探针对话框声明
     * @author wangning (2185896382@qq.com)
     * @date   2025-11-06
     */
    class GUIAnalysisDialogAPI GUISectionDialog : public QDialog
    {
        Q_OBJECT
    public:
        explicit GUISectionDialog(Radioss::FITKProbeSection* section,  Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        ~GUISectionDialog();

    private:
        /**
         * @brief    初始化
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        void init();
        /**
         * @brief    初始化UI
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        void initUI();
        /**
         * @brief    初始化点集合
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        void initNodeSet(QComboBox* box);
        /**
         * @brief    检查
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        virtual bool ckeckData();
        /**
        * @brief    调整comboBox宽度
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        virtual void adjustComboBoxViewWidth(QComboBox* box);

        /**
         * @brief    写入Python脚本
         * @param[i] oldName 编辑前名称，创建时可为空
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-26
         */
        void writePythonScript(const QString& oldName = QString());

    private slots:
        /**
        * @brief OK按钮信号
        * @return
        */
        void on_pushButtonOK_clicked();
        /**
        * @brief cancle按钮信号
        * @return
        */
        void on_pushButtonCancel_clicked();
        /**
        * @brief 拾取按钮信号
        * @return
        */
        void onPushButtonPickClicked(QComboBox* comboBox,bool isSingle = true);

        //刷新下拉框
        void onComboBoxAboutToShow(Core::FITKAbstractOperator * oper);

    private:
        Ui::GUISectionDialog* _ui{};

        Radioss::FITKProbeSection* _section{};

        Core::FITKActionOperator* _oper = nullptr;
    };
}

#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIPropertyDialog.h
 * @brief  截面属性窗口基类
 * @author wangning (2185896382@qq.com)
 * @date   2025-11-03
 *********************************************************************/
#ifndef _GUIPropertyDialog_H
#define _GUIPropertyDialog_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include <QString>

class QComboBox;

namespace Ui
{
    class GUIPropertyDialog;
}
namespace Radioss
{
    class FITKPropAbstract;
}
namespace Interface
{
    class FITKAbstractSectionManager;
}
namespace GUI
{
    /**
     * @brief  壳属性Value窗口
     * @author wangning (2185896382@qq.com)
     * @date   2025-08-26
     */
    class GUIAnalysisDialogAPI GUIPropertyDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief 构造函数
         * @param[i]  obj          属性对象
         * @param[i]  oper           operator
         * @param[i]  parent
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        explicit GUIPropertyDialog(Core::FITKActionOperator* oper, Radioss::FITKPropAbstract* obj, QWidget* parent = nullptr);
        /**
         * @brief Destroy object
         * @author wangning (2185896382@qq.com)
         * @date   2025-08-26
         */
        virtual ~GUIPropertyDialog();

    protected:
        /**
        * @brief    数据初始化
        * @return   void
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        virtual void init();
        /**
         * @brief    检查
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        virtual bool ckeckData();
        /**
         * @brief    从界面获取数据
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        virtual void getDataFormWidget()=0;
        /**
         * @brief 添加LineEdit
         * @param[i]  label          行标题
         * @param[i]  objname        LineEdit对象名称
         * @author wangning (2185896382@qq.com)
         * @date   2025-08-26
         */
        void addLineEdit(const QString& label, const QString& objname);
        /**
         * @brief 添加ComboBox
         * @param[i]  label          行标题
         * @param[i]  objname        QComboBox对象名称
         * @author wangning (2185896382@qq.com)
         * @date   2025-08-26
         */
        QComboBox* addComboBox(const QString& label, const QString& objname);

        /**
         * @brief    写入Python脚本
         * @return   void
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-15
         */
        virtual void writePythonScript();

        // 获取属性管理器
        Interface::FITKAbstractSectionManager* getSectionManager();

    protected slots:
        /**
        * @brief OK按钮信号
        * @return
        */
        virtual void on_pushButtonOK_clicked();
        /**
        * @brief Cancel按钮信号
        * @return
        */
        virtual void on_pushButtonCancel_clicked();

    protected:
        /**
         * @brief 数据对象
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        Radioss::FITKPropAbstract* _obj = nullptr;
        /**
         * @brief 操作器对象
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  是否是编辑
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        bool _isEditMode{ false };
        /**
         * @brief  写脚本时使用的编辑前名称
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-22
         */
        QString _oldNameForScript{};

    protected:
        /**
         * @brief ui
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        Ui::GUIPropertyDialog* _ui = nullptr;
    };
}

#endif

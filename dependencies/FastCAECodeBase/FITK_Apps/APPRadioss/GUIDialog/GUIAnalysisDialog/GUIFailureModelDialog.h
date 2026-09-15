/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIFailureModelDialog.h
 * @brief  失效模型窗口基类
 * @author wangning (2185896382@qq.com)
 * @date   2025-11-12
 *********************************************************************/
#ifndef _GUIFailureModelDialog_H
#define _GUIFailureModelDialog_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"

class QComboBox;
class QLineEdit;
namespace Ui
{
    class GUIFailureModelDialog;
}
namespace Radioss
{
    class FITKAbstractFailureModel;
    class FITKRadiossFailureModelManager;
}
namespace GUI
{
    /**
     * @brief  失效模型窗口基类
     * @author wangning (2185896382@qq.com)
     * @date   2025-11-12
     */
    class GUIAnalysisDialogAPI GUIFailureModelDialog : public Core::FITKDialog
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
        explicit GUIFailureModelDialog(Core::FITKActionOperator* oper, Radioss::FITKAbstractFailureModel* obj, QWidget* parent = nullptr);
        /**
         * @brief Destroy object
         * @author wangning (2185896382@qq.com)
         * @date   2025-08-26
         */
        virtual ~GUIFailureModelDialog();

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
        virtual void getDataFormWidget() = 0;
        /**
         * @brief    写入Python脚本
         * @return   void
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-26
         */
        virtual void writePythonScript();
        /**
         * @brief 添加LineEdit
         * @param[i]  label          行标题
         * @param[i]  objname        LineEdit对象名称
         * @author wangning (2185896382@qq.com)
         * @date   2025-08-26
         */
        QLineEdit* addLineEdit(const QString& label, const QString& objname);
        /**
         * @brief 添加ComboBox
         * @param[i]  label          行标题
         * @param[i]  objname        QComboBox对象名称
         * @author wangning (2185896382@qq.com)
         * @date   2025-08-26
         */
        QComboBox* addComboBox(const QString& label, const QString& objname);

        // 获取失效管理器
        Radioss::FITKRadiossFailureModelManager* getFailureManager();

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
        Radioss::FITKAbstractFailureModel* _obj = nullptr;
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
         * @date   2026-05-26
         */
        QString _oldNameForScript{};

    protected:
        /**
         * @brief ui
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        Ui::GUIFailureModelDialog* _ui = nullptr;
    };
}

#endif

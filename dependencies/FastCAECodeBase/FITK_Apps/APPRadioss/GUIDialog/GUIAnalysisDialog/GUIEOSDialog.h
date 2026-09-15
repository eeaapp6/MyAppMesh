/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIEOSDialog.h
 * @brief  状态方程基类声明
 * @author wangning (2185896382@qq.com)
 * @date   2025-10-30
 *********************************************************************/
#ifndef _GUIEOSDialog_H
#define _GUIEOSDialog_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include <QTableWidgetItem>

namespace Ui
{
    class GUIEOSDialog;
}
namespace Radioss
{
    class FITKAbstractEquationOfState;
    class FITKEquationOfStateManager;
}
namespace GUI
{
    /**
     * @brief  状态方程基类声明
     * @author wangning (2185896382@qq.com)
     * @date   2025-10-30
     */
    class GUIAnalysisDialogAPI GUIEOSDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief load object dialog
         * @param[i]  obj            My Param doc
         * @param[i]  oper           operator
         * @param[i]  parent
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        explicit GUIEOSDialog(Radioss::FITKAbstractEquationOfState* obj,
            Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy object
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        virtual ~GUIEOSDialog();
        /**
         * @brief    数据初始化
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        virtual void init();
        /**
         * @brief   创建表格
         * @param[i]  headerLabels表头
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
         void createTable(const QStringList& headerLabels);
         /**
          * @brief   通过变量名获取对应item
          * @param[i]  varName变量名
          * @author wangning (2185896382@qq.com)
          * @date   2025-05-23
          */
         QTableWidgetItem* getItemByVarName(const QString& varName);
         /**
          * @brief   通过变量名获取对应item,将值传给item
          * @param[i]  varName变量名
          * @author wangning (2185896382@qq.com)
          * @date   2025-05-23
          */
         void setItemValueByVarName(const QString& varName, double value);
         /**
         * @brief   通过变量名获取对应item的值
         * @param[i]  varName变量名
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
         double getItemValueByVarName(const QString& varName,bool* ok = nullptr);

    protected:
        /**
         * @brief    创建数据对象
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        bool createObjData();
        /**
         * @brief    编辑数据对象
         * @return   boiol
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        bool editObjData();
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

    protected slots:
        /**
         * @brief    取消事件重写
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        void reject() override;
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

    protected:
        /**
        * @brief EOS管理器
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        Radioss::FITKEquationOfStateManager* _EOSMgr = nullptr;
        /**
         * @brief 数据对象
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        Radioss::FITKAbstractEquationOfState* _obj = nullptr;
        /**
         * @brief 操作器对象
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  是否是创建
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        bool _isCreate = false;
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
        Ui::GUIEOSDialog* _ui = nullptr;

        /**
        * @brief 变量和表格列号的映射
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        QHash<QString, int> _colMap;
    };
}

#endif

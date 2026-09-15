/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIEOSPolynomialDialog.h
 * @brief   多项式状态方程窗口声明
 * @author wangning (2185896382@qq.com)
 * @date   2025-10-30
 *********************************************************************/
#ifndef _GUIEOSPolynomialDialog_H
#define _GUIEOSPolynomialDialog_H

#include "GUIEOSDialog.h"

namespace GUI
{
    /**
     * @brief 多项式状态方程窗口声明
     */
    class GUIAnalysisDialogAPI GUIEOSPolynomialDialog : public GUIEOSDialog
    {
        Q_OBJECT;
    public:
        explicit GUIEOSPolynomialDialog(Radioss::FITKAbstractEquationOfState* obj, Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        virtual~GUIEOSPolynomialDialog() = default;

    protected:
        /**
        * @brief  初始化数据
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        void init() override;

        /**
        * @brief    检查
        * @return   QString
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        virtual bool ckeckData()override;

        /**
        * @brief    从界面获取数据
        * @return   QString
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        void getDataFormWidget() override;

        /**
        * @brief    写入Python脚本
        * @return   void
        * @author   wangning (2185896382@qq.com)
        * @date     2026-05-26
        */
        void writePythonScript() override;

    };
}

#endif

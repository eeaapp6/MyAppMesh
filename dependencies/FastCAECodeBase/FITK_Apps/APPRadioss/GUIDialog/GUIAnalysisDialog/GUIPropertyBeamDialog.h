/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIPropertyBeamDialog.h
 * @brief  梁属性窗口
 * @author wangning (2185896382@qq.com)
 * @date   2025-11-03
 *********************************************************************/
#ifndef _GUIPropertyBeamDialog_H
#define _GUIPropertyBeamDialog_H

#include "GUIPropertyDialog.h"

namespace GUI
{
    /**
     * @brief  梁属性窗口
     * @author wangning (2185896382@qq.com)
     * @date   2025-08-26
     */
    class GUIAnalysisDialogAPI GUIPropertyBeamDialog : public GUIPropertyDialog
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
        explicit GUIPropertyBeamDialog(Core::FITKActionOperator* oper, Radioss::FITKPropAbstract* obj, QWidget* parent = nullptr);
        virtual ~GUIPropertyBeamDialog() = default;

    protected:
        /**
        * @brief    UI初始化
        * @return   void
        * @author wangning (2185896382@qq.com)
        * @date   2025-08-26
        */
        void initUI();
        /**
        * @brief    数据初始化
        * @return   void
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        void init() override;
        /**
         * @brief    检查
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        bool ckeckData() override;
        /**
         * @brief    从界面获取数据
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        void getDataFormWidget() override;
        /**
         * @brief    写入Python脚本
         * @return   void
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-15
         */
        void writePythonScript() override;
    };
}

#endif

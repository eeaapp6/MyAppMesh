/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIMaterialPlasTabDialog.h
 * @brief   PlasTab塑性材料参数窗口声明
 * @author guqingtao (15598887859@163.com)
 * @date   2025-08-22
 *********************************************************************/
#ifndef _GUIMaterialPlasTabDialog_H
#define _GUIMaterialPlasTabDialog_H

#include "GUIMaterialPropDialog.h"

namespace GUI
{
    /**
     * @brief  PlasTab塑性材料参数窗口
     * @author guqingtao (15598887859@163.com)
     * @date   2025-08-22
     */
    class GUIAnalysisDialogAPI GUIMaterialPlasTabDialog : public GUIMaterialPropDialog
    {
        Q_OBJECT;
    public:
        explicit GUIMaterialPlasTabDialog(Interface::FITKAbstractMaterial* obj,Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        virtual~GUIMaterialPlasTabDialog() = default;

    protected:
        /**
         * @brief    初始化UI
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-22
         */
        void initUI();
        /**
         * @brief    初始化数据
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-22
         */
        void init() override;
        /**
         * @brief    检查
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-22
         */
        virtual bool ckeckData()override;
        /**
         * @brief    从界面获取数据
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-22
         */
        void getDataFormWidget() override;

        /**
         * @brief    写入Python脚本
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-15
         */
        void writePythonScript() override;

        //设置（初始化）曲线下拉列表 isDefault是否添加默认项
        void setCurveBox(QComboBox* curveBox,bool isDefault=true);

    private:
        /**
        * @brief    刷新函数个数关联项
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-16
        */
        void updateFuncSlot(int index);

    };
}

#endif

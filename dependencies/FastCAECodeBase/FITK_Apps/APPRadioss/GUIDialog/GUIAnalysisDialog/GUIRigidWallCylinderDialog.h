/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIRigidWallCylinderDialog.h
 * @brief  刚性圆柱墙对话框
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-09
 *********************************************************************/
#ifndef _GUIRigidWallCylinderDialog_H
#define _GUIRigidWallCylinderDialog_H

#include "GUIAnalysisDialogAPI.h"
#include "GUIRigidWallDialog.h"

namespace GUI
{
    /**
     * @brief  刚性圆柱墙对话框
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-09
     */
    class GUIAnalysisDialogAPI GUIRigidWallCylinderDialog : public GUIRigidWallDialog
    {
        Q_OBJECT;
    public:

        explicit GUIRigidWallCylinderDialog(Radioss::FITKAbstractRWall* obj, Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        virtual ~GUIRigidWallCylinderDialog() = default;
    protected:
        /**
         * @brief    初始化数据
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void init() override;
        /**
         * @brief    检查
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        virtual bool ckeckData()override;
        /**
         * @brief    从界面获取数据
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void getDataFormWidget() override;
        /**
         * @brief    写入Python脚本
         * @return   void
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-15
         */
        virtual void writePythonScript() override;

    };
}

#endif

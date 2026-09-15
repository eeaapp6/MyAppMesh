/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIRigidWallPlaneDialog.h
 * @brief  刚性平面墙对话框
 * @author guqingtao (15598887859@163.com)
 * @date   2025-08-26
 *********************************************************************/
#ifndef _GUIRigidWallPlaneDialog_H
#define _GUIRigidWallPlaneDialog_H

#include "GUIAnalysisDialogAPI.h"
#include "GUIRigidWallDialog.h"

namespace GUI
{
    /**
     * @brief  刚性平面墙对话框
     * @author guqingtao (15598887859@163.com)
     * @date   2025-08-26
     */
    class GUIAnalysisDialogAPI GUIRigidWallPlaneDialog : public GUIRigidWallDialog
    {
        Q_OBJECT;
    public:

        explicit GUIRigidWallPlaneDialog(Radioss::FITKAbstractRWall* obj, Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        virtual ~GUIRigidWallPlaneDialog() = default;
    protected:
        /**
         * @brief    初始化数据
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        void init() override;
        /**
         * @brief    检查
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        virtual bool ckeckData()override;
        /**
         * @brief    从界面获取数据
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        void getDataFormWidget() override;
        /**
         * @brief    写入Python脚本
         * @return   void
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-22
         */
        virtual void writePythonScript() override;

    };
}

#endif

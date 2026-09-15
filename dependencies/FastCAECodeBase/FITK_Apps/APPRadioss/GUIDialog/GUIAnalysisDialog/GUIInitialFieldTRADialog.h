/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    GUIInitialFieldTRADialog.h
 * @brief   初始平移速度对话框
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-03
 *********************************************************************/
#ifndef __GUI_INITIAL_FIELD_TRA_DIALOG_H__
#define __GUI_INITIAL_FIELD_TRA_DIALOG_H__

#include "GUIAnalysisDialogAPI.h"
#include "GUIDialog/GUIAnalysisDialog/GUIInitialFieldDialog.h"
#include <QString>

namespace Core
{
    class FITKActionOperator;
}

namespace GUI
{
    /**
     * @brief   初始平移速度对话框
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-03
     */
    class GUIAnalysisDialogAPI GUIInitialFieldTRADialog : public GUI::GUIInitialFieldDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief     构造函数
         * @param[i]  oper 操作器
         * @param[i]  parent 父窗口
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-03
         */
        explicit GUIInitialFieldTRADialog(Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        ~GUIInitialFieldTRADialog() = default;
    protected:
        /**
         * @brief   创建初始化场（重写）
         * @return  bool 创建结果
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        virtual bool createInitialField() override;
        /**
         * @brief   修改初始化场（重写）
         * @return  bool 修改结果
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        virtual bool modifyInitialField() override;
        /**
         * @brief   写入Python脚本
         * @author  wangning (2185896382@qq.com)
         * @date    2026-05-14
         */
        void writePythonScript();
    private:
        /**
         * @brief   操作器指针
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief   编辑模式下写脚本和查找对象使用的原始名称
         * @author  wangning (2185896382@qq.com)
         * @date    2026-05-22
         */
        QString _oldNameForScript{};
    };
}
#endif  // __GUI_INITIAL_FIELD_TRA_DIALOG_H__
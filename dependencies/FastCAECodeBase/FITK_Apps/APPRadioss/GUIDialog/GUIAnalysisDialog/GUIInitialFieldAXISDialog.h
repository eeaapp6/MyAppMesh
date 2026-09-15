/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIInitialFieldAXISDialog.h
 * @brief  初始化角速度场对话框类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-07
 *********************************************************************/
#ifndef __GUI_INITIAL_FIELFAXIS_DIALOG_H__
#define __GUI_INITIAL_FIELFAXIS_DIALOG_H__

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include <QString>

namespace Ui
{
    class GUIInitialFieldAXISDialog;
}

namespace Radioss
{
    class FITKInitialFieldAxis;
    class FITKInitialFieldManager;
}

namespace Core
{
    class FITKActionOperator;
}

namespace GUI
{
    /**
     * @brief  初始化角速度场对话框类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-11-07
     */
    class GUIAnalysisDialogAPI GUIInitialFieldAXISDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief   构造函数
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-11-07
         */
        explicit GUIInitialFieldAXISDialog(Radioss::FITKInitialFieldAxis* obj, Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        explicit GUIInitialFieldAXISDialog(Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief   析构函数
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-11-07
         */
        ~GUIInitialFieldAXISDialog();
        /**
         * @brief    数据初始化
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        void init();

    private slots:
        /**
         * @brief   确认按钮点击事件
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-11-07
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief   取消按钮点击事件
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-11-07
         */
        void on_pushButton_Cancel_clicked();

    private:
        /**
         * @brief    初始化界面
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        void initWidget();
        /**
         * @brief    初始化Frame下拉框
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        void initFrameComboBox();
        /**
         * @brief    创建数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        bool createObjData();
        /**
         * @brief    边界数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        bool editObjData();
        /**
         * @brief    从界面获取数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        void getDataFormWidget();
        /**
         * @brief   写入Python脚本
         * @author  wangning (2185896382@qq.com)
         * @date    2026-05-22
         */
        void writePythonScript();

    private:
        /**
         * @brief   UI指针
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-11-07
         */
        Ui::GUIInitialFieldAXISDialog* _ui = nullptr;
        /**
        * @brief  管理器
        * @author liuzhonghua (liuzhonghuaszch@163.com)
        * @date   2025-05-23
        */
        Radioss::FITKInitialFieldManager* _mgr = nullptr;
        /**
         * @brief  数据对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-07
         */
        Radioss::FITKInitialFieldAxis* _obj = nullptr;
        /**
         * @brief  操作器对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-07
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  是否是创建
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-07
         */
        bool _isCreate = false;
        /**
         * @brief  写脚本时使用的编辑前名称
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-22
         */
        QString _oldNameForScript{};
    };
}
#endif  // __GUI_INITIAL_FIELF_DIALOG_H__
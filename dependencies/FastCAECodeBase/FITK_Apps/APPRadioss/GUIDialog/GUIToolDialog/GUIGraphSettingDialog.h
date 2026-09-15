/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIGraphSettingDialog.h
 * @brief  渲染图形设置对话框
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-10-27
 *********************************************************************/
#ifndef _GUIGraphSettingDialog_h__
#define _GUIGraphSettingDialog_h__

#include "GUIToolDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"

namespace Ui
{
    class GUIGraphSettingDialog;
}

namespace Core
{
    class FITKActionOperator;
}

namespace GUI
{
    class GUIToolDialogAPI GUIGraphSettingDialog : public GUIDialogBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] oper         操作器
         * @param[i] parent       父对象
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-27
         */
        GUIGraphSettingDialog(Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-27
         */
        ~GUIGraphSettingDialog();
    private slots:
        /**
         * @brief  点线渲染设置槽函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-10-28
         */
        void on_pushButton_GeoPointColor_clicked();
        void on_pushButton_MeshPointColor_clicked();
        void on_pushButton_GeoLineColor_clicked();
        void on_pushButton_MeshLineColor_clicked();
        void on_pushButton_OK_clicked();
        void on_pushButton_Cancel_clicked();
        void on_pushButton_Apply_clicked();
    private:
        void initUi();
    private:
        Core::FITKActionOperator* _oper{ nullptr };
        Ui::GUIGraphSettingDialog* _ui{ nullptr };
    };

}
#endif // !_GUIGraphSettingDialog_h__

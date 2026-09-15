/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
@file   FITKPluginManageDialog.h
@brief  插件管理对话框
@author ZhouZhaoYi (zhouzhaoyi@diso.cn)
@date   2025-03-07
*********************************************************************/
#ifndef FITK_PLUGIN_MANAGE_DIALOG_H
#define FITK_PLUGIN_MANAGE_DIALOG_H
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "FITKWidgetAPI.h"

namespace Ui {
    class PluginManageDialog;
}
namespace AppFrame
{
    class FITKPluginsManager;
}namespace Plugins {
    /**
     * @brief  插件管理对话框
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-03-07
     */
    class FITKWidgetAPI FITKPluginManageDialog : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        /**
         * @brief    构造函数
         * @param[i] pluginPath      插件名称列表
         * @param[i] parent          父窗口
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        explicit FITKPluginManageDialog(const QString& pluginPath, QWidget *parent = nullptr);
        /**
         * @brief    析构函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        virtual ~FITKPluginManageDialog();
    private slots:
        /**
         * @brief    添加按钮点击事件
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        void on_addButton_clicked();
        /**
         * @brief    移除按钮点击事件
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        void on_removeButton_clicked();
        /**
         * @brief    确认按钮点击事件
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        void on_buttonBox_accepted();
        /**
         * @brief    取消按钮点击事件
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        void on_buttonBox_rejected();
    private:
        /**
         * @brief    初始化
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        void init();
    private:
        /**
         * @brief  UI对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-03-07
         */
        Ui::PluginManageDialog* _ui{};
        /**
         * @brief  插件管理器
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-03-07
         */
        AppFrame::FITKPluginsManager* _pluginsManager;
        /**
         * @brief  路径对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-03-07
         */
        QString _pluginPath = nullptr;
    };
} // namespace Plugins
#endif // FITK_PLUGIN_MANAGE_DIALOG_H
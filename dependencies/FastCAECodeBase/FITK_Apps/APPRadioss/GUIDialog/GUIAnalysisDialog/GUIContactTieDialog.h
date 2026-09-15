/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIContactTieDialog.h
 * @brief  接触绑定界面窗口类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-25
 *********************************************************************/
#ifndef _GUICONTACTTIEDIALOG_H
#define _GUICONTACTTIEDIALOG_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"

namespace Ui
{
    class GUIContactTieDialog;
}

namespace Core
{
    class FITKActionOperator;
}

namespace Radioss
{
    class FITKInteractionManager;
    class FITKInteractionKinematicTied;
}

namespace GUI
{
    /**
     * @brief  接触绑定界面窗口类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class GUIAnalysisDialogAPI GUIContactTieDialog : public Core::FITKDialog
    {
        Q_OBJECT
    public:

        GUIContactTieDialog(Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        GUIContactTieDialog(Radioss::FITKInteractionKinematicTied* tie, Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        ~GUIContactTieDialog();
       
    private:
        /**
         * @brief    初始化
         * @author   guqingtao (15598887859@163.com)
         * @date   2025-09-08
         */
        void init();
        /**
         * @brief    初始化界面窗口
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-26
         */
        void initWidget();

        /**
         * @brief    获取相互作用管理器
         * @return   Interface::FITKInteractionManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-26
         */
        Radioss::FITKInteractionManager* getInteractionManager();
        /**
         * @brief    创建数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-26
         */
        bool createObjData();
        /**
         * @brief    修改数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-26
         */
        bool editObjData();
        /**
         * @brief    从界面获取数据接口
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-26
         */
        void getDataFormWidget();

        /**
         * @brief    写入Python脚本
         * @param[in] oldName 修改前名称
         * @return   void
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-25
         */
        void writePythonScript(const QString& oldName = QString());

    private slots:
        /**
         * @brief ok button clicked slot
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief cencel button clicked slot
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        void on_pushButton_Cancel_clicked();

    protected:
        /**
         * @brief    关闭槽
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-07
         */
        virtual void accept() override;

        /**
         * @brief    关闭槽
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-07
         */
        virtual void reject() override;

    private:
        /**
         * @brief  ui
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        Ui::GUIContactTieDialog* _ui{};
        /**
         * @brief  操作器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-25
         */
        Core::FITKActionOperator* _oper{};
        /**
         * @brief  数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-26
         */
        Radioss::FITKInteractionKinematicTied* _obj{};
        /**
         * @brief  是否创建
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-26
         */
        bool _isCreate = false;
    };

}

#endif

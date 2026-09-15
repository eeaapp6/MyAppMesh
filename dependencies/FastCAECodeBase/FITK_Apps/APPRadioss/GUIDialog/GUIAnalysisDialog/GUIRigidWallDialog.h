/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIRigidWallDialog.h
 * @brief  刚性壁窗口基类
 * @author guqingtao (15598887859@163.com)
 * @date   2025-08-26
 *********************************************************************/
#ifndef _GUIRigidWallDialog_H
#define _GUIRigidWallDialog_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "PickDataProvider/GUIPickInfo.h"
#include  "NodeGroupWidget.h"
#include <QString>
#include <QEvent>
#include <array>

namespace Ui
{
    class GUIRigidWallDialog;
}
namespace Radioss
{
    class FITKAbstractRWall;
    class FITKRadiossRWallManager;
}

namespace GUI
{
    /**
     * @brief  刚性壁窗口基类
     * @author guqingtao (15598887859@163.com)
     * @date   2025-08-26
     */
    class GUIAnalysisDialogAPI GUIRigidWallDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        //拾取标志
        enum class PickFlag {
            None = 1,
            Node1,
            Node2
        };
        explicit GUIRigidWallDialog(Radioss::FITKAbstractRWall* obj, Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        virtual ~GUIRigidWallDialog();
        /**
         * @brief    数据初始化
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        virtual void init() = 0;
    protected:
        /**
         * @brief    创建数据对象
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        bool createObjData();
        /**
         * @brief    编辑数据对象
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        bool editObjData();
        /**
         * @brief    检查
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        virtual bool ckeckData();
        /**
         * @brief    从界面获取数据
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        virtual void getDataFormWidget() = 0;

        /**
         * @brief    写入Python脚本
         * @return   void
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-15
         */
        virtual void writePythonScript();
        /**
         * @brief    初始化ui
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-27
         */
        void initUI();
        void pickFinishedOper();
        void enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType);
        std::array<double, 3> getXYZ(int id);
    protected slots:
        /**
         * @brief    OK按钮
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        void on_pushButtonOK_clicked();
        /**
         * @brief    取消按钮信号
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        void on_pushButtonCancel_clicked();
        /**
         * @brief    接触/滑移行为下拉框
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-08-26
         */
        void on_comboBox_Sliding_currentIndexChanged(int index);
        void clearHight();
        void on_pushButton_PickNode1_clicked();
        void on_pushButton_PickNode2_clicked();

    protected:
        /**
         * @brief  管理器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        Radioss::FITKRadiossRWallManager* _mgr = nullptr;
        /**
         * @brief  数据对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        Radioss::FITKAbstractRWall* _obj = nullptr;
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  是否创建
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        bool _isCreate = false;
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-08-26
         */
        Ui::GUIRigidWallDialog* _ui = nullptr;
        QString _oldNameForScript{};

        NodeGroupWidget* _nodeGroup1{};
        NodeGroupWidget* _nodeGroup2{};
        PickFlag _pickFlag = PickFlag::None;

    };
}

#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIGeometryBoolDialog.h
 * @brief  几何体布尔操作
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-16
 *********************************************************************/
#ifndef GUIGeometryBoolDialog_H
#define GUIGeometryBoolDialog_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GUIGeometryDialogAPI.h"
#include "PickDataProvider/GUIPickInfo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Ui { class GUIGeometryBoolDialog; }
namespace Interface
{
    class FITKAbsGeoPart;
}
namespace Core
{
    class FITKActionOperator;
}
namespace GUI {
    /**
     * @brief  几何体布尔操作
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-17
     */
    class GUIGeometryDialogAPI GUIGeometryBoolDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        //拾取标志
        enum class PickFlag {
            None = 1,
            Target,
            Tool
        };
        GUIGeometryBoolDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        ~GUIGeometryBoolDialog();
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void init();
        /**
         * @brief    处理拾取到的数据
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void pickFinishedOper();
        /**
         * @brief    设置拾取方式
         * @param[i] objType
         * @param[i] methodType
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType);

    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-16
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    取消按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    拾取目标模型
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-17
         */
        void on_Target_pick_clicked();
        /**
         * @brief    拾取工具模型
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-17
         */
        void on_Tool_pick_clicked();
        /**
         * @brief    类型选择下拉框
         * @param[i] index 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-17
         */
        void on_comboBox_type_currentIndexChanged(int index);

    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-16
         */
        Ui::GUIGeometryBoolDialog *_ui{};
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-16
         */
        Core::FITKActionOperator* _oper{};
        /**
         * @brief  几何数据管理器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-16
         */
        Interface::FITKAbsGeoPart* _geometryData{};
        /**
         * @brief  拾取标志
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-17
         */
        PickFlag _pickFlag = PickFlag::None;
        /**
         * @brief  目标模型
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-17
         */
        Interface::VirtualShape _virtualTarget;
        /**
         * @brief  工具模型
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-17
         */
        Interface::VirtualShape _virtualTool;
    };
}
#endif 

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIGeometryTransDialog.h
 * @brief  平移
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-17
 *********************************************************************/
#ifndef GUIGeometryTransDialog_H
#define GUIGeometryTransDialog_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GUIGeometryDialogAPI.h"
#include "PickDataProvider/GUIPickInfo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Ui { class GUIGeometryTransDialog; }
namespace Interface {
    class FITKAbsGeoPart;
    class VirtualShape;
}
namespace Core
{
    class FITKActionOperator;
}
namespace GUI {
    /**
     * @brief  平移
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-17
     */
    class GUIGeometryDialogAPI GUIGeometryTransDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        //拾取标志
        enum class PickFlag {
            None = 1,
            Target,
            Point1,
            Point2
        };
        GUIGeometryTransDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        ~GUIGeometryTransDialog();
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
        /**
         * @brief    从界面获取数据
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void getDataFormWidget();
        /**
         * @brief    获取点的坐标
         * @param[i] id 
         * @return   std::array<double,3>
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-17
         */
        std::array<double, 3> getPointXYZ(int id);

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
         * @brief    目标模型拾取
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-17
         */
        void on_pushButton_Pick_clicked();
        /**
         * @brief    平移方向点1拾取
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-17
         */
        void on_point_1_pick_clicked();
        /**
         * @brief    平移方向点2拾取
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-17
         */
        void on_point_2_pick_clicked();
        /**
         * @brief    平移方式下拉框
         * @param[i] index 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-17
         */
        void on_stretch_type_currentIndexChanged(int index);
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-16
         */
        Ui::GUIGeometryTransDialog *_ui{};
        /**
         * @brief  几何数据对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-16
         */
        Interface::FITKAbsGeoCommand* _obj{};
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
         * @brief  目标模型
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-17
         */
        Interface::VirtualShape _virtualOriginalModel;
        /**
         * @brief  拾取类型
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-17
         */
        PickFlag _pickFlag = PickFlag::None;
    };
}
#endif 

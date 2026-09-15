/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIGeometryFilletDialog.h
 * @brief  倒圆
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-16
 *********************************************************************/
#ifndef GUIGeometryFilletDialog_H
#define GUIGeometryFilletDialog_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GUIGeometryDialogAPI.h"
#include "PickDataProvider/GUIPickInfo.h"

namespace Ui { class GUIGeometryFilletDialog; }
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
     * @brief  倒圆
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-17
     */
    class GUIGeometryDialogAPI GUIGeometryFilletDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:

        GUIGeometryFilletDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        ~GUIGeometryFilletDialog();
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
         * @brief    拾取点的按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void on_edge_pick_clicked();
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-16
         */
        Ui::GUIGeometryFilletDialog *_ui{};
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
         * @brief  虚拓扑边列表
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-16
         */
        QList<Interface::VirtualShape> _edgeVirtualTopos{};
    };
}
#endif 

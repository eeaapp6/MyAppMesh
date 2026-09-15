/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    DeleteFloatingEdgeDialog.h
 * @brief   删除悬浮边窗口类。

 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-08-29
 *********************************************************************/

#ifndef DELETEFLOATINGEDGEDIALOG_H
#define DELETEFLOATINGEDGEDIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Ui
{
    class DeleteFloatingEdgeDialog;
}

// 前置声明。
namespace Interface
{
    class FITKAbsGeoModelOperFace;
    class FITKAbsGeoModelOperFaceDeleteFloatingEdge;
}

namespace GUI {
    /**
     * @brief   删除悬浮边窗口类。
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-08-29
     */
    class GUIDIALOGAPI DeleteFloatingEdgeDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        explicit DeleteFloatingEdgeDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper);

        /**
         * @brief   析构函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        ~DeleteFloatingEdgeDialog();

        /**
         * @brief   创建窗口。[静态]
         * @param   parent：父窗口指针
         * @param   oper：操作器
         * @param   cmd：历史模型数据[缺省]
         * @return  窗口实例
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        static DeleteFloatingEdgeDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);

    protected:
        /**
         * @brief   窗口关闭事件。[重写]
         * @param   event：关闭事件
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        void closeEvent(QCloseEvent* event) override;

    private slots:
        /**
         * @brief   槽函数 - 创建模式切换。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        void slot_typeChanged(int index);

        /**
         * @brief   槽函数 - 确定。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        void slot_ok();

        /**
         * @brief   槽函数 - 应用（预览）。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        void slot_apply();

        /**
         * @brief   槽函数 - 取消。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        void slot_cancel();

        /**
         * @brief   槽函数 - 进入拾取状态多选面。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        void slot_selectFaces();

        /**
         * @brief   槽函数 - 进入拾取状态选择主面。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        void slot_selectMainFace();

        /**
         * @brief   槽函数 - 退出拾取状态。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        void slot_deselectFaces();

        /**
         * @brief   槽函数 - 退出拾取状态。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        void slot_deselectMainFace();

        /**
         * @brief   槽函数 - 多个面形状被拾取到。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        void slot_shapeFacesSelected();

        /**
         * @brief   槽函数 - 主面形状被拾取到。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        void slot_shapeMainFaceSelected();

    private:
        /**
         * @brief   初始化界面。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        void init();

        /**
         * @brief   刷新界面。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        void reload();

        /**
         * @brief   断开所有拾取相关信号。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        void disconnectSelectSigs();

        /**
         * @brief   刷新拾取面标签文本。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        void updateFacesLabel();

        /**
         * @brief   清除暂存的拾取信息。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        void clearSelectData();

        /**
         * @brief   检查输入数据是否正确。
         * @return  是否正确
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        bool isInputValid();

        /**
         * @brief   结束创建模型。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        void finalize();

    private:
        /**
         * @brief   Qt UI 指针。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-29
         */
        Ui::DeleteFloatingEdgeDialog *ui{ nullptr };

        /**
         * @brief   预览数据。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        Interface::FITKAbsGeoModelOperFace* m_previewCmd{ nullptr };

        /**
         * @brief   临时存储的主面ID。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        Interface::VirtualShape m_tempMainFaceVShape{};

        /**
         * @brief   临时存储的拾取ID。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-22
         */
        QList<Interface::VirtualShape> m_tempVShapes;

    };
}
#endif // CHAMFERINFODIALOG_H

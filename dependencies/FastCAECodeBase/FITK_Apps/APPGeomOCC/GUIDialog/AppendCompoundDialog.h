/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    AppendCompoundDialog.h
 * @brief   添加复合对象窗口。

 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-09-07
 *********************************************************************/

#ifndef APPENDCOMPOUNDDIALOG_H
#define APPENDCOMPOUNDDIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Ui
{
    class AppendCompoundDialog;
}

// 前置声明。
namespace Interface
{
    class FITKAbsGeoModelOperCompound;
}

namespace GUI {
    /**
     * @brief   添加复合对象窗口。
     * @author  fulipeng (fulipengqd@yeah.net)
     * @date    2024-09-07
     */
    class GUIDIALOGAPI AppendCompoundDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        explicit AppendCompoundDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper);

        /**
         * @brief   析构函数。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        ~AppendCompoundDialog();

        /**
         * @brief   创建窗口。[静态]
         * @param   parent：父窗口指针
         * @param   oper：操作器
         * @param   cmd：历史模型数据[缺省]
         * @return  窗口实例
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        static AppendCompoundDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);

    protected:
        /**
         * @brief   窗口关闭事件。[重写]
         * @param   event：关闭事件
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void closeEvent(QCloseEvent* event) override;

    private slots:
        /**
         * @brief   槽函数 - 创建模式切换。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void slot_typeChanged(int index);

        /**
         * @brief   槽函数 - 确定。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void slot_ok();

        /**
         * @brief   槽函数 - 应用（预览）。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void slot_apply();

        /**
         * @brief   槽函数 - 取消。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void slot_cancel();

        /**
         * @brief   槽函数 - 进入拾取状态多选shape。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void slot_selectShapes();

        /**
         * @brief   槽函数 - 进入拾取状态选择复合对象。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void slot_selectCompound();

        /**
         * @brief   槽函数 - 退出拾取状态。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void slot_deselectShapes();

        /**
         * @brief   槽函数 - 退出拾取状态。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void slot_deselectCompound();

        /**
         * @brief   槽函数 - 多个形状形状被拾取到。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void slot_shapesSelected();

        /**
         * @brief   槽函数 - 复合对象形状被拾取到。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void slot_shapeCompoundSelected();

    private:
        /**
         * @brief   初始化界面。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void init();

        /**
         * @brief   刷新界面。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void reload();

        /**
         * @brief   断开所有拾取相关信号。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void disconnectSelectSigs();

        /**
         * @brief   刷新拾取的对象文本。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void updateShapesLabel();

        /**
         * @brief   清除暂存的拾取信息。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void clearSelectData();

        /**
         * @brief   检查输入数据是否正确。
         * @return  是否正确
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        bool isInputValid();

        /**
         * @brief   结束创建模型。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        void finalize();

    private:
        /**
         * @brief   Qt UI 指针。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        Ui::AppendCompoundDialog *ui{ nullptr };

        /**
         * @brief   预览数据。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        Interface::FITKAbsGeoModelOperCompound* m_previewCmd{ nullptr };

        /**
         * @brief   临时存储的复合体。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        Interface::VirtualShape m_tempCompoundVShape{};

        /**
         * @brief   临时存储的拾取ID。
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-07
         */
        QList<Interface::VirtualShape> m_tempVShapes;

    };
}
#endif // CHAMFERINFODIALOG_H

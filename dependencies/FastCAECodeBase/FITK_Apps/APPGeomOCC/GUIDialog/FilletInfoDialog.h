/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FilletInfoDialog.h
 * @brief   倒圆窗口类。

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-22
 *********************************************************************/

#ifndef FILLETINFODIALOG_H
#define FILLETINFODIALOG_H

#include "GUIDialogAPI.h"

#include "GUIWidget/GUIDialogBase.h"

namespace Ui
{
    class FilletInfoDialog;
}

// 前置声明。
namespace Interface
{
    class FITKAbsGeoCommand;
    class FITKAbsGeoModelFilletSolid;
}

namespace GUI {
    /**
     * @brief   倒圆窗口类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-22
     */
    class GUIDIALOGAPI FilletInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        explicit FilletInfoDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper);

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        ~FilletInfoDialog();

        /**
         * @brief   创建窗口。[静态]
         * @param   parent：父窗口指针
         * @param   oper：操作器
         * @param   cmd：历史模型数据[缺省]
         * @return  窗口实例
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        static FilletInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper,
            Interface::FITKAbsGeoCommand* cmd = nullptr);

    private slots:
        /**
         * @brief   槽函数 - 创建模式切换。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void slot_typeChanged(int index);

        /**
         * @brief   槽函数 - 确定。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void slot_ok();

        /**
         * @brief   槽函数 - 预览。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void slot_preview();

        /**
         * @brief   槽函数 - 取消。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void slot_cancel();

        /**
         * @brief   槽函数 - 进入拾取状态。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void slot_select();

        /**
         * @brief   槽函数 - 退出拾取状态。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void slot_deselect();

        /**
         * @brief   槽函数 - 形状被拾取到。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void slot_shapeSelected();

    private:
        /**
         * @brief   初始化界面。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void init();

        /**
         * @brief   刷新界面。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void reload();

        /**
         * @brief   断开所有拾取相关信号。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void disconnectSelectSigs();

        /**
         * @brief   刷新拾取边标签文本。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void updateEdgesLabel();

        /**
         * @brief   清除暂存的拾取信息。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void clearSelectData();

        /**
         * @brief   检查输入数据是否正确。
         * @return  是否正确
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        bool isInputValid();

        /**
         * @brief   结束创建模型。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void finalize() override;

    private:
        /**
         * @brief   Qt UI 指针。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        Ui::FilletInfoDialog *ui{ nullptr };

        /**
         * @brief   预览数据。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        Interface::FITKAbsGeoModelFilletSolid* m_previewCmd{ nullptr };

        /**
         * @brief   临时存储的被倒圆体所在数据ID。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        int m_tempCmdId = -1;

        /**
         * @brief   临时存储的倒圆半径。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        double m_tempRadius = 1.0;

        /**
         * @brief   临时存储的拾取边索引。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        QList<Interface::VirtualShape> m_tempEdgeTopos;

    };
}
#endif // FILLETINFODIALOG_H

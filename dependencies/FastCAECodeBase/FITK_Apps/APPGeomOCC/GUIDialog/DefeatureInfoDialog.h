/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    DefeatureInfoDialog.h
 * @brief   移除倒角窗口类。

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-26
 *********************************************************************/

#ifndef DEFEATUREINFODIALOG_H
#define DEFEATUREINFODIALOG_H

#include "GUIDialogAPI.h"

#include "GUIWidget/GUIDialogBase.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"

namespace Ui
{
    class DefeatureInfoDialog;
}

// 前置声明。
namespace Interface
{
    class FITKAbsGeoCommand;
    class FITKAbsGeoModelDefeatureSolid;
}

namespace GUI {
    /**
     * @brief   移除倒角窗口类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-26
     */
    class GUIDIALOGAPI DefeatureInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        explicit DefeatureInfoDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper);

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        ~DefeatureInfoDialog();

        /**
         * @brief   创建窗口。[静态]
         * @param   parent：父窗口指针
         * @param   oper：操作器
         * @param   cmd：历史模型数据[缺省]
         * @return  窗口实例
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        static DefeatureInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper,
            Interface::FITKAbsGeoCommand* cmd = nullptr);

    private slots:
        /**
         * @brief   槽函数 - 创建模式切换。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void slot_typeChanged(int index);

        /**
         * @brief   槽函数 - 确定。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void slot_ok();

        /**
         * @brief   槽函数 - 预览。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void slot_preview();

        /**
         * @brief   槽函数 - 取消。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void slot_cancel();

        /**
         * @brief   槽函数 - 进入拾取状态。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void slot_select();

        /**
         * @brief   槽函数 - 退出拾取状态。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void slot_deselect();

        /**
         * @brief   槽函数 - 形状被拾取到。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void slot_shapeSelected();

    private:
        /**
         * @brief   初始化界面。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void init();

        /**
         * @brief   刷新界面。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void reload();

        /**
         * @brief   断开所有拾取相关信号。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void disconnectSelectSigs();

        /**
         * @brief   刷新拾取面标签文本。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void updateFacesLabel();

        /**
         * @brief   清除暂存的拾取信息。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void clearSelectData();

        /**
         * @brief   检查输入数据是否正确。
         * @return  是否正确
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        bool isInputValid();

        /**
         * @brief   结束创建模型。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        void finalize() override;

    private:
        /**
         * @brief   Qt UI 指针。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        Ui::DefeatureInfoDialog *ui{ nullptr };

        /**
         * @brief   数据类型。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        Interface::FITKGeoEnum::FITKGeometryComType m_geoType = Interface::FITKGeoEnum::FITKGeometryComType::FGTNone;

        /**
         * @brief   预览数据。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        Interface::FITKAbsGeoModelDefeatureSolid* m_previewCmd{ nullptr };

        /**
         * @brief   临时存储的被移除倒角体所在数据ID。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        int m_tempCmdId = -1;

        /**
         * @brief   临时存储的拾取面索引列表。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        QList<Interface::VirtualShape> m_tempFaceTopos;

    };
}
#endif // DEFEATUREINFODIALOG_H

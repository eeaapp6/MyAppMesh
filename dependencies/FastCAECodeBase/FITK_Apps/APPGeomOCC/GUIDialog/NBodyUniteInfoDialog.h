/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        NBodyUniteInfoDialog.h
 * @brief       多实体布尔对话框。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-07-08
 *********************************************************************/

#ifndef __NBODYUNITEINFODIALOG_H__
#define __NBODYUNITEINFODIALOG_H__

#include "GUIDialogAPI.h"

#include "GUIWidget/GUIDialogBase.h"

namespace Ui
{
    class NBodyUniteInfoDialog;
}

// 前置声明。
namespace Interface
{
    class FITKAbsGeoCommand;
    class FITKAbsGeoOperNBodyUnite;
}

namespace GUI 
{
    /**
     * @brief       多实体布尔对话框。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-08
     */
    class GUIDIALOGAPI NBodyUniteInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        explicit NBodyUniteInfoDialog(QWidget *parent, EventOper::ParaWidgetInterfaceOper* oper);

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        ~NBodyUniteInfoDialog();

        /**
         * @brief       创建窗口。[静态]
         * @param[in]   parent：父窗口指针
         * @param[in]   oper：操作器
         * @param[in]   cmd：历史模型数据[缺省]
         * @return      窗口实例
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        static NBodyUniteInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper,
            Interface::FITKAbsGeoCommand* cmd = nullptr);

    private slots:
        /**
         * @brief       槽函数 - 创建模式切换。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void slot_typeChanged(int index);

        /**
         * @brief       槽函数 - 确定。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void slot_ok();

        /**
         * @brief       槽函数 - 预览。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void slot_preview();

        /**
         * @brief       槽函数 - 取消。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void slot_cancel();

        /**
         * @brief       槽函数 - 进入拾取状态。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void slot_select();

        /**
         * @brief       槽函数 - 退出拾取状态。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void slot_deselect();

        /**
         * @brief       槽函数 - 形状被拾取到。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void slot_shapeSelected();

    private:
        /**
         * @brief       初始化界面。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void init();

        /**
         * @brief       刷新界面。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void reload();

        /**
         * @brief       断开所有拾取相关信号。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void disconnectSelectSigs();

        /**
         * @brief       刷新拾取体标签文本。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void updateSolidsLabel();

        /**
         * @brief       清除暂存的拾取信息。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void clearSelectData();

        /**
         * @brief       检查输入数据是否正确。
         * @return      是否正确
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        bool isInputValid();

        /**
         * @brief       结束创建模型。[重写]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void finalize() override;

    private:
        /**
         * @brief       Qt UI 指针。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        Ui::NBodyUniteInfoDialog* ui{ nullptr };

        /**
         * @brief       预览数据。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        Interface::FITKAbsGeoOperNBodyUnite* m_previewCmd{ nullptr };

        /**
         * @brief       临时存储的拾取体拓扑信息。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        QList<Interface::VirtualShape> m_tempSolidTopos;

    };
}
#endif // !__NBODYUNITEINFODIALOG_H__

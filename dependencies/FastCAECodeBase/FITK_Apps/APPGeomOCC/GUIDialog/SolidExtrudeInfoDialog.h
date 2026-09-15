/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   SolidExtrudeInfoDialog.h
 * @brief  拉伸成体的信息对话框.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-31
 */
#ifndef SOLIDEXTRUDEINFODIALOG_H
#define SOLIDEXTRUDEINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSolid.h"

namespace Ui {
    class SolidExtrudeInfoDialog;
}

namespace GUI {
    /**
     * @brief  拉伸成体的信息对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-31
     */
    class GUIDIALOGAPI SolidExtrudeInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit SolidExtrudeInfoDialog(QWidget *parent);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~SolidExtrudeInfoDialog();
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static SolidExtrudeInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);

    protected:
        /**
         * @brief  校验对话框数据.
         * @return 校验结果
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        bool checkDialogData() const;
        /**
         * @brief  清理窗口资源（包括信号和数据）.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-23
         */
        void finalize() override;
    private slots:
        /**
         * @brief  创建方法下拉框槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_comb_type_currentIndexChanged(int index);
        /**
         * @brief  拉伸线拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-23
         */
        void on_pb_selectsource_clicked();
        /**
         * @brief  方向翻转槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-18
         */
        void onDirectionReverseSlot();
        /**
         * @brief  方向拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-13
         */
        void on_pb_pickdirection_clicked();
        /**
         * @brief  确定按钮槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_ok_clicked();
        /**
         * @brief  应用按钮槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_preview_clicked();
        /**
         * @brief  取消按钮槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_cancel_clicked();

    private:
        /**
         * @brief  Ui.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Ui::SolidExtrudeInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoModelExtrudeSolid* m_PreviewCmd{};
        /**
         * @brief  拉伸线的虚拓扑.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-23
         */
        Interface::VirtualShape m_SourceShape{};
    };
}

#endif // SURFACEEXTRUDEINFODIALOG_H

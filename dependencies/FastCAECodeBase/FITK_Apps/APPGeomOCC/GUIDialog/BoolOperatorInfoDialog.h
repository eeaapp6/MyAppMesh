/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   BoolOperatorInfoDialog.h
 * @brief  布尔操作信息对话框.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-22
 */
#ifndef BOOLOPERATORINFODIALOG_H
#define BOOLOPERATORINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"

namespace Ui {
    class BoolOperatorInfoDialog;
}

namespace GUI {
    /**
     * @brief  布尔操作信息对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-22
     */
    class GUIDIALOGAPI BoolOperatorInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit BoolOperatorInfoDialog(QWidget *parent);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~BoolOperatorInfoDialog();
        /**
         * @brief  设置布尔操作类型.
         * @param  type 布尔操作类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void setBoolType(Interface::FITKAbsGeoOperBool::GeoBoolOperType type);
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static BoolOperatorInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);

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
         * @brief  目标选择槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        void on_pb_selecttarget_clicked();
        /**
         * @brief  工具选择槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        void on_pb_selecttool_clicked();
        /**
         * @brief  确定按钮槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_ok_clicked();
        /**
         * @brief  预览按钮槽函数.
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
        Ui::BoolOperatorInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoOperBool* m_PreviewCmd{};
        /**
         * @brief  目标对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        Interface::VirtualShape m_Target{};
        /**
         * @brief  工具对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        Interface::VirtualShape m_Tool{};
    };
}

#endif // BOOLOPERATORINFODIALOG_H

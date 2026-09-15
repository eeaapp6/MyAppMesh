/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef CURVESPLITTERINFODIALOG_H
#define CURVESPLITTERINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPoint.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSplitter.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"

namespace Ui {
    class CurveSplitterInfoDialog;
}

namespace GUI {
    /**
     * @brief  曲线交点信息对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-21
     */
    class GUIDIALOGAPI CurveSplitterInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit CurveSplitterInfoDialog(QWidget *parent = nullptr);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~CurveSplitterInfoDialog();
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static CurveSplitterInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);


    protected:
        /**
         * @brief  检查对话框输入数据是否合法.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-30
         */
        bool checkDialogData();
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
         * @brief  分割源数据拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_sourcepick_clicked();
        /**
         * @brief  分割工具数据拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_toolpick_clicked();
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
         * @brief  UI.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Ui::CurveSplitterInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoCurveSplitter* m_PreviewCmd{};
        /**
         * @brief  临时交互对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-13
         */
        Interface::FITKAbsGeoModelPoint* m_InteractiveCmd{};
        /**
         * @brief  分割源数据.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::VirtualShape m_SourceShape{};
        /**
         * @brief  分割工具数据.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::VirtualShape m_ToolShape{};
        /**
         * @brief  交互拾取分割点的坐标.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-24
         */
        QList<double> m_InteractionPoint{};
    };
}

#endif // CURVESPLITTERINFODIALOG_H

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   CurveBridgeCurveInfoDialog.h
 * @brief  桥接曲线信息对话框.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-09-02
 */
#ifndef CURVEBRIDGECURVEINFODIALOG_H
#define CURVEBRIDGECURVEINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCurve.h"

namespace Ui {
    class CurveBridgeCurveInfoDialog;
}

namespace GUI {
    /**
    * @brief  桥接曲线信息对话框类.
    * @author YanZhiHui (chanyuantiandao@126.com)
    * @date   2024-08-21
    */
    class GUIDIALOGAPI CurveBridgeCurveInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit CurveBridgeCurveInfoDialog(QWidget *parent = nullptr);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~CurveBridgeCurveInfoDialog();
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static CurveBridgeCurveInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);


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
         * @brief  源曲线1拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_bridge_curve1pick_clicked();
        /**
         * @brief  源曲线1切换端点槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_bridge_curve1toogle_clicked();
        /**
         * @brief  源曲线2拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_bridge_curve2pick_clicked();
        /**
         * @brief  源曲线2切换端点槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_bridge_curve2toogle_clicked();
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
        Ui::CurveBridgeCurveInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoModelBridgeCurve* m_PreviewCmd{};
        /**
         * @brief  源曲线1.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::VirtualShape m_SourceCurve1{};
        /**
         * @brief  源曲线1是否使用起始点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-02
         */
        bool m_UseStartEnd1{ true };
        /**
         * @brief  曲线1的参数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-02
         */
        double m_Parameter1{};
        /**
        * @brief  源曲线2.
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-21
        */
        Interface::VirtualShape m_SourceCurve2{};
        /**
         * @brief  源曲线2是否使用起始点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-02
         */
        bool m_UseStartEnd2{ true };
        /**
         * @brief  曲线2的参数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-02
         */
        double m_Parameter2{};
    };
}

#endif // CURVEBRIDGECURVEINFODIALOG_H

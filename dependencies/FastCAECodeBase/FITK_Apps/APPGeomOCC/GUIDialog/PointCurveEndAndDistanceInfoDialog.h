/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   PointCurveEndAndDistanceInfoDialog.h
 * @brief  指定距线端点距离创建点信息对话框.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-30
 */
#ifndef POINTCURVEENDANDDIASTANCEINFODIALOG_H
#define POINTCURVEENDANDDIASTANCEINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPoint.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCurve.h"

namespace Ui {
    class PointCurveEndAndDistanceInfoDialog;
}

namespace GUI {
    /**
     * @brief  指定距线端点距离创建点信息对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-21
     */
    class GUIDIALOGAPI PointCurveEndAndDistanceInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit PointCurveEndAndDistanceInfoDialog(QWidget *parent = nullptr);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~PointCurveEndAndDistanceInfoDialog();
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static PointCurveEndAndDistanceInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);


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
         * @brief  源曲线拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_curveenddis_pick_clicked();
        /**
         * @brief  端点切换槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_curveenddis_toogle_clicked();
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
        Ui::PointCurveEndAndDistanceInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoModelCurveEndDistance* m_PreviewCmd{};
        /**
         * @brief  源曲线.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::VirtualShape m_SourceCurve{};
        /**
         * @brief  是否使用起始端点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-30
         */
        bool m_UseStartEnd{ true };
    };
}
#endif // POINTCURVEENDANDDIASTANCEINFODIALOG_H

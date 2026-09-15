/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   PointInterTwoCurveInfoDialog.h
 * @brief  曲线交点信息对话框.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-30
 */
#ifndef POINTINTERTWOCURVEINFODIALOG_H
#define POINTINTERTWOCURVEINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPoint.h"

namespace Ui {
    class PointInterTwoCurveInfoDialog;
}

namespace GUI {
    /**
     * @brief  曲线交点信息对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-21
     */
    class GUIDIALOGAPI PointInterTwoCurveInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit PointInterTwoCurveInfoDialog(QWidget *parent = nullptr);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~PointInterTwoCurveInfoDialog();
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static PointInterTwoCurveInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);


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
         * @brief  投影到线上的点拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_intertwocurves_curve1pick_clicked();
        /**
         * @brief  投影线上的拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_intertwocurves_curve2pick_clicked();
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
        Ui::PointInterTwoCurveInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoModelTwoCurveIntersection* m_PreviewCmd{};
        /**
         * @brief  要求交点的曲线1.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::VirtualShape m_TwoCurveInterCurve1{};
        /**
         * @brief  要求交点的曲线2.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::VirtualShape m_TwoCurveInterCurve2{};
        /**
         * @brief  交点点使用的索引.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        int m_InterPointIndex{};
        
    };
}
#endif // POINTINTERTWOCURVEINFODIALOG_H

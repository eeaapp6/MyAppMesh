/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   CurveCurveProjSurfaceInfoDialog.h
 * @brief  线在面上的投影对话框.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-27
 */
#ifndef CURVECURVEPROJSURFACEINFODIALOG_H
#define CURVECURVEPROJSURFACEINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCurve.h"

namespace Ui {
    class CurveCurveProjSurfaceInfoDialog;
}

namespace GUI {
    /**
     * @brief  线在面上的投影对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class GUIDIALOGAPI CurveCurveProjSurfaceInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit CurveCurveProjSurfaceInfoDialog(QWidget *parent = nullptr);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~CurveCurveProjSurfaceInfoDialog();
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static CurveCurveProjSurfaceInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);
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
         * @brief  类型下拉框切换槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-29
         */
        void on_comb_type_currentIndexChanged(int index);
        /**
         * @brief  拾取要投影的线.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-30
         */
        void on_pb_curveprojsurface_curvepick_clicked();
        /**
         * @brief  拾取投影面.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-30
         */
        void on_pb_curveprojsurface_surfacepick_clicked();
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
         * @brief  UI.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Ui::CurveCurveProjSurfaceInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoModelCurveProjectionOnSurface* m_PreviewCmd{};
        /** @name 创建线在面上的投影  */
        /*@{*/
        Interface::VirtualShape  m_CurveProjSurfaceSourceCurve{};
        Interface::VirtualShape  m_CurveProjSurfaceSourceSurface{};
        /*@}*/
    };

}
#endif // CURVECURVEPROJSURFACEINFODIALOG_H

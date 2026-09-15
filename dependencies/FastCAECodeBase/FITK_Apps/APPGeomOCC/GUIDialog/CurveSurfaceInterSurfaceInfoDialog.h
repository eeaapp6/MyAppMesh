/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   CurveSurfaceInterSurfaceInfoDialog.h
 * @brief  面面相交对话框.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-27
 */
#ifndef CURVESURFACEINTERSURFACEINFODIALOG_H
#define CURVESURFACEINTERSURFACEINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCurve.h"

namespace Ui {
    class CurveSurfaceInterSurfaceInfoDialog;
}

namespace GUI {
    /**
     * @brief  面面相交对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class GUIDIALOGAPI CurveSurfaceInterSurfaceInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit CurveSurfaceInterSurfaceInfoDialog(QWidget *parent = nullptr);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~CurveSurfaceInterSurfaceInfoDialog();
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static CurveSurfaceInterSurfaceInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);
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
         * @brief  拾取第一个面.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-30
         */
        void on_pb_surfaceintersurface_surfacepick1_clicked();
        /**
         * @brief  拾取第二个面.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-30
         */
        void on_pb_surfaceintersurface_surfacepick2_clicked();
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
        Ui::CurveSurfaceInterSurfaceInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoModelSurfaceIntersectionSurface* m_PreviewCmd{};
        /** @name 创建面面交线  */
        /*@{*/
        Interface::VirtualShape  m_SurfaceInterSurfaceSourceSurface1{};
        Interface::VirtualShape  m_SurfaceInterSurfaceSourceSurface2{};
        /*@}*/
    };

}
#endif // CURVESURFACEINTERSURFACEINFODIALOG_H

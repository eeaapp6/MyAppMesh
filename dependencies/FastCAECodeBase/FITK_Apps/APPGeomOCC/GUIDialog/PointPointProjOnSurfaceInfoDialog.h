/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   PointPointProjOnSurfaceInfoDialog.h
 * @brief  点在面上的投影.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-30
 */
#ifndef POINTPOINTOROJONSURFACEINFODIALOG_H
#define POINTPOINTOROJONSURFACEINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPoint.h"

namespace Ui {
    class PointPointProjOnSurfaceInfoDialog;
}

namespace GUI {
    /**
     * @brief  点在线上的投影信息对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-21
     */
    class GUIDIALOGAPI PointPointProjOnSurfaceInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit PointPointProjOnSurfaceInfoDialog(QWidget *parent = nullptr);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~PointPointProjOnSurfaceInfoDialog();
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static PointPointProjOnSurfaceInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);


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
        void on_pb_projectionsurface_pointpick_clicked();
        /**
         * @brief  投影线上的拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void on_pb_projectionsurface_surfacepick_clicked();
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
        Ui::PointPointProjOnSurfaceInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoModelPointProjectionOnSurface* m_PreviewCmd{};
        /**
         * @brief  点投影到面创建中的点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::VirtualShape m_PointProjOnSurfacePoint{};
        /**
         * @brief  点投影到面创建中的面.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::VirtualShape m_PointProjOnSurfaceSurface{};
        /**
         * @brief  投影点使用的索引.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        int m_ProjPointIndex{};
    };
}
#endif // POINTPOINTOROJONSURFACEINFODIALOG_H

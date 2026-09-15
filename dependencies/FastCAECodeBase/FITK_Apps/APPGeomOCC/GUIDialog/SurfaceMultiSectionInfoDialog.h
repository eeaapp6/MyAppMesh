/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef SURFACEMULTISECTIONINFODIALOG_H
#define SURFACEMULTISECTIONINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"

namespace Ui {
    class SurfaceMultiSectionInfoDialog;
}
namespace GUI {
    /**
     * @brief  封闭曲线围成曲面信息对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-31
     */
    class GUIDIALOGAPI SurfaceMultiSectionInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit SurfaceMultiSectionInfoDialog(QWidget *parent);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~SurfaceMultiSectionInfoDialog();
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static SurfaceMultiSectionInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);

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
         * @brief  旋转线拾取槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-23
         */
        void on_pb_selectsource_clicked();
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
        Ui::SurfaceMultiSectionInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoModelMultiSectionSurface* m_PreviewCmd{};
        /**
         * @brief  围成体的面.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-23
         */
        QList<Interface::VirtualShape> m_Sections{};
    };
}

#endif // SURFACEMULTISECTIONINFODIALOG_H

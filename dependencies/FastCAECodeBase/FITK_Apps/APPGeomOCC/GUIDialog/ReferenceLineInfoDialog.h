/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef REFERENCELINEINFODIALOG_H
#define REFERENCELINEINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"

namespace Ui {
    class ReferenceLineInfoDialog;
}

namespace GUI {
    /**
     * @brief  两点线段对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class GUIDIALOGAPI ReferenceLineInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit ReferenceLineInfoDialog(QWidget *parent = nullptr);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~ReferenceLineInfoDialog();
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static ReferenceLineInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoDatumLine* cmd = nullptr);
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
         * @brief  拾取第一个点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-30
         */
        void on_pb_twopoint_pickpoint1_clicked();
        /**
         * @brief  拾取第二个点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-30
         */
        void on_pb_twopoint_pickpoint2_clicked();
        /**
         * @brief  拾取点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-30
         */
        void on_pb_pointanddir_pickpoint_clicked();
        /**
         * @brief  拾取方向.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-30
         */
        void on_pb_pointanddir_pickdir_clicked();
        /**
         * @brief  方向翻转槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-18
         */
        void onDirectionReverseSlot();
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
        Ui::ReferenceLineInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoDatumLine* m_PreviewCmd{};
    };

}


#endif // REFERENCELINEINFODIALOG_H

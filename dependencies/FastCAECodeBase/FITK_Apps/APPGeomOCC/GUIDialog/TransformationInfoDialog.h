/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   TransformationInfoDialog.h
 * @brief  变换信息对话框.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-09-11
 */
#ifndef TRANSFORMATIONINFODIALOG_H
#define TRANSFORMATIONINFODIALOG_H

#include "GUIDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoTransformation.h"

namespace Ui {
    class TransformationInfoDialog;
}

namespace GUI {
    /**
     * @brief  变换信息对话框类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-22
     */
    class GUIDIALOGAPI TransformationInfoDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        explicit TransformationInfoDialog(QWidget *parent);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        ~TransformationInfoDialog();
        /**
         * @brief  设置布尔操作类型.
         * @param  type 布尔操作类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        void setTransType(Interface::FITKGeoEnum::FITKGeometryComType type);
        /**
         * @brief  静态方法.
         * @param  parent 父对象
         * @param  oper 操作器
         * @param  cmd 命令对象
         * @return 返回唯一对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        static TransformationInfoDialog* New(QWidget* parent, EventOper::ParaWidgetInterfaceOper* oper, Interface::FITKAbsGeoCommand* cmd = nullptr);

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
        void on_comb_transform_method_currentIndexChanged(int index);
        /**
         * @brief  变换选择槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        void on_pb_selectsource_clicked();
        /**
         * @brief  移动点1选择槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        void on_pb_transform_twopoint_pickpoint1_clicked();
        /**
         * @brief  移动点2选择槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        void on_pb_transform_twopoint_pickpoint2_clicked();
        /**
         * @brief  旋转轴点1选择槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        void on_pb_rotateaxis_pickpoint1_clicked();
        /**
         * @brief  旋转轴点2选择槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        void on_pb_rotateaxis_pickpoint2_clicked();
        /**
         * @brief  缩放基准点选择槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        void on_pb_scale_pickbase_clicked();
        /**
         * @brief  对称中心元素类型切换.
         * @param  index 当前索引
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        void on_comb_mirror_type_currentIndexChanged(int index);
        /**
         * @brief  对称中心元素拾取.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        void on_lb_mirror_pickref_clicked();
        /**
         * @brief  线性阵列第一方向参数定义方式切换槽函数.
         * @param  index 当前索引
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        void on_comb_rectfirst_parameters_currentIndexChanged(int index);
        /**
         * @brief  线性阵列第一方向翻转槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-18
         */
        void onRectFirstDirectionReverseSlot();
        /**
         * @brief  线性阵列第一方向参数拾取槽函数.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        void on_pb_rectfirst_pickref_clicked();
        /**
         * @brief  线性阵列第二方向参数定义方式切换槽函数.
         * @param  index 当前索引
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        void on_comb_rectsecond_parameters_currentIndexChanged(int index);
        /**
         * @brief  线性阵列第二方向翻转槽函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-18
         */
        void onRectSecondDirectionReverseSlot();
        /**
         * @brief  线性阵列第二方向参数拾取槽函数.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        void on_pb_rectsecond_pickref_clicked();
        /**
         * @brief  环形阵列第一方向参数定义方式切换槽函数.
         * @param  index 当前索引
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        void on_comb_ciraxial_parameters_currentIndexChanged(int index);
        /**
         * @brief  环形阵列第一方向参数拾取槽函数.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        void on_pb_ciraxial_pickaxis_clicked();
        /**
         * @brief  环形阵列第一方向参数翻转槽函数.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        void on_pb_ciraxial_reverseaxis_clicked();
        /**
         * @brief  环形阵列第二方向参数定义方式切换槽函数.
         * @param  index 当前索引
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        void on_comb_circrown_parameters_currentIndexChanged(int index);
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
        Ui::TransformationInfoDialog *m_Ui{};
        /**
         * @brief  临时预览对象.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-21
         */
        Interface::FITKAbsGeoTransformation* m_PreviewCmd{};
        /**
         * @brief  变换形状.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-22
         */
        Interface::VirtualShape m_SourceShape{};
        /**
         * @brief  变换类型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        Interface::FITKGeoEnum::FITKGeometryComType m_TransType{};
        /**
         * @brief  镜像中心元素.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        Interface::VirtualShape m_MirrorReference{};
        /**
         * @brief  环形整列旋转轴.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        Interface::VirtualShape m_PatternAxis{};
        /**
         * @brief  环形整列旋转轴方向是否翻转.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        bool m_Reverse{ false };
    };
}

#endif // TRANSFORMATIONINFODIALOG_H

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUICurveDialog.h
 * @brief  材料类型选择对话框声明
 * @author wangning (2185896382@qq.com)
 * @date   2025-05-23
 *********************************************************************/
#ifndef _GUICurveDialog_H
#define _GUICurveDialog_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"

namespace Ui
{
    class GUICurveDialog;
}
namespace Radioss
{
    class FITKNonLinerCurveManager;
}
namespace Plot
{
    class FITKXYPlotWindow;
}
namespace GUI
{
    /**
     * @brief 材料类型选择对话框
     */
    class GUIAnalysisDialogAPI GUICurveDialog : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        /**
        * @brief 构造函数
        * @param 若curve为空，则为创建模式
        */
        explicit GUICurveDialog(Radioss::FITKNonLinerCurve* curve = nullptr,QWidget* parent = nullptr);
        ~GUICurveDialog();
        /**
         * @brief 重写关闭逻辑
         * @return
         */
        void reject() override;

    private:
        /**
        * @brief 初始化
        * @return
        */
        void init();
        /**
         * @brief 获取曲线管理器
         * @return
         */
        Radioss::FITKNonLinerCurveManager* getNonLinerCurveMgr();
        /**
        * @brief 获取当前表格中的坐标数据
        * @param ok 表格数据是否有效
        * @return 点链表
        */
        QList<Radioss::FITKNonLinerCurve::FITKNonLinerCurvePoint> getPointList(bool& ok);
        /**
         * @brief 重置表格
         * @return
         */
        void clearTable();
        /**
         * @brief 获取当前曲线对象
         * @return 曲线对象指针
         */
        Radioss::FITKNonLinerCurve* getCurrentCurve();
        /**
         * @brief 写入Python脚本
         * @author wangning (2185896382@qq.com)
         * @date 2026-05-22
         */
        void writePythonScript(const QString& oldName, const QString& newName,
            const QList<Radioss::FITKNonLinerCurve::FITKNonLinerCurvePoint>& pointList);

    private slots:

        /**
         * @brief OK按钮信号
         * @return
         */
        void on_pushButtonApply_clicked();
        /**
         * @brief Update按钮信号
         * @return
         */
        void on_pushButtonUpdate_clicked();
        /**
         * @brief cancle按钮信号
         * @return
         */
        void on_pushButtonCancel_clicked();

    private:

        //是否为编辑状态
        bool _isCreate{ true };

        //曲线对象ID，避免非模态窗口中持有悬空指针
        int _curveID{ -1 };

        // 曲线图表
        Plot::FITKXYPlotWindow* _xyPlotWindow = nullptr;

        //
        Ui::GUICurveDialog* _ui;
    };
}

#endif

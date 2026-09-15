/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKXYPlotCurveStyle.h
 * @brief  只用于“FITKXYPlotWindow”窗口的曲线样式小窗
 * @author wangning (2185896382@qq.com)
 * @date   2024-9-29
 *********************************************************************/
#ifndef _FITK_XYPLOTCURVE_STYLE_H___
#define _FITK_XYPLOTCURVE_STYLE_H___

#include "FITKPlotWindowAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"
#include "qwt_plot_dict.h"

class QwtPlot;
class QwtPlotCurve;

namespace Ui
{
    class FITKXYPlotCurveStyle;
}
namespace Plot
{
    class FITKXYPlotWindow;

    class FITKPlotWindowAPI FITKXYPlotCurveStyle : public QDialog
    {
        Q_OBJECT

    public:
        explicit FITKXYPlotCurveStyle(FITKXYPlotWindow *parent);
        ~FITKXYPlotCurveStyle();

        /**
         * @brief 获取曲线名称并初始化到样式窗口
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        void init();

        /**
         * @brief 获取曲线原本样式并初始化到样式窗口
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        void fillCurve(QwtPlotCurve* curveitem);

    private slots:
        /**
         * @brief 选择曲线
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        void on_currentCurveChanged(int index);

        /**
         * @brief 应用
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        void on_applyButton_clicked();

        /**
         * @brief 调色板
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        void on_colorButton_clicked();

        /**
         * @brief 取消
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        void on_cancelButton_clicked();

    private:
        /**
         * @brief 曲线链表
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        QwtPlotItemList _curveList{};
        /**
         * @brief 坐标图
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        QwtPlot * _plot{};

    private:
        Ui::FITKXYPlotCurveStyle* _ui{};
    };
}

#endif

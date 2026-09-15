/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKBarChartWindow.h
 * @brief  图表-柱状图窗口
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-20
 *********************************************************************/
#ifndef _FITK_BARCHART_WINDOW_H_
#define _FITK_BARCHART_WINDOW_H_

#include "FITKPlotWindowAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"

class QwtPlot;

namespace Ui
{
    class FITKBarChartWindow;
}
namespace Plot 
{
    class FITKBarChartProperty;
    class FITKBarChartDrawManager;

    class FITKPlotWindowAPI FITKBarChartWindow : public Core::FITKAbstractGraph2DWidget
    {
        Q_OBJECT

    public:
        explicit FITKBarChartWindow(QWidget * parent=nullptr);
        virtual ~FITKBarChartWindow();

        /**
         * @brief 获取坐标图
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        QwtPlot *getPlotWidget();

        /**
         * @brief 获取窗口属性指针
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        FITKBarChartProperty* getPlotProperty();

        /**
         * @brief 获取柱状图管理
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        FITKBarChartDrawManager* getBarChartManager();

        /**
        * @brief 更新画布
        * @author wangning (2185896382@qq.com)
        * @date   2024-10-24
        */
        void updataCanvas();

    protected:
        /**
         * @brief 坐标图指针
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        QwtPlot*_plotWidget{};

        /**
         * @brief 窗口属性
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        FITKBarChartProperty* _barChartProperty{};

        /**
         * @brief 柱状条绘画管理
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        FITKBarChartDrawManager* _barChartManager{};

    private:
        Ui::FITKBarChartWindow* _ui{};
    };
}

#endif

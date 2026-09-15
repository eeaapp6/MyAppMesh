/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPolarPlotWindow.h
 * @brief  图表-极坐标窗口
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-24
 *********************************************************************/
#ifndef _FITK_POLARPLOT_WINDOW_H_
#define _FITK_POLARPLOT_WINDOW_H_

#include "FITKPlotWindowAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"

class QwtPolarPlot;

namespace Ui
{
    class FITKPolarPlotWindow;
}
namespace Plot
{
    class FITKPolarPlotProperty;
    class FITKPolarCurveDrawManager;

    class FITKPlotWindowAPI FITKPolarPlotWindow : public Core::FITKAbstractGraph2DWidget
    {
        Q_OBJECT

    public:
        explicit FITKPolarPlotWindow(QWidget * parent = nullptr);
        virtual ~FITKPolarPlotWindow();

        /**
         * @brief 获取极坐标图
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        QwtPolarPlot * getPolarWidget();

        /**
         * @brief 获取窗口属性指针
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        FITKPolarPlotProperty* getPlotProperty();

        /**
         * @brief 获取工具栏
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        QToolBar *getToolBar();

        /**
         * @brief 获取极坐标曲线管理
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        FITKPolarCurveDrawManager* getPolarCurveManager();

        /**
         * @brief 更新画布
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-24
         */
        void updataCanvas();

    protected:
        /**
         * @brief 极坐标图指针
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        QwtPolarPlot* _polarWidget{};

        /**
         * @brief 渲染窗口工具栏对象
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        QToolBar* _renderToolBar{};

        /**
         * @brief 窗口属性
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        FITKPolarPlotProperty* _polarPlotProperty;

        /**
         * @brief 极坐标曲线绘画管理
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        FITKPolarCurveDrawManager* _polarCurveDrawManager;


    private:
        Ui::FITKPolarPlotWindow* _ui{};
    };
}

#endif


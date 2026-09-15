/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKBarChartProperty.h
 * @brief  柱状图属性类  只能用于“FITKBarChartWindow”类
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-22
 *********************************************************************/
#ifndef _FITK_BARCHARTPROPERTY_H___
#define _FITK_BARCHARTPROPERTY_H___

#include "FITKPlotWindowAPI.h"
#include <QString>
class QwtPlot;

namespace Plot
{
    class FITKBarChartWindow;

    class FITKPlotWindowAPI FITKBarChartProperty
    {
    public:
        FITKBarChartProperty(FITKBarChartWindow * Window);
        ~FITKBarChartProperty();

        /**
         * @brief 设置图表窗口标题
         * @param name 标题
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setTitle(const QString &name);

        /**
         * @brief 设置坐标轴名字
         * @param yName Y轴底部名字
         * @param xName X轴左侧名字
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setAxisName(const QString & yName, const  QString & xName= QString());

    private:
        /**
         * @brief 坐标图
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        QwtPlot* _plotWidget{};

        /**
         * @brief 管理的窗口
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        FITKBarChartWindow * _window{};
    };
}



#endif
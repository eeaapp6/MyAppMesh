/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKSpectrogramProperty.h
 * @brief  云图属性类  只能用于“FITKSpectrogramWindow”类
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-28
 *********************************************************************/
#ifndef _FITK_SPECTROGRAM_PROPERTY_H___
#define _FITK_SPECTROGRAM_PROPERTY_H___

#include "FITKPlotWindowAPI.h"

class QwtPlot;

namespace Plot
{
    class FITKSpectrogramWindow;

    class FITKPlotWindowAPI FITKSpectrogramProperty
    {
    public:
        FITKSpectrogramProperty(FITKSpectrogramWindow * Window);
        ~FITKSpectrogramProperty();

        /**
         * @brief 设置图表窗口标题
         * @param name 标题
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-28
         */
        void setTitle(const QString &name);

        /**
         * @brief 设置轴端点
         * @param axis 只能输入0或2 对应y和x轴
         * @param min，max 坐标最小最大值, 当min=max时，设置无效
         * @author wangning (2185896382@qq.com)
         * @date   2025-02-21
         */
        void setAxisScale(int axis, double min, double max);

        /**
         * @brief 设置是否启用放大缩小拖动，默认启用
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        void setPlotMagnifierAndPanner(bool isUse = true);

        /**
         * @brief 设置坐标轴名字
         * @param xName X轴底部名字
         * @param yName Y轴左侧名字
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setAxisName(const QString & xName, const  QString & yName);
        void getAxisName(QString & xName, QString & yName);

    private:
        /**
         * @brief 管理的窗口
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-28
         */
        FITKSpectrogramWindow * _window{};

    };
}



#endif

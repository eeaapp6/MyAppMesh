/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKSpectrogramManager.h
 * @brief  渲染云图管理类，只能用于“FITKSpectrogramWindow”类窗口
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-28
 *********************************************************************/

#ifndef _FITK_SPECTROGRAM_MANAGER_H___
#define _FITK_SPECTROGRAM_MANAGER_H___

#include "FITKPlotWindowAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphObject.h"
#include <QMap>
#include <QColor>

class QwtPlot;
class QwtPlotSpectrogram;

namespace Plot
{
    class FITKSpectrogramWindow;
    class FITKSpectrogramManager;
    /**
     * @brief 云图曲线属性类
     * @author wangning (2185896382@qq.com)
     * @date   2024-10-28
     */
    class FITKPlotWindowAPI FITKSpectrogramDrawPro : public Core::FITKAbstractGraphObject
    {
        friend FITKSpectrogramManager;

    public:
        explicit FITKSpectrogramDrawPro();
        virtual ~FITKSpectrogramDrawPro();

        /**
         * @brief 设置数据
         * @param  value:颜色值矩阵 x:矩阵的横坐标 y:矩阵的纵坐标 
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-28
         */
        bool setData( QVector<double> &x, QVector<double> &y, QVector<QVector<double>> &value);

        /**
         * @brief 设置颜色条自定义范围（调用后颜色条不再自动跟随数据范围）
         * @param min 最小值
         * @param max 最大值
         */
        void setColorRange(double min, double max);

        /**
         * @brief 恢复颜色条自动范围（跟随默认数据 min/max）
         */
        void autoColorRange();

        /**
         * @brief 设置自定义颜色映射（替代默认的 LinearColorMapRGB）
         * @param colorStops 颜色停靠点，key 为 [0,1] 归一化位置，首尾为起始/终止色，中间为过渡色
         */
        void setColorMap(const QMap<double, QColor>& colorStops);

        /**
         * @brief 恢复为默认颜色映射（LinearColorMapRGB）
         */
        void resetColorMap();

    private:
        QVector<double> _x;
        QVector<double> _y;
        QVector<QVector<double>> _value;

        bool _useCustomColorRange{ false };
        double _customColorMin{ 0.0 };
        double _customColorMax{ 1.0 };
        QMap<double, QColor> _customColorStops;
    };

    /**
     * @brief 云图绘画管理类
     * @author wangning (2185896382@qq.com)
     * @date   2024-10-28
     */
    class FITKPlotWindowAPI FITKSpectrogramManager : public Core::FITKGraphObjManager
    {
    public:
        FITKSpectrogramManager(FITKSpectrogramWindow*);
        virtual ~FITKSpectrogramManager();
        /**
         * @brief 添加曲线
         * @param item “FITKXYCurveDrawProp”类对象
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-28
         */
        virtual void appendGraphObj(Core::FITKAbstractGraphObject* item) override;
        /**
         * @brief 移除所有对象，并析构
         * @author wangning (2185896382@qq.com)
         * @date   2025-02-21
         */
        virtual void clear() override;

    private:
        /**
         * @brief 管理的窗口
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-28
         */
        FITKSpectrogramWindow* _window;

        /**
         * @brief 云图
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-28
         */
        QwtPlotSpectrogram* _spectrogram{};
    };
}

#endif

/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
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

    private:
        QVector<double> _x;
        QVector<double> _y;
        QVector<QVector<double>> _value;
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

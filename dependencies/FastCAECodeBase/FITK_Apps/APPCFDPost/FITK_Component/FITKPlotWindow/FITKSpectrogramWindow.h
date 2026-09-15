/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKSpectrogramWindow.h
 * @brief  图表-频谱图(云图)窗口
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-28
 *********************************************************************/
#ifndef _FITK_SPECTROGRAM_WINDOW_H_
#define _FITK_SPECTROGRAM_WINDOW_H_

#include "FITKPlotWindowAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"

class QwtPlot;

namespace Ui
{
    class FITKSpectrogramWindow;
}
namespace Plot
{
    class FITKSpectrogramManager;
    class FITKSpectrogramProperty;

    class FITKPlotWindowAPI FITKSpectrogramWindow : public Core::FITKAbstractGraph2DWidget
    {
        friend FITKSpectrogramProperty;
        friend FITKSpectrogramManager;
        Q_OBJECT

    public:
        explicit FITKSpectrogramWindow(QWidget * parent = nullptr);
        virtual ~FITKSpectrogramWindow();

        /**
         * @brief 获取坐标图
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        QwtPlot * getPlotWidget();

        /**
         * @brief 获取窗口属性指针
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        FITKSpectrogramProperty * getPlotProperty();

        /**
         * @brief 获取云图曲线管理
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        FITKSpectrogramManager * getSpectrogramManager();

    protected:
        /**
         * @brief 图表指针
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
         QwtPlot* _plotWidget{};

        /**
         * @brief 窗口属性
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
         FITKSpectrogramProperty* _spectrogramProperty{};

        /**
         * @brief 云图绘画管理
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
         FITKSpectrogramManager * _spectrogramManager{};

         /**
          * @brief 是否允许使用放大镜（放大缩小）和拖动
          * @author wangning (2185896382@qq.com)
          * @date   2025-12-16
          */
         bool _isUseMagnifierAndPanner = true;

    private:
        Ui::FITKSpectrogramWindow* _ui{};
    };
}

#endif

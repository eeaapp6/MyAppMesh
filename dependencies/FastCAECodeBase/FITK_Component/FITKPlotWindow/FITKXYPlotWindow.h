/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKXYPlotWindow.h
 * @brief  图表-直角坐标系窗口
 * @author wangning (2185896382@qq.com)
 * @date   2024-9-29
 *********************************************************************/
#ifndef _FITK_XYPLOT_WINDOW_H___
#define _FITK_XYPLOT_WINDOW_H___
 
#include "FITKPlotWindowAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"

class QwtPlot;
class QwtPlotMagnifier;
class QwtPlotPanner;

namespace Ui
{
    class FITKXYPlotWindow;
}
namespace Plot
{
    class FITKXYPlotCurve;
    class FITKXYPlotProperty;
    class FITKXYCurveDrawManager;
    class FITKXYCurveDrawProp;

    class FITKPlotWindowAPI FITKXYPlotWindow : public Core::FITKAbstractGraph2DWidget 
    {
        friend FITKXYPlotProperty;
        Q_OBJECT
    public:
        explicit FITKXYPlotWindow(QWidget* parent = nullptr);
        virtual ~FITKXYPlotWindow();

        /**
         * @brief 获取坐标图
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        QwtPlot *getPlotWidget();

        /**
         * @brief 获取工具栏
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        QToolBar *getToolBar();

        /**
         * @brief 获取窗口属性指针
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        FITKXYPlotProperty* getPlotProperty();

        /**
         * @brief 获取曲线管理器
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-21
         */
        FITKXYCurveDrawManager * getCurveManager();

        /**
         * @brief 更新画布
         * @param  updataByUser 更新曲线时是否使用用户输入的轴边界，默认不使用
         * @param  axis         0：Y轴  2：X轴   对应轴更新曲线时是否使用用户输入的轴边界
         * @author wangning (2185896382@qq.com)
         * @date   2024-9-29
         */
        void updataCanvas(bool updataByUser=false,int axis=-1);

        /**
         * @brief 获取放大缩小操作器
         * @author wangning (2185896382@qq.com)
         * @date   2025-02-21
         */
        QwtPlotMagnifier *getPlotMagnifier();

        /**
         * @brief 获取拖动操作器
         * @author wangning (2185896382@qq.com)
         * @date   2025-02-21
         */
        QwtPlotPanner* getPlotPanner();

        /**
         * @brief 保存当前渲染窗口为图片
         * @param[i]  image          图片路径
         * @param[i]  w              图片宽度
         * @param[i]  h              图片高度
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-28
         */
        virtual void saveImage(const QString& image, int w = -1, int h = -1) override;

    public slots:
        /**
         * @brief 显示隐藏曲线
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-14
         */
        void legendClicked(const QVariant&, int);
        /**
         * @brief 刷新画布
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-14
         */
        void slotActionRefreshPlotEvent();
        /*
         * @brief 创建更改样式窗口
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-15
         */
        void slotActionStyleEvent();

    protected:
        /**
         * @brief 窗体属性
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        FITKXYPlotProperty * _plotProperty{};

        /**
         * @brief 曲线绘画管理
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        FITKXYCurveDrawManager * _curveDrawManager{};

        /**
         * @brief 坐标图指针
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        QwtPlot* _plotWidget{};

        /**
         * @brief 渲染窗口工具栏对象
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        QToolBar* _renderToolBar{};

        /**
         * @brief 放大缩小操作器
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        QwtPlotMagnifier *_plotMagnifier{};

        /**
         * @brief 拖动操作器
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        QwtPlotPanner *_plotPanner{};

    private:
        /**
        * @brief 工具栏初始化
        * @author wangning (2185896382@qq.com)
        * @date   2024-10-14
        */
        void initActions();
        /**
        * @brief 端点计算器，log坐标系下，计算坐标轴的最大最小值
        * @param[i] min曲线y坐标最小值,max曲线y坐标最大值
        * @author wangning (2185896382@qq.com)
        * @date   2025-5-20
        */
        void calculatorEndpoint(double &min,double& max);

    private:
        /**
         * @brief 用户设置的轴边界
         * @author wangning (2185896382@qq.com)
         * @date   2025-02-25
         */
        double _userXmax=0, _userXmin=0, _userYmax=0, _userYmin=0;

    private:
        /**
         * @brief 轴边界(所有曲线中的最大/最小值)
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        double _Xmin=1e64, _Xmax=-1e64, _Ymin=1e64, _Ymax=-1e64;

    private:
        Ui::FITKXYPlotWindow* _ui{};
    };  
}

#endif

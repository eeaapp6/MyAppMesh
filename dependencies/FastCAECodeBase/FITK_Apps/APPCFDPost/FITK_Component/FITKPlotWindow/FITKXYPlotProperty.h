/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKXYPlotProperty.h
 * @brief  直角坐标系图表属性类  只能用于“FITKXYPlotWindow”类
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-20
 *********************************************************************/

#ifndef _FITK_XYPLOT_PROPERTY_H___
#define _FITK_XYPLOT_PROPERTY_H___

#include "FITKPlotWindowAPI.h"

class QwtPlot;
class QwtPlotGrid;

namespace Plot
{
    class FITKXYPlotWindow;

    class FITKPlotWindowAPI FITKXYPlotProperty
    {
    public:
        FITKXYPlotProperty(FITKXYPlotWindow * Window);
        ~FITKXYPlotProperty();

    //外部设置属性
        /**
         * @brief 设置图表窗口标题
         * @param name 标题
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setTitle(const QString &name);

        /**
         * @brief 是否显示图例(默认显示)
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setLegendVisible(bool is);

        /**
         * @brief 是否可以点击图例(默认可以点击)
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setLegendClickable(bool is);

        /**
         * @brief 设置坐标轴名字
         * @param xName X轴底部名字
         * @param yName Y轴左侧名字
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setAxisName(const QString & xName, const  QString & yName);
        void getAxisName(QString & xName, QString & yName);

        /**
         * @brief 将普通轴设置为指数轴（如果不需要指数轴不必调用该函数）
         * @param  axis 只能输入0或2 对应y和x轴
         * @param  isLog 是否设为指数轴，默认开启
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setAxisScaleEngine(int axis,bool isLog=true);

        /**
         * @brief 是否为指数轴（如果是指数轴返回true）
         * @param  axis 只能输入0或2 对应y和x轴
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        bool isAxisScaleEngine(int axis);

        /**
         * @brief 设置轴的大刻度密度
         * @param  axis只能输入0或2 对应y和x轴  num显示的大刻度个数
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setAxisMaxMajor(int axis,int num);

        /**
         * @brief 设置图例位置 ,不调用该函数时默认生成在窗口底部
         * @param  place 只能输入1或2,对应窗口右侧和底部
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setLegendPlace(int place);

        /**
         * @brief 设置图表标题字号
         * @param  size 字号
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setTitleSize(int size);

        /**
        * @brief  获取图表标题字号
        * @author wangning (2185896382@qq.com)
        * @date   2024-10-20
        */
        int getTitleSize() const;

        /**
         * @brief 设置轴标题字号
         * @param  Xsize X轴字号 Ysize Y轴字号 
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setAxisTitleSize(int Xsize,int Ysize);

        /**
        * @brief  获取轴标题字号
        * @param  axis 只能输入0或2 对应y和x轴
        * @author wangning (2185896382@qq.com)
        * @date   2024-10-20
        */
        int getAxisTitleSize(int axis) const;

        /**
         * @brief 删除工具栏
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void disToolBar();

        /**
         * @brief 设置是否启用背景网格,不调用该函数时默认启用
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        void setPlotGrid(bool is);

        /**
         * @brief 设置轴端点
         * @param axis 只能输入0或2 对应y和x轴
         * @param min，max 坐标最小最大值, 当min=max时，设置无效，并根据当前曲线最值自动设置端点
         * @author wangning (2185896382@qq.com)
         * @date   2025-02-21
         */
        void setAxisScale(int axis, double min, double max);

        /**
         * @brief 获取当前轴端点
         * @param axis 只能输入0或2 对应y和x轴
         * @param min，max 坐标最小最大值
         * @author wangning (2185896382@qq.com)
         * @date   2025-02-21
         */
        void getAxisScale(int axis, double &min, double &max);

        /**
         * @brief 设置是否启用放大缩小，默认启用
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        void setPlotMagnifier(bool isUse=true);

        /**
        * @brief 设置是否启用拖动，默认启用
        * @author wangning (2185896382@qq.com)
        * @date   2024-10-22
        */
        void setPlotPanner(bool isUse = true);

        /**
        * @brief 设置是否启用数量级端点（只对Log轴有效）
        * @param type 只能输入0或2 对应y和x轴 ,isUse 是否启用
        * @author wangning (2185896382@qq.com)
        * @date   2024-10-22
        */
        void setLogAxisOrderOfMagnitudeEndpoint(int type,bool isUse = true);

        /**
       * @brief 获取是否启用数量级端点（只对Log轴有效）
       * @param type 只能输入0或2 对应y和x轴 
       * @author wangning (2185896382@qq.com)
       * @date   2024-10-22
       */
        bool getLogAxisOrderOfMagnitudeEndpoint(int type);

    private:
        /**
         * @brief 坐标图
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        QwtPlot* _plotWidget{};

        /**
         * @brief 背景网格
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        QwtPlotGrid * _grid{};

        /**
         * @brief 管理的窗口
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        FITKXYPlotWindow * _window{};

        /*
         * @brief 该参数只用于log轴,是否将轴端点设置为最接近的数量级
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-20
         */
        bool isOrderOfMagnitude_x = true;
        bool isOrderOfMagnitude_y = true;
    };
}

#endif

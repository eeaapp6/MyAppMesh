/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKBarChartDrawManager.h
 * @brief  渲染柱状图管理类，只能用于“FITKBarChartWindow”类窗口
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-22
 *********************************************************************/

#ifndef _FITK_BARCHARTDRAW_MANAGER_H___
#define _FITK_BARCHARTDRAW_MANAGER_H___

#include "FITKPlotWindowAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphObject.h"
#include <QColor>

class QwtPlot;
class QwtPlotMultiBarChart;
class DistroScaleDraw;
class DistroPlotBarChart;

namespace Plot
{
    class FITKBarChartWindow;
    class FITKBarChartDrawManager;
    /**
     * @brief  柱状图属性类
     * @author wangning (2185896382@qq.com)
     * @date   2024-10-22
     */
    class FITKPlotWindowAPI FITKBarChartDrawProp : public Core::FITKAbstractGraphObject
    {
        friend FITKBarChartDrawManager;
    public:
        explicit FITKBarChartDrawProp();
        virtual ~FITKBarChartDrawProp();

         /**
          * @brief 设置所有柱形条标题和柱形条组数,返回"barNames.size×groupNum"
          * @author wangning (2185896382@qq.com)
          * @date   2024-10-22
          */
        int setBarNameGroupNum(QList<QString>& barNames,int groupNum =1);

        /**
         * @brief 设置所有柱形条颜色，要求与柱形条标题顺序一致，不调用该函数时会用默认颜色，最多8种
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        void setColor(QList<QColor>& colors);

        /**
         * @brief !!该函数需最后调用!!,设置总数据，要求数据数量=柱形条个数×组数量(barNames.size×groupNum)
         * @param 
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        bool setData(QVector<double> &data);


        /**
         * @brief 自定义横坐标字符，需与_groupNum数量一致，且_groupNum大于1才可调用
         * @param
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        void setXLabel(QList<QString> & xLabel);

    private:
        /**
         * @brief 默认颜色（8个），如果需要颜色超过8个须调用setColor（）重新设置
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        QList<QColor> _colors{ QColor("chocolate"),QColor("blueviolet"),QColor("cornflowerblue"),QColor("crimson"),
            QColor("lightseagreen"),QColor("orange"),QColor("darksalmon"),QColor("aquamarine"),};
        /**
         * @brief 柱形条标题
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        QList<QString> _barNames{};

        /**
         * @brief 柱形条组数
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        int _groupNum=1;

        /**
         * @brief 柱形条数据
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        QVector<double> _data{};

        /**
         * @brief 自定义横坐标字符
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        QList<QString>  _xLabel{};

    private:
        /**
         * @brief 柱形条数据的个数
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-23
         */
        int _dataSize=-1;
    };



    /**
     * @brief 柱状图绘画管理类
     * @author wangning (2185896382@qq.com)
     * @date   2024-10-22
     */
    class FITKPlotWindowAPI FITKBarChartDrawManager : public Core::FITKGraphObjManager
    {
    public:
        FITKBarChartDrawManager(FITKBarChartWindow*);
        virtual ~FITKBarChartDrawManager();
        /**
         * @brief 添加柱状图,每次调用该函数都会将之前appendGraphObj的内容覆盖
         * @param item “FITKBarChartDrawProp”类对象
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        virtual void appendGraphObj(Core::FITKAbstractGraphObject* item) override;

        /**
         * @brief 移除显示对象,并未析构
         * @param[i]  item            显示对象
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        void removeGraphObj();

        /**
        * @brief 移除所有对象，并析构
        * @author wangning (2185896382@qq.com)
        * @date   2025-02-21
        */
        virtual void clear() override;

    private:
        /**
         * @brief 添加条组型柱状图
         * @param[i]  
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        bool appendMultiBarChart(FITKBarChartDrawProp*);

        /**
         * @brief 添加单条型柱状图
         * @param[i]  
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        bool appendUnitBarChart(FITKBarChartDrawProp*);

    private:
        /**
         * @brief 管理的窗口
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        FITKBarChartWindow* _window;

        /**
         * @brief 柱状条组
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        QwtPlotMultiBarChart * _barChartItem{};

        /**
         * @brief 柱状条
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        DistroPlotBarChart * _barChartUnit{};
    };
}

#endif

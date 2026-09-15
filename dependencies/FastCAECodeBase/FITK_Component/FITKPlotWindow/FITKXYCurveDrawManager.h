/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKXYCurveDrawManager.h
 * @brief  渲染曲线管理类，只能用于“FITKXYPlotWindow”类窗口
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-18
 *********************************************************************/

#ifndef _FITK_XYCURVEDRAW_MANAGER_H___
#define _FITK_XYCURVEDRAW_MANAGER_H___

#include "FITKPlotWindowAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphObject.h"
#include <QColor>

class QwtPlotCurve;
class QwtPlot;

namespace Plot 
{
    class FITKXYPlotWindow;
    class FITKXYCurveDrawManager;
    /**
     * @brief 直角坐标曲线属性类
     * @author wangning (2185896382@qq.com)
     * @date   2024-10-20
     */
    class FITKPlotWindowAPI FITKXYCurveDrawProp : public Core::FITKAbstractGraphObject
    {
        friend FITKXYCurveDrawManager;
    public:
        explicit FITKXYCurveDrawProp();
        virtual ~FITKXYCurveDrawProp();

        /**
         * @brief 设置曲线颜色,默认线粗为一个像素
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setColor(QColor color);

        /**
         * @brief 设置线型
         * @param 0到5分别为 无，实线，虚线，点线，点划线，点点划线
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setLineType(int style);

         /**
          * @brief 设置曲线符号，如果不调用该函数,默认没有符号
          * @param -1到4分别为 无符号，圆形，矩形，钻石，向上的三角，向下的三角
          * @author wangning (2185896382@qq.com)
          * @date   2024-10-20
          */
        void setSymbol(int style);

        /**
         * @brief 设置隐藏显示曲线
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        void setVisible(bool is);
        bool getVisible();

         /**
          * @brief 设置数据
          * @param name 曲线名
          * @param xData，yData坐标点
          * @author wangning (2185896382@qq.com)
          * @date   2024-10-20
          */
        bool setData(const QString &name, QVector<double> &xData, QVector<double> &yData);

        /**
         * @brief 获取曲线名
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        QString getCurveTitle();

        /**
         * @brief 获取所有曲线坐标点
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        QList<QPointF> getPointList();

    private:
        /**
         * @brief 曲线
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        QwtPlotCurve * _curve{};
        /**
         * @brief 是否设置过颜色
         * @author wangning (2185896382@qq.com)
         * @date   2025-02-26
         */
        bool _isSetColor=false;
    };

     /**
      * @brief 直角坐标系曲线绘画管理类
      * @author wangning (2185896382@qq.com)
      * @date   2024-10-20
      */
    class FITKPlotWindowAPI FITKXYCurveDrawManager: public Core::FITKGraphObjManager
    {
    public:
        FITKXYCurveDrawManager(FITKXYPlotWindow*);
        virtual ~FITKXYCurveDrawManager();
        /**
         * @brief 添加曲线
         * @param item “FITKXYCurveDrawProp”类对象
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-20
         */
        virtual void appendGraphObj(Core::FITKAbstractGraphObject* item) override;

        /**
         * @brief 移除显示对象，并未析构
         * @param[i]  item         显示对象
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-22
         */
        virtual void removeGraphObj(Core::FITKAbstractGraphObject* item) override;

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
         * @date   2024-10-20
         */
        FITKXYPlotWindow* _window;
        /**
         * @brief 默认颜色（只有8个）
         * @author wangning (2185896382@qq.com)
         * @date   2025-02-26
         */
        QList<QColor> _colors{ QColor("chocolate"),QColor("blueviolet"),QColor("cornflowerblue"),QColor("crimson"),
            QColor("lightseagreen"),QColor("orange"),QColor("darksalmon"),QColor("aquamarine"), };

    };
}

#endif
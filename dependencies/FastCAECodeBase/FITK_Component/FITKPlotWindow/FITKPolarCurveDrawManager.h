/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPolarCurveDrawManager.h
 * @brief  渲染极坐标图管理类  只能用于“FITKPolarPlotWindow”类
 * @author wangning (2185896382@qq.com)
 * @date   2024-10-25
 *********************************************************************/
#ifndef _FITK_POLARPLOTDRAW_MANAGER_H___
#define _FITK_POLARPLOTDRAW_MANAGER_H___

#include "FITKPlotWindowAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphObject.h"

class QwtPolarPlot;
class QwtPolarCurve;

namespace Plot
{
    class FITKPolarPlotWindow;
    class FITKPolarCurveDrawManager;
    /**
     * @brief 极坐标曲线属性类
     * @author wangning (2185896382@qq.com)
     * @date   2024-10-25
     */
    class FITKPlotWindowAPI FITKPolarCurveDrawProp : public Core::FITKAbstractGraphObject
    {
        friend FITKPolarCurveDrawManager;
    public:
        explicit FITKPolarCurveDrawProp();
        virtual ~FITKPolarCurveDrawProp();

        /**
         * @brief 设置曲线颜色,默认线粗为一个像素
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        void setColor(QColor color);

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
         * @brief 极坐标曲线
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        QwtPolarCurve* _polarCurve{};

        /**
         * @brief 极径最大值
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        double polarPlotMax{};
    };


    /**
     * @brief 极坐标系曲线绘画管理类
     * @author wangning (2185896382@qq.com)
     * @date   2024-10-25
     */
    class FITKPlotWindowAPI FITKPolarCurveDrawManager : public Core::FITKGraphObjManager
    {
    public:
        FITKPolarCurveDrawManager(FITKPolarPlotWindow * Window);
        virtual ~FITKPolarCurveDrawManager();

        /**
         * @brief 添加曲线
         * @param item “FITKPolarCurveDrawProp”类对象
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
         */
        virtual void appendGraphObj(Core::FITKAbstractGraphObject* item) override;

        /**
         * @brief 移除显示对象
         * @param[i]  item         显示对象
         * @author wangning (2185896382@qq.com)
         * @date   2024-10-25
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
         * @date   2024-10-25
         */
        FITKPolarPlotWindow * _window{};
    };
}

#endif


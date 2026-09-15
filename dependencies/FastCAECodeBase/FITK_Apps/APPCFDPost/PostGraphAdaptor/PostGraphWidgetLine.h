/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphWidgetLine.h
 * @brief  VTK线界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-09
 *********************************************************************/
#ifndef _PostGraphWidgetLine_H_
#define _PostGraphWidgetLine_H_

#include <QObject>
#include "PostGraphAdaptorAPI.h"

class vtkLineWidget2;
class vtkLineRepresentation;

namespace Interface
{
    class PostGraphWidgetLineCallBack;
    /**
     * @brief  VTK线界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-09
     */
    class PostGraphAdaptorAPI PostGraphWidgetLine : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] widget 父界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        PostGraphWidgetLine(QWidget* widget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        ~PostGraphWidgetLine();
        /**
         * @brief    设置线的位置
         * @param[i] startPoint        开始点
         * @param[i] endPoint          结束点
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void setLine(double* startPoint, double* endPoint);
        /**
         * @brief    设置是否显示
         * @param[i] isShow           是否显示
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void setIsShow(bool isShow);
    signals:
        ;
        /**
         * @brief    数据更改
         * @param[i] origin        始点
         * @param[i] normal        终点
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void sigValueChange(double* startPoint, double* endPoint);
    private:
        /**
         * @brief  线界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkLineWidget2* _widget = nullptr;
        /**
         * @brief  线界面代理
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-19
         */
        vtkLineRepresentation* _representation = nullptr;
        /**
         * @brief  回调
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        PostGraphWidgetLineCallBack* _callBack = nullptr;
    };
}

#endif
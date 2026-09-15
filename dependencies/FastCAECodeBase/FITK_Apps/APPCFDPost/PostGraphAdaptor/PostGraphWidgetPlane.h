/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphWidgetPlane.h
 * @brief  VTK平面界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-09
 *********************************************************************/
#ifndef _PostGraphWidgetPlane_H_
#define _PostGraphWidgetPlane_H_

#include <QObject>
#include "PostGraphAdaptorAPI.h"

class vtkImplicitPlaneWidget2;

namespace Interface
{
    class PostGraphWidgetPlaneCallBack;
    /**
     * @brief  VTK平面界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-09
     */
    class PostGraphAdaptorAPI PostGraphWidgetPlane : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] widget 父界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        PostGraphWidgetPlane(QWidget* widget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        ~PostGraphWidgetPlane();
        /**
         * @brief    设置边界
         * @param[i] bounds           边界[6]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void setBounds(double* bounds);
        /**
         * @brief    设置数据
         * @param[i] origin           原点[3]
         * @param[i] normal           方向[3]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void setValue(double* origin, double* normal);
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
         * @param[i] origin        原点
         * @param[i] normal        轴向
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void sigValueChange(double* origin, double* normal);
    private:
        /**
         * @brief  裁切界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkImplicitPlaneWidget2* _widget = nullptr;
        /**
         * @brief  回调
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        PostGraphWidgetPlaneCallBack* _callBack = nullptr;
    };
}

#endif
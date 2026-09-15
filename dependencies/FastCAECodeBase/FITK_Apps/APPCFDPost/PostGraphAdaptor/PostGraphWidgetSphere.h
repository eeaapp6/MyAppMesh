/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphWidgetSphere.h
 * @brief  VTK球界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-09
 *********************************************************************/
#ifndef _PostGraphWidgetSphere_H_
#define _PostGraphWidgetSphere_H_

#include <QObject>
#include "PostGraphAdaptorAPI.h"

class vtkSphereWidget;

namespace Interface
{
    class PostGraphWidgetSphereCallBack;
    /**
     * @brief  VTK球界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-09
     */
    class PostGraphAdaptorAPI PostGraphWidgetSphere : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] widget 父界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        PostGraphWidgetSphere(QWidget* widget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        ~PostGraphWidgetSphere();
        /**
         * @brief    设置是否显示
         * @param[i] isShow           是否显示
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void setIsShow(bool isShow);
        /**
         * @brief    设置数据
         * @param[i] center           中心[3]
         * @param[i] radius           半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        void setValue(double* center, double radius);
        /**
         * @brief    获取数据
         * @param[o] center           中心[3]
         * @param[o] radius           半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        void getValue(double* center, double& radius);
    signals:
        ;
        /**
         * @brief    数据更改
         * @param[i] origin        中心点
         * @param[i] normal        半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void sigValueChange(double* center, double radius);
    private:
        /**
         * @brief  裁切界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkSphereWidget* _widget = nullptr;
        /**
         * @brief  回调
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        PostGraphWidgetSphereCallBack* _callBack = nullptr;
    };
}

#endif
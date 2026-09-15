/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphWidgetSphereCallBack.h
 * @brief  VTK球面界面回调类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-09
 *********************************************************************/
#ifndef _PostGraphWidgetSphereCallBack_H_
#define _PostGraphWidgetSphereCallBack_H_

#include <QObject>
#include <vtkCommand.h>

namespace Interface
{
    /**
     * @brief  VTK球面界面回调类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-09
     */
    class PostGraphWidgetSphereCallBack : public QObject, public vtkCommand
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        PostGraphWidgetSphereCallBack() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        ~PostGraphWidgetSphereCallBack() = default;
        /**
         * @brief    回调函数重写
         * @param[i] caller         对象
         * @param[i] eventId
         * @param[i] callData
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        virtual void Execute(vtkObject *caller, unsigned long eventId, void* callData) override;
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
    };
}

#endif
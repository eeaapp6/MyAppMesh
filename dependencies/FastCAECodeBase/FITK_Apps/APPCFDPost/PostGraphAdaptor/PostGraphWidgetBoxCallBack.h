/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphWidgetBoxCallBack.h
 * @brief  VTK包围盒界面回调类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-09
 *********************************************************************/
#ifndef _PostGraphWidgetBoxCallBack_H_
#define _PostGraphWidgetBoxCallBack_H_

#include <QObject>
#include <vtkCommand.h>

namespace Interface
{
    /**
     * @brief  VTK包围盒界面回调类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-09
     */
    class PostGraphWidgetBoxCallBack : public QObject, public vtkCommand
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        PostGraphWidgetBoxCallBack() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        ~PostGraphWidgetBoxCallBack() = default;
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
         * @param[i] bounds      边界[6]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        void sigValueChange(double* bounds);
    };
}

#endif
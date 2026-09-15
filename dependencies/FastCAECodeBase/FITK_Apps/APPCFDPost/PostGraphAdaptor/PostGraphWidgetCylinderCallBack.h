/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphWidgetCylinderCallBack.h
 * @brief  VTK圆柱型界面回调类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-19
 *********************************************************************/
#ifndef _PostGraphWidgetCylinderCallBack_H_
#define _PostGraphWidgetCylinderCallBack_H_

#include <QObject>
#include <vtkCommand.h>

namespace Interface
{
    /**
     * @brief  VTK圆柱型界面回调类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-19
     */
    class PostGraphWidgetCylinderCallBack : public QObject, public vtkCommand
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        PostGraphWidgetCylinderCallBack() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        ~PostGraphWidgetCylinderCallBack() = default;
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
         * @param[i] center      中心点[3]
         * @param[i] axis        方向[3]
         * @param[i] radius      半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-11
         */
        void sigValueChange(double* center, double* axis, double radius);
    };
}
#endif
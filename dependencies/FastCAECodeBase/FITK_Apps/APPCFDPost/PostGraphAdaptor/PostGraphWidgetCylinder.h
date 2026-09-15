/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphWidgetCylinder.h
 * @brief  VTK圆柱界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-09
 *********************************************************************/
#ifndef _PostGraphWidgetCylinder_H_
#define _PostGraphWidgetCylinder_H_

#include <QObject>
#include "PostGraphAdaptorAPI.h"

class vtkImplicitCylinderWidget;
class vtkImplicitCylinderRepresentation;

namespace Interface
{
    class PostGraphWidgetCylinderCallBack;
    /**
     * @brief  VTK线界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-09
     */
    class PostGraphAdaptorAPI PostGraphWidgetCylinder : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] widget 父界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        PostGraphWidgetCylinder(QWidget* widget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        ~PostGraphWidgetCylinder();
        /**
         * @brief    设置边界
         * @param[i] bounds           边界[6]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void setBounds(double* bounds);
        /**
         * @brief    设置圆柱位置
         * @param[i] center      中心点[3]
         * @param[i] axis        方向[3]
         * @param[i] radius      半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-11
         */
        void setValue(double* center, double* axis, double radius);
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
         * @param[i] center      中心点[3]
         * @param[i] axis        方向[3]
         * @param[i] radius      半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-11
         */
        void sigValueChange(double* center, double* axis, double radius);
    private:
        /**
         * @brief  VTK圆柱界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkImplicitCylinderWidget* _widget = nullptr;
        /**
         * @brief  代理
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-11
         */
        vtkImplicitCylinderRepresentation* _representation = nullptr;
        /**
         * @brief  回调
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        PostGraphWidgetCylinderCallBack* _callBack = nullptr;
    };
}

#endif
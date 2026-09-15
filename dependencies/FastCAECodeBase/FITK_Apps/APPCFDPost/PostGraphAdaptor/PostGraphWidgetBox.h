/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphWidgetBox.h
 * @brief  VTK包围盒界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-09
 *********************************************************************/
#ifndef _PostGraphWidgetBox_H_
#define _PostGraphWidgetBox_H_

#include <QObject>
#include "PostGraphAdaptorAPI.h"

class vtkBoxWidget2;

namespace Interface
{
    class PostGraphWidgetBoxCallBack;
    class FITKVKTBoxRepresentaion;
    /**
     * @brief  VTK包围盒界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-09
     */
    class PostGraphAdaptorAPI PostGraphWidgetBox : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] widget 父界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        PostGraphWidgetBox(QWidget* widget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        ~PostGraphWidgetBox();
        /**
         * @brief    设置数据
         * @param[i] bounds      数据[6]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        void setValue(double* bounds);
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
         * @param[i] bounds   边界[6]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        void sigValueChange(double* bounds);
    private:
        /**
         * @brief  VTK界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkBoxWidget2* _widget = nullptr;
        /**
         * @brief  VTK界面代理
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-19
         */
        FITKVKTBoxRepresentaion* _representation = nullptr;
        /**
         * @brief  回调
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        PostGraphWidgetBoxCallBack* _callBack = nullptr;
    };
}

#endif
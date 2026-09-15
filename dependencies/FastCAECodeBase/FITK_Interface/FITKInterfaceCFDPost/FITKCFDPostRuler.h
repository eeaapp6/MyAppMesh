/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostRuler.h
 * @brief  后处理尺数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-11-07
 *********************************************************************/
#ifndef _FITKCFDPostRuler_H
#define _FITKCFDPostRuler_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"
#include <QColor>

namespace Interface
{
    /**
     * @brief  后处理尺数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-11-07
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostRuler :public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parentID 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-07
         */
        explicit FITKCFDPostRuler();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-07
         */
        virtual ~FITKCFDPostRuler();
        /**
         * @brief    获取后处理数据类型
         * @return   FITKPostDataType        后处理数据类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual FITKPostDataType getPostDataType() override;
        /**
         * @brief    设置点
         * @param[i] point1         点1 
         * @param[i] point2         点2
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-07
         */
        void setPoint(double* point1, double* point2);
        /**
         * @brief    获取点
         * @param[i] point1         点1
         * @param[i] point2         点2
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-07
         */
        void getPoint(double* point1, double* point2);
        /**
         * @brief    设置颜色
         * @param[i] color      颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-07
         */
        void setColor(QColor color);
        /**
         * @brief    获取颜色
         * @return   QColor     颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-07
         */
        QColor getColor();
        /**
         * @brief    获取长度
         * @return   double         长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-07
         */
        double getLength();
    protected:
        /**
         * @brief  点1
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-11-07
         */
        double _point1[3] = { 0,0,0 };
        /**
         * @brief  点2
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-11-07
         */
        double _point2[3] = { 0,0,0 };
        /**
         * @brief  
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-11-07
         */
        QColor _color = QColor::fromRgbF(0, 1, 0);
    };
}

#endif

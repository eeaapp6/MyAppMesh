/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostStreamLine.h
 * @brief  流线数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-19
 *********************************************************************/
#ifndef _FITKCFDPostStreamLine_H
#define _FITKCFDPostStreamLine_H

#include "FITKAbstractCFDPostData.h"

class vtkLineSource;
class vtkStreamTracer;

namespace Interface
{
    /**
     * @brief  流线数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-19
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostStreamLine :public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parentID      父对象id 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        FITKCFDPostStreamLine(int parentID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        virtual ~FITKCFDPostStreamLine();
        /**
         * @brief    获取后处理数据类型
         * @return   FITKPostDataType        后处理数据类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual FITKPostDataType getPostDataType();
        /**
         * @brief    获取数据
         * @return   vtkDataSet*  数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        virtual vtkDataSet* getOutput() override;
        /**
         * @brief    获取算法输出
         * @return   vtkAlgorithmOutput*   算法输出
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-01
         */
        virtual vtkAlgorithmOutput* getOutputPort() override;
        /**
         * @brief    设置矢量
         * @param[i] type         类型（点、单元）
         * @param[i] vector       矢量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void setVector(FITKPostFieldType vectorType, QString vectorName);
        /**
         * @brief    获取矢量
         * @param[o] type         类型（点、单元）
         * @param[o] vector       矢量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-27
         */
        void getVector(FITKPostFieldType& vectorType, QString& vectorName);
        /**
         * @brief    设置流线积分方向
         * @param[i] dir          方向
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void setDirection(FITKPostStreamDirection dir);
        /**
         * @brief    获取流线积分方向
         * @return   FITKPostStreamDirection    流线积分方向
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-27
         */
        FITKPostStreamDirection getDirection();
        /**
         * @brief    设置长度
         * @param[i] length       长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void setLength(double length);
        /**
         * @brief    获取长度
         * @return   double       长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-27
         */
        double getLength();
        /**
         * @brief    设置线
         * @param[i] startPoint          初始点
         * @param[i] endPoint            结束点
         * @param[i] resolution          精度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void setLine(double* startPoint, double* endPoint, int resolution);
        /**
         * @brief    获取线 
         * @param[o] startPoint          初始点 
         * @param[o] endPoint            结束点
         * @param[o] resolution          精度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void getLine(double* startPoint, double* endPoint, int& resolution);
    private:
        /**
         * @brief  线数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-19
         */
        vtkLineSource* _lineSource = nullptr;
        /**
         * @brief  流线过滤器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-19
         */
        vtkStreamTracer* _streamFilter = nullptr;
        /**
         * @brief  名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-27
         */
        QString _vectorName = "";
        /**
         * @brief  矢量类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-27
         */
        FITKPostFieldType _vectorType = FITKPostFieldType::Post_None;
        /**
         * @brief  流线积分方向类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-27
         */
        FITKPostStreamDirection _directionType = FITKPostStreamDirection::Post_None;
    };
}

#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostSlicePlane.h
 * @brief  后处理平面切片数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostSlicePlane_H
#define _FITKCFDPostSlicePlane_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"

class vtkClipDataSet;
class vtkPlane;
class vtkCutter;

namespace Interface 
{
    class FITKCFDPost3DManager;
    /**
     * @brief  后处理平面切片数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostSlicePlane : public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        FITKCFDPostSlicePlane(int parentID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostSlicePlane();
        /**
         * @brief    获取后处理数据类型
         * @return   FITKPostDataType        后处理数据类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual FITKPostDataType getPostDataType() override;
        /**
         * @brief    获取数据对象
         * @return   vtkDataSet*  vtk数据对象
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
         * @brief    设置数据
         * @param[i] origin           基础点
         * @param[i] normal           方向
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void setValue(double* origin, double* normal);
        /**
         * @brief    获取数据
         * @param[o] origin           基础点[3]
         * @param[o] normal           方向[3]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void getValue(double* origin, double* normal);
    private:
        /**
         * @brief  切片数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkCutter* _slice = nullptr;
        /**
         * @brief  plane
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkPlane* _plane = nullptr;
    };
}

#endif

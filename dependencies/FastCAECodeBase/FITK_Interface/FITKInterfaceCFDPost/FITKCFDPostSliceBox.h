/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostSliceBox.h
 * @brief  后处理包围盒切片数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostSliceBox_H
#define _FITKCFDPostSliceBox_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"

class vtkCutter;
class vtkBox;

namespace Interface
{
    class FITKCFDPost3DManager;
    /**
     * @brief  后处理包围盒切片数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostSliceBox : public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        FITKCFDPostSliceBox(int parentID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostSliceBox();
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
         * @param[i] bounds     包围盒
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        void setValue(double* bounds);
        /**
         * @brief    获取数据
         * @param[i] bounds     包围盒
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        void getValue(double* bounds);
    private:
        /**
         * @brief  切片数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkCutter* _sliceDataSet = nullptr;
        /**
         * @brief  box
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkBox* _box = nullptr;
    };
}

#endif

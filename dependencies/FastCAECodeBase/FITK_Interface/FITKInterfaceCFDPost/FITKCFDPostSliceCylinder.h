/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostSliceCylinder.h
 * @brief  后处理圆柱型切片数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostSliceCylinder_H
#define _FITKCFDPostSliceCylinder_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"

class vtkCutter;
class vtkCylinder;

namespace Interface
{
    class FITKCFDPost3DManager;
    /**
     * @brief  后处理圆柱型切片数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostSliceCylinder : public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        FITKCFDPostSliceCylinder(int parentID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostSliceCylinder();
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
         * @param[i] center      中心点[3]
         * @param[i] axis        方向[3]
         * @param[i] radius      半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-11
         */
        void setValue(double* center, double* axis, double radius);
        /**
         * @brief    获取数据
         * @param[o] center      中心点[3]
         * @param[o] axis        方向[3]
         * @param[o] radius      半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-11
         */
        void getValue(double* center, double* axis, double& radius);
    private:
        /**
         * @brief  切片数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkCutter* _sliceDataSet = nullptr;
        /**
         * @brief  cylinder
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkCylinder* _cylinder = nullptr;
    };
}

#endif

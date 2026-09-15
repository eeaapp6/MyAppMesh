/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostClipCylinder.h
 * @brief  后处理圆柱型切割数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostClipCylinder_H
#define _FITKCFDPostClipCylinder_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"

class vtkClipDataSet;
class vtkCylinder;

namespace Interface
{
    class FITKCFDPost3DManager;
    /**
     * @brief  后处理圆柱型切割数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostClipCylinder : public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        FITKCFDPostClipCylinder(int parentID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostClipCylinder();
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
         * @brief  切割数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkClipDataSet* _clipDataSet = nullptr;
        /**
         * @brief  cylinder
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkCylinder* _cylinder = nullptr;
    };
}

#endif

/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostClipBox.h
 * @brief  后处理包围盒切割数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostClipBox_H
#define _FITKCFDPostClipBox_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"

class vtkClipDataSet;
class vtkBox;

namespace Interface
{
    class FITKCFDPost3DManager;
    /**
     * @brief  后处理包围盒切割数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostClipBox : public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        FITKCFDPostClipBox(int parentID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostClipBox();
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
         * @param[i] boundary     范围[6]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-11
         */
        void setValue(double* boundary);
        /**
         * @brief    获取数据
         * @param[i] boundary     范围[6] 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-11
         */
        void getValue(double* boundary);
    private:
        /**
         * @brief  切割数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkClipDataSet* _clipDataSet = nullptr;
        /**
         * @brief  box
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkBox* _box = nullptr;
    };
}

#endif

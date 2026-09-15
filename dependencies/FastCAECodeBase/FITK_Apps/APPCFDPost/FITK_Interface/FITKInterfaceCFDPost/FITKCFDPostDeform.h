/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostDeform.h
 * @brief  后处理变形数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostDeform_H
#define _FITKCFDPostDeform_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"

class FITKDeformFilter;

namespace Interface
{
    class FITKCFDPost3DManager;
    /**
     * @brief  后处理变形数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostDeform : public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        FITKCFDPostDeform(int parentID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostDeform();
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
         * @brief 设置变形因子
         * @param[i]  fac      变形因子
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-15
         */
        void setDeformFactor(const double fac);
        /**
         * @brief 获取变形因子
         * @return double
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-15
         */
        double getDeformFactor();
        /**
         * @brief 设置不变形因子
         * @param[i]  fac           不变形因子
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-15
         */
        void setUnDeformFactor(const double fac);
        /**
         * @brief 获取不变形因子
         * @return double
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-15
         */
        double getUnDeformFactor();
        /**
         * @brief    设置变形变量
         * @param[i] va          变形变量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-15
         */
        void setDeformVariable(const QString& va);
        /**
         * @brief    获取变形变量
         * @return   QString     变形变量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-15
         */
        QString getDeformVariable();
    private:
        /**
         * @brief  形变过滤器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-15
         */
        FITKDeformFilter* _deformFilter = nullptr;
        /**
         * @brief 不变形系数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        double _unDeformFactor = 0.0;
        /**
         * @brief 变形因子
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        double _deformFactor = 1.0;
    };
}
#endif

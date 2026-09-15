/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostThreshold.h
 * @brief  后处理阈值数据类
 *         目前只针对标量（2024.10.14）
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostThreshold_H
#define _FITKCFDPostThreshold_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"

class vtkThreshold;

namespace Interface
{
    class FITKCFDPost3DManager;
    /**
     * @brief  后处理后处理阈值数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostThreshold : public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        FITKCFDPostThreshold(int parentID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostThreshold();
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
         * @param[i] lower           最小值
         * @param[i] upper           最大值
         * @param[i] fieldType       变量类型
         * @param[i] fieldName       变量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-14
         */
        void setValue(double lower, double upper, Interface::FITKPostFieldType fieldType, QString fieldName);
        /**
         * @brief    获取数据
         * @param[o] lower           最小值
         * @param[o] upper           最大值
         * @param[o] fieldType       变量类型
         * @param[o] fieldName       变量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-14
         */
        void getValue(double& lower, double& upper, Interface::FITKPostFieldType& fieldType, QString& fieldName);
    private:
        /**
         * @brief  阈值数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkThreshold* _threshold = nullptr;
        /**
         * @brief  场量名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-14
         */
        QString _fieldName = "";
        /**
         * @brief  场量类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-14
         */
        Interface::FITKPostFieldType _fieldType = Interface::FITKPostFieldType::Post_None;
    };
}

#endif

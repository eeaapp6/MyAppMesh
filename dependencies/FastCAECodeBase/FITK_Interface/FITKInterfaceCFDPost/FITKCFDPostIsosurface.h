/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostIsosurface.h
 * @brief  后处理等值面数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-31
 *********************************************************************/
#ifndef _FITKCFDPostIsosurface_H
#define _FITKCFDPostIsosurface_H

#include "FITKAbstractCFDPostData.h"

class vtkContourFilter;

namespace Interface
{
    /**
     * @brief  后处理等值面数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-31
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostIsosurface :public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        FITKCFDPostIsosurface(int parentID);
        virtual ~FITKCFDPostIsosurface();
        /**
         * @brief    获取后处理数据类型
         * @return   FITKPostDataType        后处理数据类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual FITKPostDataType getPostDataType();
        /**
         * @brief    设置变量名称
         * @param[i] filedName           变量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        void setFiledName(const QString filedName);
        /**
         * @brief  获取变量名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-26
         */
        QString getFiledName();
        /**
         * @brief    设置变量类型
         * @param[i] type                变量类型（点、单元）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void setFiledType(const FITKPostFieldType type);
        /**
         * @brief    获取变量类型
         * @return   FITKPostFieldType   变量类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        FITKPostFieldType getFiledType();
        /**
         * @brief    设置数据
         * @param[i] value               数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void setValue(const QList<double> value);
        /**
         * @brief    获取数据
         * @return   QList<double>       数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        QList<double> getValue();
        /**
         * @brief    获取数据
         * @return   vtkDataSet*  数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        virtual vtkDataSet* getOutput()override;
        /**
         * @brief    获取算法输出
         * @return   vtkAlgorithmOutput*   算法输出
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-01
         */
        virtual vtkAlgorithmOutput* getOutputPort() override;
    private:
        /**
         * @brief  过滤器对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        vtkContourFilter* _contourFilter = nullptr;
        /**
         * @brief  数据数量
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-26
         */
        int _valueSize = 0;
        /**
         * @brief  变量名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-26
         */
        QString _filedName = "";
        /**
         * @brief  变量类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-26
         */
        FITKPostFieldType _filedType = FITKPostFieldType::Post_None;
    };
}

#endif

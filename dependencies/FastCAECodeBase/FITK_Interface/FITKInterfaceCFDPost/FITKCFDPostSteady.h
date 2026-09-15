/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostSteady.h
 * @brief  稳态数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostSteady_H
#define _FITKCFDPostSteady_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKCFDPostImport.h"

class vtkDataSetReader;
class vtkAlgorithm;
class vtkActor;
class vtkDataSetMapper;
class vtkDataSet;

namespace Comp {
    class FITKGraphObjectVTK;
}

namespace Interface
{
    /**
     * @brief  稳态数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostSteady :public FITKCFDPostImport
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit FITKCFDPostSteady(QString fileType);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostSteady();
        /**
         * @brief    获取后处理数据类型
         * @return   FITKPostDataType        后处理数据类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual FITKPostDataType getPostDataType();
        /**
         * @brief    设置文件
         * @param[i] fileName 文件路劲
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual void setFile(QString& fileName) override;
        /**
         * @brief    
         * @return   vtkDataSet *
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        virtual vtkDataSet* getOutput() override;
        /**
         * @brief    
         * @return   vtkAlgorithmOutput *
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual vtkAlgorithmOutput* getOutputPort() override;
    protected:
        /**
         * @brief  文件类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-25
         */
        QString _fileType = "";
    private:
        /**
         * @brief  文件名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        QString _fileName = "";
        /**
         * @brief  数据读取对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        vtkAlgorithm* _dataReader = nullptr;
    };
}

#endif

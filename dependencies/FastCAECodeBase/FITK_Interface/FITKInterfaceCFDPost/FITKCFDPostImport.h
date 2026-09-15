/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostImport.h
 * @brief  数据导入类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostImport_H
#define _FITKCFDPostImport_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"

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
     * @brief  VTK稳态数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostImport :public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit FITKCFDPostImport();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostImport();
        /**
         * @brief    设置文件
         * @param[i] fileName 文件路径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual void setFile(QString& fileName) = 0;
    };
}

#endif

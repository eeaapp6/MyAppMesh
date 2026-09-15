/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostUnSteady.h
 * @brief  VTK瞬态数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostUnSteady_H
#define _FITKCFDPostUnSteady_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKCFDPostSteady.h"

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
     * @brief  VTK瞬态数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostUnSteady :public FITKCFDPostSteady
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit FITKCFDPostUnSteady(QString fileType, QString& file);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostUnSteady();
        /**
         * @brief    获取后处理数据类型
         * @return   FITKPostDataType        后处理数据类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual FITKPostDataType getPostDataType();
        /**
         * @brief    设置文件
         * @param[i] fileName 文件名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setFile(QString& fileName) override;
        /**
         * @brief    获取文件列表
         * @return   QStringList     文件列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        QStringList getFiles();
        /**
         * @brief    获取当前索引
         * @return   int             索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        int getCurrentIndex();
        /**
         * @brief    获取当前的时间步
         * @return   double         时间步
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        double getCurrentTime();
    private:
        /**
         * @brief  文件列表
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        QStringList _files = {};
        /**
         * @brief  当前索引
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        int _currentIndex = -1;
    };
}
#endif

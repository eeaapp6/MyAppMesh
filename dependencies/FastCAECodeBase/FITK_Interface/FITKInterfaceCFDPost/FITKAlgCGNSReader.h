/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAlgCGNSReader.h
 * @brief  VTK读取CGNS算法类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-25
 *********************************************************************/
#ifndef _FITKAlgCGNSReader_H
#define _FITKAlgCGNSReader_H

#include "FITKInterfaceCFDPostAPI.h"
#include <vtkUnstructuredGridAlgorithm.h>
#include <vtkSetGet.h>
#include <QObject>
#include <QMultiHash>

class vtkDataSet;


namespace Interface
{
    /**
     * @brief  VTK读取CGNS算法类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-25
     */
    class FITKInterfaceCFDPostAPI FITKAlgCGNSReader :public vtkUnstructuredGridAlgorithm
    {
        vtkTypeMacro(FITKAlgCGNSReader, vtkUnstructuredGridAlgorithm);
    public:
        static FITKAlgCGNSReader* New();
        void PrintSelf(ostream& os, vtkIndent indent);
        /**
         * @brief    设置文件名称
         * @param[i] fileName 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-25
         */
        void setFileName(const QString fileName);
    protected:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-25
         */
        FITKAlgCGNSReader();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-25
         */
        ~FITKAlgCGNSReader();
        /**
         * @brief    输出端口说明
         * @param[i] port           端口
         * @param[i] info           信息
         * @return   int
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-23
         */
        int FillOutputPortInformation(int port, vtkInformation* info);
        /**
         * @brief    处理外部请求
         * @param[i] request
         * @param[i] inputVector
         * @param[i] outputVector
         * @return   int
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-23
         */
        int ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
        /**
         * @brief    算法实现
         * @param[i] request
         * @param[i] inputVector
         * @param[i] outputVector
         * @return   int
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-23
         */
        int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
    private:
        /**
         * @brief    获取块数据
         * @param[i] blockData            块数据
         * @param[i] cuttentName 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-25
         */
        void getBlocks(vtkDataObject* blockData, const char* cuttentName = "");
        /**
         * @brief    获取dataset中所有point属性
         * @param[i] dataset 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-25
         */
        void getPointArray(vtkDataSet* dataset);
        /**
         * @brief    获取dataset中所有cell属性
         * @param[i] dataset 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-25
         */
        void getCellArray(vtkDataSet* dataset);
    private:
        /**
         * @brief  文件名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-25
         */
        QString _fileName = "";
        /**
         * @brief  块数据列表
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-25
         */
        QList<vtkDataSet*> _blockList ={};
        /**
         * @brief  所有point属性数据（缺少tuple）
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-25
         */
        QHash<QString, int*> _pointDataArray{};
        /**
         * @brief  所有cell属性数据（缺少tuple）
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-25
         */
        QHash<QString, int*> _cellDataArray{};

        QList<bool> _visibleStates = {};

        QMultiHash<int, int> _reslationShip = {};

        QStringList _blockNames = {};

        QStringList _bcTypes = {};
    };
}

#endif

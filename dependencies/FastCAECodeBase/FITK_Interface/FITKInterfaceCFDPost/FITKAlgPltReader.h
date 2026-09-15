/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAlgPltReader.h
 * @brief  plt文件读取算法
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-09
 *********************************************************************/
#ifndef READDATATECPLT_H
#define READDATATECPLT_H

#include "FITKInterfaceCFDPostAPI.h"

#include <QString>
#include <QStringList>
#include <QHash>

#include <vtkUnstructuredGridAlgorithm.h>

namespace Interface
{
    /**
     * @brief  plt文件读取算法
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-09
     */
    class FITKInterfaceCFDPostAPI FITKAlgPltReader : public vtkUnstructuredGridAlgorithm
    {
    public:
        /**
         * @brief    创建对象（静态）
         * @return   FITKAlgPltReader*   对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        static FITKAlgPltReader* New();
        /**
         * @brief    信息数据
         * @param[o] os        输出
         * @param[i] indent
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        void PrintSelf(ostream& os, vtkIndent indent) override;
        /**
         * @brief    设置文件名称
         * @param[i] fileName   文件名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        void setFileName(QString fileName);
        /**
         * @brief    获取文件名称
         * @return   QString    文件名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        QString getFileName();
        /**
         * @brief    获取block的数量
         * @return   int      数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        int getNumberOfBlocks();

        void setVisible(int blockIndex, bool vis);

        QStringList getBlockNames();

        //QStringList getBlockBCTypes();

        int FillOutputPortInformation(int nPort, vtkInformation* info);

        /**
         * @brief    获取点数据
         * @param[i] dataset  所要获取的数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        void getPointArray(vtkDataSet* dataset);
        /**
         * @brief    获取单元数据
         * @param[i] dataset  所要获取的数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        void getCellArray(vtkDataSet* dataset);
        /**
         * @brief    获取数据块
         * @param[i] block       所要获取的数据对象
         * @param[i] cuttentName 数据块名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        void getBlocks(vtkDataObject* block, const char* cuttentName = "");
    protected:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        FITKAlgPltReader();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        ~FITKAlgPltReader();

        int ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
        /**
         * @brief    算法实现
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-09
         */
        int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*);
    protected:
        /**
         * @brief  文件名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-09
         */
        QString _fileName = "";
    private:
        /**
         * @brief  数据块名称链表
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-09
         */
        QStringList _blockNames{};
        /**
         * @brief  数据块链表
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-09
         */
        QList<vtkDataSet*> _blockList;
        QList<bool> _visibleStates{};
        /**
         * @brief  点数据组
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-09
         */
        QHash<QString, int*> _pointDataArray{};
        /**
         * @brief  单元数据组
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-09
         */
        QHash<QString, int*> _cellDataArray{};
    };
}

#endif
